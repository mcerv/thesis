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
#include "TLine.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLatex.h"
#include "TTree.h"
#include "fstream"
using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {

  cout<<"Grabbing distributions for Sr, Am and Co and plotting them.."<<endl;









  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();



  // TCanvas *can1 = new TCanvas("can1","can1",800,600);
  // dr->prettify(can1);
  TH1D *h1 = new TH1D("h1","Collected charge for Co60 source",256,0,255);
  TH1D *h2 = new TH1D("h2","Collected charge for Am241 source ", 100, 0, 1.0);
  TH1D *h3 = new TH1D("h3","Collected charge for SR90 source",120,0,0.12);


  TFile *f3 = new TFile ("/Volumes/MACSTORAGE/TCT/Singapore/2014-06-24/results/1scdhq/SR90_Cx/1scdhq_SR90_Cx_460V.root");
  TTree *tree3 = (TTree *)f3->Get("ntuple");
  tree3->SetMakeClass(1);
  Float_t amplitude3;
  TBranch *amplitudeBranch3 = new TBranch();
  tree3->SetBranchAddress("ampl", &amplitude3, &amplitudeBranch3);
  Long64_t nentries3 = tree3->GetEntries();
  for (Long64_t i=0;i<nentries3;i++) {
      amplitudeBranch3->GetEntry(i);
      h3->Fill(amplitude3);
  }


  TFile *f2 = new TFile ("/Volumes/MACSTORAGE/TCT/Singapore/2014-06-24/results/1scdhq/AM241_Cx/1scdhq_AM241_Cx_500V.root");
  TTree *tree2 = (TTree *)f2->Get("ntuple");
  tree2->SetMakeClass(1);
  Float_t amplitude2;
  TBranch *amplitudeBranch2 = new TBranch();
  tree2->SetBranchAddress("ampl", &amplitude2, &amplitudeBranch2);
  Long64_t nentries2 = tree2->GetEntries();
  for (Long64_t i=0;i<nentries2;i++) {
      amplitudeBranch2->GetEntry(i);
      h2->Fill(amplitude2);
  }




  //cobalt
  double co[256];
  double num;

  ifstream f ("data/Co60ampl2.txt");
  for (int32_t i=0; i<256; i++) {
    f >> num >> co[i];
    // cout << num << " " << co[i] << endl;
    h1->SetBinContent(i,co[i]);
  }
  f.close();





  TCanvas *can1 = new TCanvas("can1","can1",800,600);
  dr->prettify(can1);
  dr->prettify(h1, "blue");
  dr->normalise(h1);
  h1->GetXaxis()->SetTitle("Pulse amplitude [V]");
  h1->GetYaxis()->SetTitle("Normalised entries");
  h1->GetYaxis()->SetTitleSize(0.07);
  h1->GetXaxis()->SetTitleSize(0.07);
  h1->GetYaxis()->SetTitleOffset(0.9);
  // h1->GetXaxis()->SetTitleOffset(0.07);
  h1->GetXaxis()->SetLimits(0.0,1.0);
  h1->GetXaxis()->SetRangeUser(0.0,0.2);
  // h1->SetLineColor(kBlack);
  // h1->SetLineWidth(1);
  h1->Draw();
  TLegend *leg1 = new TLegend(0.65,0.85,0.95,0.95,NULL,"brNDC");
  leg1->AddEntry(h1,"#gamma distribution","");
  leg1->Draw("same");
  can1->Update();
  can1->WaitPrimitive();
  can1->Update();


  // h3->Draw();

  TCanvas *can3 = new TCanvas("can3","can3",800,600);
  dr->prettify(can3);
  dr->prettify(h3,"blue");
  dr->normalise(h3);
  h3->GetXaxis()->SetTitle("Pulse amplitude [V]");
  h3->GetYaxis()->SetTitle("Normalised entries");
  h3->GetYaxis()->SetTitleSize(0.07);
  h3->GetXaxis()->SetTitleSize(0.07);
  h3->GetYaxis()->SetTitleOffset(0.9);
  // h3->GetXaxis()->SetRangeUser(0,0.09);
  // h3->SetLineColor(kBlack);
  // h3->SetLineWidth(1);
  h3->Draw();
  h3->Rebin(2);
  TLegend *leg3 = new TLegend(0.65,0.85,0.95,0.95,NULL,"brNDC");
  leg3->AddEntry(h3,"#beta distribution","");
  leg3->Draw("same");
  can3->Update();
  can3->WaitPrimitive();
  can3->Update();

  TCanvas *can2 = new TCanvas("can2","can2",800,600);
  dr->prettify(can2);
  dr->prettify(h2, "blue");
  dr->normalise(h2);
  h2->GetXaxis()->SetTitle("Pulse amplitude [V]");
  h2->GetYaxis()->SetTitle("Normalised entries");
  h2->GetXaxis()->SetRangeUser(0.2,0.7);
  h2->GetYaxis()->SetTitleSize(0.07);
  h2->GetXaxis()->SetTitleSize(0.07);
  h2->GetYaxis()->SetTitleOffset(0.9);
  // h2->SetLineColor(kBlack);
  // h2->SetLineWidth(1);
  h2->Draw();
  TLegend *leg2 = new TLegend(0.65,0.85,0.95,0.95,NULL,"brNDC");
  leg2->AddEntry(h3,"#alpha distribution","");
  leg2->Draw("same");
  // leg3->Draw("same");
  can2->Update();
  can2->WaitPrimitive();
  can2->Update();


  // leg->AddEntry(h2,"#beta energy","L");
  // leg->Draw("same");


  // TPad *p1 = new TPad("p1","p1",0.1,0.1,0.501,0.901);
  // p1->SetRightMargin(0.);
  // p1->SetBorderMode(0);
  // p1->Draw();
  // TPad *p2 = new TPad("p1","p1",0.5,0.1,0.901,0.901);
  // p2->SetLeftMargin(0.);
  // p2->SetBorderMode(0);
  // p2->Draw();
  // p1->cd();
  // h3->Draw();
  // p2->cd();
  // h2->Draw();


  //
  // // ------------------------------------------------------------------
  // //                      PLOTTING
  // // ------------------------------------------------------------------
  // stringstream sssEH, sssEHsam, sss;
  //
  // TApplication* app = new TApplication("app",0,0);
  // DrawFuns* dr = new DrawFuns();
  // TCanvas* can[nTemp][nVoltages];
  // TLegend* leg[nTemp][nTemp][nVoltages];
  // TLatex* tex1[nSamples][nTemp];
  // TLatex* texEH1[nSamples][nTemp];
  // TLatex* texEH1sam[nSamples][nTemp];
  // int32_t clrCnt = 0;
  // TGraph* gr[nSamples][nTemp][nVoltages];
  //
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //   for (int32_t temp = 0; temp < nTemp; temp++) {
  //     clrCnt = 0;
  //     for (int32_t volt = 0; volt < nVoltages; volt++) {
  //       if (missingprof[sample][temp][volt])
  //         continue;
  //       ss.str("");
  //       ss << sampleName[sample] << "_" << tempS[temp] << "_" << volt;
  //       cout << ss.str() <<endl;
  //       gr[sample][temp][volt] = new TGraph();
  //       gr[sample][temp][volt]->SetName(ss.str().c_str());
  //       for (int32_t i=0; i<profvec[sample][temp][volt].GetNbinsX(); i++) {
  //         gr[sample][temp][volt]->SetPoint(
  //           i, (-500 + i)*0.1 , profvec[sample][temp][volt].GetBinContent(i) );
  //       }
  //       dr->prettify(&histvec[sample][temp][volt]);
  //       gr[sample][temp][volt]->SetLineColor(kBlack);
  //       gr[sample][temp][volt]->SetLineWidth(3);
  //       leg[sample][temp][volt] = new TLegend (0.66,0.75,0.95,0.95);
  //       ss.str("");
  //       ss << radiation[temp] << " pulses";
  //       leg[sample][temp][volt]->AddEntry (&histvec[sample][temp][volt],ss.str().c_str(),"");
  //       ss.str("");
  //       ss << "Averaged pulse";
  //       leg[sample][temp][volt]->AddEntry (gr[sample][temp][volt],ss.str().c_str(), "L");
  //
  //       clrCnt++;
  //       // leg[temp][volt]->AddEntry(&histvec[sample][temp][volt], ss.str().c_str(),  "L");
  //     }
  //     // sss.str("");
  //     // sssEH.str("");
  //     // sssEHsam.str("");
  //     // sss << sampleName[sample] << "_";
  //     // sssEHsam << "#splitline{#splitline{" << tempName[temp] << "}{"
  //     //          << "U_{bias}=" << voltage[polarity[sample]] << "V}}{"
  //     //          << sampleNameState[sample] << "}";
  //     // if (polarity[sample])
  //     //   sssEH << "electrons"; //500V
  //     // else
  //     //   sssEH << "holes";
  //     // sss << tempS[temp];
  //     // tex1[sample][temp] = new TLatex(-1.5,-1.3,"MEASUREMENT");
  //     // tex1[sample][temp]->SetTextFont(132);
  //     // texEH1[sample][temp] = new TLatex(-1.5,10.5,sssEH.str().c_str());
  //     // texEH1[sample][temp]->SetTextFont(22);
  //     // texEH1sam[sample][temp] = new TLatex(10,1.7,sssEHsam.str().c_str());
  //     // texEH1sam[sample][temp]->SetTextFont(22);
  //   }
  // }
  //
  // for (int32_t sample=0; sample<nSamples; sample++) {
  //   for (int32_t temp=0; temp<nTemp; temp++) {
  //     for (int32_t volt=0; volt<nVoltages; volt++) {
  //       sss.str("");
  //       sss << "can_" << sample << "_" << temp << "_" << volt;
  //       can[temp][volt] = new TCanvas(sss.str().c_str(),sss.str().c_str(),800,600);
  //       can[temp][volt]->cd();
  //       dr->prettify(can[temp][volt]);
  //       histvec[sample][temp][volt].Draw("col");
  //       histvec[sample][temp][volt].GetXaxis()->SetTitle("Time [ns]");
  //       histvec[sample][temp][volt].GetYaxis()->SetTitle("Voltage [V]");
  //       histvec[sample][temp][volt].GetYaxis()->SetRangeUser(-0.02,0.10); //[V]
  //       // histvec[sample][temp][volt].GetYaxis()->SetLimits(-0.02,0.10); //[V](-0.02*96.62,0.10*96.62); //[V]
  //       histvec[sample][temp][volt].GetXaxis()->SetRangeUser(-2,13); //[s]
  //
  //       gr[sample][temp][volt]->GetYaxis()->SetRangeUser(-0.02,0.10); //[V]
  //       // gr[sample][temp][volt]->Scale(96.62);
  //       gr[sample][temp][volt]->Draw("same");
  //
  //
  //       leg[sample][temp][volt]->Draw("same");
  //       // leg[temp][volt]->Draw("same");
  //       // tex1[sample][temp]->Draw("same");
  //       // texEH1[sample][temp]->Draw("same");
  //       // texEH1sam[sample][temp]->Draw("same");
  //       can[temp][volt]->Update();
  //       can[temp][volt]->WaitPrimitive();
  //       ss.str("");
  //       ss << "plots/samplePulses/" << sampleName[sample] << "_" << tempS[temp] << "_" << volt << ".pdf";
  //       // can[sample]->SaveAs(ss.str().c_str());
  //       can[temp][volt]->Update();
  //     }
  //   }
  // }


  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
