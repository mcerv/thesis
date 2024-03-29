/*
  Draw functions
  2016-05-07 v2
  Matevz Cerv
*/

#include "drawfuns.h"
#define CANVASTHESIS 0

DrawFuns::DrawFuns () {

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  //RED
  // Double_t stops[NRGBs] = { 0.00, 0.34, 0.45, 0.84, 1.00 };
  // Double_t red[NRGBs]   = { 0.99, 0.99, 0.99, 0.90, 0.69 };
  // Double_t green[NRGBs] = { 0.90, 0.60, 0.25, 0.20, 0.10 };
  // Double_t blue[NRGBs]  = { 0.90, 0.60, 0.25, 0.20, 0.10 };
  //BLUE
  // Double_t stops[NRGBs] = { 0.00, 0.34, 0.59, 0.84, 1.00 };
  // Double_t blue[NRGBs]   = { 0.99, 0.99, 0.99, 0.90, 0.69 };
  // Double_t green[NRGBs] = { 0.90, 0.60, 0.25, 0.20, 0.10 };
  // Double_t red[NRGBs]  = { 0.90, 0.60, 0.25, 0.20, 0.10 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);


  TColor* clrObj = new TColor();
  clr[0] = clrObj->GetColor("#2D2D29");
  clr[1] = clrObj->GetColor("#215A6D");
  clr[2] = clrObj->GetColor("#3CA2A2");
  clr[3] = clrObj->GetColor("#92C7A3");
  clr[4] = clrObj->GetColor("#DFECE6");
  clr[5] = clrObj->GetColor("#C2CBCE");
  clr[6] = clrObj->GetColor("#A4BCC2");
  clr[7] = clrObj->GetColor("#81A8B8");
  clr[8] = clrObj->GetColor("#107FC9");
  clr[9] = clrObj->GetColor("#0E4EAD");
  clr[10] = clrObj->GetColor("#0B108C");
  clr[11] = clrObj->GetColor("#0C0F66");
  clr[12] = clrObj->GetColor("#07093D");
  clr[13] = clrObj->GetColor("#300030");
  clr[14] = clrObj->GetColor("#480048");
  clr[15] = clrObj->GetColor("#601848");
  clr[16] = clrObj->GetColor("#C04848");
  clr[17] = clrObj->GetColor("#F07241");

  clrVolt[0] = clrObj->GetColor("#00A0B0");
  clrVolt[1] = clrObj->GetColor("#6A4A3C");
  clrVolt[2] = clrObj->GetColor("#CC333F");
  clrVolt[3] = clrObj->GetColor("#EB6841");
  clrVolt[4] = clrObj->GetColor("#EDC951");
  clrVolt[5] = clrObj->GetColor("#00A0B0");
  clrVolt[6] = clrObj->GetColor("#6A4A3C");
  clrVolt[7] = clrObj->GetColor("#CC333F");
  clrVolt[8] = clrObj->GetColor("#EB6841");
  clrVolt[9] = clrObj->GetColor("#EDC951");

  clrTemp[20] = clrObj->GetColor("#FF0000");
  clrTemp[19] = clrObj->GetColor("#FF1e00");
  clrTemp[18] = clrObj->GetColor("#FF3c00");
  clrTemp[17] = clrObj->GetColor("#FF5a00");
  clrTemp[16] = clrObj->GetColor("#FF7800");
  clrTemp[15] = clrObj->GetColor("#FF9600");
  clrTemp[14] = clrObj->GetColor("#FFb400");
  clrTemp[13] = clrObj->GetColor("#FFd200");
  clrTemp[12] = clrObj->GetColor("#FFf000");
  clrTemp[11] = clrObj->GetColor("#d7ff00");
  clrTemp[10] = clrObj->GetColor("#65ff00");
  clrTemp[9] = clrObj->GetColor("#00ff10");
  clrTemp[8] = clrObj->GetColor("#C44D58");
  clrTemp[7] = clrObj->GetColor("#FF6B6B");
  clrTemp[6] = clrObj->GetColor("#92C7A3");
  clrTemp[5] = clrObj->GetColor("#4ECDC4");
  clrTemp[4] = clrObj->GetColor("#556270");
  clrTemp[3] = clrObj->GetColor("#83988E");
  clrTemp[2] = clrObj->GetColor("#45ADA8");
  clrTemp[1] = clrObj->GetColor("#547980");
  clrTemp[0] = clrObj->GetColor("#594F4F");
  // clr[9] = clrObj->GetColor("");
  delete clrObj;
  // getchar();
}

