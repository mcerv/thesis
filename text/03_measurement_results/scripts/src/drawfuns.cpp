/*
  Draw functions
  2015-07-27
  Matevz Cerv
*/

#include "drawfuns.h"


DrawFuns::DrawFuns () {

}

DrawFuns::~DrawFuns() {}

void DrawFuns::prettify (TGraph *gr ) {
  //gr->GetXaxis()->SetLogx();
    gStyle->SetOptStat(0);
    gr->SetLineColor(kRed+1);
    //gr->Draw("AL.");
    //gr->SetMarkerColor(kBlack);
    //gr->GetYaxis()->SetRangeUser(minYd,maxYd);
    //gr->GetXaxis()->SetRangeUser(0,endPulsed-startPulsed);
    gr->SetLineWidth(2);
    //gr->SetName("Graph");
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("X axis [unit]");
    gr->GetYaxis()->SetTitle("Y axis [unit]");
    gr->GetXaxis()->SetTitleSize(0.07);
    gr->GetYaxis()->SetTitleSize(0.07);
    gr->GetXaxis()->SetTitleOffset(0.7);
    gr->GetYaxis()->SetTitleOffset(0.7);
    gr->GetXaxis()->SetTitleFont(132);
    gr->GetYaxis()->SetTitleFont(132);
    gr->GetXaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetTickLength(0.01);
}

void DrawFuns::prettify (TGraphErrors *gr) {
  prettify ( (TGraph*) gr );
}

void DrawFuns::prettify (TMultiGraph *gr) {
  gr->SetTitle("");
  gr->GetXaxis()->SetTitle("X axis [unit]");
  gr->GetYaxis()->SetTitle("Y axis [unit]");
  gr->GetXaxis()->SetTitleSize(0.07);
  gr->GetYaxis()->SetTitleSize(0.07);
  gr->GetXaxis()->SetTitleOffset(0.7);
  gr->GetYaxis()->SetTitleOffset(0.7);
  gr->GetXaxis()->SetTitleFont(132);
  gr->GetYaxis()->SetTitleFont(132);
  gr->GetXaxis()->SetLabelSize(0.05);
  gr->GetYaxis()->SetLabelSize(0.05);
  gr->GetYaxis()->SetTickLength(0.01);
}

void DrawFuns::prettify (TVirtualPad *c, string histType = "th1") {
  gStyle->SetOptStat(0);
  c->SetGrid();
  // c->SetLogx();
  c->SetTopMargin(0.03);
  c->SetBottomMargin(0.12);
  c->SetLeftMargin(0.12);
  c->SetRightMargin(0.03);
  if (!histType.compare("th2")) {
    c->SetRightMargin(0.13);
  }

  c->SetTitle("");
  c->Update();

}

void DrawFuns::prettify (TVirtualPad *c) {
  prettify( (TVirtualPad*) c, "th1");
}

void DrawFuns::prettify (TCanvas *c, string histType = "th1") {
  prettify( (TVirtualPad*) c, "th1");
}

void DrawFuns::prettify (TCanvas *c) {
  prettify( (TVirtualPad*) c, "th1");
}

void DrawFuns::prettify (TH2 *h ) {
  gStyle->SetOptStat(0);
  h->SetLineColor(kRed+1);
  h->SetLineWidth(1);
  h->SetTitle("");
  h->GetXaxis()->SetTitle("X axis [unit]");
  h->GetYaxis()->SetTitle("Y axis [unit]");
  h->GetXaxis()->SetTitleSize(0.07);
  h->GetYaxis()->SetTitleSize(0.07);
  h->GetYaxis()->SetTitleOffset(0.8);
  h->GetXaxis()->SetTitleFont(132);
  h->GetYaxis()->SetTitleFont(132);
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetTickLength(0.01);
  h->SetDrawOption("colz");
}

void DrawFuns::prettify (TH1 *h) {
  prettify (h, "red");
}

void DrawFuns::prettify (TH1 *h , std::string color = "red") {
  //gr->GetXaxis()->SetLogx();
  int32_t colorNum = kRed;
  int32_t colorFill = 3005;
  if(!color.compare("red")) {
    colorNum = kRed+1;
    colorFill = 3005;
  }
  else if (!color.compare("light blue")) {
    colorNum = kBlue-7;
    colorFill = 3004;
  }
  else if (!color.compare("dark blue") || !color.compare("blue") ) {
    colorNum = kBlue-1;
    colorFill = 3004;
  }
    else if (!color.compare("dark green") || !color.compare("green") ) {
    colorNum = kGreen+3;
    colorFill = 3006;
  }
  else if (!color.compare("light green")) {
    colorNum = kGreen-1;
    colorFill = 3006;
  }
  else if (!color.compare("black")) {
    colorNum = kBlack;
    colorFill = 3007;
  }
  else {
    colorNum = kBlack;
    colorFill = 3005;
  }
  gStyle->SetOptStat(0);
  h->SetLineColor(colorNum);
  //h->Draw("AL.");
  //h->SetMarkerColor(kBlack);
  //h->GetYaxis()->SetRangeUser(minYd,maxYd);
  //h->GetXaxis()->SetRangeUser(0,endPulsed-startPulsed);
  h->SetLineWidth(2);
  //h->SetName("haph");
  h->SetTitle("");
  h->GetXaxis()->SetTitle("X axis [unit]");
  h->GetYaxis()->SetTitle("Y axis [unit]");
  h->GetXaxis()->SetTitleSize(0.07);
  h->GetYaxis()->SetTitleSize(0.07);
  h->GetYaxis()->SetTitleOffset(0.8);
  h->GetXaxis()->SetTitleFont(132);
  h->GetYaxis()->SetTitleFont(132);
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetTickLength(0.01);
  h->SetFillStyle(colorFill);
  h->SetFillColor(colorNum);

}

void DrawFuns::prettyGraphShade (int n, double *xaxis, double *yaxis,
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
      gr->SetLineWidth(2);
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

double DrawFuns::relError (double X, double errX, double Y, double errY) {
  return (double) sqrt( (errX/X)*(errX/X) + (errY/Y)*(errY/Y)  );
}

// void DrawFuns::normalise(TH1D* h) {
//   double max = h->GetMaximum();
//   for (int32_t i=0; i<h->GetNbinsX(); i++) {
//     h->SetBinContent(i, h->GetBinContent(i)/max );
//   }
// }

void DrawFuns::normalise(TH1D* h) {
  double sum = 0;
  for (int32_t i=0; i<h->GetNbinsX(); i++) {
    sum += h->GetBinContent(i);
  }
  for (int32_t i=0; i<h->GetNbinsX(); i++) {
    h->SetBinContent(i, h->GetBinContent(i)/sum );
  }
}


// void DrawFuns::normaliseSecondPeak(TH1D* h) {
//   TSpectrum *s = new TSpectrum(2);//maxpositions=2
//   s->Search(h,2,"new");
//   const int32_t nPeaks = s->GetNPeaks();
//   std::cout<<" n peaks: "<<nPeaks<<std::endl;
//
//   Float_t *xpeaks = s->GetPositionX();
//   for (int32_t i=0; i<nPeaks; i++) {
//     std::cout<<" peak "<<i<<" position "<<xpeaks[i]<<std::endl;
//   }
//
//   // double max = 0;
//   // for (int32_t i=0; i<h->GetNbinsX(); i++) {
//   //   h->SetBinContent(i, h->GetBinContent(i)/max );
//   // }
// }
