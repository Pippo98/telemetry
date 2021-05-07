#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <chrono>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <iostream>

#include <boost/filesystem.hpp>

#include "can.h"

using namespace std;
using namespace std::chrono;
using namespace boost::filesystem;

const char* CAN_DEVICE = "vcan0";
string HOME_PATH;
string FOLDER_PATH;

vector<string> CIRCUITS;
vector<string> PILOTS;
vector<string> RACES;

int id;
uint8_t* msg_data = new uint8_t[8];

// Filter and message structs
can_filter rfilter;
can_frame message;

/**
* Gest last filename with .log extension
* The filename is number.log (1.log ...)
* @param folder path
* return full path to file
*/
string get_last_fname(string path);

/**
* Gets current timestamp in seconds
*/
double get_timestamp();

/**
* Returns a string whith int expressed as Hexadecimal
* Capital letters
*
* @param num number to be converted
* @param zeros length of the final string (num = 4 => 0000A)
* return string
*/
string get_hex(int num, int zeros);

#endif
