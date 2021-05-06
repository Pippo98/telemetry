#include "telemetry.h"

int main(){
  HOME_PATH = getenv("HOME");
  FOLDER_PATH = "/Desktop/logs";

  FOLDER_PATH = HOME_PATH + FOLDER_PATH;

  if(!exists(FOLDER_PATH)){
    cout << "Folder not found!" << endl;
    cout << FOLDER_PATH << endl;
    return 0;
  }

  string fname = get_last_fname(FOLDER_PATH);

  sockaddr_can addr;
  Can* can = new Can(CAN_DEVICE, &addr);
  int sock = can->open();

  if(sock == -1){
    cout << "Failed creating socket" << endl;
    return -1;
  }
  else if(sock == -2){
    cout << "Failed bind socket" << endl;
  }

  CIRCUITS = vector<string>({
    "none",
    "Vadena",
    "Varano",
    "Povo",
    "Skio",
  });
  PILOTS = vector<string>({
    "none",
    "Ivan",
    "Filippo",
    "Mirco",
    "Nicola",
    "Davide",
  });
  RACES = vector<string>({
    "none",
    "Autocross",
    "Skidpad",
    "Endurance",
    "Acceleration",
  });
  
  int i1, i2, i3;
  while(true){

    // TODO: Add can filter
    while(true){
      can->receive(&message);

      if(message.can_id == 0xA0 && message.can_dlc >= 2){
        if(message.data[0] = 0x65 && message.data[1] == 0x01){
          // Start Telemetry
          cout << "Started" << endl;
          if(message.can_dlc >= 5){
            i1 = message.data[2];
            i2 = message.data[3];
            i3 = message.data[4];
          }else{
            i1 = 0;
            i2 = 0;
            i3 = 0;
          }
          break;
        }
      }
    }
    // TODO: remove filter

    string fname = get_last_fname(FOLDER_PATH);
    std::ofstream log(fname);
    
    if (i1 >= PILOTS.size())
      i1 = 0;
    if (i2 >= RACES.size())
      i2 = 0;
    if (i3 >= CIRCUITS.size())
      i3 = 0;
    
    std::time_t date = std::time(0);
    char* date_c = ctime(&date);

    log << "\r\n\n"
        "*** EAGLE-TRT\r\n"
        "*** Telemetry Log File\r\n"
        "*** " << date_c <<
        "\r\n"
        "*** Pilot: " << PILOTS[i1] << "\r\n"
        "*** Race: " << RACES[i2] << "\r\n"
        "*** Circuit: " << CIRCUITS[i3] << 
        "\n\n\r";


    stringstream line;
    while(true){
      can->receive(&message);
      
      line.str("");
      line << "(";
      line << to_string(get_timestamp());
      line << ")\t";
      line << CAN_DEVICE << "\t";

      line << get_hex(int(message.can_id), 3) << "#";
      for(int i = 0; i < message.can_dlc; i++){
        line << get_hex(int(message.data[i]), 2);
      }

      log << line.str() << endl;

      if(message.can_id == 0xA0 && message.can_dlc >= 2){
        if(message.data[0] == 0x65 && message.data[1] == 0x00){
          // Stop Telemetry
          cout << "Stopped" << endl;
          log.close();
          break;
        }
      }
    }
  }

  return 0;
}

string get_last_fname(string path){

  int number = 0;
  while(exists(path+"/"+to_string(number)+".log")){
    number ++;
  }

  return path+"/"+to_string(number)+".log";
}


double get_timestamp(){
  return duration_cast<duration<double, milli>>(system_clock::now().time_since_epoch()).count();
}

string get_hex(int num, int zeros){
  stringstream ss;
  ss << setw(zeros) << uppercase << setfill('0') << hex << num; 
  return ss.str();
}