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
  WaveformAna* wana = new WaveformAna();

  const int nSamples = 3;
  const int nVoltages = 15;

  float voltage[nVoltages] = {500.0, 300.0, 150.0, 75.0, 70.0, 30.0, -900.0, -800.0,
                             -700.0, -600.0, -500.0, -300.0, -150.0, -70.0, -30.0};
  string sampleName[nSamples] = {"S79","S52","S37"};
  string sampleRealName[nSamples] = {"S52","S79","S37"};
  float sampleThick[nSamples] = {515.0, 529.0, 548.0};  //um -- REAL

  TGraphErrors *gr[nSamples];
  TCanvas *c[nSamples];
  TCanvas *cHist;
  TH1F* histLan[10];
  TFile *f;
  cout<<"Starting"<<endl;
  float radPtFlux[3] = {3.63, 1, 0};
  float radPtFluxe[3] = {0.726, 0.2, 0};
  int cntRadPt = 0;
  float radPoint[100];
  float radPointe[100];



  for (int sample = 0; sample < nSamples; sample++) {
    float mean[100];
    float meane[100];
    float mpv[100];
    float mpve[100];
    float ccd[100];
    float ccde[100];
    float Q[100];
    float Qe[100];
    float field[100];
    float fielde[100];

    int num = 0;
    const float amp = 9.0; // mV/fC
    const float correctS37 = 1;//0.9;   // to account for surface problems
    const float correctS79 = 1.12;  // because I haven't taken highest CCD points
    // 1 C = 6.241e18 elecs ->
    const float fcToElecs = 6.241e3;

    string fileName = "/Volumes/MACSTORAGE/conv/";
    fileName += sampleName[sample];
    fileName += "/wanaResults.root";
    cout<<"Filename "<<fileName<<endl;
    f = new TFile(fileName.c_str());
    if (!f->IsOpen())
    {
      cout<<"****File "<< fileName <<" not open properly!"<<endl;
    }
    else
    {
      cout<<"File "<<fileName<<" open."<<endl;
    }


    //for every voltage
    for (int volt = 0; volt < nVoltages; volt++)
    {
      //hist name
      stringstream ss;
      ss << "Amplitude distribution for " << voltage[volt];
      string histName = ss.str();
      cout<<"Histname "<<histName<<endl;

      //open hist
      TH1F *hist = new TH1F();

      //read from the hist
      if ( f->Get(histName.c_str()) )
      {
        hist = (TH1F*)f->Get(histName.c_str());
        wana->loadLandau(*hist);
        if (voltage[volt]>-520) {
          wana->updateHistos("SHOW");
        } else {
          wana->updateHistos("SHOW CUT");
        }
        wana->fitLandau(mpv[num], mpve[num]);

        if (!volt) {
          //for 500 V
          // *histLan[sample] = *hist;
        }
        // cout<<" mpv["<<num<<"] = "<<mpv[num]<<"  mpve["<<num<<"] = "<<mpve[num]<<endl;

        //---- calculate ch. coll, distance ----
        Q[num] = mpv[num] / amp * 1e3 * fcToElecs; // in elecs.
        cout<<" mpv[V]="<<mpv[num]*1e3<<"  amp[mV/fC]="<<amp<<"  Q[fC]="<<Q[num]<<endl;
        ccd[num] =  Q[num] / 36; //micrometres //sampleThick[sample] /
        ccde[num] =  ccd[num] * mpve[num] / mpv[num];
        cout<<" ccd: "<<ccd[num]<<"  err: "<<ccde[num]<<endl;
        if (!sampleRealName[sample].compare("S52")) {
          if (volt==6) { // these will be plotted for fluence fitting
            radPoint[cntRadPt] = ccd[num];
            radPointe[cntRadPt] = ccde[num];
            cntRadPt++;
          }
        } else if (!sampleRealName[sample].compare("S79"))  {
          if (volt==0) { // these will be plotted for fluence fitting
            radPoint[cntRadPt] = ccd[num]*correctS79;
            radPointe[cntRadPt] = ccde[num]*correctS79;
            cntRadPt++;
          }
        } else if (!sampleRealName[sample].compare("S37")) {
          if (volt==10) { // these will be plotted for fluence fitting
            radPoint[cntRadPt] = ccd[num]*correctS37;
            radPointe[cntRadPt] = ccde[num]*correctS37;
            cntRadPt++;
          }
        }


        //-------------------------------------

        mean[num] = hist->GetMean();
        meane[num] = hist->GetRMS();
        field[num] = voltage[volt] / sampleThick[sample];
        fielde[num] = abs(field[num]) * 0.01; //1% error on fieldage
        num++;
        cout<<num<<endl;

        //save histogram
        stringstream ssave;
        ssave << "pics/" << sampleName[sample] << "_"
              << voltage[volt] << ".pdf";
        string saveName = ssave.str();
        cHist = new TCanvas("c1","c1",800,600);
        // cHist->cd();
        // hist->Draw();
        // cHist->Update();
        // cHist->WaitPrimitive();
        // cHist->SaveAs( saveName.c_str() );
        if (hist) delete hist;
        if (cHist) delete cHist;
      }
      else // if the hist doesn't exist, skip to next one
      {
        continue;
      }
    }//end of voltage for loop.

    c[sample] = new TCanvas(sampleName[sample].c_str(),sampleName[sample].c_str(),800,600);
    dr->prettify(c[sample]->cd() );
    // gr[sample] = new TGraphErrors(num,field,mean,fielde,meane);
    gr[sample] = new TGraphErrors(num,field,ccd,fielde,ccde);
    dr->prettify(gr[sample]);

    // gr[sample]->GetYaxis()->SetRangeUser(0,0.05);
    // gr[sample]->Draw("A.");
    gr[sample] = new TGraphErrors(num,field,ccd,fielde,ccde);

    // c[sample]->Update();
    // c[sample]->WaitPrimitive();

    //save to a file
    stringstream ssccd;
    ssccd.str("");
    ssccd << "ccd_" << sampleName[sample] << ".txt";
    ofstream textfile (ssccd.str().c_str());
    // textfile.open("ccd.txt");
    for (int32_t i=0; i<num; i++) {
      textfile  << field[i] << "\t"
                << ccd[i] << "\t"
                << fielde[i] << "\t"
                << ccde[i] << "\n";
    }
    textfile.close();



    //save
    stringstream ssave1;
    ssave1 << "pics/" << sampleName[sample] << ".pdf";
    string saveName1 = ssave1.str();
    c[sample]->SaveAs( saveName1.c_str() );





    if (f) delete f;
  }//end of sample for loop




  TMultiGraph* mg = new TMultiGraph();
  mg->Add(gr[0]);
  mg->Add(gr[1]);
  mg->Add(gr[2]);

  TCanvas* c2 = new TCanvas("together","together",800,600);
  dr->prettify(c2);
  dr->prettify(gr[0]);
  dr->prettify(gr[1]);
  dr->prettify(gr[2]);
  gr[1]->SetLineColor(kBlue+1);
  gr[2]->SetLineColor(kBlack);
  gr[0]->Draw("AP");
  gr[0]->GetYaxis()->SetRangeUser(0,600);
  gr[0]->GetXaxis()->SetRangeUser(-2,1);
  gr[1]->Draw("P SAME");
  gr[2]->Draw("P SAME");
  gr[0]->GetXaxis()->SetTitle("Bias voltage [V/ #mu m]");
  gr[0]->GetYaxis()->SetTitle("Charge collection distance [ #mu m]");

  TLegend* leg1 = new TLegend(0.7,0.7,0.95,0.95);
  leg1->AddEntry(gr[2],"S37 non-irrad","L");
  leg1->AddEntry(gr[0],"S79 1e14 p cm{}^{-2}","L");
  leg1->AddEntry(gr[1],"S52 3.63e14 p cm{}^{-2}","L");
  leg1->Draw("same");
  c2->Update();
  c2->WaitPrimitive();


  //------- hist of radiation damage ---------------
  // TGraph* hRad = new TGraph();
  const float Lambda0 = 1;
  const float k = 1.8e-18;
  const float k14 = 1.8e-4;
  const float irrad[10] = {0, 1, 3.63}; //e14
  const float Lambda0k = Lambda0 * k;

  // y = Lambda0 / ( Lambda0*k* x  + 1 )

  TF1* fun = new TF1("f1","530 / ( 530 * 1.8e-4 * x + 1 ) ", -2, 20);
  fun->SetLineColor(kBlue);
  fun->SetLineStyle(9);

  // fun->Draw();
  // c3->Update();
  // c3->WaitPrimitive();

  //------ add points at 500V to rad plot ------------------
  TGraphErrors* gp[3];
  gp[0] = new TGraphErrors(cntRadPt, radPtFlux, radPoint, radPtFluxe, radPointe);
  dr->prettify(gp[0]);

  TF1* fitf = new TF1("fit","[0] / ([0]*[1]*x + 1) ", 0, 20);
  fitf->SetParameter(0,500);
  fitf->SetParameter(1,1.8e-4);
  fitf->SetLineColor(kBlack);
  TFitResultPtr ptr = gp[0]->Fit("fit","SRQ");
  cout<<endl<<" --------------------------------"<<endl;
  cout<<" Fitted k = "<<ptr->Parameter(1)*10000<<" x 10^-18"<<endl;
  cout<<" --------------------------------"<<endl;

  TLegend* leg2 = new TLegend(0.7,0.7,0.95,0.95);
  leg2->SetTextFont(132);
  leg2->AddEntry(fun,"RD42 fit","l");
  leg2->AddEntry(gp[0],"CCD at >500 V","lep");
  leg2->AddEntry(fitf, "Data fit","l");


  // gp[0]->GetXaxis()->SetTitle("Radiation dose [10^{14} pions cm{}^{-2}]");
  // gp[0]->GetYaxis()->SetTitle("Charge collection distance [#mum]");
  // fitf->GetXaxis()->SetTitle("Radiation dose [10^{14} pions cm{}^{-2}]");
  // fitf->GetYaxis()->SetTitle("Charge collection distance [#mum]");
  TCanvas* c3 = new TCanvas("raddamage","raddamage",800,600);
  dr->prettify(c3);
  // c3->DrawFrame(fun->GetXmin(), fun->GetMinimum(),
  //               fun->GetXmax(), fun->GetMaximum(),
  //               "");
  fitf->Draw();
  gp[0]->Draw("AP");
  fun->Draw("SAME");
  leg2->Draw("same");
  gp[0]->GetXaxis()->SetTitle("Radiation dose [10{}^{14} #pi cm{}^{-2}]");
  gp[0]->GetYaxis()->SetTitle("Charge collection distance [ #mu m]");
  gp[0]->GetXaxis()->SetRangeUser(-2,20);

  TLatex* lat = new TLatex();



  c3->Modified();
  c3->Update();
  c3->WaitPrimitive();
  delete(c3);
  //--------------------------------------------------------

  //------- hist of landaus ------------------------------
  // histLan[sample]
  //--------------------------------------------------------





  wana = 0;
  if (wana) delete(wana);
  delete(dr);
  delete(app);

  cout<<"All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
