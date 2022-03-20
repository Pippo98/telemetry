#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <ctime>
#include <chrono>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <iostream>
#include <exception>
#include <unordered_map>

#include <cstdio>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <filesystem>

#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
using namespace rapidjson;

struct CAN_Stat_t
{
  double duration;
  uint64_t msg_count;
};

#include "can.h"
#include "utils.h"
#include "serial.h"
#include "vehicle.h"
#include "gps_logger.h"
#include "camera.h"

#include "wsclient.h"
#include "devices.pb.h"

#include "console.h"

#include "telemetry_config/json_loader.h"
#include "session_config/json_loader.h"

using namespace std;
using namespace std::chrono;
using namespace filesystem;


telemetry_config tel_conf;
session_config sesh_config;

vector<GpsLogger*> gps_loggers;
Camera camera;

thread* status_thread = nullptr;
thread* data_thread = nullptr;
thread* ws_conn_thread = nullptr;
thread* ws_cli_thread = nullptr;

unordered_map<string, double> timers;

const char *CAN_DEVICE;

bool ws_reqeust_on=false;
bool ws_reqeust_off=false;
bool state_changed = false;
unordered_map<string, uint32_t> msgs_counters;
unordered_map<string, uint32_t> msgs_per_second;
atomic<int> run_state;
mutex mtx;

string HOME_PATH;
string FOLDER_PATH;


int id;
uint8_t *msg_data = new uint8_t[8];
sockaddr_can addr;
can_filter rfilter;
can_frame message;

time_t date;
struct tm ltm;
string human_date;

std::fstream* dump_file;

WebSocketClient* ws_cli;
Chimera* chimera;
Can * can;

CAN_Stat_t can_stat;
ConnectionState_ ws_conn_state = ConnectionState_::NONE;

vector<Device *> modifiedDevices;

void create_header(string& out);
void create_folder_name(string& out);

int open_log_folder();
int open_can_socket();

void send_status();
void send_ws_data();

void log_can(double& timestamp, can_frame& msg, std::fstream& out);
void save_stat(string folder);

void on_gps_line(int id, string line);



void load_all_config(std::string&);


void on_message(client* cli, websocketpp::connection_hdl hdl, message_ptr msg);
void connect_ws();
void on_open();
void on_error(int code);
void on_close(int code);



#endif
