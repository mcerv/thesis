void plotcharge () {

  const int nvolts = 2;
  const int ndets = 8;

  string detstr[ndets] = {"1scdhq","S52","S79","S37","S50","ELSC", "S52_3-63e14","S79_1e14"};
  string voltstr[nvolts] = {"-500V", "500V"};
  string carrier[nvolts] = {"holes","electrons"};
  string filePath[nvolts][ndets];

  for (int i=0; i<ndets; i++) {
    cout << i << endl;
    filePath[0][i] = "charge_";
    filePath[0][i] += detstr[i];
    filePath[0][i] += "_";
    filePath[0][i] += voltstr[0];
    filePath[0][i] += ".root";
    filePath[1][i] = "charge_";
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
  //
  for (int volt=0; volt<nvolts; volt++) {
    c[volt] = new TCanvas (voltstr[volt].c_str(), voltstr[volt].c_str(), 1200,800);
    c[volt]->cd();
    leg[volt] = new TLegend (0.6,0.12,0.85,0.44);
    for (int det=0; det<ndets; det++) {
      file = new TFile(filePath[volt][det].c_str(), "read");
      if (file->IsOpen()) cout<<"File "<<filePath[volt][det]<<" open."<<endl;

      gr[volt][det] = (TGraph*)file->Get("Graph");
      if (!det) {
        gr[volt][det]->Draw("AC");
        gr[volt][det]->GetYaxis()->SetRangeUser(0,60e-15);
        gr[volt][det]->SetTitle("Collected charge vs temperature");
        gr[volt][det]->GetXaxis()->SetTitle("Temperature[K]");
        gr[volt][det]->GetYaxis()->SetTitle("Charge [C]");

      }
      else {
        gr[volt][det]->Draw("sameC");
      }
      gr[volt][det]->SetLineColor(color[det]);
      gr[volt][det]->SetMarkerColor(color[det]);
      file->Close();
      file = 0;
      leg[volt]->AddEntry(gr[volt][det],detstr[det].c_str(),"L");
      TLatex *lat = new TLatex(248.68,5.55e-14, carrier[volt].c_str());
      lat->Draw("same");
    }
    leg[volt]->Draw("same");
    string savePath = "plots/charge";
    savePath += voltstr[volt];
    savePath += ".pdf";
    c[volt]->SaveAs(savePath.c_str() );
  }
  //
  // for (int i=0; i<6; i++) {
  //   prof[i] = (TProfile*)file[i]->Get("avgpulses");
  //   prof[i]->Scale(0.1317e+3);
  //   gr[i] = new TGraph(1996);
  //   for(int l=0; l<1996; l++) gr[i]->SetPoint(l,(float)l/10.-50.,prof[i]->GetBinContent(l));
  //   gr[i]->SetLineColor(color[i]);
  // }
  //







  //
  // gStyle->SetOptStat(0);
  // TCanvas *d;
  // d = new TCanvas("DBM_1","Diamond Beam Monitor",1400,800);
  //
  // bool drawn = false;
  // for (int i=0; i<6; i++) {
  //   leg->AddEntry(gr[i],det[i].c_str(),"L");
  //   if (!drawn) {
	//     // gr[i]->GetXaxis()->SetRangeUser(-1.,15.);
	//     // prof[i]->GetXaxis()->SetTitle("time [ns]");
	//     // prof[i]->GetYaxis()->SetTitle("current [#muA]");
	//     // prof[i]->GetXaxis()->SetLabelSize(0.05);
	//     // prof[i]->GetYaxis()->SetLabelSize(0.05);
	//     // prof[i]->SetTitle("");
	//     // //prof[i]->GetXaxis()->SetTitleSize(0.1);
	//     // //prof[i]->GetYaxis()->SetTitleSize(0.1);
	//     // prof[i]->GetXaxis()->SetTitleOffset(1.2);
	//     // prof[i]->GetYaxis()->SetTitleOffset(0.7);
  //     //   gStyle->SetOptStat(0);
  //
	//     gr[i]->Draw();
	//     drawn = true;
	//    } else gr[i]->Draw("same");
  //  }
  //  leg->Draw("same");





}
