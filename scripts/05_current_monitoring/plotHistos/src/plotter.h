#ifndef __PLOTTER_H_DEFINED__
#define __PLOTTER_H_DEFINED__

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdint.h>
#include <TApplication.h>
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



using namespace std;

class Plotter
{
public:
  Plotter();
  //Plotter(int32_t, int32_t); //mV range, area hist scaling factor
  ~Plotter();
  //void initHistFwhm();
  void initHist_v4();
  //TH1I* getHistFwhm() {return _histFwhm;};
  //void fillHistFwhm( int32_t, uint8_t*);
  void fillHist_v4( int32_t, uint8_t*);
  void fillWaveform( int32_t, uint8_t*);
  void analyseWaveform( int32_t, uint8_t*);
  void showHistFwhm();
  void showHist_v4();
  void showWaveform();
  void setMonitorValues(int32_t, int32_t, int32_t, float, float, double, int32_t);
  TCanvas* getCanvas() {return _can;};


  /// histogramming v5 -until 20150325-1
  void initHist_v5();
  void showHist_v5();
  void fillHist_v5( int32_t, uint8_t*);

  /// histogramming v6 -as of 20150325-2
  void initHist_v6();
  void showHist_v6();
  void fillHist_v6( int32_t, uint8_t*);


private:
  //histogramming
  TApplication* _app;
  TGraph* _wave;
  TCanvas* _can;
  TH1D* _histFwhm_v4;
  TH1D* _histFwhm_acc_v4;
  TH1D* _histFwhm_rej_v4;
  TH1D* _histMaxampl_v4;
  TH1D* _histMaxampl_acc_v4;
  TH1D* _histMaxampl_rej_v4;
  TH1D* _histArea_v4;
  TH1D* _histArea_acc_v4;
  TH1D* _histArea_rej_v4;
  TH1D* _histArea_acc_fit_v4;
  TLegend* _leg;

  //histogram v5 - max. resolution area hist
  TH1D* _extHistArea_v5;
  TH1D* _extHistArea_acc_v5;
  TH1D* _extHistArea_rej_v5;

  //histogram v6 - 2D map of fwhm (0-63) and slope (0--15)
  TH2D* _extHistFwhmSlope_v6;


  //monitoring values
  int32_t _fwhmThres;
  int32_t _nPulses;
  int32_t _nPulses_acc;
  float   _freq;
  float   _freq_acc;
  double _inputRange;
  int32_t _areaHistScale;


  void plotsCalibration(double ADC_mV, double ADC_pVs);
  void plotsNeutronMeas(double ADC_mV, double ADC_pVs, double Area_MeV);


};



#endif
