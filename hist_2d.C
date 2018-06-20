// 2D histogram first data file e+e-
// Author: Grecia Castelazo

#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"

void hist_2d(std::string filename)
{
  std::cout << filename << std::endl;
  TFile *f2 = new TFile(filename.c_str(),"read");
  TTree *t2 = (TTree*)f2->Get("t");

  t2->Print("ALL");
  const Int_t nmaxParticle=500;
  Int_t nParticle_;
  Float_t geneta_[nmaxParticle];
  Float_t genphi_[nmaxParticle];

  //get branches
  t2->SetBranchAddress("nParticle",&nParticle_);
  t2->SetBranchAddress("eta",geneta_);
  t2->SetBranchAddress("phi",genphi_);

  //create histogram
  TH2F* hist_p2 = new TH2F("hhist_p2","2d histogram phi eta",40,-4,3,40,-20,20);

  hist_p2->SetMarkerColorAlpha(kRed,0.35);
  
  //read all entries and fill the histogram
  Int_t n_entries = (Int_t)t2->GetEntries();
  for (Int_t j = 0 ; j < n_entries; j++){
    t2->GetEntry(j);
    
    for(Int_t k = 0; k < nParticle_; k++) {
      hist_p2->Fill(geneta_[k],genphi_[k]);
    }
  }

  TH1D* projh2X = hist_p2->ProjectionX();
  TH1D* projh2Y = hist_p2->ProjectionY();

  // Define Fit function
  TF1* fit = new TF1("fit", "gaus", -5, 5);
  projh2Y->Fit(fit);
  projh2X->Fit(fit);

  TFile *w2 = new TFile("outfile2.root","recreate");
  hist_p2->Write("",TObject::kOverwrite);

  w2->Close();
  delete w2;

  TCanvas* canv_p2yz = new TCanvas("c0","YZ projection",400,400);
  projh2Y->Draw("hist e2");
  fit->Draw("same");
  canv_p2yz->SaveAs("histogram2dyz2.pdf");

  TCanvas* canv_p2zx = new TCanvas("c1","ZX projection",400,400);
  projh2X->Draw("hist e2");
  fit->Draw("same");
  canv_p2zx->SaveAs("histogram2dzx2.pdf");

  TCanvas* canv_p2 = new TCanvas("c2","2d histogram phi eta",400,400);
  hist_p2->Draw("surf2");
  fit->Draw("same");
  canv_p2->SaveAs("histogram2d.pdf");
    
  f2->Close();
  delete f2;

  return;

}
