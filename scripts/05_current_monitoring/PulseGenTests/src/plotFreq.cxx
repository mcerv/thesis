#include "defines.h"
#include "plots.h"
#include <string.h>
#include <vector>
#include <sstream>
//#include "histana.h"
#include "measset.h"
#include "drawfuns.h"
//root
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TApplication.h"

#define FIXFACTORAMPL 1.05

using namespace std;



void plotFreq();

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  TApplication *app = new TApplication("app",0,0);

  plotFreq();


  delete(app);








  //delete psaSet;
  //delete scopeSet;
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------



void plotFreq() {

  DrawFuns *dr = new DrawFuns();
  ifstream inFile;
  inFile.open(Form("data/freq.txt"));

  const int arrlen = 100;
  int nSample = 0;
  string name;
  char delim;
  double freqOsc[arrlen], freqPsa[arrlen], ratio[arrlen];
  double corrFactor = 0;

  //first line
  for (int i=0; i<2; i++) {
    inFile >> name;
    cout << name << endl;
  }


  // read other lines
  while (1) {
    if (!inFile.good()) break;
    inFile >> freqOsc[nSample] >> delim;
    if (!inFile.good()) break;
    inFile >> freqPsa[nSample];

    //from kHz to MHz
    freqOsc[nSample] = freqOsc[nSample]/1000;
    freqPsa[nSample] = freqPsa[nSample]/1000;

    if (!nSample) corrFactor = freqOsc[nSample]/freqPsa[nSample];
    ratio[nSample] = 100*freqPsa[nSample]/freqOsc[nSample]*corrFactor;
    nSample++;
  }

  //plot things

  TCanvas *c1 = new TCanvas("Frequency","Frequency", 1200,800);
  TGraph *gr1 = new TGraphErrors(nSample, freqOsc, ratio);
  gPad->SetLogy();
  gPad->SetLogx();
  dr->prettify(c1);
  gr1->Draw("AP");
  dr->prettify(gr1);
  gr1->GetXaxis()->SetTitle("Trigger rate [10^{6} s^{-1}]");
  gr1->GetYaxis()->SetTitle("% analysed pulses");

  c1->Update();
  c1->WaitPrimitive();


}
