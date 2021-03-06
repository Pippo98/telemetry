#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include <exception>
#include <unordered_map>

#include "utils.h"
#include "devices.h"
#include "ubxparser.h"
#include "devices.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
using namespace google::protobuf;
using namespace google::protobuf::util;
using namespace std;

class Chimera{
public:
  Chimera();
  ~Chimera();

  /**
  * Fills the sensor values basing on ids and payload
  *
  * @param timestamp value that will be setted to the sensor (can be current timestamp or the one in the logfile)
  * @param id integer defining CAN message id
  * @param data is the message payload
  * @param size of the payload
  * return vector containing pointers to devices modified with this message
  */
  void parse_message(const double& timestamp, const int &id, const uint8_t data[], const int &size, vector<Device*>&);

  int parse_gps(Gps* gps_device, const double& timestamp, string& line);

  /**
  * Defines a filename to every device called DeviceName.extension
  * Used when creating csv
  *
  * @param base_path folder in which the file will be defined
  * @param extension of the filename
  */
  void add_filenames(string base_path, string extension);
  /**
  * Opens all the files of each device
  */
  void open_all_files();
  /**
  * Closes all files of each device
  */
  void close_all_files();
  /**
  * Closes the file index of every device.
  * So if added 2 filenames index can be 0 or 1

  * @param index of the files of each device
  */
  void close_files(int index);

  /**
  * Writes CSV header to the index of the file
  *
  * @param index of the files of each device
  */
  void write_all_headers(int index);

  /**
  * Sets all the values in the serialized object
  */
  void serialize();

  /*
  * Serialize only the pointer of the device passed
  */
  void serialize_device(Device*);

  /**
  * From protobuf object to string serialized
  *
  * @param out serialized string in output
  */
  void serialized_to_string(string *out){
    chimera_proto->SerializeToString(out);
  }

  /**
  * Human readable string of the serialized object
  *
  * @param outout string
  */
  void serialized_to_text(string *out){
    TextFormat::PrintToString(*chimera_proto, out);
  }
  /**
  * JSON readable string of the serialized object
  *
  * @param outout string
  */
  void serialized_to_json(string *out){
    MessageToJsonString(*chimera_proto, out);
  }

  /**
  * Clears serialized object, so repeated fields won't grow up
  *
  */
  void clear_serialized(){
    chimera_proto->Clear();
  }

  Imu* accel;
  Imu* gyro;

  Encoder* encoder_left;
  Encoder* encoder_right;

  Bms* bms_lv;
  Bms* bms_hv;

  Inverter* inverter_left;
  Inverter* inverter_right;

  Pedals* pedal;
  Steer* steer;

  State* ecu_state;
  State* bms_hv_state;
  State* steering_wheel_state;

  Ecu* ecu;

  Temperature* temp_fl;
  Temperature* temp_fr;
  Temperature* temp_rl;
  Temperature* temp_rr;

  Gps* gps1;
  Gps* gps2;

  /**
  * Vector containing all the devices
  */
  vector<Device*> devices;

  vector<string> proto_names;
  vector<Message *> proto_messages;
  unordered_map<string, string> device_headers;

private:
  // protobuffer object
  devices::Chimera *chimera_proto;
};

class Fenice{

};



#endif //VEHICLE_H
