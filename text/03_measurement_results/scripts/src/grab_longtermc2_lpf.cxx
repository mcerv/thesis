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
  cout<<"Grabbing pulse profiles and plotting a longterm C2 decrease.."<<endl;

  stringstream ss, sslpf;
  TFile* f;
  double samplingTime = 0.1; //ns
  const int nSamples = 12;
  const int nVoltages = 2;
  const int nTemp = 1;

  double voltage[nVoltages] = {-500.0, 500.0};
  int32_t polarity[nSamples] = {0, 0, 1, 1, //0-holes, 1-electrons
                                0, 0, 1, 1,
                                0, 0, 1, 1};

  string sampleName[nSamples] = {"S52", "S52after", "S52e", "S52eafter",
                                 "S79", "S79after", "S79e", "S79eafter",
                                 "S37", "S37after", "S37e", "S37eafter"};

  string sampleNameFull[nSamples] = {
    "S52 3.6e14 #pi cm^{-2}", "S52 3.6e14 #pi cm^{-2}", "S52 3.6e14 #pi cm^{-2}", "S52 3.6e14 #pi cm^{-2}",
    "S79 1e14 #pi cm^{-2}",   "S79 1e14 #pi cm^{-2}",   "S79 1e14 #pi cm^{-2}",   "S79 1e14 #pi cm^{-2}",
    "S37",                    "S37",                    "S37",                    "S37"   };

  string sampleNameState[nSamples] = {
    "not pumped","pumped","not pumped", "pumped",
    "not pumped","pumped","not pumped", "pumped",
    "not pumped","pumped","not pumped", "pumped"  };

  // int32_t nFilesIndividual[nSamples] = { 44, 137,  82, 133,
  //                                       150, 454, 116, 440,
  //                                       557, 635, 565, 623 };
  // int32_t numSkip[nSamples] = { 4, 14,  8, 13,
  //                              15, 45, 11, 44,
  //                              55, 63, 56, 62};

  int32_t nFilesIndividual[nSamples] = { 44, 137,  82, 133,
                                        150, 450, 116, 440,
                                        550, 550, 550, 550 };
  int32_t numSkip[nSamples] = { 4, 14,  8, 13,
                               15, 45, 11, 44,
                               55, 55, 55, 55};

  string tempS[nTemp] = {"1"};
  double tempN[nTemp] = {1};

  int32_t cntFiles = 0;
  int32_t cntFilesOpen = 0;
  int32_t cntPulses = 0;
  const int32_t nFiles = 550;
  // int32_t nFilesIndividual = nFiles;

  TProfile profvec[nSamples][nTemp][nFiles];
  bool missingprof[nSamples][nTemp][nFiles];
  int32_t avgTimestamp[nSamples][nTemp][nFiles];


  //get the profiles and respective timestamps from the file
  for (int32_t sample = 5; sample < 6; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) { // only one  - "1"
      for (int32_t num = 0; num < nFilesIndividual[sample]; num++) {
        ss.str("");
        ss << "/Volumes/WINSTORAGE/Meas/Hysteresis/2015-08-06/longtermC2/"
           << sampleName[sample]
           << "/conv/";
        ss << sampleName[sample]
           << "_1_" << voltage[polarity[sample]]
           << "V_" << num << ".root";
        TProfile prof;
        cntFiles++;
        cout << "\r" << "Opening file " << ss.str() << flush;// << endl;
        f = new TFile(ss.str().c_str(),"read");
        if (!f->IsOpen()) {
          cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
          missingprof[sample][temp][num] = true;
          // profvec prof;
          continue;
        }
        cntFilesOpen++;
        // if (!(TProfile*)f->Get("avgpulses_filt")) {
        if (!(TProfile*)f->Get("avgpulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgpulses!" << endl;
          missingprof[sample][temp][num] = true;
          continue;
        }
        if (!((TParameter<Int_t>*)f->Get("avgTimestamp")) ) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgTimestamp!" << endl;
          missingprof[sample][temp][num] = true;
          continue;
        }
        avgTimestamp[sample][temp][num] = (((TParameter<Int_t>*)f->Get("avgTimestamp"))->GetVal());

        cntPulses++;
      	prof = (*(TProfile*)f->Get("avgpulses"));
        prof.Scale(131.7);
        prof.GetXaxis()->SetRangeUser(-5,20); //nanoseconds
        ss.str("");
        ss << sampleName[sample] << "_" << tempS[temp] << "_" << num;
        prof.SetName(ss.str().c_str());
        profvec[sample][temp][num] = prof;
        missingprof[sample][temp][num] = false;
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


  // ------------------------------------------------------------------
  //                      PLOTTING
  // ------------------------------------------------------------------
  stringstream sssEH, sssEHsam, sss;

  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  TCanvas* can[nSamples];
  TMultiGraph* mg[nSamples];
  TLegend* leg[nSamples];
  TLatex* tex1[nSamples][nTemp];
  TLatex* texEH1[nSamples][nTemp];
  TLatex* texEH1sam[nSamples][nTemp];
  int32_t clrCnt = 0;
  // Change to graphs
  TGraph* gr[nSamples][nTemp][nFiles];
  TGraph* grLPF[nSamples][nTemp][nFiles]; //low pass FILTER

  for (int32_t sample = 5; sample < 6; sample++) {

    mg[sample] = new TMultiGraph();
    leg[sample] = new TLegend(0.83,0.7,0.95,0.95);
    for (int32_t temp = 0; temp < nTemp; temp++) {
      clrCnt = 0;
      for (int32_t num = 15; num < 16; num+=numSkip[sample]) {
        if (missingprof[sample][temp][num])
          continue;
        ss.str("");
        sslpf.str("");
        ss << sampleName[sample] << "_" << num;
        sslpf << sampleName[sample] << "_" << num << "_lpf";
        gr[sample][temp][num] = new TGraph();
        gr[sample][temp][num]->SetName(ss.str().c_str());
        for (int32_t i=0; i<profvec[sample][temp][num].GetNbinsX(); i++) {
          gr[sample][temp][num]->SetPoint(
            i, (-500 + i)*0.1 , profvec[sample][temp][num].GetBinContent(i) );
        }
        dr->prettify(gr[sample][temp][num]);
        gr[sample][temp][num]->SetLineColor(dr->clrTemp[clrCnt]);
        mg[sample]->Add(gr[sample][temp][num]);

        //---------------- LOW PASS FILTER ----------------------
        double LPF = 1;
        double correction = 20; // I've no idea why.
        for (int32_t a=0; a<10; a++) {
          TProfile profLPF = profvec[sample][temp][num]; // to get all the parameters.
          for (int32_t i=1; i<profvec[sample][temp][num].GetNbinsX(); i++) {
            profLPF.SetBinContent(i,
              correction * (LPF * profvec[sample][temp][num].GetBinContent(i) + (1-LPF) * profLPF.GetBinContent(i-1) ) );
          }
          grLPF[sample][temp][a] = new TGraph();
          grLPF[sample][temp][a]->SetName(sslpf.str().c_str());
          for (int32_t i=0; i<profLPF.GetNbinsX(); i++) {
            grLPF[sample][temp][a]->SetPoint(
              i, (-500 + i)*0.1 , profLPF.GetBinContent(i) );
          }
          dr->prettify(grLPF[sample][temp][a]);
          grLPF[sample][temp][a]->SetLineColor(dr->clrTemp[clrCnt+1]);
          //-------------------------------------------------------
          mg[sample]->Add(grLPF[sample][temp][a]);
          LPF/=2;
        }
        clrCnt++;

        ss.str("");
        ss << avgTimestamp[sample][temp][num] << " s";
        leg[sample]->AddEntry(gr[sample][temp][num], ss.str().c_str(),  "L");
        // leg[sample]->AddEntry(grLPF[sample][temp][0], sslpf.str().c_str(),  "L");
      }
      sss.str("");
      sssEH.str("");
      sssEHsam.str("");
      sss << sampleName[sample] << "_";
      sssEHsam << "#splitline{#splitline{" << sampleNameFull[sample] << "}{"
               << "U_{bias}=" << voltage[polarity[sample]] << "V}}{"
               << sampleNameState[sample] << "}";
      if (polarity[sample])
        sssEH << "electrons"; //500V
      else
        sssEH << "holes";
      sss << tempS[temp];
      tex1[sample][temp] = new TLatex(-1.5,-1.3,"MEASUREMENT");
      tex1[sample][temp]->SetTextFont(132);
      texEH1[sample][temp] = new TLatex(-1.5,10.5,sssEH.str().c_str());
      texEH1[sample][temp]->SetTextFont(22);
      texEH1sam[sample][temp] = new TLatex(10,1.7,sssEHsam.str().c_str());
      texEH1sam[sample][temp]->SetTextFont(22);
    }
  }

  for (int32_t sample=5; sample<6; sample++) {
    sss.str("");
    sss << "can_" << sample;
    can[sample] = new TCanvas(sss.str().c_str(),sss.str().c_str(),800,600);
    dr->prettify(can[sample]);
    mg[sample]->Draw("AL");
    dr->prettify(mg[sample]);
    mg[sample]->GetXaxis()->SetTitle("Time [ns]");
    mg[sample]->GetYaxis()->SetTitle("Current [#mu A]");
    mg[sample]->GetYaxis()->SetRangeUser(-1.8,12.8); //[V]
    mg[sample]->GetXaxis()->SetRangeUser(-2,15); //[s]
    leg[sample]->Draw("same");
    tex1[sample][0]->Draw("same");
    texEH1[sample][0]->Draw("same");
    texEH1sam[sample][0]->Draw("same");
    can[sample]->Update();
    can[sample]->WaitPrimitive();
    ss.str("");
    // ss << "plots/c2lpf/" << sampleName[sample] << ".pdf";
    // can[sample]->SaveAs(ss.str().c_str());
    can[sample]->Update();
  }

  // delete can;
  // delete leg;
  delete dr;
  delete app;




  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------