#ifndef __PULSEANA_H_DEFINED__
#define __PULSEANA_H_DEFINED__


#include <fstream>
#include "TApplication.h"
#include "TCanvas.h"
#include "defines.h"
#include "pulse.h"
#include "drawfuns.h"
#include "TFile.h"
#include <string.h>
#include <sstream>
#include <vector>
#define MAXBUFFBYTES 2e31-1

using namespace std;

class PulseAna {

public:
  PulseAna();
  ~PulseAna();

  void readPulseFile(string fName, string version);
  void printHistLine(int32_t);
  void extractHistogramData();//version 4 or 5 or 6
  void writeToHistFile (string fName);
  void fillWaveform (std::vector<char>);
  void fillWaveformC2(std::vector<char>);
  void analyse();
  void showPulses();
  void plot();
  void plotCx();
  void clear();

  //initialise buffers
  int32_t m_exthistfwhmslope[NBINS_FWHM_SLOPE];
  int32_t m_histfwhm[NBINS];
  int32_t m_histmaxampl[NBINS];
  int32_t m_histalphamaxampl[NBINS];
  int32_t m_histarea[NBINS];
  int32_t m_histalphaarea[NBINS];
  int32_t m_histslope[NBINS_FWHM_SLOPE];
  int32_t m_exthistarea[NBINS_EXTAREA];
  int32_t m_exthistalphaarea[NBINS_EXTAREA];


  std::vector<Pulse> m_vp;

private:
  TCanvas* _can;

};


#endif
