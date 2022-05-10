using namespace std;

#include "zmqConn.h"
#include "zhelpers.hpp"

// passing address (string) and port (int) it will enstablish a connection
ZMQ::ZMQ(char* address, char* port, int openMode) {
    //Connection(address, port, openMode);
    this->address = address;
    this->port = port;
    this->openMode = openMode;
}

thread* ZMQ::start() {
    thread* t;

    if(openMode == ZMQ_PUB) {
        t = startPub(); // start a thread for publishing
    } else if(openMode == ZMQ_SUB) {
        t = startSub(); // start a thread for subscribing
    }

    return t;
}

thread* ZMQ::startPub() {
    context = new zmq::context_t(1);
    socket = new zmq::socket_t(*context, ZMQ_PUB);

    stringstream server;

    server << "tcp://" << address << ":" << port;
    //cout << "Trying to connect at " << server.str() << endl;

    try {
        (*socket).bind(server.str());
    } catch (zmq::error_t e) {
        clbk_on_error(e.num());
        clbk_on_close(e.num());
    }
    
    //cout << "Connection enstablished." << endl << "Wait a second..." << endl;

    // Ensure publisher connection has time to complete
    sleep(1);

    open = true;

    if(clbk_on_open) {
        clbk_on_open();
    }

    thread* telemetry_thread = new thread(&ZMQ::pubLoop, this);

    return telemetry_thread;
}

thread* ZMQ::startSub() {
    context = new zmq::context_t(1);
    socket = new zmq::socket_t(*context, ZMQ_SUB);

    stringstream server;

    server << "tcp://" << address << ":" << port;
    //cout << "Trying to connect at " << server.str() << endl;

    try {
        (*socket).connect(server.str());
    } catch(zmq::error_t& e) {
        if(clbk_on_error) {
            clbk_on_error(e.num());
        }

        if(clbk_on_close) {
            clbk_on_close(e.num());
        }
    }

    //cout << "Connection enstablished." << endl << "Wait a second..." << endl;

    // Ensure subscriber connection has time to complete
    sleep(1);

    open = true;

    if(clbk_on_open) {
        clbk_on_open();
    }

    thread* telemetry_thread = new thread(&ZMQ::subLoop, this);

    return telemetry_thread;
}

void ZMQ::subscribe(string topic, int len) {
    try {
        (*socket).setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), 4);
    } catch(zmq::error_t& e) {
        clbk_on_error(e.num());
    }

    if(clbk_on_subscribe) {
        clbk_on_subscribe(topic);
    }
}

void ZMQ::unsubscribe(string topic, int len) {
    try {
        (*socket).setsockopt(ZMQ_UNSUBSCRIBE, topic.c_str(), len);
    } catch(zmq::error_t& e) {
        clbk_on_error(e.num());
    }

    if(clbk_on_unsubscribe) {
        clbk_on_unsubscribe(topic);
    }
}

// before ending the program remember to close the connection
void ZMQ::closeConnection() {
    socket->close();
    context->close();

    if(clbk_on_close) {
        clbk_on_close(0);   // 0 means no error
    }

    done = true;
    cv.notify_all();
    //cout << "Connection closed." << endl;
}

void ZMQ::pubLoop() {
    while(!open) usleep(10000);
    while(!done) {
        unique_lock<mutex> lck(mtx);

        while(!new_data && !done) cv.wait(lck);

        // Adding this because code can be stuck waiting for cv
        // when actually the socket is being closed
        if(done) break;

        message msg = buff_send.front();
        
        this->sendMessage(msg.topic, msg.payload);

        buff_send.pop();

        if(buff_send.empty()) new_data = false;
    }
}

void ZMQ::subLoop() {
    while(!open) usleep(10000);
    while(!done) {
        string topic, data;

        try {
            topic = s_recv(*socket);
            data = s_recv(*socket);
        } catch(zmq::error_t& e) {
            clbk_on_error(e.num());
        }

        message msg;

        {
            unique_lock<mutex> lck(mtx);
            
            msg.topic = topic;
            msg.payload = data;

            if(clbk_on_message) {
                clbk_on_message(socket, msg);
            }
        }
    }
}

void ZMQ::clearData() {
    unique_lock<mutex> guard(mtx);

    while(!buff_send.empty()) {
        buff_send.pop();
        new_data = false;
    }
}

void ZMQ::setData(string id, string data) {
    unique_lock<mutex> guard(mtx);
    
    message msg;

    msg.topic = id;
    msg.payload = data;

    buff_send.push(msg);

    if(buff_send.size() > 20) {
        buff_send.pop();
    }

    if(!new_data) new_data = true;

    cv.notify_all();
}

// passing the topic and the message it will send it
void ZMQ::sendMessage(string topic, string msg) {
    //cout << topic << ": " << msg << endl;
    int rc;
    rc = s_sendmore(*socket, topic); // #id
    if(rc < 0) {
        if(clbk_on_error) {
            clbk_on_error(rc);
        }
    }

    rc = s_send(*socket, msg); // message

    if(rc < 0) {
        if(clbk_on_error) {
            clbk_on_error(rc);
        }
    }
}

void ZMQ::receiveMessage(string& topic, string& payload) {
    try {
        topic = s_recv(*socket);
        payload = s_recv(*socket);
    } catch(zmq::error_t& e) {
        if(clbk_on_error) {
            clbk_on_error(e.num());
        }

        /* does it make sense to close the connection here?
        if(clbk_on_close) {
            clbk_on_close(e.num());
        }
        */
    }
}

void ZMQ::stop() {
    scoped_lock guard(mtx);
    done = true;
    open = false;

    cv.notify_all();

    if(clbk_on_close) {
        clbk_on_close(1000);
    }
}

void ZMQ::reset() {
    scoped_lock guard(mtx);
    done = false;
    open = false;
    new_data = false;
    errorCode = NULL;
    clearData();
}

////////////////////////////////////////////////////////////////////
//////////////////////////    CALLBACKS   //////////////////////////
////////////////////////////////////////////////////////////////////

void ZMQ::add_on_open(function<void()> clbk) {
    clbk_on_open = clbk;
}

void ZMQ::add_on_close(function<void(int)> clbk) {
    clbk_on_close = clbk;
}

void ZMQ::add_on_error(function<void(int)> clbk) {
    clbk_on_error = clbk;
}


void ZMQ::add_on_message(function<void(zmq::socket_t*, message)> clbk) {
    clbk_on_message = clbk;
}

void ZMQ::add_on_subscribe(function<void(string)> clbk) {
    clbk_on_subscribe = clbk;
}

void ZMQ::add_on_unsubscribe(function<void(string)> clbk) {
    clbk_on_unsubscribe = clbk;
}