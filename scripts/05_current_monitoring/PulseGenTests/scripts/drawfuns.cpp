void prettifyGraph (TGraphErrors *gr) {
  prettifyGraph ( (TGraph*) gr );
}

void prettifyGraph (TGraph *gr ) {
  //gr->GetXaxis()->SetLogx();
    gStyle->SetOptStat(0);
    gr->SetLineColor(kRed+1);
    //gr->Draw("AL.");
    //gr->SetMarkerColor(kBlack);
    //gr->GetYaxis()->SetRangeUser(minYd,maxYd);
    //gr->GetXaxis()->SetRangeUser(0,endPulsed-startPulsed);
    gr->SetLineWidth(3);
    //gr->SetName("Graph");
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("X axis [unit]");
    gr->GetYaxis()->SetTitle("Y axis [unit]");
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetYaxis()->SetTitleSize(0.05);
    gr->GetYaxis()->SetTitleOffset(0.8);
    gr->GetXaxis()->SetTitleFont(132);
    gr->GetYaxis()->SetTitleFont(132);
    gr->GetXaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetTickLength(0.01);
}

void prettifyCanvas (TCanvas *c) {
  c->SetGrid();
  c->SetLogx();
  c->SetTopMargin(0.03);
  c->SetBottomMargin(0.1);
  c->SetLeftMargin(0.1);
  c->SetRightMargin(0.03);
  cout<< "grid set" << endl;
  c->Update();
}

void prettyGraphShade (int n, double *xaxis, double *yaxis,
                                double *xerr,  double *yerr  ) {
      //to call it:
      TGraph *gr = new TGraph(2*n);
      for (int i=0;i<n;i++) {
        gr->SetPoint(i,xaxis[i],yaxis[i]-yerr[i]);
        gr->SetPoint(n+i,xaxis[n-i-1],yaxis[n-i-1]+yerr[n-i-1]);
      }
      gr->SetFillStyle(1001);
      gr->SetFillColorAlpha(kRed-3, 0.30);
      //gr->SetFillColor(kRed-3);
      gr->SetLineWidth(3);
      gr->Draw("F");

  	  //shade - clock cycle
      // TGraph *grshade1 = new TGraph(4);
      // grshade1->SetPoint(0,0,-10);
      // grshade1->SetPoint(1,6.2,-10);
      // grshade1->SetPoint(3,0,50);
      // grshade1->SetPoint(2,6.2,50);
      // grshade1->SetFillStyle(3005);
      // grshade1->SetFillColor(16);
      // grshade1->Draw("F");
}

double relError (double X, double errX, double Y, double errY) {
  return (double) sqrt( (errX/X)*(errX/X) + (errY/Y)*(errY/Y)  );
}
