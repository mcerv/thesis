#ifndef __DRAWFUNS_H_DEFINED__
#define __DRAWFUNS_H_DEFINED__


#include "TGraphErrors.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>


class DrawFuns {

public:
  void prettify (TGraph *gr );
  void prettify (TGraphErrors *gr );
  void prettify (TCanvas *c);
  void prettify (TVirtualPad *c);
  void prettify (TH1 *h );
  void prettify (TH1 *h , std::string color);//color
  void prettify (TH2 *h );
  void prettyGraphShade (int n, double *xaxis, double *yaxis,
                                double *xerr,  double *yerr  );

  double relError (double X, double errX, double Y, double errY);
  void normalise(TH1D*); //normalises the histogram to maximum 1.
  //void normaliseSecondPeak(TH1D*); //normalises the histogram to maximum 1.

  DrawFuns();
  ~DrawFuns();
};




#endif
