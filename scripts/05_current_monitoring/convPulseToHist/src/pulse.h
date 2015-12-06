#ifndef __PULSE_H_DEFINED__
#define __PULSE_H_DEFINED__

#include <TCanvas.h>
#include <TH1I.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TPad.h>
#include <TF1.h>
#include "defines.h"
#include <TStyle.h>
#include <TLatex.h>
#include <TGaxis.h>
#include <TPaveStats.h>
#include <TList.h>
#include <iostream>
#include <sstream>

using namespace std;

class Pulse {

public:
  //histogram buffers
  TGraph* getGraph();

  Pulse();
  Pulse(const Pulse&);
  Pulse(int32_t); //serial number
  Pulse(int64_t,  //timestamp
        int32_t,  //serial number - trig count
        int16_t,  //max amplitude
        int16_t,  //width
        int16_t,  //area
        int16_t,  //slope
        int16_t,  //cut flags
        int16_t  //classification
        );
  ~Pulse();

  void setWf(int32_t,  //waveform length
             int32_t*,   //time axis
             int32_t*    //amplitude axis
             );
  void print();         //print the pulse values

  std::vector<int32_t>X;     //time axis
  std::vector<int32_t>Y;     //amplitude axis

  // int64_t m_number;         //pulse number received
  int64_t m_timestamp;      //pulse timestamp
  int32_t m_trigCount;      //pulse trig count from file
  int16_t m_ampl;           //pulse Amplitude
  int16_t m_width;          //pulse Width
  int16_t m_area;           //pulse area
  int16_t m_slope;          //pulse falling slope
  int16_t m_cutFlags;       //pulse cut flags (ampl,fwhm,slope,area)
  int16_t m_class;          //pulse classification
  int16_t m_wfLen;          //waveform length


private:
  TGraph m_graph;


};



#endif
