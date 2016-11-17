#include <string.h>
#include <sstream>
#include "drawfuns.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "TMultiGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include <fstream>

using namespace std;

//---fit function------
//---------------------

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  DrawFuns* dr = new DrawFuns();
  TApplication* app = new TApplication("app",0,0);

  const int nSamples = 6;
  // double xAxis[nSamples] = {0,1,3.6, 10, 50}; //e-14
  // double kurfuerst[nSamples] = {500,305, 222, 111, 56};
  double xAxis[nSamples] = {0,1,10, 13, 29,59}; //e-14
  double kurfuerst[nSamples] = {510,410, 180, 140, 110,90};
  for (int i=0; i<nSamples; i++) {
    kurfuerst[i]*=1.1;
  }

  TGraph *gr = new TGraph(nSamples,xAxis,kurfuerst);
  TF1* fitf = new TF1("fit","[0] / ([0]*[1]*x + 1) ", 0, 20);
  fitf->SetParameter(0,500);
  fitf->SetParameter(1,2.2e-4);
  fitf->SetLineColor(kBlack);
  TFitResultPtr ptr = gr->Fit("fit","SR");
  cout<<endl<<" --------------------------------"<<endl;
  cout<<" Fitted k = "<<ptr->Parameter(1)*10000
      <<" +- "<<ptr->ParError(1)*10000
      <<" x 10^-18"
      <<endl;
  cout<<" --------------------------------"<<endl;

  TCanvas *c = new TCanvas("c","c",800,600);
  dr->prettify(c);
  gr->Draw("AP");
  dr->prettify(gr);
  // fitf->Draw("same");
  c->Update();
  c->WaitPrimitive();
  c->Update();



  delete(dr);
  delete(app);

  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
