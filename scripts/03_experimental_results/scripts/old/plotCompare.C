void plotCompare () {

  string det[6] = {"1scdhq","S52","S79","S37","S50","ELSC"};

  string filePath[6];
  filePath[0] = "../2014-07-11/TCT_results/1scdhq/295k/1scdhq_295k_400V.root";
  filePath[1] = "../TCT_results/S52/295k/S52_295k_500V.root";
  filePath[2] = "../TCT_results/S79/295k/S79_295k_500V.root";
  filePath[3] = "../TCT_results/S37/295k/S37_295k_500V.root";
  filePath[4] = "../TCT_results/S50/295k/S50_295k_500V.root";
  filePath[5] = "../TCT_results/ELSC/295k/ELSC_295k_500V.root";

  TFile *file[6];
  TProfile *prof[6];
  TGraph *gr[6];
  int color[8] = {1,2,3,4,6,12,8,9};
  TLegend *leg = new TLegend(0.6,0.6,1,1);


  for (int i=0; i<6; i++) {
    file[i] = new TFile(filePath[i].c_str(), "read");
    if (file[i]->IsOpen()) cout<<"File "<<filePath[i]<<" open."<<endl;
  }

  for (int i=0; i<6; i++) {
    prof[i] = (TProfile*)file[i]->Get("avgpulses");
    prof[i]->Scale(0.1317e+3);
    gr[i] = new TGraph(1996);
    for(int l=0; l<1996; l++) gr[i]->SetPoint(l,(float)l/10.-50.,prof[i]->GetBinContent(l));
    gr[i]->SetLineColor(color[i]);
  }









  gStyle->SetOptStat(0);
  TCanvas *d;
  d = new TCanvas("DBM_1","Diamond Beam Monitor",1400,800);

  bool drawn = false;
  for (int i=0; i<6; i++) {
    leg->AddEntry(gr[i],det[i].c_str(),"L");
    if (!drawn) {
	    // gr[i]->GetXaxis()->SetRangeUser(-1.,15.);
	    // prof[i]->GetXaxis()->SetTitle("time [ns]");
	    // prof[i]->GetYaxis()->SetTitle("current [#muA]");
	    // prof[i]->GetXaxis()->SetLabelSize(0.05);
	    // prof[i]->GetYaxis()->SetLabelSize(0.05);
	    // prof[i]->SetTitle("");
	    // //prof[i]->GetXaxis()->SetTitleSize(0.1);
	    // //prof[i]->GetYaxis()->SetTitleSize(0.1);
	    // prof[i]->GetXaxis()->SetTitleOffset(1.2);
	    // prof[i]->GetYaxis()->SetTitleOffset(0.7);
      //   gStyle->SetOptStat(0);

	    gr[i]->Draw();
	    drawn = true;
	   } else gr[i]->Draw("same");
   }
   leg->Draw("same");


  for (int i=0; i<6; i++) {
    file[i]->Close();
  }



}
