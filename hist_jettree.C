// histograms for jet distribution
// Author: Grecia Castelazo

#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

void hist_jettree(std::string filename)
{
  std::cout << filename << std::endl;
  TFile *f3 = new TFile(filename.c_str(),"read");
  TTree *t3 = (TTree*)f3->Get("akR4ESchemeJetTree");

  t3->Print("ALL");
  const Int_t nmaxref = 500;
  Int_t nref_;
  Float_t genjtpt_[nmaxref];
  Float_t genjteta_[nmaxref];
  Float_t genjtphi_[nmaxref];

  // get branches
  t3->SetBranchAddress("nref",&nref_);
  t3->SetBranchAddress("jtpt",genjtpt_);
  t3->SetBranchAddress("jteta",genjtphi_);
  t3->SetBranchAddress("jtphi",genjtphi_);

  // create histogram 
  TH1F* hist_p3 = new TH1F("hnref","nref distribution",20,0,20);
  TH1F* hist_p4 = new TH1F("hgenjtpt","jtpt distribution",100,0,100);
  TH1F* hist_p5 = new TH1F("hgenjteta","jteta distribution",100,-4,4);
  TH1F* hist_p6 = new TH1F("hgenjtphi","jtphi distribution",100,-4,4);

  hist_p3->SetMarkerColorAlpha(kRed, 0.35);
  hist_p4->SetMarkerColorAlpha(kRed, 0.35);
  hist_p5->SetMarkerColorAlpha(kRed, 0.35);
  hist_p6->SetMarkerColorAlpha(kRed, 0.35);

  //read all entries and fill the histograms
  Int_t nentries = (Int_t)t3->GetEntries();
  for (Int_t k = 0; k < nentries; k++) {
    t3->GetEntry(k);
    hist_p3->Fill(nref_);

    for(Int_t kI = 0; kI < nref_; kI++){
      hist_p4->Fill(genjtpt_[kI]);
      hist_p5->Fill(genjteta_[kI]);
      hist_p6->Fill(genjtphi_[kI]);
    }
  }

  TFile *w3 = new TFile("outfile3.root","recreate");
  hist_p3->Write("", TObject::kOverwrite);
  hist_p4->Write("", TObject::kOverwrite);
  hist_p5->Write("", TObject::kOverwrite); 
  hist_p6->Write("", TObject::kOverwrite);

  w3->Close();
  delete w3;

  TCanvas* canv_p3 = new TCanvas("hnref","nref distribution",400,400);
  hist_p3->Draw("hist e1");
  canv_p3->SaveAs("hist3.pdf");

  TCanvas* canv_p4 = new TCanvas("hgenjtpt","jtpt distribution",400,400);
  hist_p4->Draw("hist e1");
  canv_p4->SaveAs("hist4.pdf");

  TCanvas* canv_p5 = new TCanvas("hgeneta","jteta distribution",400,400);
  hist_p5->Draw("hist e1");
  canv_p5->SaveAs("hist5.pdf");

  TCanvas* canv_p6 = new TCanvas("hgenjtphi","jtphi distribution",400,400);
  hist_p6->Draw("hist e1");
  canv_p6->SaveAs("hist6.pdf");

  f3->Close();
  delete f3;

  return;

}
