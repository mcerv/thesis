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





using namespace std;

// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<"PLOTTING THE DBM DATABASE! "<<endl;


  //---------- DATABASE ENTRIES -------------------------
  const int nEntriesMax = 477;
  const int nColumns = 20;
  string tmp;
  string directory[nEntriesMax];
  string moduleID[nEntriesMax];
  int chipSN[nEntriesMax];
  int runNumber[nEntriesMax];
  string runType[nEntriesMax];
  string date[nEntriesMax];
  int configNum[nEntriesMax];
  int threshold[nEntriesMax];
  int voltage[nEntriesMax];
  double triggers[nEntriesMax];
  double effRaw[nEntriesMax];
  double effSignal[nEntriesMax];
  double effCorrected[nEntriesMax];
  double noiseFraction[nEntriesMax];
  double averageRate[nEntriesMax];
  double signalRate[nEntriesMax];
  double timeD[nEntriesMax];
  int totDrop[nEntriesMax];
  double lowPixels[nEntriesMax];
  double lowPixelsErr[nEntriesMax];

  string directoryBuf[nEntriesMax];
  string moduleIDBuf[nEntriesMax];
  int chipSNBuf[nEntriesMax];
  int runNumberBuf[nEntriesMax];
  string runTypeBuf[nEntriesMax];
  string dateBuf[nEntriesMax];
  int configNumBuf[nEntriesMax];
  int thresholdBuf[nEntriesMax];
  int voltageBuf[nEntriesMax];
  double triggersBuf[nEntriesMax];
  double effRawBuf[nEntriesMax];
  double effSignalBuf[nEntriesMax];
  double effCorrectedBuf[nEntriesMax];
  double noiseFractionBuf[nEntriesMax];
  double averageRateBuf[nEntriesMax];
  double signalRateBuf[nEntriesMax];
  double timeDBuf[nEntriesMax];
  int totDropBuf[nEntriesMax];
  double lowPixelsBuf[nEntriesMax];
  double lowPixelsErrBuf[nEntriesMax];

  //accepted modules
  const int nDiamond = 18;
  string acceptedModules[nDiamond] = {"MDBM-03", "MDBM-09", "MDBM-12", "MDBM-106",
                                      "MDBM-21", "MDBM-22", "MDBM-23", "MDBM-24",
                                      "MDBM-25", "MDBM-26", "MDBM-27", "MDBM-28",
                                      "MDBM-31", "MDBM-33", "MDBM-34", "MDBM-35",
                                      "MDBM-36", "MDBM-37"};
  //-----------------------------------------------------

  TApplication* app = new TApplication("app",0,0);
  DrawFuns* dr = new DrawFuns();
  stringstream ss;
  // ss << "../data/runsOcc1.csv";
  ss << "../data/runs1.csv";
  cout << "Opening " << ss.str() << endl;
  ifstream f (ss.str().c_str());


  for (int col=0; col<nColumns; col++) {
    f >> tmp;
    cout << tmp << ", ";
  }
  cout << endl;

  for (int i=0; i<nEntriesMax; i++) {
    f >> directoryBuf[i];
    f >> moduleIDBuf[i];
    f >> chipSNBuf[i];
    f >> runNumberBuf[i];
    f >> runTypeBuf[i];
    f >> dateBuf[i];
    f >> configNumBuf[i];
    f >> thresholdBuf[i];
    f >> voltageBuf[i];
    f >> triggersBuf[i];
    f >> effRawBuf[i];
    f >> effSignalBuf[i];
    f >> effCorrectedBuf[i];
    f >> noiseFractionBuf[i];
    f >> averageRateBuf[i];
    f >> signalRateBuf[i];
    f >> timeDBuf[i];
    f >> totDropBuf[i];
    f >> lowPixelsBuf[i];
    f >> lowPixelsErrBuf[i];
  }
  f.close();
  cout << " The database is buffered in. Cleaning up according to cuts... " << endl;

  int cnt = 0;
  for (int i = 0; i < nEntriesMax; i++) {
    //apply cuts
    if ( !runTypeBuf[i].compare("Unknown") )            continue;
    if ( !runTypeBuf[i].compare("Occupancy") )          continue;
    // if ( !moduleIDBuf[i].compare(0,4,"MSBM") )            continue;
    // if ( voltageBuf[i] == 0 )                           continue;
    if ( triggersBuf[i] < 30000 )                       continue;
    if ( noiseFractionBuf[i] > 0.3 )                       continue;
    directory[cnt]        = directoryBuf[i];
    moduleID[cnt]         = moduleIDBuf[i];
    chipSN[cnt]           = chipSNBuf[i];
    runNumber[cnt]        = runNumberBuf[i];
    runType[cnt]          = runTypeBuf[i];
    date[cnt]             = dateBuf[i];
    configNum[cnt]        = configNumBuf[i];
    threshold[cnt]        = thresholdBuf[i];
    voltage[cnt]          = voltageBuf[i];
    triggers[cnt]         = triggersBuf[i];
    effRaw[cnt]           = effRawBuf[i];
    effSignal[cnt]        = effSignalBuf[i];
    effCorrected[cnt]     = effCorrectedBuf[i];
    noiseFraction[cnt]    = noiseFractionBuf[i];
    averageRate[cnt]      = averageRateBuf[i];
    signalRate[cnt]       = signalRateBuf[i];
    timeD[cnt]            = timeDBuf[i];
    totDrop[cnt]          = totDropBuf[i];
    lowPixels[cnt]        = lowPixelsBuf[i];
    lowPixelsErr[cnt]     = lowPixelsErrBuf[i];
    cnt++;
  }
  const int nEntries = cnt;

  cout << " After cuts there are " << nEntries << " out of "
       << nEntriesMax << " entries left. " << endl;


  //============================================================================
  cout << " Plotting all corrected efficiencies: " << endl;
  TCanvas *c = new TCanvas ("eff","eff", 800, 600);
  dr->prettify(c);
  TH1D *histEffCorrected = new TH1D ("histeffcorrected","histeffcorrected",50, 0, 100);
  TH1D *histEffCorrectedAcc = new TH1D ("histeffcorrectedAcc","histeffcorrectedAcc",50, 0, 100);
  TH1D *histEffCorrectedSi = new TH1D ("histeffcorrectedsi","histeffcorrectedsi",50, 0, 100);
  for (int i=0; i<nEntries; i++) {
    if ( !runType[i].compare("Occupancy") )          continue;
    if ( !moduleID[i].compare(0,4,"MDBM") )
      histEffCorrected->Fill(effCorrected[i]*100);
    else
      histEffCorrectedSi->Fill(effCorrected[i]*100);

    bool isAccepted = false;
    for (int d=0; d<nDiamond; d++) {
      if (!moduleID[i].compare( acceptedModules[d] ) )
        isAccepted = true;
    }
    if (isAccepted)
      histEffCorrectedAcc->Fill(effCorrected[i]*100);
  }

  dr->prettify(histEffCorrected);
  dr->prettify(histEffCorrectedAcc, "red");
  dr->prettify(histEffCorrectedSi, "blue");
  histEffCorrected->Draw();
  histEffCorrectedSi->Draw("same");
  histEffCorrected->GetXaxis()->SetTitle("Pseudo-efficiency [\%]");
  histEffCorrected->GetYaxis()->SetTitle("Entries");
  histEffCorrected->GetYaxis()->SetRangeUser(0,25);
  TLegend *leg = new TLegend (0.2, 0.7, 0.5, 0.9);
  leg->AddEntry(histEffCorrected, "C modules", "L");
  leg->AddEntry(histEffCorrectedSi, "Si modules", "L");
  leg->Draw("same");
  // histEffCorrectedAcc->Draw("same");
  c->Update();
  c->WaitPrimitive();
  c->Update();
  delete histEffCorrected;
  delete histEffCorrectedSi;
  delete histEffCorrectedAcc;
  delete c;


  //============================================================================
  cout << " Plotting all disconnected pixels: " << endl;
  c = new TCanvas ("disconnected","disconnected", 800, 600);
  dr->prettify(c);
  TH1D *histDisconnected = new TH1D ("histDisconnected","histDisconnected",50, 0, 30);
  TH1D *histDisconnectedSi = new TH1D ("histDisconnectedSi","histDisconnectedSi",50, 0, 30);
  int cntOcc=0;
  int cntOccAcc=0;
  for (int i=0; i<nEntries; i++) {
    if ( runType[i].compare("Occupancy") )          continue; //only occupancy now
    if ( lowPixels[i] < 0) lowPixels[i] = 1;
    if ( !moduleID[i].compare(0,4,"MDBM") )
      histDisconnected->Fill((double)lowPixels[i]/268.800);
    else
      histDisconnectedSi->Fill((double)lowPixels[i]/268.800);
    cntOcc++;

    // bool isAccepted = false;
    // for (int d=0; d<nDiamond; d++) {
    //   if (!moduleID[i].compare( acceptedModules[d] ) )
    //     isAccepted = true;
    // }
    // if (isAccepted) {
    //   histDisconnectedAcc->Fill(lowPixels[i]);
    //   cntOccAcc++;
    // }
  }
  cout << " Found " << cntOcc << " occupancy runs, " << cntOccAcc << " accepted." << endl;
  dr->prettify(histDisconnected, "red");
  dr->prettify(histDisconnectedSi, "blue");
  histDisconnectedSi->Draw();
  histDisconnected->Draw("same");
  histDisconnectedSi->GetXaxis()->SetTitle("Disconnected region [\%]");
  histDisconnectedSi->GetYaxis()->SetTitle("Entries");
  TLegend *leg1 = new TLegend(0.6, 0.7, 0.9, 0.9);
  leg1->AddEntry(histDisconnected, "C modules", "L");
  leg1->AddEntry(histDisconnectedSi, "Si modules", "L");
  leg1->Draw("same");
  c->Update();
  c->WaitPrimitive();
  c->Update();

  delete histDisconnected;
  delete histDisconnectedSi;
  delete c;



  // f >> month[0] >> month[1] >> month[2]
  //   >> month[3] >> month[4] >> month[5]
  //   >> month[6] >> month[7] >> month[8];
  // f >> IBL[0] >> IBL[1] >> IBL[2]
  //   >> IBL[3] >> IBL[4] >> IBL[5]
  //   >> IBL[6] >> IBL[7] >> IBL[8];
  // f >> Si[0] >> Si[1] >> Si[2]
  //   >> Si[3] >> Si[4] >> Si[5]
  //   >> Si[6] >> Si[7] >> Si[8];
  // f >> C[0] >> C[1] >> C[2]
  //   >> C[3] >> C[4] >> C[5]
  //   >> C[6] >> C[7] >> C[8];
  // f >> C1[0] >> C1[1] >> C1[2]
  //   >> C1[3] >> C1[4] >> C1[5]
  //   >> C1[6] >> C1[7] >> C1[8];
  // f.close();
  //
  // for (int32_t i=0; i<9; i++) {
  //   cout << month[i] << " : "
  //        << " ibl " << IBL[i]
  //        << " Si  "  << Si[i]
  //        << " C  "   << C[i]
  //        << " C1  "   << C1[i]
  //        << endl;
  // }
  //
  // Long64_t year = 2013;
  // const Int_t n = 9;
  // // string months[n] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  // int X0 = TDatime(1995,01,01,00,00,00).Convert();
  // int startDate = TDatime(year, 02, 01, 00, 00, 00).Convert() - X0;
  // int endDate = TDatime(year, 10, 31, 23, 59, 59).Convert() - X0;
  // int timeData[n]={0};
  // int yData[n]={0};
  // TCanvas *canvas = new TCanvas("canvas","Yearly Plot",800,600);
  // dr->prettify(canvas);
  //
  // for (Long64_t j=0; j < n; j++){
  //         timeData[j] = TDatime(year, j+2, 15, 00, 00, 00).Convert() - X0;
  // }
  //
  //
  //
  // TH1F *h = new TH1F("h","test",n,startDate,endDate);
  // dr->prettify(h);
  // h->SetLineColor(kBlack);
  // h->SetLineWidth(1);
  // h->GetXaxis()->SetNdivisions(-n);
  // for (Long64_t j=0;j<n;j++) h->Fill(month[j].c_str(),0);
  // h->SetMinimum(0);
  // h->SetMaximum(40);
  // h->Draw();
  //
  // TGraph *gr1 = new TGraph(n,timeData,IBL);
  // TGraph *gr2 = new TGraph(n,timeData,Si);
  // TGraph *gr3 = new TGraph(n,timeData,C);
  // TGraph *gr4 = new TGraph(n,timeData,C1);
  // dr->prettify(gr1);
  // dr->prettify(gr2);
  // dr->prettify(gr3);
  // dr->prettify(gr4);
  // gr1->SetMarkerStyle(20);
  // gr2->SetMarkerStyle(21);
  // gr3->SetMarkerStyle(22);
  // gr4->SetMarkerStyle(23);
  // gr1->SetMarkerSize(1.4);
  // gr2->SetMarkerSize(1.4);
  // gr3->SetMarkerSize(1.4);
  // gr4->SetMarkerSize(1.4);
  // gr1->Draw("PL");
  // gr2->Draw("PL");
  // gr3->Draw("PL");
  // gr4->Draw("PL");
  //
  // TLegend *leg = new TLegend(0.25,0.6,0.60,0.87);
  // leg->AddEntry(gr1, "IBL, 12 units", "LEP");
  // leg->AddEntry(gr2, "Si, 24 units", "LEP");
  // leg->AddEntry(gr3, "C, 43 units", "LEP");
  // leg->AddEntry(gr4, "C accepted, 20 units", "LEP");
  //
  // leg->Draw("same");
  //
  // h->GetYaxis()->SetTitle("Produced units");
  // h->GetXaxis()->SetTitle("2013");
  // h->GetYaxis()->SetRangeUser(0,45);
  // canvas->Update();
  // canvas->WaitPrimitive();


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
