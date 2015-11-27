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
#include "TEllipse.h"





using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<"PLOTTING DETECTOR AREA WRT YEARS"<<endl;


  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();

  TGraph *gr;
  string column[3];
  string detName[10];
  double year[10];
  double area[10];
  TLatex *tex[10];

  // TLegend *leg = new TLegend (0.64,0.4,0.96,0.8);

  stringstream ss;
  ss << "../data/detArea.txt";
  cout << "Opening " << ss.str() << endl;
  ifstream f (ss.str().c_str());

  f >> column[0] >> column[1] >> column[2];
  cout << column[0] << " " << column[1] << " " << column[2] << endl;
  int32_t i=0;
  while (f.good()) {
    f >> detName[i] >> year[i] >> area[i];
    cout << detName[i] << " " << year[i] << " " << area[i] << endl;
    i++;
  }
  f.close();

  gr = new TGraph(i-1, year, area);


  // ss.str("");
  // TCanvas *c1 = new TCanvas ("c1","c1", 800,600);
  // gr[proc]->Draw("AP");
  // c1->Update();
  // c1->WaitPrimitive();

  // ss << << proc;
  // leg->AddEntry(gr[proc],procedure[proc].c_str(),"lep");
  // mg->Add(gr[proc]);


  TCanvas *c = new TCanvas ("c","c", 800,600);
  dr->prettify(c);
  gPad->SetLogy();
  gr->Draw("AP");
  dr->prettify(gr);
  gr->SetMarkerStyle(21);
  gr->SetMarkerSize(1.5);
  gr->GetYaxis()->SetRangeUser(0.9, 400);
  // mg->GetXaxis()->SetLimits(0.9, 100000);
  gr->GetXaxis()->SetRangeUser(2000, 2019);
  gr->GetXaxis()->SetLimits(2000, 2019);
  gr->GetXaxis()->SetTitle("Year");
  gr->GetYaxis()->SetTitle("Active area [cm^{2}]");

  // el1->Draw("same");

  for (int32_t a=0; a<i; a++) {
    tex[a] = new TLatex (year[a],area[a]*1.1,detName[a].c_str());
    tex[a]->Draw("same");
  }

  // leg->Draw("same");


  c->Update();
  c->WaitPrimitive();
  c->Update();






  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
