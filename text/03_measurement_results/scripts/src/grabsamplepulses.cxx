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

  cout<<"Grabbing pulse profiles for Sr, Am and Co and plotting them.."<<endl;


  stringstream ss;
  TFile* f;
  double samplingTime = 0.1; //ns
  const int nSamples = 1;
  const int nVoltages = 1;
  const int nTemp = 3;
  double voltage[nVoltages] = {-500.0};
  string sampleName[nSamples] = {"S37"};
  string sampleRealName[nSamples] = {"S37"};
  string tempS[nTemp] = {"Am241", "Sr90", "Co60"};
  string radiation[nTemp] = {"#alpha", "#beta", "#gamma"};
  double tempN[nTemp] = {295, 295, 295};

  // vector< vector< vector <TProfile> > > profvec;
  // vector< vector< vector <bool> > > missingprof;
  TProfile profvec[nSamples][nTemp][nVoltages];
  TH2F histvec[nSamples][nTemp][nVoltages];
  bool missingprof[nSamples][nTemp][nVoltages];

  int32_t cntFiles = 0;
  int32_t cntFilesOpen = 0;
  int32_t cntPulses = 0;


  //get the profiles from the file
  // profvec.resize(nSamples);
  // missingprof.resize(nSamples);
  for (int32_t sample = 0; sample < nSamples; sample++) {
    // profvec.at(sample).resize(nTemp);
    // missingprof.at(sample).resize(nTemp);
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        ss.str("");
        ss << "/Volumes/WINSTORAGE/Meas/Hysteresis/2015-08-28/samplePulses/S37/conv/";
        ss << sampleName[sample]
           << "_" << tempS[temp] << "_" << voltage[volt] << "V_0.root";
        TProfile prof;
        TH2F hist;
        cntFiles++;
        cout << "\r" << "Opening file " << ss.str() << flush;// << endl;
        f = new TFile(ss.str().c_str(),"read");
        if (!f->IsOpen()) {
          cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
          missingprof[sample][temp][volt] = true;
          // profvec prof;
          continue;
        }
        cntFilesOpen++;
        if (!(TProfile*)f->Get("avgpulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgpulses!" << endl;
          missingprof[sample][temp][volt] = true;
          // profvec[sample][temp][volt] = prof;
          continue;
        }
        if (!(TH2F*)f->Get("pulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include histogram pulses!" << endl;
          missingprof[sample][temp][volt] = true;
          // profvec[sample][temp][volt] = prof;
          continue;
        }
        cntPulses++;
      	prof = (*(TProfile*)f->Get("avgpulses"));
        // prof.Scale(96.62);
        prof.GetXaxis()->SetRangeUser(-5,9); //nanoseconds
        hist = (*(TH2F*)f->Get("pulses"));
        hist.GetXaxis()->SetRangeUser(-5,9);
        // hist.Scale(96.62);
        ss.str("");
        ss << sampleName[sample] << "_" << tempS[temp] << "_" << voltage[volt];
        prof.SetName(ss.str().c_str());
        ss << "_hist";
        hist.SetName(ss.str().c_str());
        profvec[sample][temp][volt] = prof;
        histvec[sample][temp][volt] = hist;
        // histvec[sample][temp][volt];//.Scale(96.62);
        missingprof[sample][temp][volt] = false;
        f->Close();
        delete(f);
      }
    }
  }
  cout<<endl<<endl<<endl;
  cout<<" Files requested: "<<cntFiles
      <<", failed to open: "<<cntFiles-cntFilesOpen
      <<", no pulse found: "<<cntFiles-cntPulses
      <<endl<<endl;




  // //----------------- Save the pulses -------------------------------
  // TDirectory* dirp;
  // TDirectory* dirs;
  // TDirectory* dirt;
  // stringstream sst;
  //
  // //------------ open file for saving plots --------------------------
  // sst.str("");
  // sst << "plots/pulses.root";
  // TFile* filePlots = new TFile(sst.str().c_str(),"recreate");
  // cout<<" Saving in file "<<sst.str()<<endl;
  //
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //   sst.str("");
  //   sst << "Sample_" << sampleName[sample];
  //   dirs = filePlots->mkdir(sst.str().c_str());
  //   dirs->cd();
  //   // filePlots->cd(sst.str().c_str());
  //   for (int32_t temp = 0; temp < nTemp; temp++) {
  //     for (int32_t volt = 0; volt < nVoltages; volt++) {
  //       if (missingprof[sample][temp][volt]) {
  //         continue;
  //       }
  //       profvec[sample][temp][volt].Write();
  //     }
  //   }
  // }
  // filePlots->Close();
  // delete(filePlots);



  // ------------------------------------------------------------------
  //                      PLOTTING
  // ------------------------------------------------------------------
  stringstream sssEH, sssEHsam, sss;

  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  TCanvas* can[nTemp][nVoltages];
  TLegend* leg[nTemp][nTemp][nVoltages];
  TLatex* tex1[nSamples][nTemp];
  TLatex* texEH1[nSamples][nTemp];
  TLatex* texEH1sam[nSamples][nTemp];
  int32_t clrCnt = 0;
  TGraph* gr[nSamples][nTemp][nVoltages];

  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      clrCnt = 0;
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        if (missingprof[sample][temp][volt])
          continue;
        ss.str("");
        ss << sampleName[sample] << "_" << tempS[temp] << "_" << volt;
        cout << ss.str() <<endl;
        gr[sample][temp][volt] = new TGraph();
        gr[sample][temp][volt]->SetName(ss.str().c_str());
        for (int32_t i=0; i<profvec[sample][temp][volt].GetNbinsX(); i++) {
          gr[sample][temp][volt]->SetPoint(
            i, (-500 + i)*0.1 , profvec[sample][temp][volt].GetBinContent(i) );
        }
        dr->prettify(&histvec[sample][temp][volt]);
        gr[sample][temp][volt]->SetLineColor(kBlack);
        gr[sample][temp][volt]->SetLineWidth(4);
        leg[sample][temp][volt] = new TLegend (0.66,0.75,0.95,0.95);
        ss.str("");
        ss << radiation[temp] << " pulses";
        leg[sample][temp][volt]->AddEntry (&histvec[sample][temp][volt],ss.str().c_str(),"");
        ss.str("");
        ss << "Averaged pulse";
        leg[sample][temp][volt]->AddEntry (gr[sample][temp][volt],ss.str().c_str(), "L");

        clrCnt++;
        // leg[temp][volt]->AddEntry(&histvec[sample][temp][volt], ss.str().c_str(),  "L");
      }
      // sss.str("");
      // sssEH.str("");
      // sssEHsam.str("");
      // sss << sampleName[sample] << "_";
      // sssEHsam << "#splitline{#splitline{" << tempName[temp] << "}{"
      //          << "U_{bias}=" << voltage[polarity[sample]] << "V}}{"
      //          << sampleNameState[sample] << "}";
      // if (polarity[sample])
      //   sssEH << "electrons"; //500V
      // else
      //   sssEH << "holes";
      // sss << tempS[temp];
      // tex1[sample][temp] = new TLatex(-1.5,-1.3,"MEASUREMENT");
      // tex1[sample][temp]->SetTextFont(132);
      // texEH1[sample][temp] = new TLatex(-1.5,10.5,sssEH.str().c_str());
      // texEH1[sample][temp]->SetTextFont(22);
      // texEH1sam[sample][temp] = new TLatex(10,1.7,sssEHsam.str().c_str());
      // texEH1sam[sample][temp]->SetTextFont(22);
    }
  }

  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t temp=0; temp<nTemp; temp++) {
      for (int32_t volt=0; volt<nVoltages; volt++) {
        sss.str("");
        sss << "can_" << sample << "_" << temp << "_" << volt;
        can[temp][volt] = new TCanvas(sss.str().c_str(),sss.str().c_str(),800,600);
        can[temp][volt]->cd();
        dr->prettify(can[temp][volt]);
        histvec[sample][temp][volt].Draw("col");
        histvec[sample][temp][volt].GetXaxis()->SetTitle("Time [ns]");
        histvec[sample][temp][volt].GetXaxis()->SetTitleFont(42);
        histvec[sample][temp][volt].GetYaxis()->SetTitleFont(42);
        histvec[sample][temp][volt].GetYaxis()->SetTitle("Voltage [V]");
        histvec[sample][temp][volt].GetYaxis()->SetRangeUser(-0.02,0.10); //[V]
        // histvec[sample][temp][volt].GetYaxis()->SetLimits(-0.02,0.10); //[V](-0.02*96.62,0.10*96.62); //[V]
        histvec[sample][temp][volt].GetXaxis()->SetRangeUser(-2,13); //[s]

        gr[sample][temp][volt]->GetYaxis()->SetRangeUser(-0.02,0.10); //[V]
        // gr[sample][temp][volt]->Scale(96.62);
        gr[sample][temp][volt]->Draw("same");


        leg[sample][temp][volt]->Draw("same");
        // leg[temp][volt]->Draw("same");
        // tex1[sample][temp]->Draw("same");
        // texEH1[sample][temp]->Draw("same");
        // texEH1sam[sample][temp]->Draw("same");
        can[temp][volt]->Update();
        can[temp][volt]->WaitPrimitive();
        ss.str("");
        ss << "plots/samplePulses/" << sampleName[sample] << "_" << tempS[temp] << "_" << volt << ".pdf";
        // can[sample]->SaveAs(ss.str().c_str());
        can[temp][volt]->Update();
      }
    }
  }


  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
