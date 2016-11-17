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
#include "TMultiGraph.h"
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
#include "TMath.h"

#define DEBUG 0
#define S52 0
#define S79 1
#define S37 2
#define S52_363e14 3
#define S79_1e14 4
#define SCDHQ 5

Double_t fNfree2(Double_t *x, Double_t *par)
{
	Double_t Ntot = par[0];
	Double_t Ni = par[1];
	Double_t Na = Ntot - Ni;
	Double_t trel = par[2];
	Double_t Ea = par[3];
	Double_t k = 8.6e-5;

	return Na + Ni/(1 + trel*TMath::Exp(Ea / (k*x[0])));
}



using namespace std;


// ------------------------------ MAIN PROGRAM ----------------------
int32_t main (void) {
  cout<<endl<<"Plotting charge vs temperature.."<<endl;

  TApplication *app = new TApplication("app",0,0);
  DrawFuns *dr = new DrawFuns();
  const int nvolts = 2;
  const int ndets = 5;

  // string detstr[ndets] = {"1scdhq","S52","S79","S37","S50","ELSC", "S52_3-63e14","S79_1e14"};
  string detstr[ndets] = {"S52","S79","S37","S79_1e14","S52_3-63e14"};
  string detName[ndets] = {"S52","S79","S37","S79 1#times10^{14} #pi cm^{-2}","S52 3.63#times10^{14} #pi cm^{-2}"};
  string voltstr[nvolts] = {"-500V", "500V"};
  string carrier[nvolts] = {"holes","electrons"};
  string colors[ndets] = {"red", "blue", "green", "orange", ""};
  string filePath[nvolts][ndets];

  for (int i=0; i<ndets; i++) {
    cout << i << endl;
    filePath[0][i] = "../data/charge/charge_";
    filePath[0][i] += detstr[i];
    filePath[0][i] += "_";
    filePath[0][i] += voltstr[0];
    filePath[0][i] += ".root";
    filePath[1][i] = "../data/charge/charge_";
    filePath[1][i] += detstr[i];
    filePath[1][i] += "_";
    filePath[1][i] += voltstr[1];
    filePath[1][i] += ".root";

    cout << filePath[0][i] << endl;
    cout << filePath[1][i] << endl;
  }

  TFile *file;
  TProfile *prof[nvolts][ndets];
  TGraph *gr[nvolts][ndets];
  int color[8] = {1,2,3,4,6,12,8,9};
  TLegend *leg[nvolts];// = new TLegend(0.6,0.6,1,1);
  TCanvas *c[nvolts];
  //
  // GET DATA TO GRAPHS FROM FILES
  for (int volt=0; volt<nvolts; volt++) {
    for (int det=0; det<ndets; det++) {
      file = new TFile(filePath[volt][det].c_str(), "read");
      if (file->IsOpen()) cout<<"File "<<filePath[volt][det]<<" open."<<endl;
      gr[volt][det] = (TGraph*)file->Get("Graph");
      file->Close();
      file = 0;
    }
  }


	// DIFF
	TCanvas *can = new TCanvas("c","c",800,700);
	dr->prettify(can);
	TMultiGraph *mg[nvolts];
	for(int i=0; i<nvolts; i++) {
		mg[i] = new TMultiGraph();
	}
	// gr[0][1]->Draw("AP");
	// gr[0][3]->Draw("sameP");
	const int ncompares = 2;
	TGraph *diff[nvolts][ncompares];
	string entry[nvolts][ncompares] = {"S79 electrons","S79 holes","S52 electrons","S52 holes"};
	TLegend *leg1 = new TLegend(0.65, 0.75, 0.95, 0.975);

	for (int volt=0; volt<nvolts; volt++) {
		for (int compare=0; compare<ncompares; compare++) {
			int nonir, ir;
			if (compare == 0) {nonir=1, ir=3;}//S79
									 else {nonir=0; ir=4;}//S52
			diff[volt][compare] = new TGraph();
			dr->prettify(diff[volt][compare]);
			diff[volt][compare]->GetXaxis()->SetTitle("Temperature [K]");
			diff[volt][compare]->GetYaxis()->SetTitle("Charge irrad/non-irrad [%]");

			diff[volt][compare]->GetYaxis()->SetRangeUser(61,115);
			diff[volt][compare]->GetXaxis()->SetRangeUser(0,310);
			diff[volt][compare]->SetMarkerStyle(26 + compare);
			diff[volt][compare]->SetMarkerSize(0.7 + compare*0.2);

			int a=0;
			for (int i=0; i<gr[volt][nonir]->GetN(); i++ ) {
				double x,y, x2,y2;
				bool found = false;
				gr[volt][nonir]->GetPoint(i, x, y);
				cout << "-------------------------" << endl;
				cout << "x = " << x << "   y = " << y << endl;
				for (int j=0; j<gr[volt][ir]->GetN(); j++) {
					gr[volt][ir]->GetPoint(j, x2, y2);
					cout << "   - " << x2 << "  " << y2 << endl;
					if (x == x2) {
						found = true;
						cout << "   FOUND! " << endl;
						break;
					}
				}
				if (found) {
					diff[volt][compare]->SetPoint(a,x,y2/y*100);
					a++;
				}
			}
			leg1->AddEntry(diff[volt][compare],entry[volt][compare].c_str(),"LEP");
			mg[volt]->Add(diff[volt][compare]);
		}
	}

	//plot CCE vs TEMP
	for (int volt=0; volt<nvolts; volt++) {
		mg[volt]->Draw("APL");
		dr->prettify(mg[volt]);
		mg[volt]->GetXaxis()->SetTitle("Temperature [K]");
		mg[volt]->GetYaxis()->SetTitle("Irrad/non-irrad [%]");
		mg[volt]->GetYaxis()->SetRangeUser(40,121);
		leg1->Draw("same");

		// can->Update();
		// can->WaitPrimitive();
		// can->Update();
	}




	// plot Charge + CCE vs temp
  for (int volt=0; volt<nvolts; volt++) {
    c[volt] = new TCanvas (voltstr[volt].c_str(), voltstr[volt].c_str(), 800 ,600);
    dr->prettify(c[volt]->cd() );
    c[volt]->SetTopMargin(0.06);
    c[volt]->SetBottomMargin(0.16);
		TPad *pad1 = new TPad("pad1","pad1",0,0.35,1,1);
		dr->prettify(pad1);
		pad1->SetBottomMargin(0);
		pad1->SetTopMargin(0.1);
		pad1->Draw();
		c[volt]->cd();
		TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.35);
		dr->prettify(pad2);
    pad2->SetTopMargin(0);
		pad2->SetBottomMargin(0.33);
    pad2->Draw();
		pad1->cd();

    leg[volt] = new TLegend (0.162907,0.438127,0.382206,0.879599);
    for (int det=0; det<ndets; det++) {
      dr->prettify(gr[volt][det]);
      gr[volt][det]->GetXaxis()->SetRangeUser(0,310);
      gr[volt][det]->GetYaxis()->SetRangeUser(8e-15,60.1e-15);
      gr[volt][det]->GetYaxis()->SetLimits(0,60);
      gr[volt][det]->SetMarkerStyle(24-det);
      gr[volt][det]->SetMarkerSize(0.9);
      gr[volt][det]->SetLineColor(kBlack);
      gr[volt][det]->SetLineWidth(1);


      if (!det) {
        gr[volt][det]->Draw("AP");
        // gr[volt][det]->GetYaxis()->SetRangeUser(0,60e-15);
        // gr[volt][det]->GetYaxis()->SetLimits(0,60);
        // gr[volt][det]->SetTitle("Collected charge vs temperature");
        gr[volt][det]->GetXaxis()->SetTitle("Temperature [K]");
        gr[volt][det]->GetYaxis()->SetTitle("Charge [C]");

      }
      else {
        gr[volt][det]->Draw("sameP");
      }
    // gr[volt][det]->SetLineColor(color[det]);
      // gr[volt][det]->SetMarkerColor(color[det]);
      leg[volt]->AddEntry(gr[volt][det],detName[det].c_str(),"LEP");
      TLatex *lat = new TLatex(248.68,5.55e-14, carrier[volt].c_str());
      lat->SetTextFont(42);
      lat->Draw("same");
    }
  TF1* funcc = new TF1("Nfree2a",fNfree2,40,300,4);
    if (!volt) {
      funcc->FixParameter(0,52.4e-15);
      funcc->FixParameter(1,33.9e-15);
      funcc->FixParameter(2,4.45e-5);
      funcc->FixParameter(3,0.08);
    } else {
      funcc->FixParameter(0,50.5e-15);
      funcc->FixParameter(1,32.7e-15);
      funcc->FixParameter(2,4.41e-5);
      funcc->FixParameter(3,0.08);
    }
    funcc->Draw("same");
    leg[volt]->AddEntry(funcc,"S52 by H. Jansen");
    leg[volt]->AddEntry(diff[volt][0],"S79 irrad. CCE","P");
    leg[volt]->AddEntry(diff[volt][1],"S52 irrad. CCE","P");
    leg[volt]->Draw("same");
		pad2->cd();
		mg[volt]->Draw("AP");
		mg[volt]->GetXaxis()->SetLimits(0,311);
		mg[volt]->GetXaxis()->SetRangeUser(0,311);
		mg[volt]->GetYaxis()->SetLimits(53,115);
		mg[volt]->GetYaxis()->SetRangeUser(53,115);
		mg[volt]->GetXaxis()->SetLabelSize(0.07);
		mg[volt]->GetYaxis()->SetLabelSize(0.07);
		mg[volt]->GetXaxis()->SetTitleSize(0.10);
		mg[volt]->GetYaxis()->SetTitleSize(0.10);
		mg[volt]->GetXaxis()->SetTitleOffset(1.4);
		mg[volt]->GetYaxis()->SetTitleOffset(0.5);


    string savePath = "../plots/charge";
    savePath += voltstr[volt];
    savePath += ".pdf";
    // c[volt]->SaveAs(savePath.c_str() );

    c[volt]->Update();
    c[volt]->WaitPrimitive();
    c[volt]->Update();
  }


  cout<<" All done."<<endl;
  return 0;
}//-------------------- end of main -----------------------------------------
