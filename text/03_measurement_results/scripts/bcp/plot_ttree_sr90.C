{
    gROOT->Reset();

    
    TH1F *h1 = new TH1F("h1","Collected charge for SR90 source",120,0,0.12);
    TH1F *h2 = new TH1F("h2","Collected charge for SR90 source",120,0,0.12);
    TH1F *h3 = new TH1F("h3","Collected charge for SR90 source",120,0,0.12);
    
    
    //   Connect file generated in $ROOTSYS/test
    TFile f1("../../results/S37/SR90_Cx/S37_SR90_Cx_548V.root");
    //TFile f2("../results/S50/AM241_Cx/S50_AM241_Cx_500V.root");
    TFile f3("../../results/1scdhq/SR90_Cx/1scdhq_SR90_Cx_460V.root");
    
    TTree *tree1 = (TTree *)f1->Get("ntuple");
    //TTree *tree2 = (TTree *)f2->Get("ntuple");
    TTree *tree3 = (TTree *)f3->Get("ntuple");
    
    // To use SetBranchAddress() with simple types (e.g. double, int)
    // instead of objects (e.g. std::vector<Particle>).
    tree1->SetMakeClass(1);
    //tree2->SetMakeClass(1);
    tree3->SetMakeClass(1);
    
    Float_t amplitude1;
    TBranch *amplitudeBranch1 = new TBranch();
   // Float_t amplitude2;
   // TBranch *amplitudeBranch2 = new TBranch();
    Float_t amplitude3;
    TBranch *amplitudeBranch3 = new TBranch();
    // Connect the branch "fEventSize" with the variable
    // eventSize that we want to contain the data.
    // While we are at it, ask the tree to save the branch
    // in eventSizeBranch
    tree1->SetBranchAddress("ampl", &amplitude1, &amplitudeBranch1);
    //tree2->SetBranchAddress("ampl", &amplitude2, &amplitudeBranch2);
    tree3->SetBranchAddress("ampl", &amplitude3, &amplitudeBranch3);
    // First, get the total number of entries
    Long64_t nentries = tree1->GetEntries();
    for (Long64_t i=0;i<nentries;i++) {
        amplitudeBranch1->GetEntry(i);
        h1->Fill(amplitude1);
    }
   /* Long64_t nentries = tree2->GetEntries();
    for (Long64_t i=0;i<nentries;i++) {
        amplitudeBranch2->GetEntry(i);
        h2->Fill(amplitude2);
    }*/
    Long64_t nentries = tree3->GetEntries();
    for (Long64_t i=0;i<nentries;i++) {
        amplitudeBranch3->GetEntry(i);
        h3->Fill(amplitude3);
    }
    h1->Draw();
   // h2->Draw("same");
    h3->Draw("same");
    
    h1->GetXaxis()->SetTitle("Pulse amplitude [V]");
    h1->GetYaxis()->SetTitle("N. entries");
    
    h1->GetYaxis()->SetRangeUser(0,1000);
    
    h1->SetLineColor(kBlack);
    h2->SetLineColor(kRed);
    h3->SetLineColor(kBlue);
    
    h1->SetLineWidth(1.2);
  //  h2->SetLineWidth(1.2);
    h3->SetLineWidth(1.2);
    //legend
    TLegend *leg = new TLegend(0.65,0.60,0.80,0.75,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetFillStyle(4000);
    leg->SetShadowColor(0);
    leg->SetLineColor(0);
    leg->SetTextSize(0.04);
    leg->SetFillColor(kWhite);
    leg->AddEntry(h1,"S37 @ 548 V","L");
   // leg->AddEntry(h2,"S50","L");
    leg->AddEntry(h3,"1scdhq @ 460 V","L");
    leg->Draw();
    
    
}