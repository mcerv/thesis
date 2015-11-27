#include <string.h>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include "drawfuns.h"
#include <vector>
#include "TFile.h"
#include "TF1.h"
#include "TApplication.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TParameter.h"
#include "TLine.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLatex.h"

#define DEBUG 0
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4
#define SCDHQ 5




using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<"Grabbing the source scans and taking occupancy plots.."<<endl;

  stringstream ss;
  TFile* f;
  const int nPlots=4;
  string plotName[nPlots] = { "../plots/MDBM-23-eff.pdf", "../plots/MDBM-23-occ.pdf",
                              "../plots/MSBM-34-eff.pdf", "../plots/MSBM-34-occ.pdf"};
  string fileName[nPlots] = {  "../data/MDBM-23-eff.root",
                          "../data/MDBM-23-occ.root",
                          "../data/MSBM-34-eff.root",
                          "../data/MSBM-34-occ.root" };
  string histName[nPlots] = {  "Occupancy/Plane0ClusterOccupancy",
                          "Occupancy/Plane0ClusterOccupancy",
                          "Occupancy/Plane0ClusterOccupancy",
                          "Occupancy/Plane0ClusterOccupancy" };
  int max[nPlots] = { 40, 100, 100, 750 };

  TApplication *app = new TApplication("app",0,0);
  DrawFuns *dr = new DrawFuns();
  TCanvas *can = new TCanvas("ca","ca",800,600);
  dr->prettify(can);

  TH2D *hist[nPlots];
  for (int i=0; i<nPlots; i++) {

    f = new TFile(fileName[i].c_str());
    if (!(TH1D*)f->Get(histName[i].c_str() ) ) {
      cout<<" File " << fileName[i].c_str() << " doesn't include the occ plot!" << endl;
    }
    hist[i] = new TH2D( *(TH2D*)f->Get(histName[i].c_str() )  );
    dr->prettify(hist[i]);
    hist[i]->Draw("col");

    hist[i]->GetXaxis()->SetTitle("X position [um]");
    hist[i]->GetXaxis()->SetTicks("b");
    hist[i]->GetYaxis()->SetTitle("Y position [um]");
    hist[i]->SetMaximum(max[i]);
    can->SetRightMargin(0.06);
    can->Update();
    can->WaitPrimitive();
    can->Update();
    can->SaveAs(plotName[i].c_str());
    can->Clear();
    f->Close();
    delete f;
  }

  // dr->prettify(can);
  // for (int i=0; i<nPlots; i++) {
  //   hist[i]->Draw();
  //   dr->prettify(hist[i]);
  //   can->Update();
  //   can->WaitPrimitive();
  //   can->Update();
  //   can->Clear();
  //
  // }

        // cout << "\r" << "Opening file " << ss.str() << flush;// << endl;
        // f = new TFile(ss.str().c_str(),"read");
        // if (!f->IsOpen()) {
        //   cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
        //   missingprof[sample][temp][num] = true;
        //   // profvec prof;
        //   continue;
        // }
        // cntFilesOpen++;
        // // if (!(TProfile*)f->Get("avgpulses_filt")) {
        // if (!(TProfile*)f->Get("avgpulses")) {
        //   cout<<" File " << ss.str().c_str() << " doesn't include avgpulses!" << endl;
        //   missingprof[sample][temp][num] = true;
        //   continue;
        // }
        // if (!((TParameter<Int_t>*)f->Get("avgTimestamp")) ) {
        //   cout<<" File " << ss.str().c_str() << " doesn't include avgTimestamp!" << endl;
        //   missingprof[sample][temp][num] = true;
        //   continue;
        // }
        // avgTimestamp[sample][temp][num] = (((TParameter<Int_t>*)f->Get("avgTimestamp"))->GetVal());
        //

  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
