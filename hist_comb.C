// Combined jets
// Author: Grecia Castelazo
// a_J dijet asymmetry
// x_J dijet inbounds
// deltaPhi

#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"

void hist_comb(std::string filename)
{
  std::cout << filename << std::endl;
  TFile *f5 = new TFile(filename.c_str(),"read");
  TTree *t5 = (TTree*)f5->Get("akR4ESchemeJetTree");

  t5->Print("ALL");
  const Int_t nmaxref = 500;
  Int_t nref_;
  Float_t genjtpt_[nmaxref];
  Float_t genjteta_[nmaxref];
  Float_t genjtphi_[nmaxref];

  // branches                                                                     
  t5->SetBranchAddress("nref",&nref_);
  t5->SetBranchAddress("jtpt",genjtpt_);
  t5->SetBranchAddress("jtphi",genjtphi_);

  //a_j histogram
  TH1F* hist_aj = new TH1F("aj_h",";a_J;Counts",100,0,100);
  TH1F* hist_xj = new TH1F("xj_h",";x_J;Counts",100,0,100);
  TH1F* hist_dphi = new TH1F("dphi_h",";d_Phi;Counts",100,-TMath::Pi(), TMath::Pi());

  hist_aj->SetMarkerColorAlpha(kRed, 0.35);
  
  // read all entries and fill the histograms
  Int_t nentries = (Int_t)t5->GetEntries();
  for (Int_t i = 0; i < nentries; i ++) {
    t5->GetEntry(i);
    Float_t tempLeadjtpt_ = -999.;
    Float_t tempSubjtpt_ = -999.;

    Float_t tempLeadjtphi_ = -999.;
    Float_t tempSubjtphi_ = -999.;

    for(Int_t j = 0; j < nref_; j++){
      // get leading jet                                                                                                    
       if(genjtpt_[j] > tempLeadjtpt_){
        tempSubjtpt_ = tempLeadjtpt_;
        tempSubjtphi_ = tempLeadjtphi_;

        tempSubjtpt_ = genjtpt_[j];
        tempSubjtphi_ = genjtphi_[j];
      }
      // get subleading jet                                                                                                   
      else if(genjtpt_[j] > tempSubjtpt_){
        tempSubjtpt_ = genjtpt_[j];
        tempSubjtphi_ = genjtphi_[j];
      }
    }
    if(tempLeadjtpt_ < 40.) continue;
    if(tempSubjtpt_ < 15.) continue;
    Float_t dphi = tempLeadjtphi_ - tempSubjtphi_;
    
    if(dphi > TMath::Pi()) dphi -= 2*TMath::Pi();
    else if(dphi < -TMath::Pi()) dphi += 2*TMath::Pi();
    
    hist_xj->Fill(TMath::Abs(dphi));
    
    if(TMath::Abs(dphi) < 7.*TMath::Pi()/8.) continue;
    
    Float_t aj = (tempLeadjtpt_ - tempSubjtpt_)/(tempLeadjtpt_+ tempSubjtpt_);
    Float_t xj = tempSubjtpt_ / tempLeadjtpt_;
    
    hist_aj->Fill(aj);
    hist_xj->Fill(xj);
  }
  
  gStyle->SetOptStat(0);
  
  TFile *w5 = new TFile("outfile5.root","recreate");
  hist_aj->Write("",TObject::kOverwrite);
  hist_xj->Write("",TObject::kOverwrite);
  hist_dphi->Write("",TObject::kOverwrite);

  w5->Close();
  delete w5;

  TCanvas* canv_aj = new TCanvas("aj_h","aj distribution",400,400);
  hist_aj->Draw("hist e1");
  canv_aj->SaveAs("hist_aj.pdf");

  TCanvas* canv_xj = new TCanvas("xj_h","xj distribution",400,400);
  hist_aj->Draw("hist e1");
  canv_aj->SaveAs("hist_xj.pdf");

  TCanvas* dphi = new TCanvas("dphi_h","dphi distribution",400,400);
  hist_aj->Draw("hist e1");
  canv_aj->SaveAs("hist_xj.pdf");

  delete hist_aj;
  delete hist_xj;
  delete hist_dphi;

  f5->Close();
  delete f5;
  
  return;
}
