#include "drawfuns.cpp"

void plotMeas() {


  ifstream inFile;
  inFile.open(Form("noisemeas_scope.csv"));

  //read file
  const int arrlen = 100;
  int nSample = 0;
  string name;
  string picPath;
  char delim;
  double noiseDb[arrlen], noiseDbStd[arrlen], noise[arrlen], noiseStd[arrlen], noiseAvg[arrlen],
         maxAmpl[arrlen], maxAmplStd[arrlen], maxAmplAvg[arrlen],
         area[arrlen], areaStd[arrlen], areaAvg[arrlen],
         width[arrlen], widthStd[arrlen], widthAvg[arrlen];
  double s2n[arrlen], s2nStd[arrlen];
  double trash;
  string picture[arrlen];

  // first line
  cout << "Lines:" << endl;
  for (int i=0; i<14; i++) {
    inFile >> name;
    cout << name << endl;
  }

  // read second line
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> name;

  // read other lines
  while (1) {
      //reading results
      inFile >> noiseDb[nSample] >> delim;
      inFile >> noise[nSample] >> delim;
      inFile >> noiseStd[nSample] >> delim;
      inFile >> noiseAvg[nSample] >> delim;
      inFile >> maxAmpl[nSample] >> delim;
      inFile >> maxAmplStd[nSample] >> delim;
      inFile >> maxAmplAvg[nSample] >> delim;
      inFile >> area[nSample] >> delim;
      inFile >> areaStd[nSample] >> delim;
      inFile >> areaAvg[nSample] >> delim;
      inFile >> width[nSample] >> delim;
      inFile >> widthStd[nSample] >> delim;
      inFile >> widthAvg[nSample] >> delim;
      inFile >> picture[nSample];

      noiseDbStd[nSample] = 0;
      cout << noiseDb[nSample] << endl;
      //calculate S/N ratio. take the current noise agaist the amplitude of non noised run.
      s2n[nSample] = maxAmpl[0]/noise[nSample];
      s2nStd[nSample] = relError (maxAmpl[0], maxAmplStd[0],
                                  noise[nSample], noiseStd[nSample]);
      cout << "s2n of " << nSample << " is " << s2n[nSample] << endl;
      nSample++;


      if (!inFile.good()) break;

  }
  inFile.close();



  //plot things
  TCanvas *c1 = new TCanvas("Width v S2N","Width vs S2N", 1200,800);
  TGraphErrors *gr1 = new TGraphErrors(nSample, s2n, width, s2nStd, widthStd);
  gr1->Draw("AL.");
  prettifyCanvas(c1);
  prettifyGraph(gr1);
  gr1->GetYaxis()->SetRangeUser(0,10);

  TCanvas *c2 = new TCanvas("Amplitude vs S2N","Amplitude vs S2N", 1200,800);
  TGraphErrors *gr2 = new TGraphErrors(nSample, s2n, maxAmpl, s2nStd, maxAmplStd);
  gr2->Draw("AL.");
  prettifyCanvas(c2);
  prettifyGraph(gr2);
  gr2->GetYaxis()->SetRangeUser(0,100);
  prettyGraphShade(nSample, s2n, maxAmpl, s2nStd, maxAmplStd);
  

  TCanvas *c3 = new TCanvas("Area vs S2N","Area vs S2N", 1200,800);
  TGraphErrors *gr3 = new TGraphErrors(nSample, s2n, area, s2nStd, areaStd);
  gr3->Draw("AL.");
  prettifyCanvas(c3);
  prettifyGraph(gr3);
  gr3->GetYaxis()->SetRangeUser(0,600);




  inFile.open(Form("noisemeas_PSA.csv"));
  nSample = 0;
  // first line
  cout << "Lines:" << endl;
  for (int i=0; i<14; i++) {
    inFile >> name;
    cout << name << endl;
  }

  // read second line
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> trash >> delim;
  inFile >> name;

  // read other lines
  while (1) {
    //reading results
    inFile >> noiseDb[nSample] >> delim;
    inFile >> noise[nSample] >> delim;
    inFile >> noiseStd[nSample] >> delim;
    inFile >> noiseAvg[nSample] >> delim;
    inFile >> maxAmpl[nSample] >> delim;
    inFile >> maxAmplStd[nSample] >> delim;
    inFile >> maxAmplAvg[nSample] >> delim;
    inFile >> area[nSample] >> delim;
    inFile >> areaStd[nSample] >> delim;
    inFile >> areaAvg[nSample] >> delim;
    inFile >> width[nSample] >> delim;
    inFile >> widthStd[nSample] >> delim;
    inFile >> widthAvg[nSample] >> delim;
    inFile >> picture[nSample];

    noiseDbStd[nSample] = 0;
    cout << noiseDb[nSample] << endl;
    //calculate S/N ratio. take the current noise agaist the amplitude of non noised run.
    s2n[nSample] = maxAmpl[0]/noise[nSample];
    s2nStd[nSample] = relError (maxAmpl[0], maxAmplStd[0],
                                noise[nSample], noiseStd[nSample]);
    cout << "s2n of " << nSample << " is " << s2n[nSample] << endl;
    nSample++;


    if (!inFile.good()) break;

  }


  c1->cd();
  TGraphErrors *gr4 = new TGraphErrors(nSample, s2n, width, s2nStd, widthStd);
  gr4->Draw("same");
  prettifyGraph(gr4);
  gr4->SetLineColor(kBlue+1);

  c2->cd();
  TGraphErrors *gr5 = new TGraphErrors(nSample, s2n, maxAmpl, s2nStd, maxAmplStd);
  gr5->Draw("same");
  prettifyGraph(gr5);
  gr5->SetLineColor(kBlue+1);
  prettyGraphShade(nSample, s2n, maxAmpl, s2nStd, maxAmplStd);

  c3->cd();
  TGraphErrors *gr6 = new TGraphErrors(nSample, s2n, area, s2nStd, areaStd);
  gr6->Draw("same");
  prettifyGraph(gr6);
  gr6->SetLineColor(kBlue+1);



  inFile.close();
}
