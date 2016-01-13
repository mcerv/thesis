#include "drawfuns.cpp"





void plotBandwidthLimit() {

  const int nFiles = 2;
  ifstream inFile[nFiles];
  inFile[0].open(Form("PSApulsegen/Width/C3width00000.txt"));
  inFile[1].open(Form("PSApulsegen/Width/C2width00000.txt"));

  //read file
  const int arrlen = 2000;
  const int maxSamples = 200;
  int nSample[nFiles];
  string name;
  char delim;
  double taxis[nFiles][arrlen], ampl[nFiles][arrlen];


  // first line
  for (int i=0; i<nFiles; i++) {
    inFile[i] >> name;
    inFile[i] >> name;
    inFile[i] >> name;
    inFile[i] >> name >> name;
    inFile[i] >> name;
    cout<<" columns: " << name << endl;
    nSample[i] = 0;
  }


  // read other lines
  while (1) {
    for (int i=0; i<nFiles; i++) {
      cout<<" i "<<i<<" nSample "<<nSample[i]<<endl;
      //inFile[i] >> taxis[i][nSample] >> delim >> ampl[i][nSample];
      inFile[i] >> taxis[i][nSample[i]] >> delim >> ampl[i][nSample[i]];

      //convert to ns
      taxis[i][nSample[i]] = taxis[i][nSample[i]] * 1e9;

      //shift the waveform in X axis
      if (i == 1) {
        taxis[i][nSample[i]] = taxis[i][nSample[i]] - 1.8; //order of ns
      }


      //convert to mV
      ampl[i][nSample[i]] = ampl[i][nSample[i]] * 1e3;



      nSample[i]++;
    }
    if (nSample[0] >= maxSamples) break;
    if (!inFile.good()) break;
  }

  //shift the red graph.



  //plot things
  TCanvas *c1 = new TCanvas("Waveform","Waveform", 1200,800);
  TGraph *gr1 = new TGraph(nSample[1], taxis[1], ampl[1]);
  gr1->Draw("AL.");
  prettifyGraph ( (TGraph*)gr1);
  gr1->GetXaxis()->SetRangeUser(-2,5);
  gr1->GetXaxis()->SetTitle("Time [ns]");
  gr1->GetYaxis()->SetTitle("Amplitude [mV]");

  //c1->SetLogy(0);

  //TCanvas *c2 = new TCanvas("Amplitude vs Width","Amplitude vs Width", 1200,800);
  TGraph *gr2 = new TGraph(nSample[0], taxis[0], ampl[0]);
  gr2->Draw("same");
  prettifyGraph ( (TGraph*)gr2);
  gr2->SetLineColor(kBlue+2);

  TLegend *leg = new TLegend(0.6,0.75,0.9,0.9);
  leg->AddEntry(gr1, "1 GHz bandwidth limit","L");
  leg->AddEntry(gr2, "200 MHz bandwidth limit","L");
  leg->SetTextSize(0.03);
  leg->Draw();

  //prettifyCanvas (c1);

  //
  // TCanvas *c3 = new TCanvas("Area","Area", 1200,800);
  // TGraphErrors *gr3 = new TGraphErrors(nSample, s2n, area, noiseDbStd, areaStd);
  // gr3->Draw("AL*");



}
