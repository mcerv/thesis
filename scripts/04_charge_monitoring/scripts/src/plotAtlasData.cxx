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
#include "AtlasStyle.h"
#include "AtlasUtils.h"

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
  cout<<endl<<" Grabbing the source scans and taking occupancy plots.."<<endl;



  stringstream ss;
  TFile* f;
  const int nFiles=2;
  const int nPlots=2;
  const int nSaves=4;
  // string plotName[nPlots] = { "../plots/MDBM-23-eff.pdf", "../plots/MDBM-23-occ.pdf",
  //                             "../plots/MSBM-34-eff.pdf", "../plots/MSBM-34-occ.pdf"};
  string fileName[nFiles] = { "../data/Z0hist.root",
                              "../data/D0hist.root" };
  string plotName[nFiles][nSaves] = { "../plots/Z0.pdf", "../plots/Z0.eps", "../plots/Z0.ps", "../plots/Z0.png",
                              "../plots/D0.pdf", "../plots/D0.eps", "../plots/D0.ps", "../plots/D0.png" };
  string histName[nPlots] = { "histpaired_0" , "histunpaired_0"  };
  string histLeg[nPlots] = {  "Paired bunches" ,"Unpaired bunches"};
  string xAx[nFiles] = { "Z0 [mm]", "D0 [mm]"};
  int yaxis[nPlots] = {100, 160};
  // int max[nPlots] = { 40, 100, 100, 750 };

  TApplication *app = new TApplication("app",0,0);
  DrawFuns *dr = new DrawFuns();
  TCanvas *can = new TCanvas("ca","ca",800,600);
  TLegend *leg = new TLegend (0.62,0.77,0.93,0.93);

  TH1D *hist[nPlots];
  for (int i=0; i<nFiles; i++) {
            SetAtlasStyle();

    dr->prettify(can);
      gStyle->SetGridColor(kGray);
      gStyle->SetGridWidth(1);
      gStyle->SetGridStyle(1001);
  can->SetRightMargin(0.06);
    f = new TFile(fileName[i].c_str());
    for (int plot=0; plot<nPlots; plot++) {
      if (!(TH1D*)f->Get(histName[plot].c_str() ) ) {
        cout<<" File " << fileName[plot].c_str() << " doesn't include the occ plot!" << endl;
      }
      hist[plot] = new TH1D( *(TH1D*)f->Get(histName[plot].c_str() )  );

      if (!plot) {
        // dr->prettify(hist[plot], "blue");
        // hist[plot]->GetXaxis()->SetTitle(xAx[plot].c_str() );
        hist[plot]->GetXaxis()->SetTitle("Z0 [mm]" );
        hist[plot]->GetXaxis()->SetRangeUser(-601,601);
        hist[plot]->GetYaxis()->SetRangeUser(0,yaxis[i]+1);
        hist[plot]->GetYaxis()->SetLimits(0,yaxis[i]+1);
        hist[plot]->GetYaxis()->SetTitle("Entries");
        hist[plot]->SetLineStyle(1);
          hist[plot]->GetXaxis()->SetTitleSize(0.05);
          hist[plot]->GetYaxis()->SetTitleSize(0.05);
          hist[plot]->GetYaxis()->SetTitleOffset(1.2);
          hist[plot]->GetXaxis()->SetTitleOffset(1.2);
          hist[plot]->GetXaxis()->SetLabelOffset(0.007);
          hist[plot]->GetXaxis()->SetTitleFont(42);
          hist[plot]->GetYaxis()->SetTitleFont(42);
          hist[plot]->GetXaxis()->SetLabelFont(42);
          hist[plot]->GetYaxis()->SetLabelFont(42);
          hist[plot]->GetXaxis()->SetLabelSize(0.04);
          hist[plot]->GetYaxis()->SetLabelSize(0.04);
          hist[plot]->GetYaxis()->SetTickLength(0.01);

        hist[plot]->Draw();
        SetAtlasStyle();
      }
      else {
        // dr->prettify(hist[plot],"red");
        // hist[plot]->GetXaxis()->SetTitle(xAx[plot].c_str() );
        hist[plot]->GetXaxis()->SetTitle("D0 [mm]" );
          hist[plot]->GetXaxis()->SetTitleSize(0.05);
          hist[plot]->GetYaxis()->SetTitleSize(0.05);
          hist[plot]->GetYaxis()->SetTitleOffset(1.2);
          hist[plot]->GetXaxis()->SetTitleOffset(1.2);
          hist[plot]->GetXaxis()->SetLabelOffset(0.007);
          hist[plot]->GetXaxis()->SetTitleFont(42);
          hist[plot]->GetYaxis()->SetTitleFont(42);
          hist[plot]->GetXaxis()->SetLabelFont(42);
          hist[plot]->GetYaxis()->SetLabelFont(42);
          hist[plot]->GetXaxis()->SetLabelSize(0.04);
          hist[plot]->GetYaxis()->SetLabelSize(0.04);
          hist[plot]->GetYaxis()->SetTickLength(0.01);
        // hist[plot]->GetXaxis()->SetRangeUser(-150,151);
        hist[plot]->GetYaxis()->SetRangeUser(0,yaxis[i]+1);
        hist[plot]->GetYaxis()->SetLimits(0,yaxis[i]+1);
        hist[plot]->GetYaxis()->SetTitle("Entries");
        hist[plot]->SetLineStyle(2);
        hist[plot]->Draw("same");


      }



      leg->AddEntry(hist[plot], histLeg[plot].c_str(), "L" );
      // hist[plot]->SetMaximum(max[i]);
      // can->Update();
      // can->WaitPrimitive();
    }
    int color = 1;
    double x = 0.18, y=0.88;
    leg->SetLineWidth(0);
    leg->Draw();
    SetAtlasStyle();
    TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize);
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(color);
    l.DrawLatex(x,y,"ATLAS Internal");
    l.DrawLatex(x,y-0.055,"DBM");
    can->Update();
    can->WaitPrimitive();
    can->Update();
    for (int s=0; s<nSaves; s++)
      can->SaveAs(plotName[i][s].c_str());
    can->Clear();
    leg->Clear();
    for (int a=0; a<nPlots; a++) {
      hist[a]->Clear();
    }
    f->Close();
    delete f;
  }




  cout<<" All done."<<endl << endl;
  return 0;
}//-------------------- end of main -----------------------------------------
