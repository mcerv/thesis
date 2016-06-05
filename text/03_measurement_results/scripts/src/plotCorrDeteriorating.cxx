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
#define PARTICLESPERSEC 7




using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<"Plotting Deteriorating correlations"<<endl;


  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();

  TMultiGraph *mg = new TMultiGraph();
  const int32_t nFiles = 2;
  TGraph *gr[nFiles];
  string column[3];

  double run[nFiles][1000];
  double delay[nFiles][1000];
  double corr[nFiles][1000];
  double corrNorm[nFiles][1000];
  string fileTitle[nFiles] = { "Elecs",
                          "Holes"  };
  string fileTitleLeg[nFiles] = { "S79 1#times10^{14} #pi cm^{-2} - Electrons",
                          "S79 1#times10^{14} #pi cm^{-2} - Holes"  };


  TLegend *leg = new TLegend (0.5,0.76,0.95,0.95);

  TF1* decayfun = new TF1("decay","[0] + ([1]-[0])*exp(-(x+[3])/[2])");
  decayfun->SetParName(0,"chgInf");     //charge at infinity
  decayfun->SetParName(1,"chgStart");   //starting charge
  decayfun->SetParName(2,"tauRec");     //recombination
  decayfun->SetParName(3,"xDisp");      //displacement in x direction
  decayfun->SetParameter(0,0.01);
  decayfun->SetParameter(1,1);
  decayfun->SetParameter(2,5000);
  decayfun->SetParameter(3,0);
  decayfun->SetParLimits(0,0.01,1);
  decayfun->SetParLimits(1,0.9,1.3);
  decayfun->SetParLimits(2,1,20000);
  decayfun->SetParLimits(3,-2000,1000);

  TFitResultPtr ptr[nFiles];


  for (int32_t fil=0; fil<nFiles; fil++) {
    stringstream ss;
    ss << "data/deteriorating" << fileTitle[fil] << ".txt";
    cout << "Opening " << ss.str() << endl;
    ifstream f (ss.str().c_str());

    f >> column[0] >> column[1] >> column[2];
    cout << column[0] << " " << column[1] << " " << column[2] << endl;
    int32_t i=0;
    while (f.good()) {
      f >> run[fil][i] >> delay[fil][i] >> corr[fil][i];
      corrNorm[fil][i] = corr[fil][0] / corr[fil][i];
      //time to particles per seconds
      delay[fil][i] = delay[fil][i]*PARTICLESPERSEC;
      // cout << run[fil][i] << " " << delay[fil][i] << " " << corr[fil][i]
      //      << " " << corrNorm[fil][i] << endl;
      i++;
    }
    f.close();

    gr[fil] = new TGraph(i-2, &delay[fil][1], &corrNorm[fil][1]);
    dr->prettify(gr[fil]);
    gr[fil]->SetMarkerStyle(23+fil);
    gr[fil]->SetMarkerSize(1.2);

    //FIT the decay function
    leg->AddEntry(gr[fil],fileTitleLeg[fil].c_str(),"lep");

    // ptr[fil] = gr[fil]->Fit("decay", "S" ); //used to fit the TAU

    ss.str("");


    // ss << << fil;
    // ss.str("");
    // ss << "#tau_{corr} = ("
    //    << setprecision(3) << ptr[fil]->Parameter(2)
    //    << "#pm"
    //    << setprecision(2) << ptr[fil]->ParError(2)
    //    << ")"
    //    << " s^{-1}";
      //  cout << ss.str() << endl;
    // leg->AddEntry(gr[fil], ss.str().c_str(), "");    // if (!fil)
    mg->Add(gr[fil]);
  }
  // for (int32_t fil=0; fil<nFiles; fil++) {
  //   stringstream ss;
  //
  // }
  delete decayfun;
  TCanvas *c = new TCanvas ("c","c", 800,600);
  dr->prettify(c);
  // gPad->SetLogx();
  gPad->SetLogy();
  mg->Draw("AP");
  dr->prettify(mg);
  mg->GetXaxis()->SetRangeUser(0.9, 100000);
  // mg->GetXaxis()->SetLimits(0.9, 100000);
  mg->GetYaxis()->SetRangeUser(0.01,1.01);
  mg->GetXaxis()->SetTitle("Fluence #Phi [#alpha_{5.5 MeV}]");
  mg->GetYaxis()->SetTitle("#frac{#sigma(#Phi)}{#sigma(0)}");
  leg->Draw("same");


  c->Update();
  c->WaitPrimitive();
  c->Update();






  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