DrawFuns::~DrawFuns() {
}


void DrawFuns::setColourScheme(string scheme) {
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  if (!scheme.compare("blue")) {
      //BLUE
      Double_t stops[NRGBs] = { 0.00, 0.34, 0.59, 0.84, 1.00 };
      Double_t blue[NRGBs]  = { 0.99, 0.99, 0.99, 0.90, 0.69 };
      Double_t green[NRGBs] = { 0.90, 0.60, 0.25, 0.20, 0.10 };
      Double_t red[NRGBs]   = { 0.90, 0.60, 0.25, 0.20, 0.10 };
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
      gStyle->SetNumberContours(NCont);
  }
  else if (!scheme.compare("red")) {
      //RED
      Double_t stops[NRGBs] = { 0.00, 0.34, 0.45, 0.84, 1.00 };
      Double_t red[NRGBs]   = { 0.99, 0.99, 0.99, 0.90, 0.69 };
      Double_t green[NRGBs] = { 0.90, 0.60, 0.25, 0.20, 0.10 };
      Double_t blue[NRGBs]  = { 0.90, 0.60, 0.25, 0.20, 0.10 };
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
      gStyle->SetNumberContours(NCont);
  }
  else
    throw " ERROR!  DrawFuns::setColourScheme : wrong arg. Either blue or red.";
}



void DrawFuns::prettify (TGraph *gr ) {
  //gr->GetXaxis()->SetLogx();
    gStyle->SetOptStat(0);
    gr->SetLineWidth(2);
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("X axis [unit]");
    gr->GetYaxis()->SetTitle("Y axis [unit]");
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetYaxis()->SetTitleSize(0.05);
    gr->GetXaxis()->SetTitleOffset(1.2);
    gr->GetYaxis()->SetTitleOffset(1.2);
    gr->GetXaxis()->SetLabelOffset(0.007);
    gr->GetXaxis()->SetLabelSize(0.04);
    gr->GetYaxis()->SetLabelSize(0.04);
    gr->GetYaxis()->SetTickLength(0.01);
    gr->GetXaxis()->SetTitleFont(42);
    gr->GetYaxis()->SetTitleFont(42);
    gr->GetXaxis()->SetLabelFont(42);
    gr->GetYaxis()->SetLabelFont(42);


    gr->SetMarkerColor(kBlack);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.5);
}

void DrawFuns::prettify (TGraphErrors *gr) {
  prettify ( (TGraph*) gr );
}

void DrawFuns::prettify (TMultiGraph *gr) {
  gr->SetTitle("");
  gr->GetXaxis()->SetTitle("X axis [unit]");
  gr->GetYaxis()->SetTitle("Y axis [unit]");
  gr->GetXaxis()->SetTitleSize(0.05);
  gr->GetYaxis()->SetTitleSize(0.05);
  gr->GetXaxis()->SetTitleOffset(1.2);
  gr->GetYaxis()->SetTitleOffset(1.2);
  gr->GetXaxis()->SetLabelOffset(0.007);
  gr->GetXaxis()->SetLabelSize(0.04);
  gr->GetYaxis()->SetLabelSize(0.04);
  gr->GetYaxis()->SetTickLength(0.01);
  gr->GetXaxis()->SetTitleFont(42);
  gr->GetYaxis()->SetTitleFont(42);
  gr->GetXaxis()->SetLabelFont(42);
  gr->GetYaxis()->SetLabelFont(42);
}

