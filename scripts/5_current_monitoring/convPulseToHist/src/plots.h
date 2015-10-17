#ifndef __PLOTS_H_DEFINED__
#define __PLOTS_H_DEFINED__

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

class Plots {

public:
  //histogram buffers
  //TCanvas *can;
  TH1D *histFwhm_v4;
  TH1D *histFwhm_acc_v4;
  TH1D *histFwhm_rej_v4;
  TH1D *histMaxampl_v4;
  TH1D *histMaxampl_acc_v4;
  TH1D *histMaxampl_rej_v4;
  TH1D *histArea_v4;
  TH1D *histArea_acc_v4;
  TH1D *histArea_rej_v4;
  TH1D *histArea_acc_fit_v4;
  TLegend *leg;
  //histogram v5 - max. resolution area hist
  TH1D *extHistArea_v5;
  TH1D *extHistArea_acc_v5;
  TH1D *extHistArea_rej_v5;
  //histogram v6 - 2D map of fwhm (0-63) and slope (0--15)
  TH2D *extHistFwhmSlope_v6;

  Plots();
  Plots(int32_t);
  ~Plots();
  Plots( const Plots& obj );

  

private:



};



#endif
