#include "drawfuns.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TF1.h"
#include "TGraph.h"
#include "Tlegend.h"
#include <iostream>
#include <sstream>

using namespace std;

int32_t main() {
  TApplication* app = new TApplication ("app",0,0);
  DrawFuns* dr = new DrawFuns();
  TF1* fun[2];
  TGraph* gr[2];
  stringstream ss;
  for (int32_t i=0; i<2; i++) {
    ss.str("");
    ss << "Efwhm_"<<i;
    fun[i] = new TF1(ss.str().c_str(), "2.35*sqrt([0]*[1]*x)",1,5600);
    gr[i] = new TGraph(fun[i]);
  }

  TCanvas* can = new TCanvas("fwhm","fwhm",900,600);
  dr->prettify(can);

  //parameters for diamond
  fun[0]->SetParameter(0,13e-3);//Ei
  fun[0]->SetParameter(1,0.08);//fano factor
  //parameters for silicon
  fun[1]->SetParameter(0,3.6e-3);//Ei
  fun[1]->SetParameter(1,0.115);//fano factor
  // fun[1]->Draw("same");


  for (int32_t i=0; i<2; i++) {
    gr[i] = new TGraph(fun[i]);
  }
  dr->prettify(gr[0]);
  dr->prettify(gr[1]);
  gr[0]->GetXaxis()->SetTitle("Deposited energy [keV]");
  gr[0]->GetYaxis()->SetTitle("Energy resolution FWHM [keV]");
  gr[0]->GetXaxis()->SetRangeUser(20, 6000);
  gr[0]->GetXaxis()->SetLimits(20, 6000);
  gr[0]->GetYaxis()->SetRangeUser(0.2, 6);
  gr[0]->GetYaxis()->SetLimits(0.2, 6);
  gr[0]->Draw("AL");
  gr[1]->SetLineColor(kBlue+1);
  gr[1]->Draw("same");
  gr[0]->SetLineWidth(2);
  gr[1]->SetLineWidth(2);
  gPad->SetLogx();
  gPad->SetLogy();
  can->Update();


  //legend
  TLegend* leg = new TLegend(0.2,0.74,0.45,0.95);
  // leg->SetHeader("Energy resolution");
  // leg->SetTextFont(132);
  leg->AddEntry(gr[0],"Diamond","l");
  leg->AddEntry(gr[1],"Silicon","l");
  leg->Draw();
  // fun[0]->GetXaxis()->SetRangeUser(0,50000);

  can->Update();
  can->WaitPrimitive();
  delete(can);
  delete(dr);
  delete(app);
  cout<<"Done."<<endl;
  return 0;
}
