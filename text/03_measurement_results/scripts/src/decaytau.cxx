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




using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  Double_t clrScheme[6][20];
  // Double_t clrBase[6] = {kAzure, kTeal, kViolet, kPink, kGreen, kOrange};
  Double_t clrBase[6] = {kRed, kBlue, kGreen, kOrange, kViolet, kBlack };
  for (int32_t sample=0; sample<6;sample++) {
    Double_t clr = clrBase[sample];
    for (int32_t temp=0; temp<20;temp++) {
      clrScheme[sample][temp] = clrBase[sample] + temp/4 - 4.0;
    }
  }


  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  cout<<"Fitting a decay function.."<<endl;

  //set global variables and objects
  stringstream ss, ssleg;
  TCanvas* can = new TCanvas("can","can",800,600);
  // TGraph* norm = new TGraph();
  // TGraph* diff = new TGraph();
  double samplingTime = 0.1; //ns
  //set the input data
  const int nSamples = 5;
  const int nVoltages = 4;
  // const int nVoltages = 14;
  const int nTemp = 18;

  // double voltage[nVoltages] = {500.0, 400.0, 300.0, 200.0, 100.0, -100.0, -200.0, -300.0, -400.0, -500.0};
  // double voltage[nVoltages] = {700.0, 600.0, 500.0, 400.0, -400.0, -500.0, -600.0, -700.0};
  // double voltage[nVoltages] = {700.0, -700.0};
  // double voltage[nVoltages] = {700.0, 600.0, 500.0, 400.0, 300.0, 200.0, 100.0,
  //                       -100.0, -200.0, -300.0, -400.0, -500.0, -600.0, -700.0};
  double voltage[nVoltages] = {500.0, 400.0, -400.0, -500.0};
  // string sampleRealName[nSamples] = {"S52","S79","S37","S79_1e14","S52_3-63e14"};
  string sampleRealName[nSamples] = {"S52","S79","S37","S52 3.6#times10^{14} #pi cm^{-2}", "S79 1#times10^{14} #pi cm^{-2}"};
  string sampleName[nSamples] = {"S52","S79","S37","S52_3-63e14","S79_1e14"};

  double irrad[nSamples] = {0,0,0,3.63,1};
  double irrade[nSamples] = {0,0,0,0.76,0.21};
  // string sampleName[nSamples] = {"S79_1e14","S52_3-63e14","S37"};
  // string sampleName[nSamples] = {"S52", "S52_3-63e14"};
  // string sampleName[nSamples] = {"S79_1e14"};
  string tempS[nTemp] = {"4k","7k","10k","15k","25k","40k","60k","75k","95k",
              "115k","135k","160k","180k","210k","230k","260k","280k","295k"};
  double tempN[nTemp] = {4,7,10,15,25,40,60,75,95,115,135,160,180,210,230,260,280,295};



  //------------get the profiles from the file-------------------------
  TProfile* prof[nSamples][nTemp][nVoltages];
  bool missingprof[nSamples][nTemp][nVoltages];
  TFile* f = new TFile ("plots/pulses.root","read");// = new TFile();
  if (!f->IsOpen()) {
    cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
    return 0;
  }
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        ss.str("");
        ss << "Sample_" << sampleName[sample] << "/" << sampleName[sample]
           << "_" << tempS[temp] << "_" << voltage[volt];
        if (!(TProfile*)f->Get(ss.str().c_str() ) ) {
          cout<<" File " << ss.str().c_str() << " doesn't include " << ss.str() << endl;
          prof[sample][temp][volt] = new TProfile();
          missingprof[sample][temp][volt] = true;
          continue;
        }
        prof[sample][temp][volt] = ((TProfile*)f->Get(ss.str().c_str() ));
        missingprof[sample][temp][volt] = false;
      }
    }
  }//----------------- end of reading in -------------------------



  double spaceCharge[nSamples][nTemp][nVoltages];
  double maxNonIrrad[nSamples][nTemp][nVoltages];
  double spaceChargeDisp[nSamples][nTemp][nVoltages];
  double tauDecayVolt[nSamples][nTemp][nVoltages];
  double tauDecayVoltDec[2][nSamples][nTemp][nVoltages];
  double tauDecayTemp[nSamples][nVoltages][nTemp];
  double tauDecayTempDec[2][nSamples][nVoltages][nTemp];
  for (int32_t i=0; i<nSamples; i++) {
    for (int32_t j=0; j<nTemp; j++) {
      for (int32_t k=0; k<nVoltages; k++) {
        spaceCharge[i][j][k] = 0; // space charge for individual pulses non-irrad
        spaceChargeDisp[i][j][k] = 0; // space charge displacement for individual pulses non-irrad
        tauDecayVolt[i][j][k] = -10; // -10 in case it's not found
        tauDecayVoltDec[1][i][j][k] = -10; // -10 in case it's not found
        tauDecayVoltDec[0][i][j][k] = -10; // -10 in case it's not found
        tauDecayTemp[i][k][j] = -10; // -10 in case it's not found
        tauDecayTempDec[1][i][k][j] = -10; // -10 in case it's not found
        tauDecayTempDec[0][i][k][j] = -10; // -10 in case it's not found
        maxNonIrrad[i][j][k] = -10; //-10 in case it's not found
      }
    }
  }

  //create the fit function
  TF1* decayfun = new TF1("decay","[4]*x + [5] + [6] + [0] + ([1]-[0])*exp(-(x+[3])/[2])",1.5,7.5);
  TF1* decaydoublefun = new TF1("decaydouble","[0] + ([1]-[0])*exp(-(x+[3])/[2])",1.5,7.5);
  TF1* spacechgfun = new TF1("spacechg","[1]*x + [0]",1.5,7.5);
  // TF1* decayfun = new TF1("decay","[0] + ([1]-[0])*exp(-(x+[3])/[2])",1.5,7.5);
  decayfun->SetParName(0,"chgInf");     //charge at infinity
  decayfun->SetParName(1,"chgStart");   //starting charge
  decayfun->SetParName(2,"tauRec");     //recombination
  decayfun->SetParName(3,"xDisp");      //displacement in x direction
  decayfun->SetParName(6,"yDisp");      //displacement in y direction
  decayfun->SetParameter("chgInf", 0);
  // decayfun->SetParLimits(0,-6,2);
  decayfun->FixParameter(0,0);          //fix the charge in infinity to 0
  decayfun->SetParameter("chgStart", 7); //starting current
  decayfun->SetParLimits(1,1,20);
  decayfun->SetParameter("tauRec", 10); //recombination time
  decayfun->SetParLimits(2,0,400);
  decayfun->SetParameter("xDisp",-1);   //displacement for 1 ns
  decayfun->SetParLimits(3,-5,5);
  decayfun->SetParameter("yDisp",0);   //displacement for 1 ns
  decayfun->SetParLimits(6,-5,5);
  //space charge
  decayfun->SetParName(4,"spaceChg");   //space charge
  decayfun->SetParName(5,"spaceChgDisp");   //space charge displacement
  decayfun->FixParameter(4,0); //slope due to space charge
  decayfun->FixParameter(5,0); //displacement due to space charge
  decayfun->FixParameter(6,0); //displacement due to space charge
  decayfun->SetParName(4,"spaceChg");   //space charge
  decayfun->SetParName(5,"spaceChgDisp");   //space charge displacement
  spacechgfun->SetParName(0,"spaceChg");   //space charge
  spacechgfun->SetParName(1,"spaceChgDisp");   //space charge displacement

  decaydoublefun->SetParName(0,"chgInf");     //charge at infinity
  decaydoublefun->SetParName(1,"chgStart");   //starting charge
  decaydoublefun->SetParName(2,"tauRec");     //recombination
  decaydoublefun->SetParName(3,"xDisp");      //displacement in x direction
  decaydoublefun->FixParameter(0,0);
  decaydoublefun->SetParameter(1,7);
  decaydoublefun->SetParameter(2,10);
  decaydoublefun->SetParameter(3,-1);
  decaydoublefun->SetParLimits(1,0,15);
  decaydoublefun->SetParLimits(2,0,400);
  decaydoublefun->SetParLimits(3,-5,5);










  //fit the function to the profiles
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        if (missingprof[sample][temp][volt]) {
          continue; //not a profile here
        }
        cout<<"\r"<<" sam "<<sample<<" temp "<<temp<<" volt "<<volt<<flush;
          // profvec.at(sample).at(temp).at(volt).Draw();
          // can->Update();
          // can->WaitPrimitive();
          // getchar();
          // can->Clear();

        //set the fit limits
        //get the max and thres of the top
        double max = (double)prof[sample][temp][volt]->GetMaximum();
        double maxBin = prof[sample][temp][volt]->GetMaximumBin();
        double marginFall = 0 ;
        double marginRise = 0 ;
        double thresRise = 0;
        double thresFall = 0;

        if (temp < 7) { //7 -> 75 K
          marginFall = 25;
          marginRise = 30;
          thresRise = max * 0.1; //because the tail is longer, 20%
          thresFall = max * 0.25;
        } else if (7 <= temp && temp < 11) { //between 75k and (excl)160k
          marginFall = 20;
          marginRise = 25;
          thresRise = max * 0.2;// when it crosses the 30% thres on fall slope
          thresFall = max * 0.4;// when it crosses the 30% thres on fall slope
        } else {
          marginFall = 23;
          marginRise = 25;
          thresRise = max * 0.1;// when it crosses the 10% thres on fall slope
          thresFall = max * 0.1;// when it crosses the 10% thres on fall slope
        }
        int32_t riseEdgeBin = 0;//(int32_t)prof->GetMaximumBin();
        int32_t fallEdgeBin = 0;
        for (int32_t bin=maxBin; bin>(maxBin-200); bin--) {
          if (prof[sample][temp][volt]->GetBinContent(bin) >= thresRise) {
            riseEdgeBin = bin;
          }
        }
        for (int32_t bin=maxBin; bin<(maxBin+200); bin++) {
          if (prof[sample][temp][volt]->GetBinContent(bin) >= thresFall) {
            fallEdgeBin = bin;
          }
        }
        int32_t pulseWidthBin = fallEdgeBin-riseEdgeBin;
        if (pulseWidthBin < 60) { //for fast narrow pulses
          marginRise-= 10;
          marginFall-= 10;
          cout<<" Very fast "<<flush;//<<endl;
        } else if (pulseWidthBin < 80) {
          marginRise-= 5;
          marginFall-= 5;
          cout<<" Fast "<<flush;//<<endl;
        }

        // cout << " Found max " << max << " at bin " << maxBin
        //      << " rising edge " <<riseEdgeBin
        //      << " and falling edge " << fallEdgeBin;// << endl;
        //transform bin numbers into [ns] - fit cut
        double fitMin = (-500 + riseEdgeBin + marginRise)*0.1;
        double fitMax = (-500 + fallEdgeBin - marginFall)*0.1;
        // cout << " Fitting from " << fitMin << " to " << fitMax << "... ";

        //-------------------------- FIT FUNCTIONS ------------------------
        //for the non-irradiated sample take the maximum and save it.

        // for the non-irradiated samples fit the space charge and save it.
        // if (!sampleName[sample].compare("S52") ||
        //     !sampleName[sample].compare("S37") ||
        //     !sampleName[sample].compare("S79")) {
        if (0) { //switch on or off space charge fitting
          spacechgfun->SetParameter("spaceChgDisp",max); //max of the pulse as a starting point
          spacechgfun->SetParameter("spaceChg",0); //starting space charge is 0

          TFitResultPtr fptr = prof[sample][temp][volt]->Fit("spacechg","RSQ", "", fitMin, fitMax);

          spaceCharge[sample][temp][volt] = fptr->Parameter(1);
          spaceChargeDisp[sample][temp][volt] = fptr->Parameter(0);
          cout<<" ** Space charge fit: displacement "<<spaceChargeDisp[sample][temp][volt]
              <<", slope "<<spaceCharge[sample][temp][volt]<<flush;//<<endl;

        } // ------------ end fitting space charge -----

        // for the irradiatied samples (and S37), fix the space charge and fit the decay.
        // if ( !sampleName[sample].compare("S79_1e14") ||
        //      !sampleName[sample].compare("S52_3-63e14") ||
        //      !sampleName[sample].compare("S37") ) {
        if(1) {
          //fix the space charge of the non-irrad
          int32_t sampleNonIrrad=sample;   //-----** THIS NEEDS TO BE CHANGED BY HAND!!!!!! TODO
          if ( !sampleName[sample].compare("S79_1e14")) {
            sampleNonIrrad = 1;
          }
          if ( !sampleName[sample].compare("S52_3-63e14")) {
            sampleNonIrrad = 0;
          }
          // cout<<" parameter 4 fixed to "<<spaceCharge[sampleNonIrrad][temp][volt]<<endl;
          decayfun->FixParameter(4,spaceCharge[sampleNonIrrad][temp][volt]); //slope due to space charge
          decayfun->FixParameter(5,0); //displacement due to space charge
          // decayfun->FixParameter(4,0); //slope due to space charge
          // decayfun->FixParameter(5,0); //displacement due to space charge

          // special fit with p0 fixed
          decaydoublefun->FixParameter(1,prof[sampleNonIrrad][temp][volt]->GetMaximum() );
          TFitResultPtr fptrdouble = prof[sample][temp][volt]->Fit("decaydouble","RSQ", "", fitMin, fitMax);

        if (!fptrdouble->IsEmpty()) { //if the fit is not empty, fill arrays
            cout<<" SECONDARY FIY: fitted Tau "<<fptrdouble->Parameter(2)<<flush<<endl;
          }

          TFitResultPtr fptr = prof[sample][temp][volt]->Fit("decay","RSQ", "", fitMin, fitMax);
          if (!fptr->IsEmpty()) { //if the fit is not empty, fill arrays
            tauDecayVolt[sample][temp][volt] = fptr->Parameter(2); //tau fit
            tauDecayTemp[sample][volt][temp] = fptr->Parameter(2); //tau fit
            if (!(temp%2)) {
              // cout<<"even temp    ";
              tauDecayVoltDec[0][sample][temp][volt] = fptr->Parameter(2); //tau fit
            } else {
              // cout<<"odd temp    ";
              tauDecayVoltDec[1][sample][temp][volt] = fptr->Parameter(2); //tau fit
            }
            if (!(volt%2)) {
              // cout<<"even volt   "<<endl;
              tauDecayTempDec[0][sample][volt][temp] = fptr->Parameter(2);
            } else {
              // cout<<"odd volt   "<<endl;
              tauDecayTempDec[1][sample][volt][temp] = fptr->Parameter(2);
            }
            cout<<" fitted Tau "<<fptr->Parameter(2)
                <<" spaceChg "<<fptr->Parameter(4)
                <<" yDisp "<<fptr->Parameter(6)<<flush<<endl;
          }
          else {
            continue; //empty fit
          }

        } // ----------- end fitting tau --------


        //-----------------------------------------------------------------
        if (DEBUG) {
          can->Update();
          can->WaitPrimitive();
          can->Clear();
        }

      }
    }
  }
  delete(can);
  delete(decayfun);
  cout<<endl<<" ------------ Fitting finished.. -----------"<<endl<<endl;
  //----------------- end of decay fitting ---------------------





  // delete all profile pointers.
  for (int32_t sample = 0; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        delete prof[sample][temp][volt];
      }
    }
  }

  f->Close();
  delete(f);









  //------------ open file for saving plots --------------------------
  TFile* filePlots = new TFile("plots/plotsdecay.root","update");
  //------------------------------------------------------------------


  // //----------------- Save the pulses -------------------------------
  // TDirectory* dirp;
  // TDirectory* dirs;
  // TDirectory* dirt;
  // stringstream sst;
  // for (int32_t sample = 0; sample < nSamples; sample++) {
  //   sst.str("");
  //   sst << "Sample_" << sampleName[sample];
  //   dirs = filePlots->mkdir(sst.str().c_str());
  //   dirs->cd();
  //   // filePlots->cd(sst.str().c_str());
  //   for (int32_t temp = 0; temp < nTemp; temp++) {
  //     for (int32_t volt = 0; volt < nVoltages; volt++) {
  //       profvec.at(sample).at(temp).at(volt).Write();
  //     }
  //   }
  // }
  // filePlots->cd();
  // //-----------------------------------------------------------------



  //----------------- Prepare the tau data-------------------------------
  //remove all non-fitted inputs.
  TGraph* grTemp[nSamples][nVoltages];
  int32_t nRemovedTemp[nSamples][nVoltages];


  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t volt=0; volt<nVoltages; volt++) {
      nRemovedTemp[sample][volt] = 0;
    }
  }
  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t volt=0; volt<nVoltages; volt++) {
      grTemp[sample][volt] = new TGraph(nTemp,tempN,tauDecayTemp[sample][volt]);
      for (int32_t temp=0; temp<nTemp; temp++) {
        // cout<<" sample "<<sample<<" temp "<<temp<<" voltage "<<volt
        //     <<" tauDecayTemp "<<tauDecayTemp[sample][volt][temp]<< endl;
        if (tauDecayTemp[sample][volt][temp] < -1.0){
          // -10 in case it's not found. we delete the entry.
          grTemp[sample][volt]->RemovePoint(temp-nRemovedTemp[sample][volt]);
          nRemovedTemp[sample][volt]++;
        }
      }
    }
  }


  //----------------- Plot decay vs Temperature-------------------------------
  cout<<endl<<" Plotting the decay vs temperature... "<<endl;
  can = new TCanvas("can","can",800,600);
  dr->prettify(can);
  can->SetLogy();
  TLegend* legTemp = new TLegend(0.7,0.65,0.95,0.95); //for All, Odd and Even
  bool first3 = true;

  for (int32_t sample=0; sample<nSamples; sample++) {
    for (int32_t volt=0; volt<nVoltages; volt++) {
      if (!grTemp[sample][volt]->GetN()) {
        continue; //if the graph is empty, get out
      }
      dr->prettify(grTemp[sample][volt]);
      if (first3) { //first draw
        first3 = false;
        grTemp[sample][volt]->Draw ("AL.");
        grTemp[sample][volt]->GetYaxis()->SetRangeUser(1,450);
        grTemp[sample][volt]->GetXaxis()->SetRangeUser(1,450);
        grTemp[sample][volt]->GetXaxis()->SetLimits(1,450);
        grTemp[sample][volt]->GetXaxis()->SetTitle("Temperature [K]");
        grTemp[sample][volt]->GetYaxis()->SetTitle("#tau [s{}^{-1}]");
      }
      else {
        grTemp[sample][volt]->Draw ("L SAME");
      }
      grTemp[sample][volt]->SetLineColor(clrScheme[sample][volt]);
      ssleg.str("");
      ssleg << sampleName[sample];// << "_" << voltage[volt];
      if (!volt) {
        legTemp->AddEntry(grTemp[sample][volt],ssleg.str().c_str(),"l");
      }

    }
  }
  legTemp->SetHeader("For #pm 400 V, #pm 500 V");
  legTemp->SetTextSize(0.04);
  legTemp->Draw("same");
  can->Update();
  can->Write();
  can->WaitPrimitive();
  legTemp->Clear();
  delete(can);
  //------------------------------------------------------------





  //----------------- Prepare the tau data-------------------------------
  TGraphErrors *grTau[nSamples];
  double tempNTau[nSamples][nTemp], tauTau[nSamples][nTemp], tempNTaue[nSamples][nTemp], tauTaue[nSamples][nTemp];

  for (int32_t sample=0; sample<nSamples; sample++) {
    int32_t nTempTau=0;
    for (int32_t temp=0; temp<nTemp; temp++) {
      double taumean=0;
      double taurms=0;
      if (tempN[temp]>70 && tempN[temp]<150) continue; //skip the region between 75 andd 150
      int32_t tmpVol=0;
      for (int32_t volt=0; volt<nVoltages; volt++) {
        if (tauDecayTemp[sample][volt][temp] < -1.0) continue;
        taumean += tauDecayTemp[sample][volt][temp];
        tmpVol++;
      }
      taumean/= tmpVol;
      for (int32_t volt=0; volt<nVoltages; volt++) {
        if (tauDecayTemp[sample][volt][temp] < -1.0) continue;
        taurms += (taumean-tauDecayTemp[sample][volt][temp])*(taumean-tauDecayTemp[sample][volt][temp]);
      }
      taurms = sqrt(taurms/tmpVol);

      tempNTau[sample][nTempTau] = tempN[temp];
      tempNTaue[sample][nTempTau] = 0.01*tempN[temp];
      tauTau[sample][nTempTau] = taumean;
      tauTaue[sample][nTempTau] = taurms;

      cout<<nTempTau<<"\t"
          <<tempN[temp]<<"\t"
          <<0.01*tempN[temp]<<"\t"
          <<taumean<<"\t"
          <<taurms
          <<endl;
      nTempTau++;
    }
    grTau[sample]=new TGraphErrors(nTempTau, tempNTau[sample], tauTau[sample], tempNTaue[sample], tauTaue[sample]);
  }



  //----------------- Plot mean and rms of +-4/500 decay vs Temperature-------------------------------
  cout<<endl<<" Plotting the MEAN decay vs temperature... "<<endl;
  can = new TCanvas("canmean","canmean",800,600);
  dr->prettify(can);
  can->SetLogy();
  can->SetLogx();
  TLegend* legTau = new TLegend(0.22,0.57,0.52,0.8); //for All, Odd and Even
  TMultiGraph *mgTau = new TMultiGraph();
  bool first4 = true;

  for (int32_t sample=0; sample<nSamples; sample++) {
    if (!grTau[sample]->GetN()) {
      continue; //if the graph is empty, get out
    }
    dr->prettify(grTau[sample]);
    grTau[sample]->SetMarkerStyle(24-sample);
    // grTau[sample]->SetLineColor(clrScheme[sample][volt]);
    mgTau->Add(grTau[sample]);

    ssleg.str("");
    ssleg << sampleRealName[sample];// << "_" << voltage[volt];
    legTau->AddEntry(grTau[sample],ssleg.str().c_str(),"lep");
  }
  mgTau->Draw("AP");
  dr->prettify(mgTau);
  mgTau->GetYaxis()->SetRangeUser(4,500);
  mgTau->GetYaxis()->SetLimits(4,500);
  mgTau->GetXaxis()->SetRangeUser(3,400);
  mgTau->GetXaxis()->SetLimits(3,400);
  mgTau->GetXaxis()->SetTitle("Temperature [K]");
  mgTau->GetYaxis()->SetTitle("Carrier lifetime #tau [ns{}^{-1}]");

  // legTau->SetHeader("For #pm 400 V, #pm 500 V");
  // legTau->SetTextSize(0.04);
  legTau->Draw("same");
  TLatex *texTau = new TLatex(3.3,4.3,"MEASUREMENT");
  texTau->SetTextFont(42);
  // texTau->Draw("same");
  can->Update();
  can->Write();
  can->WaitPrimitive();
  legTau->Clear();
  delete(can);
  //------------------------------------------------------------





  //----------------- Prepare the tau data-------------------------------
  // from 4 to 60 and from 160 to 295 K
  TGraphErrors *grTau1[nSamples];
  TGraphErrors *grTau12[nSamples];
  TGraphErrors *grTau1All = new TGraphErrors();
  // double tempNTau1[nSamples][nTemp], tauTau1[nSamples][nTemp], tempNTaue1[nSamples][nTemp], tauTaue1[nSamples][nTemp];

  for (int32_t sample=0; sample<nSamples; sample++) {
    int32_t nTempTau70=0;
    int32_t nTempTau150=0;
    double taumean70=0;
    double taurms70=0;
    double taumean150=0;
    double taurms150=0;
    double taumeanall=0;
    double taurmsall=0;

    for (int32_t temp=0; temp<nTemp; temp++) {
      if (tempN[temp]<70) {
        for (int32_t volt=0; volt<nVoltages; volt++) {
          if (tauDecayTemp[sample][volt][temp] < -1.0) continue;
          taumean70 += tauDecayTemp[sample][volt][temp];
          nTempTau70++;
        }
      }
      else if (tempN[temp]>150) {
        for (int32_t volt=0; volt<nVoltages; volt++) {
          if (tauDecayTemp[sample][volt][temp] < -1.0) continue;
          taumean150 += tauDecayTemp[sample][volt][temp];
          nTempTau150++;
        }
      }
    }
    taumeanall = taumean70+taumean150;
    taumeanall = taumeanall/(nTempTau150+nTempTau70);
    taumean70 /= nTempTau70;
    taumean150 /= nTempTau150;
    for (int32_t temp=0; temp<nTemp; temp++) {
      if (tempN[temp]<70) {
        for (int32_t volt=0; volt<nVoltages; volt++) {
          if (tauDecayTemp[sample][volt][temp] < -1.0) continue;
          taurms70 += (taumean70-tauDecayTemp[sample][volt][temp])*(taumean70-tauDecayTemp[sample][volt][temp]);
        }
      }
      else if (tempN[temp]>150) {
        for (int32_t volt=0; volt<nVoltages; volt++) {
          if (tauDecayTemp[sample][volt][temp] < -1.0) continue;
          taurms150 += (taumean150-tauDecayTemp[sample][volt][temp])*(taumean150-tauDecayTemp[sample][volt][temp]);
        }
      }
    }
    taurmsall = taurms70 + taurms150;


    taurms70 = sqrt(taurms70/nTempTau70);
    taurms150 = sqrt(taurms150/nTempTau150);
    taurmsall = sqrt(taurmsall/(nTempTau150+nTempTau70));
    cout << taumean70 << " " << taurms70 << " " << taumean150 << " " << taurms150 << " "<< taumeanall << " "<< taurmsall <<endl;

    grTau1[sample]=new TGraphErrors();
    grTau1[sample]->SetPoint(0, 0, taumean70);
    grTau1[sample]->SetPointError(0, 0, taurms70);
    grTau1[sample]->SetPoint(1, 1, taumean150);
    grTau1[sample]->SetPointError(1, 0, taurms150);
    grTau1All->SetPoint(sample, irrad[sample], taumeanall);
    grTau1All->SetPointError(sample, irrade[sample], taurmsall);
  }




  //----------------- Plot mean and rms of +-4/500 decay vs Temperature-------------------------------
  cout<<endl<<" Plotting the MEAN decay vs temperature... "<<endl;
  can = new TCanvas("canmean","canmean",800,600);
  dr->prettify(can);
  can->SetLogy();
  can->SetLogx();
  TLegend* legTau1 = new TLegend(0.7,0.65,0.95,0.95); //for All, Odd and Even
  TMultiGraph *mgTau1 = new TMultiGraph();
  cout<<"1"<<endl;
  for (int32_t sample=0; sample<nSamples; sample++) {
    if (!grTau1[sample]->GetN()) {
      continue; //if the graph is empty, get out
    }
    dr->prettify(grTau1[sample]);
    grTau1[sample]->SetMarkerStyle(24-sample);
    // grTau[sample]->SetLineColor(clrScheme[sample][volt]);
    mgTau1->Add(grTau1[sample]);

    ssleg.str("");
    ssleg << sampleRealName[sample];// << "_" << voltage[volt];
    legTau1->AddEntry(grTau1[sample],ssleg.str().c_str(),"lep");
  }
  // cout<<"1"<<endl;
  mgTau1->Draw("AP");
  dr->prettify(mgTau1);
  mgTau1->GetYaxis()->SetRangeUser(4,500);
  mgTau1->GetXaxis()->SetLimits(4,500);
  mgTau1->GetXaxis()->SetRangeUser(3,450);
  mgTau1->GetXaxis()->SetLimits(3,450);
  mgTau1->GetXaxis()->SetTitle("Temperature [K]");
  mgTau1->GetYaxis()->SetTitle("#tau [s^{-1}]");
