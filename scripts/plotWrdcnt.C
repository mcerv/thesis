{
  TCanvas* can = new TCanvas("Wordcount", "Wordcount", 900, 600);
  ifstream f;
  int x[100000];
  int y[100000];
  char blah;
  int cnt = 0;
  f.open("../wrdcnt.txt");
  while(1) {
    if (!f.good()) break;
    if (cnt==0) {
      f >> x[cnt] >> y[cnt];
    }
    else {
      f >> blah >> blah >> x[cnt] >> y[cnt];
    }
    cout<<"x "<<x[cnt]<<" y "<<y[cnt]<<endl;
    cnt++;
  }
  f.close();

  cnt--; // we don't want the last line.

  TGraph* gr = new TGraph(cnt, x, y);
  gr->Draw("AL.");
  gr->SetTitle("Thesis wordcount");
  gr->GetXaxis()->SetTitle("Time");
  gr->GetYaxis()->SetTitle("Words");



  // if (!f.is_open()) {
  //   cout<<"Error while opening the wrdcnt file."<<endl;
  // }
  // f.close();
  cout<<"Done"<<endl;
}
