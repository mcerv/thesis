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
#include "TDatime.h"
#include "THStack.h"




using namespace std;

void yearlyPlot(){

}


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<"PLOTTING PRODUCTION WITH TIME"<<endl;


  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();

  TGraph *gr[3];
  TMultiGraph *mg = new TMultiGraph();
  string month[20];
  int32_t C[20];
  int32_t C1[20]; //good
  int32_t Si[20];
  int32_t IBL[20];

  // TLatex *tex[10];

  // TLegend *leg = new TLegend (0.64,0.4,0.96,0.8);

  stringstream ss;
  ss << "../data/production.txt";
  cout << "Opening " << ss.str() << endl;
  ifstream f (ss.str().c_str());


  f >> month[0] >> month[1] >> month[2]
    >> month[3] >> month[4] >> month[5]
    >> month[6] >> month[7] >> month[8];
  f >> IBL[0] >> IBL[1] >> IBL[2]
    >> IBL[3] >> IBL[4] >> IBL[5]
    >> IBL[6] >> IBL[7] >> IBL[8];
  f >> Si[0] >> Si[1] >> Si[2]
    >> Si[3] >> Si[4] >> Si[5]
    >> Si[6] >> Si[7] >> Si[8];
  f >> C[0] >> C[1] >> C[2]
    >> C[3] >> C[4] >> C[5]
    >> C[6] >> C[7] >> C[8];
  f >> C1[0] >> C1[1] >> C1[2]
    >> C1[3] >> C1[4] >> C1[5]
    >> C1[6] >> C1[7] >> C1[8];
  f.close();

  for (int32_t i=0; i<9; i++) {
    cout << month[i] << " : "
         << " ibl " << IBL[i]
         << " Si  "  << Si[i]
         << " C  "   << C[i]
         << " C1  "   << C1[i]
         << endl;
  }

  Long64_t year = 2013;
  const Int_t n = 9;
  // string months[n] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  int X0 = TDatime(1995,01,01,00,00,00).Convert();
  int startDate = TDatime(year, 02, 01, 00, 00, 00).Convert() - X0;
  int endDate = TDatime(year, 10, 31, 23, 59, 59).Convert() - X0;
  int timeData[n]={0};
  int yData[n]={0};
  TCanvas *canvas = new TCanvas("canvas","Yearly Plot",800,600);
  dr->prettify(canvas);

  for (Long64_t j=0; j < n; j++){
          timeData[j] = TDatime(year, j+2, 15, 00, 00, 00).Convert() - X0;
  }



  TH1F *h = new TH1F("h","test",n,startDate,endDate);
  dr->prettify(h);
  h->SetLineColor(kBlack);
  h->SetLineWidth(1);
  h->GetXaxis()->SetNdivisions(-n);
  for (Long64_t j=0;j<n;j++) h->Fill(month[j].c_str(),0);
  h->SetMinimum(0);
  h->SetMaximum(91);
  h->Draw();

  TGraph *gr1 = new TGraph(n,timeData,IBL);
  TGraph *gr2 = new TGraph(n,timeData,Si);
  TGraph *gr3 = new TGraph(n,timeData,C);
  TGraph *gr4 = new TGraph(n,timeData,C1);
  dr->prettify(gr1);
  dr->prettify(gr2);
  dr->prettify(gr3);
  dr->prettify(gr4);
  gr1->SetMarkerStyle(20);
  gr2->SetMarkerStyle(21);
  gr3->SetMarkerStyle(22);
  gr4->SetMarkerStyle(23);
  // gr1->SetMarkerSize(1.4);
  // gr2->SetMarkerSize(1.4);
  // gr3->SetMarkerSize(1.4);
  // gr4->SetMarkerSize(1.4);
  // gr1->Draw("PL");
  // gr2->Draw("PL");
  // gr3->Draw("PL");
  // gr4->Draw("PL");

  TH1I* h1 = new TH1I("h1","h1",n,startDate, endDate);
  TH1I* h2 = new TH1I("h2","h2",n,startDate, endDate);
  TH1I* h3 = new TH1I("h3","h3",n,startDate, endDate);
  TH1I* h4 = new TH1I("h4","h4",n,startDate, endDate);
  h1->GetXaxis()->SetNdivisions(-n);
  h2->GetXaxis()->SetNdivisions(-n);
  h3->GetXaxis()->SetNdivisions(-n);
  h4->GetXaxis()->SetNdivisions(-n);
  dr->prettify(h1,"green");
  dr->prettify(h2,"blue");
  dr->prettify(h3,"red");
  dr->prettify(h4,"green");
  for (int i=0; i<n; i++) {
    h1->Fill(month[i].c_str(), IBL[i]);
    h2->Fill(month[i].c_str(), Si[i]);
    h3->Fill(month[i].c_str(), C[i]);
    h4->Fill(month[i].c_str(), C1[i]);
  }
  THStack *hs = new THStack("hs","");
  hs->Add(h1);
  hs->Add(h2);
  hs->Add(h3);
  // hs->Add(h4);
  // h1->GetYaxis()->SetTitle("Produced units");
  // h1->GetXaxis()->SetTitle("Year 2013");
  // h2->GetYaxis()->SetTitle("Produced units");
  // h2->GetXaxis()->SetTitle("Year 2013");
  // h3->GetYaxis()->SetTitle("Produced units");
  // h3->GetXaxis()->SetTitle("Year 2013");
  // h4->GetYaxis()->SetTitle("Produced units");
  // h4->GetXaxis()->SetTitle("Year 2013");
  // hs->GetHistogram()->GetXaxis()->SetTitle("Year 2013");
  // hs->GetHistogram()->GetYaxis()->SetTitle("Produced units");
  h->GetXaxis()->SetTitle("Year 2013");
  h->GetYaxis()->SetTitle("Produced units");
  hs->Draw("b same");
  h1->SetBarWidth(0.8);
  h2->SetBarWidth(0.8);
  h3->SetBarWidth(0.8);
  h4->SetBarWidth(0.8);


  TLegend *leg = new TLegend(0.29,0.73,0.60,0.87);
  // leg->AddEntry(gr1, "IBL, 12 units", "P");
  // leg->AddEntry(gr2, "Si, 24 units", "P");
  // leg->AddEntry(gr3, "C, 43 units", "P");
  // leg->AddEntry(gr4, "C accepted, 20 units", "P");
  leg->AddEntry(h3, "pCVD, 43 units", "F");
  leg->AddEntry(h2, "Si, 24 units", "F");
  leg->AddEntry(h1, "IBL, 12 units", "F");
  // leg->AddEntry(h4, "C accepted, 20 units", "P");

  leg->Draw("same");
  // hs->GetYaxis()->SetRangeUser(0,45);
  canvas->Update();
  canvas->WaitPrimitive();
  canvas->Update();