// cout<<"1"<<endl;
  // legTau1->SetHeader("For #pm 400 V, #pm 500 V");
  // legTau1->SetTextSize(0.04);
  legTau1->Draw("same");
  can->Update();
  can->Write();
  // can->WaitPrimitive();
  legTau1->Clear();
  delete(can);




  can = new TCanvas("canmean","canmean",800,600);
  dr->prettify(can);
  can->SetLogy();
  dr->prettify(grTau1All);
  grTau1All->SetMarkerStyle(20);
  grTau1All->Draw("AP");
  grTau1All->GetXaxis()->SetTitle("Radiation dose [10^{14} #pi cm^{-2}]");
  grTau1All->GetYaxis()->SetTitle("Carrier lifetime #tau [ns^{-1}]");
  grTau1All->GetXaxis()->SetRangeUser(-0.3,5);
  grTau1All->GetXaxis()->SetLimits(-0.3,5);
  grTau1All->GetYaxis()->SetRangeUser(4,505);
  grTau1All->GetYaxis()->SetLimits(4,505);
  TLatex *tex3 = new TLatex(-0.23,4.5,"MEASUREMENT");
  tex3->SetTextFont(42);
  // tex3->Draw("same");

  TF1* fitf = new TF1("fit","[0] / ([0]*[1]*x + 1) ", 0, 20);
  fitf->SetParameter(0,300);
  fitf->SetParLimits(0,100,700);
  fitf->SetParameter(1,3e-2);
  fitf->SetLineColor(kBlack);
  TFitResultPtr ptr = grTau1All->Fit("fit","SR");
  cout<<endl<<" --------------------------------"<<endl;
  cout<<" Fitted kTau = "<<ptr->Parameter(1)
      <<" +- "<<ptr->ParError(1)
      // <<" x 10^-18"
      <<endl;
  cout<<" --------------------------------"<<endl;
  TLegend *legfit = new TLegend(0.62,0.82,0.95,0.95);
  legfit->AddEntry(grTau1All, "Averaged lifetime","lep");
  legfit->AddEntry(fitf, "Fit: k = (3.5#pm0.8)#times10^{-16} ","L");
  legfit->Draw("same");




  can->Update();
  can->Write();
  can->WaitPrimitive();
  legTau1->Clear();
  delete(can);
  //------------------------------------------------------------















  //----------------- Prepare the tau data for decimated temp range--------------
  //remove all non-fitted inputs.
  TGraph* grTempDec[2][nSamples][nVoltages];
  int32_t nRemovedTempDec[2][nSamples][nVoltages];
  for (int32_t dec=0; dec<2; dec++) {
    for (int32_t sample=0; sample<nSamples; sample++) {
      for (int32_t volt=0; volt<nVoltages; volt++) {
        nRemovedTempDec[dec][sample][volt] = 0;
      }
    }
  }

  for (int32_t dec=0; dec<2; dec++) {
    for (int32_t sample=0; sample<nSamples; sample++) {
      for (int32_t volt=0; volt<nVoltages; volt++) {
        grTempDec[dec][sample][volt] = new TGraph(nTemp,tempN,tauDecayTempDec[dec][sample][volt]);
        for (int32_t temp=0; temp<nTemp; temp++) {
          // cout<<" sample "<<sample<<" temp "<<temp<<" voltage "<<volt
          //     <<" tauDecayTempOdd "<<tauDecayTempDec[dec][sample][volt][temp]<< endl;
          if (tauDecayTempDec[dec][sample][volt][temp] < -1.0){
            // -10 in case it's not found. we delete the entry.
            grTempDec[dec][sample][volt]->RemovePoint(temp-nRemovedTempDec[dec][sample][volt]);
            nRemovedTempDec[dec][sample][volt]++;
          }
        }
      }
    }
  }

  //----------------- Plot decimated decay vs Voltage-------------------------------
  cout<<endl<<" Plotting the decimated decay vs temperature... "<<endl;
  can = new TCanvas("can","can",800,600);
  dr->prettify(can);
  can->SetLogy();
  // TLine* line1;
  // TLine* line2;
  bool first = true;

  for (int32_t dec=0; dec<2; dec++) {
    for (int32_t sample=0; sample<nSamples; sample++) {
      for (int32_t volt=0; volt<nVoltages; volt++) {

        if (!grTempDec[dec][sample][volt]->GetN()) {
          continue; //if the graph is empty, get out
        }
        dr->prettify(grTempDec[dec][sample][volt]);

        if (first) { //first draw
          first = false;
          grTempDec[dec][sample][volt]->Draw ("AL.");
          grTempDec[dec][sample][volt]->GetYaxis()->SetRangeUser(1,450); //tau
          grTempDec[dec][sample][volt]->GetXaxis()->SetLimits(1,450); //temp
          grTempDec[dec][sample][volt]->GetXaxis()->SetTitle("Temperature [K]");
          grTempDec[dec][sample][volt]->GetYaxis()->SetTitle("#tau [s{}^{-1}]");
          // can->Update();
          // can->WaitPrimitive();
        }
        else {
          grTempDec[dec][sample][volt]->Draw ("L SAME");
        }
        grTempDec[dec][sample][volt]->SetLineColor(clrScheme[sample][volt]);
        ssleg.str("");
        ssleg << sampleName[sample] << "_" << voltage[volt];
        legTemp->AddEntry(grTempDec[dec][sample][volt],ssleg.str().c_str(),"l");
        // can->Update();
        // can->WaitPrimitive();
      }
    }
    legTemp->Draw("same");
    can->Update();
    can->Write();
    // can->WaitPrimitive();
    can->Clear();
    legTemp->Clear();
    first = true;
  }

  delete(can);
  //------------------------------------------------------------




  //----------------- Prepare the tau data-------------------------------
  //remove all non-fitted inputs.
  TH2D* mapTau[nSamples];
  int32_t nRemovedMap[nSamples];
  stringstream ssmap;
  for (int32_t sample=0; sample<nSamples; sample++) {
      nRemovedMap[sample] = 0;
  }
  for (int32_t sample=0; sample<nSamples; sample++) {
    ssmap.str("");
    ssmap << "Tau_map_"<<sampleName[sample];
    mapTau[sample] = new TH2D(ssmap.str().c_str(),ssmap.str().c_str(),
              nTemp, 0, nTemp, nVoltages, 0, nVoltages);
    // cout<<"sample "<< sample<<endl;
    for (int32_t volt=0; volt<nVoltages; volt++) {
      // cout<<"    volt "<< volt<<endl;
      for (int32_t temp=0; temp<nTemp; temp++) {
        // cout<<"       temp "<< temp;
        if (tauDecayTemp[sample][volt][temp] >= 0.0){
          mapTau[sample]->Fill(temp,volt,tauDecayTemp[sample][volt][temp]);
          // cout<<" filling "<< tauDecayTemp[sample][volt][temp];
        }
        // cout<<endl;
      }
    }
  }

  //----------------- Plot decimated decay vs Voltage-------------------------------
  cout<<endl<<" Plotting the map of decimated decay vs temperature... "<<endl;
  can = new TCanvas("can","can",nSamples*400,500);
  can->Divide(3,3);

  for (int32_t sample=0; sample<nSamples; sample++) {
    // dr->prettify(can->cd(sample));
    can->cd(sample+1);
    gPad->SetLogz();
    mapTau[sample]->Draw("colz");
    mapTau[sample]->GetZaxis()->SetRangeUser(1,450);
  }

  TH2D* mapTauDiff[nSamples];
  if (nSamples == 5) {
      mapTauDiff[0] = new TH2D(*mapTau[3]); //copy the S52
      mapTauDiff[1] = new TH2D(*mapTau[4]); //copy the S79
      mapTauDiff[0]->Divide(mapTau[0]);
      mapTauDiff[1]->Divide(mapTau[1]);
      can->cd(7);
      gPad->SetLogz();
      mapTauDiff[0]->Draw("colz");
      mapTauDiff[0]->GetZaxis()->SetRangeUser(0,1);
      can->cd(8);
      gPad->SetLogz();
      mapTauDiff[1]->Draw("colz");
      mapTauDiff[1]->GetZaxis()->SetRangeUser(0,1);
  }

  can->Update();
  can->Write();
  // can->WaitPrimitive();
  can->Clear();
  delete(can);
  //------------------------------------------------------------


  //---------------- close the file ----------------------------
  filePlots->Close();
  delete(filePlots);


  cout<<"All done."<<endl;
  delete(dr);
  delete(app);
  return 0;
}//-------------------- end of main -----------------------------------------
