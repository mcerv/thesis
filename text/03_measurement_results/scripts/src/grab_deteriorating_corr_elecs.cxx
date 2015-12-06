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

double ADCtoV (double y) {
  return 0.000804 * y - 0.05; //V
}

double weightedError (double w, double avg, double current) {
  return w * (avg - current) * (avg - current);
}

double correlation (TH2F p, TProfile pavg) {
  const int32_t RoiMin = 480;//samples, -2 ns
  const int32_t RoiMax = 620;//samples, 12 ns
  const int32_t Roi = RoiMax - RoiMin;
  const int32_t histMax = 560;
  double CorrSum = 0;
  for (int32_t i=RoiMin; i<RoiMax; i++) {
    double corr = 0;
    for (int32_t adc=0; adc<histMax/3; adc++) { // we don't need the full y range
      if (p.GetBinContent(i,adc) > 0) {
        corr += weightedError(p.GetBinContent(i,adc), pavg.GetBinContent(i), ADCtoV(adc) );
        // cout << i << "\t" << adc << "\t"
        //      << p.GetBinContent(i,adc) << "\t"
        //      << ADCtoV(adc) << "\t"
        //      << pavg.GetBinContent(i) << "\t"
        //      << corr << "\t"
        //      << CorrSum
        //      << endl;
      }
    }
    CorrSum += corr;
  }
  return CorrSum;
}

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<"Grabbing pulse profiles and plotting a longterm C2 decrease of CORRELATION.."<<endl;



  cout<<"Calculating DETERIORATING  ... ELECS"<<endl;

  stringstream ss;
  TFile* f;
  double samplingTime = 0.1; //ns
  const int nSamples = 1;
  const int nVoltages = 1;
  const int nTemp = 1;

  double voltage[nVoltages] = {500.0};
  int32_t polarity[nSamples] = {0};

  string sampleName[nSamples] = {"S79eafter"};
  string sampleNameFull[nSamples] = { "S79 1e14 #pi cm^{-2}"};
  string sampleNameState[nSamples] = {"primed"};

  int32_t nFilesIndividual[nTemp] = { 220};
  int32_t numSkip[nTemp] = { 1 };

  string tempS[nTemp] = { "1"};

  double delay[nTemp] = {1}; //seconds. -1 because it's a clean pulse

  // string tempName[nTemp] = {"Clean",
  //                           "The worst",
  //                           "Sr90 5 min",
  //                           "Sr90 10 min" };
  // double tempN[nTemp] = {1, 2, 3, 4};

  int32_t cntFiles = 0;
  int32_t cntFilesOpen = 0;
  int32_t cntPulses = 0;
  const int32_t nFiles = 230;
  // int32_t nFilesIndividual = nFiles;

  TProfile profvec[nSamples][nTemp][nFiles];
  TH2F pulse[nSamples][nTemp][nFiles];
  bool missingprof[nSamples][nTemp][nFiles];
  int32_t avgTimestamp[nSamples][nTemp][nFiles];


  //get the profiles and respective timestamps from the file
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) { // only one  - "1"
      for (int32_t num = 0; num < nFilesIndividual[temp]; num+= numSkip[temp]) {
        ss.str("");
        ss << "/Users/mcerv/Documents/MarieCurieTALENT/Software/tmpScLifetime/"
           << sampleName[sample]
           << "/conv/";
        ss << sampleName[sample]
           << "_"
           << tempS[temp]
           << "_" << voltage[sample]
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
        if (!(TH2F*)f->Get("pulses")) {
          cout<<" File " << ss.str().c_str() << " doesn't include pulses!" << endl;
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
        // prof.Scale(131.7);
        prof.GetXaxis()->SetRangeUser(-5,20); //nanoseconds
        ss.str("");
        ss << sampleName[sample] << "_" << tempS[temp] << "_" << num;
        prof.SetName(ss.str().c_str());
        profvec[sample][temp][num] = prof;
        pulse[sample][temp][num] = (*(TH2F*)f->Get("pulses"));
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
  //                      CORRELATION CALCULATION!!!!!
  // ------------------------------------------------------------------

  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();

  // CALCULATE CORRELATION FOR THE NEW PULSE - SAMPLE 0, TEMP 0, NUM 0
  // BETWEEN -2 AND 12 NS
  TCanvas *c = new TCanvas("can","can",800,600);
  dr->prettify(c);
  TH2F p = pulse[0][0][0];
  TProfile pavg = profvec[0][0][0];
  p.Draw("col");
  pavg.Draw("same");



  double corrNew = correlation (p, pavg);
  cout << " ====================== " << endl;
  cout << " Correlation for the clear pulse: " << corrNew << endl;
  cout << " ====================== " << endl;

  c->Update();
  c->WaitPrimitive();
  c->Update();


  ofstream fout ("data/deterioratingElecs.txt");
  fout << "Run" << "\t" << "Delay[s]" << "\t" << "Correlation" << endl;

  // here, only temperature will change.
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t num = 0; num < nFilesIndividual[temp]; num+=numSkip[temp]) {
        if (missingprof[sample][temp][num])
          continue;

        pulse[sample][temp][num].Draw("col");
        pavg.Draw("same");
        cout << " corr for temp " << temp << " : "
           << correlation (pulse[sample][temp][num], pavg )
           << " delay timestamp " << avgTimestamp[sample][temp][num]
           << endl;

        fout << num << "\t" << avgTimestamp[sample][temp][num] << "\t" << correlation (pulse[sample][temp][num], pavg ) << endl;

        // c->Update();
        // c->WaitPrimitive();
        // c->Update();
      }
    }
  }

  fout.close();

  // delete can;
  // delete leg;
  delete dr;
  delete app;



  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
