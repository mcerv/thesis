#include "drawfuns.cpp"





void plotWidth() {

  ifstream inFile;
  inFile.open(Form("width.txt"));

  //read file
  const int arrlen = 100;
  int nSample = 0;
  string name;
  char delim;

  double maxAmpl[arrlen], maxAmplStd[arrlen],
         area[arrlen], areaStd[arrlen],
         width[arrlen], widthStd[arrlen],
         maxAmplPSA[arrlen], maxAmplStdPSA[arrlen],
         areaPSA[arrlen], areaStdPSA[arrlen],
         widthPSA[arrlen], widthStdPSA[arrlen];
  double ratioWidth[arrlen], ratioWidthStd[arrlen];

  // first line
  cout << "Lines:" << endl;
  for (int i=0; i<12; i++) {
    inFile >> name;
    cout << name << endl;
  }

  // read other lines
  while (1) {
    //reading results
    inFile >> maxAmpl[nSample] >> delim;
    inFile >> maxAmplStd[nSample] >> delim;
    inFile >> area[nSample] >> delim;
    inFile >> areaStd[nSample] >> delim;
    inFile >> width[nSample] >> delim;
    inFile >> widthStd[nSample] >> delim;
    inFile >> maxAmplPSA[nSample] >> delim;
    inFile >> maxAmplStdPSA[nSample] >> delim;
    inFile >> areaPSA[nSample] >> delim;
    inFile >> areaStdPSA[nSample] >> delim;
    inFile >> widthPSA[nSample] >> delim;
    inFile >> widthStdPSA[nSample];

    ratioWidth[nSample] = widthPSA[nSample]/width[nSample];
    ratioWidthStd[nSample] = relError(width[nSample], widthStd[nSample],
                                    widthPSA[nSample], widthStdPSA[nSample]);
    //cout <<ratioWidthStd[nSample]<<endl;
    nSample++;

    if (!inFile.good()) break;

  }



  //plot things
  TCanvas *c1 = new TCanvas("Area vs Width","Area vs Width", 1200,800);
  TGraphErrors *gr1 = new TGraphErrors(nSample, width, ratioWidth, widthStd, ratioWidthStd);
  gr1->Draw("AL.");
  prettifyGraph ( (TGraph*)gr1);
  prettifyCanvas (c1);
  prettyGraphShade(nSample, width, ratioWidth, widthStd, ratioWidthStd);

  TCanvas *c2 = new TCanvas("Amplitude vs Width","Amplitude vs Width", 1200,800);
  TGraphErrors *gr2 = new TGraphErrors(nSample, width, maxAmpl, widthStd, maxAmplStd);
  gr2->Draw("AL.");
  prettifyGraph ( (TGraph*)gr2);
  TGraphErrors *gr3 = new TGraphErrors(nSample, width, maxAmplPSA, widthStd, maxAmplStdPSA);
  gr3->Draw("same");
  prettifyGraph ( (TGraph*)gr3);
  prettifyCanvas (c2);
  //
  // TCanvas *c3 = new TCanvas("Area","Area", 1200,800);
  // TGraphErrors *gr3 = new TGraphErrors(nSample, s2n, area, noiseDbStd, areaStd);
  // gr3->Draw("AL*");



}