void DrawFuns::prettify (TVirtualPad *c, string histType = "th1") {
  if (CANVASTHESIS) {

  } else {
    gStyle->SetOptStat(0);
    c->SetGrid();
    gStyle->SetGridColor(kGray);
    gStyle->SetGridWidth(1);
    gStyle->SetGridStyle(1001);
    // c->SetLogx();
    c->SetTopMargin(0.02);
    c->SetBottomMargin(0.18);
    c->SetLeftMargin(0.15 );
    c->SetRightMargin(0.03);
    // if (!histType.compare("th2")) {
    //   c->SetRightMargin(0.15);
    // }
    c->SetTitle("");
    c->Update();
  }


}

void DrawFuns::prettify (TVirtualPad *c) {
  prettify( (TVirtualPad*) c, "th1");
}

void DrawFuns::prettify (TCanvas *c, string histType = "th1") {
  prettify( (TVirtualPad*) c, histType);
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
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleOffset(1.0);
  h->GetXaxis()->SetLabelOffset(0.007);
  h->GetXaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleFont(42);
  h->GetXaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelSize(0.04);
  h->GetYaxis()->SetLabelSize(0.04);
  h->GetYaxis()->SetTickLength(0.01);
  h->SetDrawOption("colz");
}

void DrawFuns::prettify (TH1 *h) {
  prettify (h, "red");
}

void DrawFuns::prettify (TLine *l ) {
  l->SetLineWidth(3);
  // l->SetLineColor(kRed+1);
  TColor* colobj = new TColor();
  l->SetLineColor(colobj->GetColor("#EE6D6D") );

}


void DrawFuns::prettify (TH1 *h , std::string color = "red") {
  //gr->GetXaxis()->SetLogx();
  int32_t colorNum = kRed;
  int32_t colorFill = 3005;
  TColor* clrObj = new TColor();
  if(!color.compare("red")) {

    // colorNum = kRed+1;
    // colorFill = 3005;
    colorNum = clrObj->GetColor("#952929");
    colorFill = 1001;
  }
  else if (!color.compare("light blue")) {
    colorNum = kBlue-7;
    colorFill = 3004;
  }
  else if (!color.compare("dark blue") || !color.compare("blue") ) {
    // colorNum = kBlue-1;
    // colorFill = 3004;
    colorNum = clrObj->GetColor("#418f8f");
    colorFill = 1001;
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
  else if (!color.compare("gray")) {
    colorNum = kGray+2;
    colorFill = 3005;
  }
  else {
    colorNum = kBlack;
    colorFill = 3005;
  }
  gStyle->SetOptStat(0);
  // h->SetLineColor(colorNum);
  h->SetLineColor(colorNum);
  h->SetFillColorAlpha(colorNum, 0.6);
  h->SetFillStyle(colorFill);
  //h->Draw("AL.");
  //h->SetMarkerColor(kBlack);
  //h->GetYaxis()->SetRangeUser(minYd,maxYd);
  //h->GetXaxis()->SetRangeUser(0,endPulsed-startPulsed);
  h->SetLineWidth(1);
  //h->SetName("haph");
  h->SetTitle("");
  h->GetXaxis()->SetTitle("X axis [unit]");
  h->GetYaxis()->SetTitle("Y axis [unit]");
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetLabelOffset(0.007);
  h->GetXaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleFont(42);
  h->GetXaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelSize(0.04);
  h->GetYaxis()->SetLabelSize(0.04);
  h->GetYaxis()->SetTickLength(0.01);

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
  for (int32_t i=1; i<=h->GetNbinsX(); i++) {
    sum += h->GetBinContent(i);
  }
  for (int32_t i=1; i<=h->GetNbinsX(); i++) {
    if (h->GetBinContent(i) != 0)
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
