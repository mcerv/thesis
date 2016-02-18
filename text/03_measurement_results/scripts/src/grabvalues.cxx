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
#include "TParameter.h"

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

  cout<<"Grabbing pulse profiles and saving into a single file.."<<endl;


  stringstream ss;
  TFile* f;
  double samplingTime = 0.1; //ns
  //set the input data
  const int nSamples = 5;
  const int nVoltages = 16;
  // const int nVoltages = 14;
  const int nTemp = 18;

  // double voltage[nVoltages] = {500.0, 400.0, 300.0, 200.0, 100.0, -100.0, -200.0, -300.0, -400.0, -500.0};
  // double voltage[nVoltages] = {700.0, 600.0, 500.0, 400.0, -400.0, -500.0, -600.0, -700.0};
  // double voltage[nVoltages] = {700.0, -700.0};
  double voltage[nVoltages] = {700.0, 600.0, 500.0, 400.0, 300.0, 200.0, 150.0, 100.0,
                        -100.0, -150.0, -200.0, -300.0, -400.0, -500.0, -600.0, -700.0};
  // double voltage[nVoltages] = {500.0, 400.0, -400.0, -500.0};
  string sampleName[nSamples] = {"S52","S79", "S37" , "S52_3-63e14", "S79_1e14"};
  // string sampleName[nSamples] = {"S79_1e14","S52_3-63e14","S37"};
  // string sampleName[nSamples] = {"S52", "S52_3-63e14"};
  // string sampleName[nSamples] = {"S79_1e14"};
  string tempS[nTemp] = {"4k","7k","10k","15k","25k","40k","60k","75k","95k",
              "115k","135k","160k","180k","210k","230k","260k","280k","295k"};
  // double tempN[nTemp] = {4,7,10,15,25,40,60,75,95,115,135,160,180,210,230,260,280,295};
  // double tempN[nTemp] = {4,7,10,15,25,35,40,50,60,65,75,95,115,130,135,150,160,170,180,190,210,230,260,280,295};
  float amp = 138.;
  float Qconstant = 2e-11/amp; // hence charge is in C



  //--------------------- Variables for the values we want to extract ----------
  TProfile profvec[nSamples][nTemp][nVoltages];
  bool missingprof[nSamples][nTemp][nVoltages];
  double Vdrift[nSamples][nTemp][nVoltages];
  double t_c[nSamples][nTemp][nVoltages];
  double charge[nSamples][nTemp][nVoltages];
  double S_x[nSamples][nTemp][nVoltages];
  double E_x[nSamples][nTemp][nVoltages];
  double S_xe[nSamples][nTemp][nVoltages];
  double E_xe[nSamples][nTemp][nVoltages];
  //----------------------------------------------------------------------------

  int32_t cntFiles = 0;
  int32_t cntFilesOpen = 0;
  int32_t cntPulses = 0;
  //get the profiles from the file
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        ss.str("");
        if (!sampleName[sample].compare("S52") ||
            !sampleName[sample].compare("S37") ||
            !sampleName[sample].compare("S79")) {
          ss << "/Volumes/MACSTORAGE/TCT/TCT_results/";
          // ss << "/Volumes/hram/work/TCT/TCT_results/";
        } else if (!sampleName[sample].compare("S52_3-63e14")) {
          // ss << "/Volumes/hram/work/TCT/2014-07-29/TCT_results/";
          ss << "/Volumes/MACSTORAGE/TCT/2014-07-29/TCT_results/";
        } else if (!sampleName[sample].compare("S79_1e14")) {
          // ss << "/Volumes/hram/work/TCT/2014-07-28/TCT_results/";
          ss << "/Volumes/MACSTORAGE/TCT/2014-07-28/TCT_results/";
        } else if (!sampleName[sample].compare("1scdhq")) {
          // ss << "/Volumes/hram/work/TCT/2014-07-28/TCT_results/";
          ss << "/Volumes/MACSTORAGE/TCT/2014-07-11/TCT_results/";
        }
        ss << sampleName[sample] << "/" << tempS[temp] << "/" << sampleName[sample]
           << "_" << tempS[temp] << "_" << voltage[volt] << "V.root";
        // the sample dataset
        // ss << "data/" << sampleName[sample] << "/" << tempS[temp] << "/" << sampleName[sample]
        //    << "_" << tempS[temp] << "_" << voltage[volt] << "V.root";
        TProfile prof;
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
        if (!(TProfile*)f->Get("avgpulses_filt")) {
          cout<<" File " << ss.str().c_str() << " doesn't include avgpulses!" << endl;
          missingprof[sample][temp][volt] = true;
          continue;
        }
        // if (!((TParameter<float>*)f->Get("IntCorr"))->GetVal() ) {
        //   cout<<" File " << ss.str().c_str() << " doesn't include IntCorr!" << endl;
        //   missingprof[sample][temp][volt] = true;
        //   continue;
        // }
        // if (!((TParameter<float>*)f->Get("newS_x"))->GetVal() ) {
        //   cout<<" File " << ss.str().c_str() << " doesn't include newS_x!" << endl;
        //   missingprof[sample][temp][volt] = true;
        //   continue;
        // }
        // if (!((TParameter<float>*)f->Get("enewS_x"))->GetVal() ) {
        //   cout<<" File " << ss.str().c_str() << " doesn't include enewS_x!" << endl;
        //   missingprof[sample][temp][volt] = true;
        //   continue;
        // }
        // if (!((TParameter<float>*)f->Get("newE_x"))->GetVal() ) {
        //   cout<<" File " << ss.str().c_str() << " doesn't include newE_x!" << endl;
        //   missingprof[sample][temp][volt] = true;
        //   continue;
        // }
        // if (!((TParameter<float>*)f->Get("enewE_x"))->GetVal() ) {
        //   cout<<" File " << ss.str().c_str() << " doesn't include enewE_x!" << endl;
        //   missingprof[sample][temp][volt] = true;
        //   continue;
        // }
        // S_x[sample][temp][volt] = (double) ((TParameter<float>*)f->Get("newS_x"))->GetVal();
	      // E_x[sample][temp][volt] = (double)((TParameter<float>*)f->Get("newE_x"))->GetVal();
	      // S_xe[sample][temp][volt] = (double)((TParameter<float>*)f->Get("enewS_x"))->GetVal();
	      // E_xe[sample][temp][volt] = (double)((TParameter<float>*)f->Get("enewE_x"))->GetVal();
	      // charge[sample][temp][volt] = (double)((TParameter<float>*)f->Get("IntCorr"))->GetVal()*Qconstant;

        cntPulses++;
      	prof = (*(TProfile*)f->Get("avgpulses_filt"));
        prof.Scale(131.7);
        prof.GetXaxis()->SetRangeUser(-5,20); //nanoseconds
        ss.str("");
        ss << sampleName[sample] << "_" << tempS[temp] << "_" << voltage[volt];
        prof.SetName(ss.str().c_str());
        profvec[sample][temp][volt] = prof;
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



  //Start graphs
  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  TGraphErrors* gVdrift[nSamples][nTemp];//
  TMultiGraph* gMultiVdrift[nSamples];

  // //prepare the values
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //   double thick = 0;
  //   switch (sample) {
  //     case S52 : thick = 515e-4;
  //     case S37 : thick = 548e-4;
  //     case S79 : thick = 519e-4;
  //     case S52_363e14 : thick = 515e-4;
  //     case S79_1e14 : thick = 515e-4;
  //   }
  //   int32_t clrCnt=0;
  //   gMultiVdrift[sample] = new TMultiGraph();
  //   for (int32_t temp = 0; temp < nTemp; temp++) {
  //     if (!(temp==11 || temp==13 || temp==15 || temp==17)) {
  //       continue;
  //     }
  //     gVdrift[sample][temp] = new TGraphErrors();
  //     int32_t cnt = 0;
  //     for (int32_t volt = 0; volt < nVoltages/2; volt++) {
  //       if (missingprof[sample][temp][volt])
  //         continue;
  //       t_c[sample][temp][volt] = (E_x[sample][temp][volt]-S_x[sample][temp][volt])*1e-9; //into ns
  //       Vdrift[sample][temp][volt] = thick/t_c[sample][temp][volt];
  //       cout<<"sample "<<sample<<" temp "<<temp<<" volt "<<volt
  //           <<" 1/Vdrift"<<1/Vdrift[sample][temp][volt]<<endl;
  //       gVdrift[sample][temp]->SetPoint(cnt, 1/Vdrift[sample][temp][volt], charge[sample][temp][volt] );
  //       cnt++;
  //     }
  //     dr->prettify(gVdrift[sample][temp]);
  //     gVdrift[sample][temp]->SetLineColor(dr->clrTemp[clrCnt]);
  //     clrCnt++;
  //     gMultiVdrift[sample]->Add(gVdrift[sample][temp]);
  //   }
  // }
  //
  // TCanvas* can = new TCanvas("can","can",800,600);
  // dr->prettify(can);
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //   if (!sample)
  //     gMultiVdrift[sample]->Draw("AL*");
  //   else
  //     gMultiVdrift[sample]->Draw("L* SAME");
  //   dr->prettify(gMultiVdrift[sample]);
  //   gMultiVdrift[sample]->GetYaxis()->SetRangeUser(20e-15,55e-15);
  //   gMultiVdrift[sample]->GetXaxis()->SetRangeUser(0,0.3e-6);
  //   gMultiVdrift[sample]->GetYaxis()->SetTitle("Measured charge [C]");
  //   gMultiVdrift[sample]->GetXaxis()->SetTitle("1/v_{drift} [s/cm]");
  //   can->Update();
  //   can->WaitPrimitive();
  //   can->Update();
  //   can->Clear();
  // }
  //
  //
  //
  //
  // delete can;
  //
  //
  //
  //
  //
  //
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //   for (int32_t temp = 0; temp < nTemp; temp++) {
  //       delete gVdrift[sample][temp];
  //   }
  // }



  //----------------- Save the pulses -------------------------------
  TDirectory* dirp;
  TDirectory* dirs;
  TDirectory* dirt;
  stringstream sst;

  //------------ open file for saving plots --------------------------
  sst.str("");
  sst << "plots/pulses.root";
  TFile* filePlots = new TFile(sst.str().c_str(),"recreate");
  cout<<" Saving in file "<<sst.str()<<endl;

  for (int32_t sample = 0; sample < nSamples; sample++) {
    sst.str("");
    sst << "Sample_" << sampleName[sample];
    dirs = filePlots->mkdir(sst.str().c_str());
    dirs->cd();
    // filePlots->cd(sst.str().c_str());
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        if (missingprof[sample][temp][volt]) {
          continue;
        }
        profvec[sample][temp][volt].Write();
      }
    }
  }
  filePlots->Close();
  delete(filePlots);
  delete dr;
  delete app;
  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
