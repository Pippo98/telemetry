using namespace std;

#include <iostream>
#include <string>
#include <unistd.h>

#include "wsclient.h"

void onMessage(const int& id, const GenericMessage& msg) {
    cout << id << " RECV: " << msg.data << endl;
}

void onError(const int& id, const int& code, const string& message) {
    cout << id << " Error: " << code << " - " << message << endl;
}

void onClose(const int& id, const int& code) {
    cout << id << " Connection closed: " << code << endl;
}

void onOpen(const int& id) {
    cout << id << " Connection opened" << endl;
}

int main() {
    WebSocketClient cli1;
    cli1.init("192.168.43.207", "3000", 0);
    WebSocketClient cli2;
    cli2.init("192.168.43.207", "3000", 0);

    //cli1.add_on_message(onMessage);
    cli1.addOnOpen(onOpen);
    cli1.addOnClose(onClose);
    cli1.addOnError(onError);

    cli2.addOnOpen(onOpen);
    cli2.addOnClose(onClose);
    cli2.addOnError(onError);
    cli2.addOnMessage(onMessage);

    thread *cli1_thread = cli1.start();
    thread *cli2_thread = cli2.start();


    while(true) {
        sleep(1);
        cli1.setData(GenericMessage("Test Message"));
    }

    cli1.closeConnection();
    cli2.closeConnection();

    return 0;
}