#include "drawfuns.cpp"

void plotFreq() {


  ifstream inFile;
  inFile.open(Form("freq.txt"));

  const int arrlen = 100;
  int nSample = 0;
  string name;
  char delim;
  double freqOsc[arrlen], freqPsa[arrlen], ratio[arrlen];
  double corrFactor = 0;

  //first line
  for (int i=0; i<2; i++) {
    inFile >> name;
    cout << name << endl;
  }


  // read other lines
  while (1) {
    inFile >> freqOsc[nSample] >> delim;
    inFile >> freqPsa[nSample];

    //from kHz to MHz
    freqOsc[nSample] = freqOsc[nSample]/1000;
    freqPsa[nSample] = freqPsa[nSample]/1000;

    if (!nSample) corrFactor = freqOsc[nSample]/freqPsa[nSample];
    ratio[nSample] = freqPsa[nSample]/freqOsc[nSample]*corrFactor;
    nSample++;
    if (!inFile.good()) break;
  }

  //plot things
  TCanvas *c1 = new TCanvas("Frequency","Frequency", 1200,800);
  TGraph *gr1 = new TGraphErrors(nSample, freqOsc, ratio);
  gr1->Draw("AL.");
  prettifyGraph(gr1);
  prettifyCanvas(c1);


}
