#include "vehicle.h"

int main(){
  Chimera chimera;

  for(auto device : chimera.devices){
    cout << device->get_id() << " " << device->name() << endl;
  }

  return 0;
}