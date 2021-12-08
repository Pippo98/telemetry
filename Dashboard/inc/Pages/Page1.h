#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "devices.pb.h"

#include "Page.h"
#include "Graph.h"

using namespace std;

class Page1 : public Page
{
public:
  Page1(string name, int w, int h);

  virtual int Draw();

  virtual bool OnMouseMove(MouseMovedEvent& e);
private:
  vector<double> x;
  vector<vector<double>> ys;

  Graph* graph1;
  Graph* graph2;
  
  uint64_t frame_count=0;
};
