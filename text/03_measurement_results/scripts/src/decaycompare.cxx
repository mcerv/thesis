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

#define DEBUG 0
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4




using namespace std;

double bisection (double t0, double t1, double current0, double area0) {
  const int32_t maxIterations = 100;
  const double minError = 1e-4;
  double rangeMin = 0.1;
  double rangeMax = 400;
  TF1* decayarea = new TF1 ("decayarea", "[0] - [1]*( -x*exp(-[3]/x) + x*exp(-[2]/x) )", 0, 400);
  decayarea->SetParameter(0,area0); //pulse area [uA * ns]
  decayarea->SetParameter(1,current0); //initial charge / current [uA]
  decayarea->SetParameter(2,t0); //time 0 [ns]
  decayarea->SetParameter(3,t1); //time 1 [ns]

  double tau = 0; //starting point of Tau
  double y = 0;  //has to be minimised towards 0
  int32_t iteration=0;
  for (iteration=0; iteration<maxIterations; iteration++) {
    tau = (rangeMax+rangeMin)/2;
    y = decayarea->Eval(tau);
    if (y==0.0) {
      break; //we found the exact tau;
    } else if (y > 0.0) {
      rangeMin = tau;
    } else if (y < 0.0) {
      rangeMax = tau;
    }
    if (abs(y) < minError) {
      break;
    }
  }

  cout<<" Bisection results: Tau="<<tau<<" y="<<y<<" nIterations="<<iteration<<endl;
  if (DEBUG == 2) {
    TCanvas* can = new TCanvas("can","can",800,600);
    decayarea->Draw();
    can->Update();
    can->WaitPrimitive();
    can->Update();
    can->Clear();
    delete can;
  }
  delete decayarea;
  return tau;
}



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

  //set global variables and objects
  stringstream ss;
  // TCanvas* can = new TCanvas("can","can",800,600);
  TFile* f = new TFile ("plots/plots.root","read");// = new TFile();
  if (!f->IsOpen()) {
    cout<<" File " << ss.str().c_str() << " not open properly!" << endl;
    return 0;
  }



  double samplingTime = 0.1; //ns
  //set the input data
  const int nSamples = 5;
  const int nVoltages = 4;
  // const int nVoltages = 6;
  const int nTemp = 18;

  // double voltage[nVoltages] = {500.0, 400.0, 300.0, 200.0, 100.0, -100.0, -200.0, -300.0, -400.0, -500.0};
  // double voltage[nVoltages] = {700.0, 600.0, 500.0, 400.0, -400.0, -500.0, -600.0, -700.0};
  // double voltage[nVoltages] = {700.0, -700.0};
  // double voltage[nVoltages] = {700.0, 600.0, 500.0, 400.0, 300.0, 200.0, 100.0,
  //                       -100.0, -200.0, -300.0, -400.0, -500.0, -600.0, -700.0};
  double voltage[nVoltages] = {500.0, 400.0, -400.0, -500.0};
  string sampleName[nSamples] = {"S52","S79","S37","S52_3-63e14","S79_1e14" };
  // string sampleName[nSamples] = {"S79_1e14","S52_3-63e14","S37"};
  // string sampleName[nSamples] = {"S52", "S52_3-63e14"};
  // string sampleName[nSamples] = {"S79_1e14"};
  string tempS[nTemp] = {"4k","7k","10k","15k","25k","40k","60k","75k","95k",
              "115k","135k","160k","180k","210k","230k","260k","280k","295k"};
  double tempN[nTemp] = {4,7,10,15,25,40,60,75,95,115,135,160,180,210,230,260,280,295};

  vector< vector< vector <TProfile> > > profvec;
  vector< vector< vector <bool> > > missingprof;

  double spaceCharge[nSamples][nTemp][nVoltages];
  double chi2[nSamples][nTemp][nVoltages];
  double maxNonIrrad[nSamples][nTemp][nVoltages];
  double spaceChargeDisp[nSamples][nTemp][nVoltages];
  double tauDecayVolt[nSamples][nTemp][nVoltages];
  double tauDecayVoltDec[2][nSamples][nTemp][nVoltages];
  double tauDecayTemp[nSamples][nVoltages][nTemp];
  double tauDecayArea[nSamples][nVoltages][nTemp];
  double tauDecayTempDec[2][nSamples][nVoltages][nTemp];
  for (int32_t i=0; i<nSamples; i++) {
    for (int32_t j=0; j<nTemp; j++) {
      for (int32_t k=0; k<nVoltages; k++) {
        spaceCharge[i][j][k] = 0; // space charge for individual pulses non-irrad
        spaceChargeDisp[i][j][k] = 0; // space charge displacement for individual pulses non-irrad
        tauDecayVolt[i][j][k] = -10; // -10 in case it's not found
        tauDecayVoltDec[1][i][j][k] = -10; // -10 in case it's not found
        tauDecayVoltDec[0][i][j][k] = -10; // -10 in case it's not found
        tauDecayArea[i][k][j] = -10; // -10 in case it's not found
        tauDecayTemp[i][k][j] = -10; // -10 in case it's not found
        tauDecayTempDec[1][i][k][j] = -10; // -10 in case it's not found
        tauDecayTempDec[0][i][k][j] = -10; // -10 in case it's not found
        maxNonIrrad[i][j][k] = -10; //-10 in case it's not found
        chi2[i][j][k] = -10; //-10 in case it's not found
      }
    }
  }



  //get the profiles from the file
  profvec.resize(nSamples);
  missingprof.resize(nSamples);
  for (int32_t sample = 0; sample < nSamples; sample++) {
    profvec.at(sample).resize(nTemp);
    missingprof.at(sample).resize(nTemp);
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        ss.str("");
        ss << "Sample_" << sampleName[sample] << "/" << sampleName[sample]
           << "_" << tempS[temp] << "_" << voltage[volt];

        TProfile prof;

        if (!(TProfile*)f->Get(ss.str().c_str() ) ) {
          cout<<" File " << ss.str().c_str() << " doesn't include " << ss.str() << endl;
          missingprof.at(sample).at(temp).push_back(true);
          profvec.at(sample).at(temp).push_back(prof);
          continue;
        }
      	prof = (*(TProfile*)f->Get(ss.str().c_str() ));
        // prof[sample][temp][volt] = new TProfile(*(TProfile*)f->Get("avgpulses_filt"));
        // *prof[sample][temp][volt] = *(TProfile*)f->Get("avgpulses_filt");
        // prof->SetTitle(ss.str().c_str());
        profvec.at(sample).at(temp).push_back(prof);
        missingprof.at(sample).at(temp).push_back(false);

      }
    }
  }
  cout<<endl;
  f->Close();
  delete(f);





  //create the fit function
  TF1* decayfun = new TF1("decay","[4]*x + [5] + [6] + [0] + ([1]-[0])*exp(-(x+[3])/[2])",1.5,7.5);
  TF1* decaydoublefun = new TF1("decaydouble","[0] + ([1]-[0])*exp(-(x+[3])/[2])",1.5,7.5);
  TF1* spacechgfun = new TF1("spacechg","[1]*x + [0]",1.5,7.5);

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
  decaydoublefun->SetParLimits(3,-2,1);



  TCanvas *can = new TCanvas("Irrad","Irrad",800,600);

  //fit the function to the profiles, but only to irrad ones
  cout<<"Fitting a decay function.."<<endl;
  for (int32_t sample = S52_363e14; sample < nSamples; sample++) {
    for (int32_t temp = 0; temp < nTemp; temp++) {
      for (int32_t volt = 0; volt < nVoltages; volt++) {
        if (missingprof[sample][temp][volt]) {
          continue; //not a profile here
        }
        cout<<"\r"<<" sam "<<sample<<" temp "<<temp<<" volt "<<volt<<flush;

        //set the fit limits
        //get the max and thres of the top
        double max = (double)profvec[sample][temp][volt].GetMaximum();
        double maxBin = profvec[sample][temp][volt].GetMaximumBin();
        double marginFall = 0 ;
        double marginRise = 0 ;
        double thresRise = 0;
        double thresFall = 0;

        if (temp <= 7) { //7 -> 75 K
          marginFall = 25;
          marginRise = 30;
          thresRise = max * 0.1; //because the tail is longer, 20%
          thresFall = max * 0.25;
        } else if (8 <= temp && temp < 11) { //between 75k and (excl)160k
          marginFall = 20;
          marginRise = 35;
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
          if (profvec[sample][temp][volt].GetBinContent(bin) >= thresRise) {
            riseEdgeBin = bin;
          }
        }
        for (int32_t bin=maxBin; bin<(maxBin+200); bin++) {
          if (profvec[sample][temp][volt].GetBinContent(bin) >= thresFall) {
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

        //transform bin numbers into [ns] - fit cut
        double fitMinBin = riseEdgeBin + marginRise;
        double fitMaxBin = fallEdgeBin - marginFall;
        double fitMin = (-500 + riseEdgeBin + marginRise)*0.1;
        double fitMax = (-500 + fallEdgeBin - marginFall)*0.1;

        //-------------------------- FIT FUNCTIONS ------------------------
        //for the non-irradiated sample take the maximum and save it.

        // for the irradiatied samples (and S37), fix the space charge and fit the decay.
        // if ( !sampleName[sample].compare("S79_1e14") ||
        //      !sampleName[sample].compare("S52_3-63e14") ||
        //      !sampleName[sample].compare("S37") ) {
        int32_t sampleNonIrrad = 0;
        if(1) {
          //fix the space charge of the non-irrad
          sampleNonIrrad=sample;   //-----** THIS NEEDS TO BE CHANGED BY HAND!!!!!! TODO
          if ( sample==S79_1e14) {
            sampleNonIrrad = S79;
          }
          if ( sample==S52_363e14 ) {
            sampleNonIrrad = S52;
          }

          //set the max current/charge
          double maxCur = profvec[sampleNonIrrad][temp][volt].GetMaximum() ;
          //get the area from binMin to binMax
          double area = 0;
          for (int32_t bin=fitMinBin; bin<fitMaxBin; bin++) {
            area+= profvec[sample][temp][volt].GetBinContent(bin);
          } //area in 0.1 ns * uA
          area /= 10; //to get it in ns
          cout << "fitMin "<< fitMin << " fitMax " << fitMax << " maxCur "
               << maxCur << " area " << area << endl;
          tauDecayArea[sample][volt][temp] = bisection (fitMin,fitMax,maxCur,area);


          // special fit with p0 fixed
          decaydoublefun->FixParameter(1,profvec.at(sampleNonIrrad).at(temp).at(volt).GetMaximum() );

          TFitResultPtr fptr = profvec[sample][temp][volt].Fit("decaydouble","RSQ", "", fitMin, fitMax);
          if (!fptr->IsEmpty()) { //if the fit is not empty, fill arrays
            tauDecayVolt[sample][temp][volt] = fptr->Parameter(2); //tau fit
            tauDecayTemp[sample][volt][temp] = fptr->Parameter(2); //tau fit
            chi2[sample][volt][temp] = decaydoublefun->GetChisquare()/decaydoublefun->GetNDF();
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
                <<" max "<<fptr->Parameter(1)
                <<" xDisp "<<fptr->Parameter(3)
                <<" Chi2/NDF "<<decaydoublefun->GetChisquare()/decaydoublefun->GetNDF()
                <<flush<<endl;
          }
          else {
            continue; //empty fit
          }

        } // ----------- end fitting tau --------


        //-----------------------------------------------------------------
        if (DEBUG) {
          profvec.at(sampleNonIrrad).at(temp).at(volt).Draw();
          profvec.at(sample).at(temp).at(volt).Draw("same");
          can->Update();
          can->WaitPrimitive();
          can->Clear();
        }

      }
    }
  }
  delete(decaydoublefun);
  delete(can);
  cout<<endl<<" ------------ Fitting finished.. -----------"<<endl<<endl;
  //----------------- end of decay fitting ---------------------

//
//
//
//
//
//
//
//
//
//   //------------ open file for saving plots --------------------------
//   TFile* filePlots = new TFile("plots/plots.root","recreate");
//   //------------------------------------------------------------------
//

  //----------------- Prepare the tau data-------------------------------
  //remove all non-fitted inputs.
  TH2D* mapTau[nSamples];
  TH2D* mapTauArea[nSamples];
  TH2D* mapChi2[nSamples];
  TH1D* histChi2[nSamples];
  int32_t nRemovedMap[nSamples];
  int32_t nRemovedChi2[nSamples];
  stringstream ssmap;

  for (int32_t sample=0; sample<nSamples; sample++) {
      nRemovedMap[sample] = 0;
      nRemovedChi2[sample] = 0;
  }
  for (int32_t sample=0; sample<nSamples; sample++) {
    ssmap.str("");
    ssmap << "Tau_area_map_"<<sampleName[sample];
    mapTauArea[sample] = new TH2D(ssmap.str().c_str(),ssmap.str().c_str(),
              nTemp, 0, nTemp, nVoltages, 0, nVoltages);
    ssmap.str("");
    ssmap << "Tau_map_"<<sampleName[sample];
    mapTau[sample] = new TH2D(ssmap.str().c_str(),ssmap.str().c_str(),
              nTemp, 0, nTemp, nVoltages, 0, nVoltages);
    ssmap << "_Chi2";
    mapChi2[sample] = new TH2D(ssmap.str().c_str(),ssmap.str().c_str(),
              nTemp, 0, nTemp, nVoltages, 0, nVoltages);
    ssmap << "_hist";
    histChi2[sample] = new TH1D(ssmap.str().c_str(),ssmap.str().c_str(),
              nTemp*nVoltages, 0, nTemp*nVoltages);
    for (int32_t volt=0; volt<nVoltages; volt++) {
      for (int32_t temp=0; temp<nTemp; temp++) {
        if (tauDecayTemp[sample][volt][temp] >= 0.0){
          mapTau[sample]->Fill(temp,volt,tauDecayTemp[sample][volt][temp]);
          mapTauArea[sample]->Fill(temp,volt,tauDecayArea[sample][volt][temp]);
          mapChi2[sample]->Fill(temp,volt,chi2[sample][volt][temp]);
          histChi2[sample]->Fill(nTemp*volt + temp, chi2[sample][volt][temp]);
        }
        else {
          histChi2[sample]->Fill(nTemp*volt + temp, -1);
        }
      }
    }
  }

  //----------------- Plot decimated decay vs Voltage-------------------------------
  cout<<endl<<" Plotting the map of decimated decay vs temperature... "<<endl;
  can = new TCanvas("can","can",nSamples*400,1000);
  can->Divide(5,2);
  TH2D* mapTauDiff[nSamples];
  int32_t cntCd = 1;
  for (int32_t sample=S52_363e14; sample<nSamples; sample++) {
    // dr->prettify(can->cd(sample));
    dr->prettify(can->cd(cntCd), "th2");
    gPad->SetLogz();
    mapTau[sample]->Draw("colz");
    mapTau[sample]->GetZaxis()->SetRangeUser(0.01,45);

    cntCd++;
    dr->prettify(can->cd(cntCd), "th2");
    mapChi2[sample]->Draw("colz");
    mapChi2[sample]->GetZaxis()->SetRangeUser(0.01,45);

    cntCd++;
    dr->prettify(can->cd(cntCd));
    histChi2[sample]->Draw();

    cntCd++;
    dr->prettify(can->cd(cntCd), "th2");
    gPad->SetLogz();
    mapTauArea[sample]->Draw("colz");
    mapTauArea[sample]->GetZaxis()->SetRangeUser(0.01,45);

    cntCd++;
    mapTauDiff[sample] = new TH2D(*mapTau[sample]); //copy the S52
    mapTauDiff[sample]->Divide(mapTauArea[sample]);
    dr->prettify(can->cd(cntCd), "th2");
    gPad->SetLogz();
    mapTauDiff[sample]->Draw("colz");
    mapTauDiff[sample]->GetZaxis()->SetRangeUser(0.01,1);
    cntCd++;
  }

  can->Update();
  // can->Write();
  can->WaitPrimitive();
  can->Clear();
  delete(can);
  //------------------------------------------------------------


  for (int32_t i=0; i<profvec.size(); i++) {
    for (int32_t j=0; j<profvec.at(i).size(); j++) {
        profvec.at(i).at(j).clear();
        missingprof.at(i).at(j).clear();
    }
  }

  delete(dr);
  delete(app);
  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
