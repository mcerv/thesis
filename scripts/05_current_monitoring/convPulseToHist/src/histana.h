#ifndef __HISTANA_H_DEFINED__
#define __HISTANA_H_DEFINED__


#include <fstream>
#include "TApplication.h"
#include "TCanvas.h"
#include "defines.h"
#include "plots.h"
#include "drawfuns.h"
#include <string.h>
#include <sstream>
#define MAXBUFFBYTES 262000
#define HISTBYTES 150000

using namespace std;

class HistAna {

public:
  HistAna();
  ~HistAna();

  void readFileIntoBuffer(string fName, vector<uint8_t>& USBdata);
  void extractHistogramData(vector<uint8_t>& USBdata, Plots *plot, string version);//version 4 or 5 or 6
  void addEntry (Plots *plot);
  vector<Plots> m_vp; //vector of plots

  void plotReactor();
  TH1D* extToNormal(TH1D*);//transform long histogram (4096) to normal (256);

private:

};


#endif
