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
#include "TMath.h"

#define DEBUG 0
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4
#define SCDHQ 5

Double_t fNfree2(Double_t *x, Double_t *par)
{
	Double_t Ntot = par[0];
	Double_t Ni = par[1];
	Double_t Na = Ntot - Ni;
	Double_t trel = par[2];
	Double_t Ea = par[3];
	Double_t k = 8.6e-5;

	return Na + Ni/(1 + trel*TMath::Exp(Ea / (k*x[0])));
}



using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<endl<<"Plotting charge vs temperature.."<<endl;

  TApplication *app = new TApplication("app",0,0);
  DrawFuns *dr = new DrawFuns();
  const int nvolts = 2;
  const int ndets = 5;

  // string detstr[ndets] = {"1scdhq","S52","S79","S37","S50","ELSC", "S52_3-63e14","S79_1e14"};
  string detstr[ndets] = {"S52","S79","S37","S79_1e14","S52_3-63e14"};
  string detName[ndets] = {"S52","S79","S37","S79 1#times10^{14} #pi cm^{-2}","S52 3.63#times10^{14} #pi cm^{-2}"};
  string voltstr[nvolts] = {"-500V", "500V"};
  string carrier[nvolts] = {"holes","electrons"};
  string colors[ndets] = {"red", "blue", "green", "orange", ""};
  string filePath[nvolts][ndets];

  for (int i=0; i<ndets; i++) {
    cout << i << endl;
    filePath[0][i] = "../data/charge/charge_";
    filePath[0][i] += detstr[i];
    filePath[0][i] += "_";
    filePath[0][i] += voltstr[0];
    filePath[0][i] += ".root";
    filePath[1][i] = "../data/charge/charge_";
    filePath[1][i] += detstr[i];
    filePath[1][i] += "_";
    filePath[1][i] += voltstr[1];
    filePath[1][i] += ".root";

    cout << filePath[0][i] << endl;
    cout << filePath[1][i] << endl;
  }

  TFile *file;
  TProfile *prof[nvolts][ndets];
  TGraph *gr[nvolts][ndets];
  int color[8] = {1,2,3,4,6,12,8,9};
  TLegend *leg[nvolts];// = new TLegend(0.6,0.6,1,1);
  TCanvas *c[nvolts];
  //
  //
  for (int volt=0; volt<nvolts; volt++) {
    c[volt] = new TCanvas (voltstr[volt].c_str(), voltstr[volt].c_str(), 800 ,600);
    dr->prettify(c[volt]->cd() );
    c[volt]->SetTopMargin(0.06);
    leg[volt] = new TLegend (0.49,0.21,0.9,0.50);
    for (int det=0; det<ndets; det++) {
      file = new TFile(filePath[volt][det].c_str(), "read");
      if (file->IsOpen()) cout<<"File "<<filePath[volt][det]<<" open."<<endl;

      gr[volt][det] = (TGraph*)file->Get("Graph");
      dr->prettify(gr[volt][det], colors[det]);
      gr[volt][det]->GetXaxis()->SetRangeUser(0,310);
      gr[volt][det]->GetYaxis()->SetRangeUser(0,60.1e-15);
      gr[volt][det]->GetYaxis()->SetLimits(0,60);
      gr[volt][det]->GetYaxis()->SetLimits(0,60);
      gr[volt][det]->GetYaxis()->SetLimits(0,60);
      gr[volt][det]->SetMarkerStyle(24-det);
      gr[volt][det]->SetLineColor(kBlack);
      gr[volt][det]->SetLineWidth(1);

      if (!det) {
        gr[volt][det]->Draw("AP");
        // gr[volt][det]->GetYaxis()->SetRangeUser(0,60e-15);
        // gr[volt][det]->GetYaxis()->SetLimits(0,60);
        // gr[volt][det]->SetTitle("Collected charge vs temperature");
        gr[volt][det]->GetXaxis()->SetTitle("Temperature [K]");
        gr[volt][det]->GetYaxis()->SetTitle("Charge [C]");

      }
      else {
        gr[volt][det]->Draw("sameP");
      }
      // gr[volt][det]->SetLineColor(color[det]);
      // gr[volt][det]->SetMarkerColor(color[det]);
      file->Close();
      file = 0;
      leg[volt]->AddEntry(gr[volt][det],detName[det].c_str(),"LEP");
      TLatex *lat = new TLatex(248.68,5.55e-14, carrier[volt].c_str());
      lat->SetTextFont(42);
      lat->Draw("same");
    }
    TF1* funcc = new TF1("Nfree2a",fNfree2,40,300,4);
    if (!volt) {
      funcc->FixParameter(0,52.4e-15);
      funcc->FixParameter(1,33.9e-15);
      funcc->FixParameter(2,4.45e-5);
      funcc->FixParameter(3,0.08);
    } else {
      funcc->FixParameter(0,50.5e-15);
      funcc->FixParameter(1,32.7e-15);
      funcc->FixParameter(2,4.41e-5);
      funcc->FixParameter(3,0.08);
    }
    funcc->Draw("same");
    leg[volt]->AddEntry(funcc,"S52 by H. Jansen");

    leg[volt]->Draw("same");
    string savePath = "../plots/charge";
    savePath += voltstr[volt];
    savePath += ".pdf";
    c[volt]->SaveAs(savePath.c_str() );

    c[volt]->Update();
    c[volt]->WaitPrimitive();
    c[volt]->Update();
  }

  // stringstream ss;
  // TFile* f;
  // const int nPlots=4;
  // string plotName[nPlots] = { "../plots/MDBM-23-eff.pdf", "../plots/MDBM-23-occ.pdf",
  //                             "../plots/MSBM-34-eff.pdf", "../plots/MSBM-34-occ.pdf"};
  // string fileName[nPlots] = {  "../data/MDBM-23-eff.root",
  //                         "../data/MDBM-23-occ.root",
  //                         "../data/MSBM-34-eff.root",
  //                         "../data/MSBM-34-occ.root" };
  // string histName[nPlots] = {  "Occupancy/Plane0ClusterOccupancy",
  //                         "Occupancy/Plane0ClusterOccupancy",
  //                         "Occupancy/Plane0ClusterOccupancy",
  //                         "Occupancy/Plane0ClusterOccupancy" };
  // int max[nPlots] = { 40, 100, 100, 750 };
  //
  // TApplication *app = new TApplication("app",0,0);
  // DrawFuns *dr = new DrawFuns();
  // TCanvas *can = new TCanvas("ca","ca",800,600);
  // dr->prettify(can);
  //
  // TH2D *hist[nPlots];
  // for (int i=0; i<nPlots; i++) {
  //
  //   f = new TFile(fileName[i].c_str());
  //   if (!(TH1D*)f->Get(histName[i].c_str() ) ) {
  //     cout<<" File " << fileName[i].c_str() << " doesn't include the occ plot!" << endl;
  //   }
  //   hist[i] = new TH2D( *(TH2D*)f->Get(histName[i].c_str() )  );
  //   dr->prettify(hist[i]);
  //   hist[i]->Draw("col");
  //
  //   hist[i]->GetXaxis()->SetTitle("X position [um]");
  //   hist[i]->GetXaxis()->SetTicks("b");
  //   hist[i]->GetYaxis()->SetTitle("Y position [um]");
  //   hist[i]->SetMaximum(max[i]);
  //   can->SetRightMargin(0.06);
  //   can->Update();
  //   can->WaitPrimitive();
  //   can->Update();
  //   can->SaveAs(plotName[i].c_str());
  //   can->Clear();
  //   f->Close();
  //   delete f;
  // }

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
