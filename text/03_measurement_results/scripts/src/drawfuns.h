#ifndef __DRAWFUNS_H_DEFINED__
#define __DRAWFUNS_H_DEFINED__


#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TColor.h"
#include "TExec.h"
// #include "TMPalette.h"
//#include "TSpectrum.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>

using namespace std;

class DrawFuns {

public:
  void prettify (TGraph *gr );
  void prettify (TGraphErrors *gr );
  void prettify (TMultiGraph *gr );
  void prettify (TCanvas *c, string histType); //string is th1 or th2 (margins)
  void prettify (TCanvas *c);
  void prettify (TVirtualPad *c, string histType); //string is th1 or th2 (margins)
  void prettify (TVirtualPad *c);
  void prettify (TH1 *h );
  void prettify (TH1 *h , std::string color);//color
  void prettify (TH2 *h );
  void prettyGraphShade (int n, double *xaxis, double *yaxis,
                                double *xerr,  double *yerr  );

  double relError (double X, double errX, double Y, double errY);
  void normalise(TH1D*); //normalises the histogram to maximum 1.
  //void normaliseSecondPeak(TH1D*); //normalises the histogram to maximum 1.
  void setColourScheme(string); // for 2D histograms
  DrawFuns();
  ~DrawFuns();

  TColor color[22];
  TColor color2[22];
  int32_t clr[22];
  int32_t clrTemp[22];
  int32_t clrVolt[10];


};




#endif
