// histograms for inclusive/leading/subleading                                                                                 
// Author: Grecia Castelazo                                                                                       

#include <iostream>
#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

void work06202018(std::string filename)
{
  std::cout << filename << std::endl;
  TFile *f4 = new TFile(filename.c_str(),"read");
  TTree *t4 = (TTree*)f3->Get("akR4ESchemeJetTree");

  t4->Print("ALL");
  const Int_t nmaxref = 500;
  Int_t nref_;
  Float_t genjtpt_[nmaxref];
  Float_t genjteta_[nmaxref];
  Float_t genjtphi_[nmaxref];
  // branches
  t4->SetBranchAddress("nref",&nref_);
  t4->SetBranchAddress("jtpt",genjtpt_);
  t4->SetBranchAddress("jteta",genjtphi_);
  t4->SetBranchAddress("jtphi",genjtphi_);

  // nref histogram                                                                                              
  TH1F* hist_p0 = new TH1F("hnref","nref distribution",20,0,20);
  // Pt histograms
  TH1F* hist_p1inc = new TH1F("hgenjtpt","jtpt distribution",100,0,100);
  TH1F* hist_p1lead = new TH1F("hgenjteta","jteta distribution",100,-4,4);
  TH1F* hist_p1sub = new TH1F("hgenjtphi","jtphi distribution",100,-4,4);
  // Phi histograms
  TH1F* hist_p2inc = new TH1F("hgenjtpt","jtpt distribution",100,0,100);
  TH1F* hist_p2lead = new TH1F("hgenjteta","jteta distribution",100,-4,4);
  TH1F* hist_p2sub = new TH1F("hgenjtphi","jtphi distribution",100,-4,4);
  // eta histograms
  TH1F* hist_p3inc = new TH1F("hgenjtpt","jtpt distribution",100,0,100);
  TH1F* hist_p3lead = new TH1F("hgenjteta","jteta distribution",100,-4,4);
  TH1F* hist_p3sub = new TH1F("hgenjtphi","jtphi distribution",100,-4,4);

  hist_p0->SetMarkerColorAlpha(kRed, 0.35);

  //read all entries and fill the histograms                                                                      
  Int_t nentries = (Int_t)t4->GetEntries();
  for (Int_t i = 0; i < nentries; i ++) {
    t4->GetEntry(i);
    hist_p0->Fill(nref_);
    
    Float_t tempLeadjtpt_ = -999.;
    Float_t tempSubjtpt_ = -999.;

    Float_t tempLeadjtphi_ = -999.;
    Float_t tempSubjtphi_ = -999.;

    Float_t tempLeadjteta_ = -999.;
    Float_t tempSubjteta_ = -999.;
    
    for(Int_t j = 0; j < nref_; j++){
      hist_p1inc->Fill(genjtpt_[j]);
      hist_p2inc->Fill(genjteta_[j]);
      hist_p3inc->Fill(genjtphi_[j]);
      
      // get leading jet
      if(genjtpt_[j] > tempLeadjtpt_){
	tempSubjtpt_ = tempLeadjtpt_;
	tempSubjtphi_ = tempLeadjtphi_;
	tempSubjteta_ = tempLeadjteta_;

	tempSubjtpt_ = genjtpt_[j];
	tempSubjtphi_ = genjtphi_[j];
	tempSubjteta_ = genjteta-[j];
      }
      // get subleading jet
      else if(genjtpt_[j] > tempSubjtpt_){
	tempSubjtpt_ = genjtpt_[j];
	tempSubjtphi_ = genjtphi_[j];
	tempSubjteta_ = genjteta_[j];
      }
    }
    hist_p1lead->Fill(tempLeadjtpt);
    hist_p1sub->Fill(tempSubjtpt);

    hist_p2lead->Fill(tempLeadjtphi);
    hist_p2sub->Fill(tempSubjtphi);

    hist_p3lead->Fill(tempLeadjteta);
    hist_p3sub->Fill(tempLeadjteta);
  }

  TFile *w4 = new TFile("outfile3.root","recreate");
  hist_p0->Write("", TObject::kOverwrite);

  hist_p1inc->Write("", TObject::kOverwrite);
  hist_p1lead->Write("", TObject::kOverwrite);
  hist_p1sub->Write("", TObject::kOverwrite);

  hist_p2inc->Write("", TObject::kOverwrite);
  hist_p2lead->Write("", TObject::kOverwrite);
  hist_p2sub->Write("", TObject::kOverwrite);

  hist_p3inc->Write("", TObject::kOverwrite);
  hist_p3lead->Write("", TObject::kOverwrite);
  hist_p3sub->Write("", TObject::kOverwrite);

  w4->Close();
  delete w4;

  TCanvas* canv_p0 = new TCanvas("hnref","nref distribution",400,400);
  hist_p0->Draw("hist e1");
  canv_p0->SaveAs("hist0.pdf");

  TCanvas* canv_p1inc = new TCanvas("hgenjtpt","jtpt inclusive",400,400);
  hist_p1inc->Draw("hist e1");
  canv_p1inc->SaveAs("hist1inc.pdf");

  TCanvas* canv_p1lead = new TCanvas("hjtptlead","jtpt leading",400,400);
  hist_p1lead->Draw("hist e1");
  canv_p1lead->SaveAs("hist1lead.pdf");

  TCanvas* canv_p1sub = new TCanvas("hjtptsub","jtpt subleading",400,400);
  hist_p1sub->Draw("hist e1");
  canv_p1sub->SaveAs("hist1sub.pdf");

  TCanvas* canv_p2inc = new TCanvas("hgenjtphi","jtphi inclusive",400,400);
  hist_p2inc->Draw("hist e1");
  hist_p2inc->SaveAs("hist2inc.pdf");
		
  TCanvas* canv_p2lead = new TCanvas("hjtphilead","jtpt subleading",400,400);
  hist_p2lead->Draw("hist e1");
  canv_p2lead->SaveAs("hist2lead.pdf");

  TCanvas* canv_p2sub = new TCanvas("hgenphisub","jtphi inclusive",400,400);
  hist_p2sub->Draw("hist e1");
  hist_p2sub->SaveAs("hist2sub.pdf");

  TCanvas* canv_p3inc = new TCanvas("hgenjteta","jtphi inclusive",400,400);
  hist_p2inc->Draw("hist e1");
  hist_p2inc->SaveAs("hist3inc.pdf");

  TCanvas* canv_p3lead = new TCanvas("hjtetalead","jtpt subleading",400,400);
  hist_p2lead->Draw("hist e1");
  canv_p2lead->SaveAs("hist3lead.pdf");

  TCanvas* canv_p2sub = new TCanvas("hjtetasub","jtphi inclusive",400,400);
  hist_p2sub->Draw("hist e1");
  hist_p2sub->SaveAs("hist3sub.pdf");
		  
  delete hist_p0;
  delete hist_p1inc;
  delete hist_p1lead;
  delete hist_p1sub;
  delete hist_p2inc;
  delete hist_p2lead;
  delete hist_p2sub;
  delete hist_p3inc;
  delete hist_p3lead;
  delete hist_p3sub;
		    
  f4->Close();
  delete f4;

  return;

}
