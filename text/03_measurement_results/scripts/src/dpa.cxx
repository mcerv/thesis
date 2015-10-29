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
#include <fstream>

using namespace std;

//---fit function------
//---------------------

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  DrawFuns* dr = new DrawFuns();
  TApplication* app = new TApplication("app",0,0);

  TGraph *gr[3];
  TMultiGraph *mg = new TMultiGraph();
  string fileName[3];
  fileName[0] = "data/DPAinDiamondP.csv";
  fileName[1] = "data/DPAinDiamondN.csv";
  fileName[2] = "data/DPAinDiamondPi.csv";
  string radType[3] = {"DPA, p","DPA, n","DPA, #pi"};
  TLegend* leg = new TLegend(0.6,0.5,0.95,0.95);
  // cout<<"Filename "<<fileName<<endl;

  for (int32_t i=0; i<3; i++) {
    gr[i] = new TGraph(fileName[i].c_str());
    dr->prettify(gr[i]);
    gr[i]->SetLineStyle(2*i+i);
    gr[i]->SetLineWidth(2);
    leg->AddEntry(gr[i],radType[i].c_str(),"l");
    mg->Add(gr[i]);
  }




  TCanvas* c2 = new TCanvas("together","together",800,600);
  dr->prettify(c2);
  gPad->SetLogx();
  gPad->SetLogy();
  mg->Draw("AL");
  dr->prettify(mg);
  mg->GetXaxis()->SetTitle("Energy [GeV]");
  mg->GetYaxis()->SetTitle("Displacements per atom");
  mg->GetYaxis()->SetRangeUser(5e-23,2.5e-21);
  // leg->AddEntry(lin,"Linearised PSI operating range","L");

  TLatex *tex1 = new TLatex(0.0007,6e-23,"SIMULATION");
  tex1->SetTextFont(132);
  tex1->Draw("same");


  TFitResultPtr ptr;
  TF1* lin = new TF1("pol1","pol1");
  // ptr = gr[2]->Fit(lin,"SRQ","",0.2,0.4);
  //dpa at 300, 330, 270 MeV
  // double n = ptr->Parameter(0);
  // double k = ptr->Parameter(1);
  // //y = k*x + n;
  // double y300, y330, y270;
  // y300 = k*0.3+n;
  // y330 = k*0.33+n;
  // y270 = k*0.27+n;
  // cout<<" k = "<<k<<"  n = "<<n<<endl;
  // cout << "DPA at 300 = "<<y300 <<endl
  //  << "DPA at 330 = "<<y330<< " , " << (y330-y300)/y300<<"\% of y300" <<endl
  //   << "DPA at 270 = "<<y270<< " , " << (y270-y300)/y300<<"\% of y300"
  //   <<endl<<flush;
  //
  // stringstream sstex;
  // // sstex << "DPA (300#pm 10\% MeV) = "
  // sstex << "DPA(PSI) = "
  //       << "(1.25#pm0.09)#times10 ^{-22}";
  // TLatex *tex2 = new TLatex(0.12,2e-22,sstex.str().c_str());
  // tex2->SetTextFont(132);
  // tex2->Draw("same");



  TGraphErrors *rd42 = new TGraphErrors();
  TGraphErrors *mypi = new TGraphErrors();
  TGraphErrors *rd42pi = new TGraphErrors();
  double p24 = 8.3e-23;
  //4.3 pions
  rd42->SetPoint(0, 24,p24);
  rd42->SetPointError(0, 24*0.01,p24*0.01);
  rd42->SetPoint(1, 0.800,p24*1.7);
  rd42->SetPointError(1,  0.800*0.01, p24*0.1);
  rd42->SetPoint(2, 0.070, p24*2.65);
  rd42->SetPointError(2, 0.070*0.01, p24*0.15);
  rd42->SetPoint(3, 0.025, p24*4.5);
  rd42->SetPointError(3,  0.025*0.01, p24*0.5);

  rd42pi->SetPoint(0, 0.2,p24*2.75);
  rd42pi->SetPointError(0, 0.2*0.01,p24*0.25);

  mypi->SetPoint(0, 0.2,p24*4.3);
  mypi->SetPointError(0, 0.2*0.2,p24*1.2);

  dr->prettify(rd42);
  dr->prettify(rd42pi);
  dr->prettify(mypi);

  rd42->SetMarkerStyle(22);
  rd42pi->SetMarkerStyle(20);
  mypi->SetMarkerStyle(21);

  rd42->SetMarkerSize(1.5);
  rd42pi->SetMarkerSize(1.5);
  mypi->SetMarkerSize(1.5);


  rd42->Draw("P SAME");
  rd42pi->Draw("P SAME");
  mypi->Draw("P SAME");


  leg->AddEntry(rd42, "RD42, p","lep");
  leg->AddEntry(rd42pi, "RD42, #pi","lep");
  leg->AddEntry(mypi, "Measured, #pi","lep");
  leg->Draw("same");

  c2->Update();
  c2->WaitPrimitive();
  c2->Update();





  delete(dr);
  delete(app);

  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
