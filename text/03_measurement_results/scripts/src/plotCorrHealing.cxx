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
#include "TH2.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLatex.h"
#include <fstream>

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
  cout<<"Plotting Healing correlations"<<endl;


  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();

  TMultiGraph *mg = new TMultiGraph();
  const int32_t nProcedures = 5;
  TGraph *gr[nProcedures];
  string column[3];
  double run[nProcedures][10];
  double delay[nProcedures][10];
  double corr[nProcedures][10];
  double corrNorm[nProcedures][10];
  string procedure[5] = { "(1) No source, HV ON",
                          "(2) No source, HV OFF",
                          "(3) ^{60}Co, HV OFF",
                          "(4) ^{90}Sr, HV ON",
                          "(5) ^{90}Sr, HV OFF",
                        };

  TLegend *leg = new TLegend (0.64,0.4,0.96,0.8);

  for (int32_t proc=0; proc<nProcedures; proc++) {
    stringstream ss;
    ss << "data/healing" << proc << ".txt";
    cout << "Opening " << ss.str() << endl;
    ifstream f (ss.str().c_str());

    f >> column[0] >> column[1] >> column[2];
    cout << column[0] << " " << column[1] << " " << column[2] << endl;
    int32_t i=0;
    while (f.good()) {
      f >> run[proc][i] >> delay[proc][i] >> corr[proc][i];
      corrNorm[proc][i] = corr[proc][0] / corr[proc][i];
      cout << run[proc][i] << " " << delay[proc][i] << " " << corr[proc][i]
           << " " << corrNorm[proc][i] << endl;
      i++;
    }
    f.close();

    gr[proc] = new TGraph(i-2, &delay[proc][1], &corrNorm[proc][1]);
    dr->prettify(gr[proc]);
    gr[proc]->SetMarkerStyle(20+proc);
    gr[proc]->SetMarkerSize(1.8);
    ss.str("");
    // TCanvas *c1 = new TCanvas ("c1","c1", 800,600);
    // gr[proc]->Draw("AP");
    // c1->Update();
    // c1->WaitPrimitive();

    // ss << << proc;
    leg->AddEntry(gr[proc],procedure[proc].c_str(),"lep");
    mg->Add(gr[proc]);
  }

  TCanvas *c = new TCanvas ("c","c", 800,600);
  dr->prettify(c);
  gPad->SetLogx();
  gPad->SetLogy();
  mg->Draw("APL");
  dr->prettify(mg);
  mg->GetXaxis()->SetRangeUser(0.9, 100000);
  // mg->GetXaxis()->SetLimits(0.9, 100000);
  mg->GetYaxis()->SetRangeUser(0.01,1);
  mg->GetXaxis()->SetTitle("Time [s]");
  mg->GetYaxis()->SetTitle("Shape correlation");
  leg->Draw("same");


  c->Update();
  c->WaitPrimitive();
  c->Update();






  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
