#include <string.h>
#include <sstream>
#include "drawfuns.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TApplication.h"
#include "waveformana.h"
#include "TMultiGraph.h"
#include "TF1.h"
#include "TLegend.h"

using namespace std;

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  int32_t voltage[4] = {-600,-700,-800,-900};
  stringstream ss;
  ss.str("");
  ss << "/Volumes/MACSTORAGE/conv/S79/wanaResultsExtra.root";
  cout <<" opening INPUT file "<<ss.str()<<endl;
  TFile* f = new TFile(ss.str().c_str(),"read");
  TFile* fana = new TFile("/Volumes/MACSTORAGE/conv/S79/wanaResults.root","UPDATE");
  TH1D* h;
  TApplication* app = new TApplication("app",0,0);
  TCanvas* can = new TCanvas("can","can",800,600);

  for (int32_t i=0; i<4; i++) {
    ss.str("");
    ss << "Amplitude distribution for "<<voltage[i];
    cout<<" getting the "<<ss.str()<<endl;
    f->cd();
    h = (TH1D*)f->Get(ss.str().c_str());
    h->Draw();
    can->Update();
    can->WaitPrimitive();
    can->Clear();
    fana->cd();
    cout<<" writing into file."<<endl;
    fana->Append(h);
    h->Write();
    fana->Write();
  }
  f->Close();
  fana->Close();
  delete(can);
  delete(fana);
  delete(app);
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
