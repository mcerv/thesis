
{

  int nSamples = 3;
  int nVoltages = 11;

  float voltage[nVoltages] = {500.0, 300.0, 150.0, 75.0, 70.0, 30.0, -500.0, -300.0, -150.0, -70.0, -30.0};
  string sampleName[nSamples] = {"S79","S52","S37"};
  string sampleRealName[nSamples] = {"S52","S79","S37"};
  float sampleThick[nSamples] = {515.0, 529.0, 537.0};  //um

  TGraphErrors *gr[nSamples];
  TCanvas *c[nSamples];
  TCanvas *cHist;
  TFile *f;
  cout<<"Starting"<<endl;

  for (int sample = 0; sample < nSamples; sample++)
  {
    float mean[100];
    float meane[100];
    float field[100];
    float fielde[100];
    int num = 0;


    //get and open filename
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
        mean[num] = hist->GetMean();
        meane[num] = hist->GetRMS();
        field[num] = voltage[volt] / sampleThick[sample];
        fielde[num] = field[num] * 0.01; //1% error on fieldage
        num++;
        cout<<num<<endl;

        //save histogram
        stringstream ssave;
        ssave << "pics/" << sampleName[sample] << "_"
              << voltage[volt] << ".pdf";
        string saveName = ssave.str();
        cHist = new TCanvas();
        cHist->cd();
        hist->Draw();
        cHist->SaveAs( saveName.c_str() );
        if (hist) delete hist;
        if (cHist) delete cHist;
      }
      else // if the hist doesn't exist, skip to next one
      {
        continue;
      }
    }//end of voltage for loop.

    c[sample] = new TCanvas(sampleName[sample].c_str(),sampleName[sample].c_str(),800,600);
    c[sample]->cd();
    gr[sample] = new TGraphErrors(num,field,mean,fielde,meane);


    gr[sample]->GetYaxis()->SetRangeUser(0,0.05);
    gr[sample]->Draw("A*");


    //save
    stringstream ssave1;
    ssave1 << "pics/" << sampleName[sample] << ".pdf";
    string saveName1 = ssave1.str();
    c[sample]->SaveAs( saveName1.c_str() );



    if (f) delete f;










  }//end of sample for loop




}