//  yearlyPlot();
  // gr = new TGraph(8, year, area);


  // ss.str("");
  // TCanvas *c1 = new TCanvas ("c1","c1", 800,600);
  // gr[proc]->Draw("AP");
  // c1->Update();
  // c1->WaitPrimitive();

  // ss << << proc;
  // leg->AddEntry(gr[proc],procedure[proc].c_str(),"lep");
  // mg->Add(gr[proc]);

  //
  // TCanvas *c = new TCanvas ("c","c", 800,600);
  // dr->prettify(c);
  // gPad->SetLogy();
  // gr->Draw("AP");
  // dr->prettify(gr);
  // gr->SetMarkerStyle(21);
  // gr->SetMarkerSize(1.5);
  // gr->GetYaxis()->SetRangeUser(0.9, 400);
  // // mg->GetXaxis()->SetLimits(0.9, 100000);
  // gr->GetXaxis()->SetRangeUser(2000, 2019);
  // gr->GetXaxis()->SetLimits(2000, 2019);
  // gr->GetXaxis()->SetTitle("Year");
  // gr->GetYaxis()->SetTitle("Active area [cm^{2}]");
  //
  // // el1->Draw("same");
  //
  // for (int32_t a=0; a<i; a++) {
  //   tex[a] = new TLatex (year[a],area[a]*1.1,detName[a].c_str());
  //   tex[a]->Draw("same");
  // }
  //
  // // leg->Draw("same");
  //
  //
  // c->Update();
  // c->WaitPrimitive();
  // c->Update();






  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
