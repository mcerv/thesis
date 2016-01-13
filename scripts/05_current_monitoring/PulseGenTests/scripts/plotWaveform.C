#include "drawfuns.cpp"





void plotWaveform() {

  const int nFiles = 20;
  ifstream inFile[nFiles];
  stringstream ss;
  ss.str("");

  for (int i=0; i<nFiles/2; i++) {
    ss << "PSApulsegen/Noise/C3noise0000" << i << ".txt";
    inFile[2*i].open(Form(ss.str().c_str()));
    ss.str("");
    ss << "PSApulsegen/Noise/F1noise0000" << i << ".txt";
    inFile[2*i+1].open(Form(ss.str().c_str()));
    //cout <<ss.str()<<endl;
    cout<< "inFile[" <<2*i<< "]: " << inFile[2*i].IsOpen()
        << " inFile[" <<2*i+1<< "]: "<<inFile[2*i+1].IsOpen()
        << endl;
  }


  //cout <<" ********** debug *** "<<endl;


  //read file
  const int arrlen = 2000;
  const int maxSamples = 1000;
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
    //cout<<" columns: " << name << endl;
    nSample[i] = 0;
  }

  // read other lines
  while (1) {
    for (int i=0; i<nFiles; i++) {
      //cout<<" i "<<i<<" nSample "<<nSample[i]<<endl;
      //inFile[i] >> taxis[i][nSample] >> delim >> ampl[i][nSample];
      inFile[i] >> taxis[i][nSample[i]] >> delim >> ampl[i][nSample[i]];


      //convert to ns
      taxis[i][nSample[i]] = taxis[i][nSample[i]] * 1e9;

      //shift the waveform in X axis
      //if (i == 1) {
      //  taxis[i][nSample[i]] = taxis[i][nSample[i]] - 1.8; //order of ns
      //}

      //convert to mV
      ampl[i][nSample[i]] = ampl[i][nSample[i]] * 1e3;
      //cout << ampl[i][nSample[i]] << " ";

      nSample[i]++;
    }
    if (nSample[0] >= maxSamples) break;
    if (!inFile.good()) break;
  }

  //shift the red graph.

  for (int i=0; i<nFiles; i++) {
    for (int s=0; s<nSample[i]; s++) {
      cout << taxis[i][s] << " ";
    }
    cout << endl;
  }



  //plot things
  TCanvas *c[nFiles/2];
  TGraph *gr[nFiles];
  TLegend *leg[nFiles/2];
  for (int i=0; i<nFiles/2; i++) {
    ss.str("");
    ss << "Waveform " << i;
    c[i] = new TCanvas(ss.str().c_str(),ss.str().c_str(), 1200,800);
    c[i]->cd();
    gr[2*i] = new TGraph(nSample[2*i], taxis[2*i], ampl[2*i]);
    gr[2*i]->Draw("AL.");
    prettifyGraph ( (TGraph*)gr[2*i]);
    //gr[i]->GetXaxis()->SetRangeUser(-2,5);
    gr[2*i]->GetXaxis()->SetTitle("Time [ns]");
    gr[2*i]->GetYaxis()->SetTitle("Amplitude [mV]");
    gr[2*i+1] = new TGraph(nSample[2*i+1], taxis[2*i+1], ampl[2*i+1]);
    gr[2*i+1]->Draw("same");
    prettifyGraph ( (TGraph*)gr[2*i+1]);
    gr[2*i+1]->SetLineColor(kBlue+2);

    leg[i] = new TLegend(0.6,0.75,0.9,0.9);
    leg[i]->AddEntry(gr[2*i], "Input signal","L");
    leg[i]->AddEntry(gr[2*i+1], "Averaged signal","L");
    leg[i]->SetTextSize(0.03);
    leg[i]->Draw("same");
  }

  //prettifyCanvas (c1);

  //
  // TCanvas *c3 = new TCanvas("Area","Area", 1200,800);
  // TGraphErrors *gr3 = new TGraphErrors(nSample, s2n, area, noiseDbStd, areaStd);
  // gr3->Draw("AL*");



}
