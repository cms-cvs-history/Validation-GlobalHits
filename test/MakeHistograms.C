///////////////////////////////////////////////////////////////////////////////
// Macro to produce histograms from the GlobalValProducer
//
// root -b -q MakeHistograms.C
///////////////////////////////////////////////////////////////////////////////
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

void MakeHistograms()
{
  gROOT->Reset();
  //http://root.cern.ch/root/html/src/TStyle.cxx.html#TStyle:SetOptStat
  gStyle->SetOptStat("emruo");

  // setup names
  TString srcname = "GlobalValProducer.root";
  TString treename = "Events";
  TString brnchname = "PGlobalSimHit_gvp_GlobalStats.obj";

  // clear memory of file name
  delete gROOT->GetListOfFiles()->FindObject(srcname);

  // open source file
  TFile *srcfile = new TFile(srcname);
  
  // get tree from file
  TTree *srcevts = dynamic_cast<TTree*>(srcfile->Get(treename));
  assert(srcevts != 0);
  
  // get branch from tree
  TBranch *srcbrnch = srcevts->GetBranch(brnchname);
  assert(srcbrnch != 0);

  // get number of events
  Int_t nsrcevts = srcevts->GetEntries();

  // set access to branch as proper object
  PGlobalSimHit srcGlobalStats;
  srcbrnch->SetAddress(&srcGlobalStats);

  // create canvas
  Int_t cWidth = 928, cHeight = 1218;
  TCanvas *myCanvas = new TCanvas("gvp","gvp",cWidth,cHeight);
  
  // open output ps file
  TString filename = "GlobalValHistograms";
  TString psfile = filename+".ps";
  TString psfileopen = filename+".ps[";
  TString psfileclose = filename+".ps]";
  myCanvas->Print(psfileopen);

  // open output root file
  TString rootfile = filename+".root";
  TFile *f1 = new TFile(rootfile,"recreate");
  if (f1->IsZombie()) {
    cout << "***Unable to open " << outroot << endl;
    return;
  }

  // create label
  TLatex *label = new TLatex();
  label->SetNDC();
  TString labeltitle;

  // create attributes
  Int_t srccolor = kBlue;
  Int_t linewidth = 2;

  // create histograms
  // MC
  TH1F *hMCRGP[2];
  hMCRGP[0] = 
    new TH1F("MCRGP_1003","RawGenParticles",100,0.,5000.);
  hMCRGP[1] = 
    new TH1F("MCRGP2_1013","RawGenParticles",100,0.,500.);
  for (Int_t i = 0; i < 2; ++i) {
    hMCRGP[i]->GetXaxis()->SetTitle("Number of Raw Generated Particles");
    hMCRGP[i]->GetYaxis()->SetTitle("Count");
    hMCRGP[i]->SetLineColor(srccolor);
    hMCRGP[i]->SetLineWidth(linewidth);
  }
 
  // vertex
  TH1F *hMCG4Vtx[2];
  hMCG4Vtx[0] = 
    new TH1F("MCG4Vtx_1001","G4 Vertices",100,0.,50000.);
  hMCG4Vtx[1] = 
    new TH1F("MCG4Vtx2_1011","G4 Vertices",100,-0.5,99.5);
  for (Int_t i = 0; i < 2; ++i) {
    hMCG4Vtx[i]->GetXaxis()->SetTitle("Number of Vertices");
    hMCG4Vtx[i]->GetYaxis()->SetTitle("Count");
    hMCG4Vtx[i]->SetLineColor(srccolor);
    hMCG4Vtx[i]->SetLineWidth(linewidth);
  }
  TH1F *hGeantVtxX = 
    new TH1F("GeantVtxX_1101","Geant vertex x/micrometer",101,-50.,50.);
  hGeantVtxX->GetXaxis()->SetTitle("x of Vertex (um)");
  hGeantVtxX->GetYaxis()->SetTitle("Count");
  hGeantVtxX->SetLineColor(srccolor);
  hGeantVtxX->SetLineWidth(linewidth);
  TH1F *hGeantVtxY =
    new TH1F("GeantVtxY_1102","Geant vertex y/micrometer",101,-50.,50.);
  hGeantVtxY->GetXaxis()->SetTitle("y of Vertex (um)");
  hGeantVtxY->GetYaxis()->SetTitle("Count");
  hGeantVtxY->SetLineColor(srccolor);
  hGeantVtxY->SetLineWidth(linewidth);
  TH1F *hGeantVtxZ =
    new TH1F("GeantVtxZ_1103","Geant vertex z/millimeter",101,-250.,250.);
  hGeantVtxZ->GetXaxis()->SetTitle("z of Vertex (mm)");
  hGeantVtxZ->GetYaxis()->SetTitle("Count");
  hGeantVtxZ->SetLineColor(srccolor);
  hGeantVtxZ->SetLineWidth(linewidth);

  // tracks
  TH1F *hMCG4Trk[2];
  hMCG4Trk[0] = 
    new TH1F("MCG4Trk_1002","G4 Tracks",150,0.,15000.);
  hMCG4Trk[1] = 
    new TH1F("MCG4Trk2_1012","G4 Tracks",150,-0.5,99.5);  
  for (Int_t i = 0; i < 2; ++i) {
    hMCG4Trk[i]->GetXaxis()->SetTitle("Number of Tracks");
    hMCG4Trk[i]->GetYaxis()->SetTitle("Count");
    hMCG4Trk[i]->SetLineColor(srccolor);
    hMCG4Trk[i]->SetLineWidth(linewidth);
  }
  TH1F *hGeantTrkPt =
    new TH1F("GeantTrkPt_1201","Geant track pt/GeV",100,0.,200.);
  hGeantTrkPt->GetXaxis()->SetTitle("pT of Track (GeV)");
  hGeantTrkPt->GetYaxis()->SetTitle("Count");
  hGeantTrkPt->SetLineColor(srccolor);
  hGeantTrkPt->SetLineWidth(linewidth);
  TH1F *hGeantTrkE =
    new TH1F("GeantTrkE_1202","Geant track E/GeV",100,0.,5000.);
  hGeantTrkE->GetXaxis()->SetTitle("E of Track (GeV)");
  hGeantTrkE->GetYaxis()->SetTitle("Count");
  hGeantTrkE->SetLineColor(srccolor);
  hGeantTrkE->SetLineWidth(linewidth);

  // ecal
  TH1F *hCaloEcal[2];
  hCaloEcal[0] = 
    new TH1F("hCaloEcal_2101","Ecal hits",100,0.,10000.);
  hCaloEcal[1] = 
    new TH1F("hCaloEcal2_2111","Ecal hits",100,-0.5,99.5);
  TH1F *hCaloEcalE[2];
  hCaloEcalE[0] = 
    new TH1F("hCaloEcalE_2102","Ecal hits, energy/GeV",100,0.,10.);
  hCaloEcalE[1] = 
    new TH1F("hCaloEcalE2_2112","Ecal hits, energy/GeV",100,0.,0.1);
  TH1F *hCaloEcalToF[2];
  hCaloEcalToF[0] = 
    new TH1F("hCaloEcalToF_2103","Ecal hits, ToF/ns",100,0.,1000.);
  hCaloEcalToF[1] = 
    new TH1F("hCaloEcalToF2_2113","Ecal hits, ToF/ns",100,0.,100.);
  for (Int_t i = 0; i < 2; ++i) {
    hCaloEcal[i]->GetXaxis()->SetTitle("Number of Hits");
    hCaloEcal[i]->GetYaxis()->SetTitle("Count");
    hCaloEcal[i]->SetLineColor(srccolor);
    hCaloEcal[i]->SetLineWidth(linewidth);
    hCaloEcalE[i]->GetXaxis()->SetTitle("Energy of Hits (GeV)");
    hCaloEcalE[i]->GetYaxis()->SetTitle("Count");
    hCaloEcalE[i]->SetLineColor(srccolor);
    hCaloEcalE[i]->SetLineWidth(linewidth);
    hCaloEcalToF[i]->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
    hCaloEcalToF[i]->GetYaxis()->SetTitle("Count");
    hCaloEcalToF[i]->SetLineColor(srccolor);
    hCaloEcalToF[i]->SetLineWidth(linewidth);
  }
  TH1F *hCaloEcalPhi =
    new TH1F("hCaloEcalPhi_2104","Ecal hits, phi/rad",100,-3.2,3.2);
  hCaloEcalPhi->GetXaxis()->SetTitle("Phi of Hits (rad)");
  hCaloEcalPhi->GetYaxis()->SetTitle("Count");
  hCaloEcalPhi->SetLineColor(srccolor);
  hCaloEcalPhi->SetLineWidth(linewidth);
  TH1F *hCaloEcalEta =
    new TH1F("hCaloEcalEta_2105","Ecal hits, eta",100,-5.5,5.5);
  hCaloEcalEta->GetXaxis()->SetTitle("Eta of Hits");
  hCaloEcalEta->GetYaxis()->SetTitle("Count");
  hCaloEcalEta->SetLineColor(srccolor);
  hCaloEcalEta->SetLineWidth(linewidth);

  // preshower
  TH1F *hCaloPreSh[2];
  hCaloPreSh[0] = 
    new TH1F("hCaloPreSh_2201","PreSh hits",100,0.,10000.);
  hCaloPreSh[1] = 
    new TH1F("hCaloPreSh2_2211","PreSh hits",100,-0.5,99.5);
  TH1F *hCaloPreShE[2];
  hCaloPreShE[0] = 
    new TH1F("hCaloPreShE_2202","PreSh hits, energy/GeV",100,0.,10.);
  hCaloPreShE[1] = 
    new TH1F("hCaloPreShE2_2212","PreSh hits, energy/GeV",100,0.,0.1);
  TH1F *hCaloPreShToF[2];
  hCaloPreShToF[0] = 
    new TH1F("hCaloPreShToF_2203","PreSh hits, ToF/ns",100,0.,1000.);
  hCaloPreShToF[1] = 
    new TH1F("hCaloPreShToF2_2213","PreSh hits, ToF/ns",100,0.,100.);
  for (Int_t i = 0; i < 2; ++i) {
    hCaloPreSh[i]->GetXaxis()->SetTitle("Number of Hits");
    hCaloPreSh[i]->GetYaxis()->SetTitle("Count");
    hCaloPreSh[i]->SetLineColor(srccolor);
    hCaloPreSh[i]->SetLineWidth(linewidth);
    hCaloPreShE[i]->GetXaxis()->SetTitle("Energy of Hits (GeV)");
    hCaloPreShE[i]->GetYaxis()->SetTitle("Count");
    hCaloPreShE[i]->SetLineColor(srccolor);
    hCaloPreShE[i]->SetLineWidth(linewidth);
    hCaloPreShToF[i]->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
    hCaloPreShToF[i]->GetYaxis()->SetTitle("Count");
    hCaloPreShToF[i]->SetLineColor(srccolor);
    hCaloPreShToF[i]->SetLineWidth(linewidth);
  }
  TH1F *hCaloPreShPhi =
    new TH1F("hCaloPreShPhi_2204","PreSh hits, phi/rad",100,-3.2,3.2);
  hCaloPreShPhi->GetXaxis()->SetTitle("Phi of Hits (rad)");
  hCaloPreShPhi->GetYaxis()->SetTitle("Count");
  hCaloPreShPhi->SetLineColor(srccolor);
  hCaloPreShPhi->SetLineWidth(linewidth);
  TH1F *hCaloPreShEta =
    new TH1F("hCaloPreShEta_2205","PreSh hits, eta",100,-5.5,5.5);
  hCaloPreShEta->GetXaxis()->SetTitle("Eta of Hits");
  hCaloPreShEta->GetYaxis()->SetTitle("Count");
  hCaloPreShEta->SetLineColor(srccolor);
  hCaloPreShEta->SetLineWidth(linewidth);

  // hcal
  TH1F *hCaloHcal[2];
  hCaloHcal[0] = 
    new TH1F("hCaloHcal_2301","Hcal hits",100,0.,10000.);
  hCaloHcal[1] = 
    new TH1F("hCaloHcal2_2311","Hcal hits",100,-0.5,99.5);
  TH1F *hCaloHcalE[2];
  hCaloHcalE[0] = 
    new TH1F("hCaloHcalE_2302","Hcal hits, energy/GeV",100,0.,10.);
  hCaloHcalE[1] = 
    new TH1F("hCaloHcalE2_2312","Hcal hits, energy/GeV",100,0.,0.1);
  TH1F *hCaloHcalToF[2];
  hCaloHcalToF[0] = 
    new TH1F("hCaloHcalToF_2303","Hcal hits, ToF/ns",100,0.,1000.);
  hCaloHcalToF[1] = 
    new TH1F("hCaloHcalToF2_2313","Hcal hits, ToF/ns",100,0.,100.);
  for (Int_t i = 0; i < 2; ++i) {
    hCaloHcal[i]->GetXaxis()->SetTitle("Number of Hits");
    hCaloHcal[i]->GetYaxis()->SetTitle("Count");
    hCaloHcal[i]->SetLineColor(srccolor);
    hCaloHcal[i]->SetLineWidth(linewidth);
    hCaloHcalE[i]->GetXaxis()->SetTitle("Energy of Hits (GeV)");
    hCaloHcalE[i]->GetYaxis()->SetTitle("Count");
    hCaloHcalE[i]->SetLineColor(srccolor);
    hCaloHcalE[i]->SetLineWidth(linewidth);
    hCaloHcalToF[i]->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
    hCaloHcalToF[i]->GetYaxis()->SetTitle("Count");
    hCaloHcalToF[i]->SetLineColor(srccolor);
    hCaloHcalToF[i]->SetLineWidth(linewidth);
  }
  TH1F *hCaloHcalPhi =
    new TH1F("hCaloHcalPhi_2304","Hcal hits, phi/rad",100,-3.2,3.2);
  hCaloHcalPhi->GetXaxis()->SetTitle("Phi of Hits (rad)");
  hCaloHcalPhi->GetYaxis()->SetTitle("Count");
  hCaloHcalPhi->SetLineColor(srccolor);
  hCaloHcalPhi->SetLineWidth(linewidth);
  TH1F *hCaloHcalEta =
    new TH1F("hCaloHcalEta_2305","Hcal hits, eta",100,-5.5,5.5);
  hCaloHcalEta->GetXaxis()->SetTitle("Eta of Hits");
  hCaloHcalEta->GetYaxis()->SetTitle("Count");
  hCaloHcalEta->SetLineColor(srccolor);
  hCaloHcalEta->SetLineWidth(linewidth);

  // pixel tracker
  TH1F *hTrackerPx[2];
  hTrackerPx[0] = 
    new TH1F("hTrackerPx_3101","Pixel hits",100,0.,10000.);
  hTrackerPx[1] = 
    new TH1F("hTrackerPx2_3111","Pixel hits",100,-0.5,99.5);  
  for (Int_t i = 0; i < 2; ++i) {
    hTrackerPx[i]->GetXaxis()->SetTitle("Number of Pixel Hits");
    hTrackerPx[i]->GetYaxis()->SetTitle("Count");
    hTrackerPx[i]->SetLineColor(srccolor);
    hTrackerPx[i]->SetLineWidth(linewidth);
  }
  hTrackerPxPhi =
    new TH1F("hTrackerPxPhi_3102","Pixel hits phi/rad",100,-3.2,3.2);
  hTrackerPxPhi->GetXaxis()->SetTitle("Phi of Hits (rad)");
  hTrackerPxPhi->GetYaxis()->SetTitle("Count");
  hTrackerPxPhi->SetLineColor(srccolor);
  hTrackerPxPhi->SetLineWidth(linewidth);
  hTrackerPxEta =
    new TH1F("hTrackerPxEta_3103","Pixel hits eta",100,-3.5,3.5);
  hTrackerPxEta->GetXaxis()->SetTitle("Eta of Hits");
  hTrackerPxEta->GetYaxis()->SetTitle("Count");
  hTrackerPxEta->SetLineColor(srccolor);
  hTrackerPxEta->SetLineWidth(linewidth);
  hTrackerPxBToF = 
    new TH1F("hTrackerPxBToF_3104","Pixel barrel hits, ToF/ns",100,0.,40.);
  hTrackerPxBToF->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
  hTrackerPxBToF->GetYaxis()->SetTitle("Count");
  hTrackerPxBToF->SetLineColor(srccolor);
  hTrackerPxBToF->SetLineWidth(linewidth);
  hTrackerPxBR = 
    new TH1F("hTrackerPxBR_3106","Pixel barrel hits, R/cm",100,0.,50.);
  hTrackerPxBR->GetXaxis()->SetTitle("R of Hits (cm)");
  hTrackerPxBR->GetYaxis()->SetTitle("Count");
  hTrackerPxBR->SetLineColor(srccolor);
  hTrackerPxBR->SetLineWidth(linewidth);
  hTrackerPxFToF = 
    new TH1F("hTrackerPxFToF_3105","Pixel forward hits, ToF/ns",100,0.,50.);
  hTrackerPxFToF->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
  hTrackerPxFToF->GetYaxis()->SetTitle("Count");
  hTrackerPxFToF->SetLineColor(srccolor);
  hTrackerPxFToF->SetLineWidth(linewidth);
  hTrackerPxFZ = 
    new TH1F("hTrackerPxFZ_3107","Pixel forward hits, Z/cm",201,-100.,100.);
  hTrackerPxFZ->GetXaxis()->SetTitle("Z of Hits (cm)");
  hTrackerPxFZ->GetYaxis()->SetTitle("Count");
  hTrackerPxFZ->SetLineColor(srccolor);
  hTrackerPxFZ->SetLineWidth(linewidth);

  // strip tracker
  TH1F *hTrackerSi[2];
  hTrackerSi[0] = 
    new TH1F("hTrackerSi_3201","Silicon hits",100,0.,10000.);
  hTrackerSi[1] = 
    new TH1F("hTrackerSi2_3211","Silicon hits",100,-0.5,99.5); 
  for (Int_t i = 0; i < 2; ++i) { 
    hTrackerSi[i]->GetXaxis()->SetTitle("Number of Silicon Hits");
    hTrackerSi[i]->GetYaxis()->SetTitle("Count");
    hTrackerSi[i]->SetLineColor(srccolor);
    hTrackerSi[i]->SetLineWidth(linewidth);
  }
  hTrackerSiPhi =
    new TH1F("hTrackerSiPhi_3202","Silicon hits phi/rad",100,-3.2,3.2);
  hTrackerSiPhi->GetXaxis()->SetTitle("Phi of Hits (rad)");
  hTrackerSiPhi->GetYaxis()->SetTitle("Count");
  hTrackerSiPhi->SetLineColor(srccolor);
  hTrackerSiPhi->SetLineWidth(linewidth);
  hTrackerSiEta =
    new TH1F("hTrackerSiEta_3203","Silicon hits eta",100,-3.5,3.5);
  hTrackerSiEta->GetXaxis()->SetTitle("Eta of Hits");
  hTrackerSiEta->GetYaxis()->SetTitle("Count");
  hTrackerSiEta->SetLineColor(srccolor);
  hTrackerSiEta->SetLineWidth(linewidth);
  hTrackerSiBToF = 
    new TH1F("hTrackerSiBToF_3204","Silicon barrel hits, ToF/ns",100,0.,50.);
  hTrackerSiBToF->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
  hTrackerSiBToF->GetYaxis()->SetTitle("Count");
  hTrackerSiBToF->SetLineColor(srccolor);
  hTrackerSiBToF->SetLineWidth(linewidth);
  hTrackerSiBR = 
    new TH1F("hTrackerSiBR_3206","Silicon barrel hits, R/cm",100,0.,200.);
  hTrackerSiBR->GetXaxis()->SetTitle("R of Hits (cm)");
  hTrackerSiBR->GetYaxis()->SetTitle("Count");
  hTrackerSiBR->SetLineColor(srccolor);
  hTrackerSiBR->SetLineWidth(linewidth);
  hTrackerSiFToF = 
    new TH1F("hTrackerSiFToF_3205","Silicon forward hits, ToF/ns",100,0.,75.);
  hTrackerSiFToF->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
  hTrackerSiFToF->GetYaxis()->SetTitle("Count");
  hTrackerSiFToF->SetLineColor(srccolor);
  hTrackerSiFToF->SetLineWidth(linewidth);
  hTrackerSiFZ = 
    new TH1F("hTrackerSiFZ_3207","Silicon forward hits, Z/cm",201,-300.,300.);
  hTrackerSiFZ->GetXaxis()->SetTitle("Z of Hits (cm)");
  hTrackerSiFZ->GetYaxis()->SetTitle("Count");
  hTrackerSiFZ->SetLineColor(srccolor);
  hTrackerSiFZ->SetLineWidth(linewidth);

  //muon common
  TH1F *hMuon[2];
  hMuon[0] = 
    new TH1F("hMuon_4001","Muon hits",100,0.,10000.);
  hMuon[1] = 
    new TH1F("hMuon2_4011","Muon hits",100,-0.5,99.5); 
  for (Int_t i = 0; i < 2; ++i) { 
    hMuon[i]->GetXaxis()->SetTitle("Number of Muon Hits");
    hMuon[i]->GetYaxis()->SetTitle("Count");
    hMuon[i]->SetLineColor(srccolor);
    hMuon[i]->SetLineWidth(linewidth);
  }  
  hMuonPhi =
    new TH1F("hMuonPhi_4002","Muon hits phi/rad",100,-3.2,3.2);
  hMuonPhi->GetXaxis()->SetTitle("Phi of Hits (rad)");
  hMuonPhi->GetYaxis()->SetTitle("Count");
  hMuonPhi->SetLineColor(srccolor);
  hMuonPhi->SetLineWidth(linewidth);
  hMuonEta =
    new TH1F("hMuonEta_4003","Muon hits eta",100,-3.5,3.5);
  hMuonEta->GetXaxis()->SetTitle("Eta of Hits");
  hMuonEta->GetYaxis()->SetTitle("Count");
  hMuonEta->SetLineColor(srccolor);
  hMuonEta->SetLineWidth(linewidth);

  //CSC muon
  TH1F *hMuonCscToF[2];
  hMuonCscToF[0] = 
    new TH1F("hMuonCscToF_4201","Muon CSC hits, ToF/ns",100,0.,250.);
  hMuonCscToF[1] = 
    new TH1F("hMuonCscToF2_4202","Muon CSC hits, ToF/ns",100,0.,50.);
  for (Int_t i = 0; i < 2; ++i) {   
    hMuonCscToF[i]->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
    hMuonCscToF[i]->GetYaxis()->SetTitle("Count");
    hMuonCscToF[i]->SetLineColor(srccolor);
    hMuonCscToF[i]->SetLineWidth(linewidth);
  }  
  hMuonCscZ = 
    new TH1F("hMuonCscZ_4203","Muon CSC hits, Z/cm",100,0.,1500.);
  hMuonCscZ->GetXaxis()->SetTitle("Z of Hits (cm)");
  hMuonCscZ->GetYaxis()->SetTitle("Count");
  hMuonCscZ->SetLineColor(srccolor);
  hMuonCscZ->SetLineWidth(linewidth);

  //DT Muon
  TH1F *hMuonDtToF[2];
  hMuonDtToF[0] = 
    new TH1F("hMuonDtToF_4101","Muon DT hits, ToF/ns",100,0.,250.);
  hMuonDtToF[1] = 
    new TH1F("hMuonDtToF2_4102","Muon DT hits, ToF/ns",100,0.,50.);
  for (Int_t i = 0; i < 2; ++i) {   
    hMuonDtToF[i]->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
    hMuonDtToF[i]->GetYaxis()->SetTitle("Count");
    hMuonDtToF[i]->SetLineColor(srccolor);
    hMuonDtToF[i]->SetLineWidth(linewidth);
  }  
  hMuonDtR = 
    new TH1F("hMuonDtR_4103","Muon DT hits, R/cm",100,0.,1500.);
  hMuonDtR->GetXaxis()->SetTitle("R of Hits (cm)");
  hMuonDtR->GetYaxis()->SetTitle("Count");
  hMuonDtR->SetLineColor(srccolor);
  hMuonDtR->SetLineWidth(linewidth);

  //RPC forward muon
  TH1F *hMuonRpcFToF[2];
  hMuonRpcFToF[0] = 
    new TH1F("hMuonRpcFToF_4304","Muon RPC forward  hits, ToF/ns",100,0.,250.);
  hMuonRpcFToF[1] = 
    new TH1F("hMuonRpcFToF2_4305","Muon RPC forward hits, ToF/ns",100,0.,50.);
  for (Int_t i = 0; i < 2; ++i) {   
    hMuonRpcFToF[i]->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
    hMuonRpcFToF[i]->GetYaxis()->SetTitle("Count");
    hMuonRpcFToF[i]->SetLineColor(srccolor);
    hMuonRpcFToF[i]->SetLineWidth(linewidth);
  }  
  hMuonRpcFZ = 
    new TH1F("hMuonRpcFZ_4306","Muon RPC forward  hits, Z/cm",100,0.,1500.);
  hMuonRpcFZ->GetXaxis()->SetTitle("Z of Hits (cm)");
  hMuonRpcFZ->GetYaxis()->SetTitle("Count");
  hMuonRpcFZ->SetLineColor(srccolor);
  hMuonRpcFZ->SetLineWidth(linewidth);
  
  //RPC Barrel muon
  TH1F *hMuonRpcBToF[2];
  hMuonRpcBToF[0] = 
    new TH1F("hMuonRpcBToF_4101","Muon RPC barrel hits, ToF/ns",100,0.,250.);
  hMuonRpcBToF[1] = 
    new TH1F("hMuonRpcBToF2_4102","Muon RPC barrel hits, ToF/ns",100,0.,50.);
  for (Int_t i = 0; i < 2; ++i) {   
    hMuonRpcBToF[i]->GetXaxis()->SetTitle("Time of Flight of Hits (ns)");
    hMuonRpcBToF[i]->GetYaxis()->SetTitle("Count");
    hMuonRpcBToF[i]->SetLineColor(srccolor);
    hMuonRpcBToF[i]->SetLineWidth(linewidth);
  }  
  hMuonRpcBR = 
    new TH1F("hMuonRpcBR_4103","Muon RPC barrel hits, R/cm",100,0.,1500.);
  hMuonRpcBR->GetXaxis()->SetTitle("R of Hits (cm)");
  hMuonRpcBR->GetYaxis()->SetTitle("Count");
  hMuonRpcBR->SetLineColor(srccolor);
  hMuonRpcBR->SetLineWidth(linewidth);

  // cycle through events in tree
  for (Int_t evt = 0; evt < nsrcevts; ++evt) {
    srcbrnch->GetEntry(evt);

    int nPxlBrlHits = srcGlobalStats.getnPxlBrlHits();
    int nPxlFwdHits = srcGlobalStats.getnPxlFwdHits();
    int nPxlHits = nPxlBrlHits + nPxlFwdHits;
    int nSiBrlHits = srcGlobalStats.getnSiBrlHits();
    int nSiFwdHits = srcGlobalStats.getnSiFwdHits();
    int nSiHits = nSiBrlHits + nSiFwdHits;    
    int nMuonDtHits = srcGlobalStats.getnMuonDtHits();
    int nMuonCscHits = srcGlobalStats.getnMuonCscHits();
    int nMuonRpcBrlHits = srcGlobalStats.getnMuonRpcBrlHits();
    int nMuonRpcFwdHits = srcGlobalStats.getnMuonRpcFwdHits();
    int nMuonHits = nMuonDtHits + nMuonCscHits + nMuonRpcBrlHits +
      nMuonRpcFwdHits;

    // get stats for number of objects
    for (Int_t i = 0; i < 2; ++i) {
      hMCRGP[i]->Fill((float)srcGlobalStats.getnRawGenPart());
      hMCG4Vtx[i]->Fill((float)srcGlobalStats.getnG4Vtx());
      hMCG4Trk[i]->Fill((float)srcGlobalStats.getnG4Trk());
      hCaloEcal[i]->Fill((float)srcGlobalStats.getnECalHits());
      hCaloPreSh[i]->Fill((float)srcGlobalStats.getnPreShHits());
      hCaloHcal[i]->Fill((float)srcGlobalStats.getnHCalHits());
      hTrackerPx[i]->Fill((float)nPxlHits);
      hTrackerSi[i]->Fill((float)nSiHits);
      hMuon[i]->Fill((float)nMuonHits);
    }
    
    // get G4Vertex info
    std::vector<PGlobalSimHit::Vtx> G4Vtx = srcGlobalStats.getG4Vtx();
    for (Int_t i = 0; i < G4Vtx.size(); ++i) {
      hGeantVtxX->Fill(G4Vtx[i].x);
      hGeantVtxY->Fill(G4Vtx[i].y);
      hGeantVtxZ->Fill(G4Vtx[i].z);
    }

    // get G4Track info
    std::vector<PGlobalSimHit::Trk> G4Trk = srcGlobalStats.getG4Trk();
    for (Int_t i = 0; i < G4Trk.size(); ++i) {
      hGeantTrkPt->Fill(G4Trk[i].pt);
      hGeantTrkE->Fill(G4Trk[i].e);
    }

    // get Ecal info
    std::vector<PGlobalSimHit::CalHit> ECalHits = 
      srcGlobalStats.getECalHits();
    for (Int_t i = 0; i < ECalHits.size(); ++i) {
      for (Int_t j = 0; j < 2; ++j) {
	hCaloEcalE[j]->Fill(ECalHits[i].e);
	hCaloEcalToF[j]->Fill(ECalHits[i].tof);
      }
      hCaloEcalPhi->Fill(ECalHits[i].phi);
      hCaloEcalEta->Fill(ECalHits[i].eta);
    }

    // get PreShower info
    std::vector<PGlobalSimHit::CalHit> PreShHits = 
      srcGlobalStats.getPreShHits();
    for (Int_t i = 0; i < PreShHits.size(); ++i) {
      for (Int_t j = 0; j < 2; ++j) {
	hCaloPreShE[j]->Fill(PreShHits[i].e);
	hCaloPreShToF[j]->Fill(PreShHits[i].tof);
      }
      hCaloPreShPhi->Fill(PreShHits[i].phi);
      hCaloPreShEta->Fill(PreShHits[i].eta);
    }

    // get Hcal info
    std::vector<PGlobalSimHit::CalHit> HCalHits = 
      srcGlobalStats.getHCalHits();
    for (Int_t i = 0; i < HCalHits.size(); ++i) {
      for (Int_t j = 0; j < 2; ++j) {
	hCaloHcalE[j]->Fill(HCalHits[i].e);
	hCaloHcalToF[j]->Fill(HCalHits[i].tof);
      }
      hCaloHcalPhi->Fill(HCalHits[i].phi);
      hCaloHcalEta->Fill(HCalHits[i].eta);
    }

    // get Pixel Barrel info
    std::vector<PGlobalSimHit::BrlHit> PxlBrlHits = 
      srcGlobalStats.getPxlBrlHits();
    for (Int_t i = 0; i < PxlBrlHits.size(); ++i) {
      hTrackerPxPhi->Fill(PxlBrlHits[i].phi);
      hTrackerPxEta->Fill(PxlBrlHits[i].eta);
      hTrackerPxBToF->Fill(PxlBrlHits[i].tof);
      hTrackerPxBR->Fill(PxlBrlHits[i].r);
    }

    // get Pixel Forward info
    std::vector<PGlobalSimHit::FwdHit> PxlFwdHits = 
      srcGlobalStats.getPxlFwdHits();
    for (Int_t i = 0; i < PxlFwdHits.size(); ++i) {
      hTrackerPxPhi->Fill(PxlFwdHits[i].phi);
      hTrackerPxEta->Fill(PxlFwdHits[i].eta);
      hTrackerPxFToF->Fill(PxlFwdHits[i].tof);
      hTrackerPxFZ->Fill(PxlFwdHits[i].z);
    }

    // get Strip Barrel info
    std::vector<PGlobalSimHit::BrlHit> SiBrlHits = 
      srcGlobalStats.getSiBrlHits();
    for (Int_t i = 0; i < SiBrlHits.size(); ++i) {
      hTrackerSiPhi->Fill(SiBrlHits[i].phi);
      hTrackerSiEta->Fill(SiBrlHits[i].eta);
      hTrackerSiBToF->Fill(SiBrlHits[i].tof);
      hTrackerSiBR->Fill(SiBrlHits[i].r);
    }

    // get Strip Forward info
    std::vector<PGlobalSimHit::FwdHit> SiFwdHits = 
      srcGlobalStats.getSiFwdHits();
    for (Int_t i = 0; i < SiFwdHits.size(); ++i) {
      hTrackerSiPhi->Fill(SiFwdHits[i].phi);
      hTrackerSiEta->Fill(SiFwdHits[i].eta);
      hTrackerSiFToF->Fill(SiFwdHits[i].tof);
      hTrackerSiFZ->Fill(SiFwdHits[i].z);
    }

    // get Muon CSC info
    std::vector<PGlobalSimHit::FwdHit> MuonCscHits = 
      srcGlobalStats.getMuonCscHits();
    for (Int_t i = 0; i < MuonCscHits.size(); ++i) {
      hMuonPhi->Fill(MuonCscHits[i].phi);
      hMuonEta->Fill(MuonCscHits[i].eta);
      for (Int_t j = 0; j < 2; ++j) {
	hMuonCscToF[j]->Fill(MuonCscHits[i].tof);
      }
      hMuonCscZ->Fill(MuonCscHits[i].z);
    }    

    // get Muon DT info
    std::vector<PGlobalSimHit::BrlHit> MuonDtHits = 
      srcGlobalStats.getMuonDtHits();
    for (Int_t i = 0; i < MuonDtHits.size(); ++i) {
      hMuonPhi->Fill(MuonDtHits[i].phi);
      hMuonEta->Fill(MuonDtHits[i].eta);
      for (Int_t j = 0; j < 2; ++j) {
	hMuonDtToF[j]->Fill(MuonDtHits[i].tof);
      }
      hMuonDtR->Fill(MuonDtHits[i].r);
    }

    // get Muon RPC forward info
    std::vector<PGlobalSimHit::FwdHit> MuonRpcFwdHits = 
      srcGlobalStats.getMuonRpcFwdHits();
    for (Int_t i = 0; i < MuonRpcFwdHits.size(); ++i) {
      hMuonPhi->Fill(MuonRpcFwdHits[i].phi);
      hMuonEta->Fill(MuonRpcFwdHits[i].eta);
      for (Int_t j = 0; j < 2; ++j) {
	hMuonRpcFToF[j]->Fill(MuonRpcFwdHits[i].tof);
      }
      hMuonRpcFZ->Fill(MuonRpcFwdHits[i].z);
    }    

    // get Muon RPC barrel info
    std::vector<PGlobalSimHit::BrlHit> MuonRpcBrlHits = 
      srcGlobalStats.getMuonRpcBrlHits();
    for (Int_t i = 0; i < MuonRpcBrlHits.size(); ++i) {
      hMuonPhi->Fill(MuonRpcBrlHits[i].phi);
      hMuonEta->Fill(MuonRpcBrlHits[i].eta);
      for (Int_t j = 0; j < 2; ++j) {
	hMuonRpcBToF[j]->Fill(MuonRpcBrlHits[i].tof);
      }
      hMuonRpcBR->Fill(MuonRpcBrlHits[i].r);
    }    
  } // end loop through events

  // output to root file
  f1->Write();

  // draw plots
  // MC info
  TCanvas *Canvas1 = new TCanvas("MCRGP","MCRGP",cWidth,cHeight);
  Canvas1->Divide(1,2);
  label->SetTextSize(0.03);
  label->SetTextAlign(22);
  myCanvas = Canvas1;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Monte Carlo RawGenPart");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hMCRGP[i]->Draw();
  }
  myCanvas->Print(psfile);

  // vertex info
  TCanvas *Canvas2 = new TCanvas("G4Vtx","G4Vtx",cWidth,cHeight);
  Canvas2->Divide(2,3);
  myCanvas = Canvas2;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Geant4 Vertices");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hMCG4Vtx[i]->Draw();
  }
  myCanvas->cd(3);
  hGeantVtxX->Draw();
  myCanvas->cd(4);
  hGeantVtxY->Draw();
  myCanvas->cd(5);
  hGeantVtxZ->Draw();
  myCanvas->Print(psfile);

  //track info
  TCanvas *Canvas3 = new TCanvas("G4Trk","G4Trk",cWidth,cWidth);
  Canvas3->Divide(2,2);
  myCanvas = Canvas3;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Geant4 Tracks");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hMCG4Trk[i]->Draw();
  }
  myCanvas->cd(3);
  hGeantTrkPt->Draw();
  myCanvas->cd(4);
  hGeantTrkE->Draw();
  myCanvas->Print(psfile);

  //ECal info
  TCanvas *Canvas4 = new TCanvas("ECalHits","ECalHits",cWidth,cHeight);
  Canvas4->Divide(2,4);
  myCanvas = Canvas4;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"ECal Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hCaloEcal[i]->Draw();
    myCanvas->cd(i+3);
    hCaloEcalE[i]->Draw();
    myCanvas->cd(i+5);
    hCaloEcalToF[i]->Draw();
  }
  myCanvas->cd(7);
  hCaloEcalPhi->Draw();
  myCanvas->cd(8);
  hCaloEcalEta->Draw();
  myCanvas->Print(psfile);

  //PreSh info
  TCanvas *Canvas5 = new TCanvas("PreShHits","PreShHits",cWidth,cHeight);
  Canvas5->Divide(2,4);
  myCanvas = Canvas5;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"PreShower Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hCaloPreSh[i]->Draw();
    myCanvas->cd(i+3);
    hCaloPreShE[i]->Draw();
    myCanvas->cd(i+5);
    hCaloPreShToF[i]->Draw();
  }
  myCanvas->cd(7);
  hCaloPreShPhi->Draw();
  myCanvas->cd(8);
  hCaloPreShEta->Draw();
  myCanvas->Print(psfile);

  //HCal info
  TCanvas *Canvas6 = new TCanvas("HCalHits","HCalHits",cWidth,cHeight);
  Canvas6->Divide(2,4);
  myCanvas = Canvas6;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"HCal Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hCaloHcal[i]->Draw();
    myCanvas->cd(i+3);
    hCaloHcalE[i]->Draw();
    myCanvas->cd(i+5);
    hCaloHcalToF[i]->Draw();
  }
  myCanvas->cd(7);
  hCaloHcalPhi->Draw();
  myCanvas->cd(8);
  hCaloHcalEta->Draw();
  myCanvas->Print(psfile);

  //Pixel Tracker info
  TCanvas *Canvas7 = new TCanvas("PixelHits","PixelHits",cWidth,cHeight);
  Canvas7->Divide(2,4);
  myCanvas = Canvas7;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Pixel Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hTrackerPx[i]->Draw();
  }
  myCanvas->cd(3);
  hTrackerPxPhi->Draw();
  myCanvas->cd(4);
  hTrackerPxEta->Draw();
  myCanvas->cd(5);
  hTrackerPxBToF->Draw();
  myCanvas->cd(6);
  hTrackerPxBR->Draw();
  myCanvas->cd(7);
  hTrackerPxFToF->Draw();
  myCanvas->cd(8);
  hTrackerPxFZ->Draw();
  myCanvas->Print(psfile);

  //Strip Tracker info
  TCanvas *Canvas8 = new TCanvas("StripHits","StripHits",cWidth,cHeight);
  Canvas8->Divide(2,4);
  myCanvas = Canvas8;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Silicon Strip Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hTrackerSi[i]->Draw();
  }
  myCanvas->cd(3);
  hTrackerSiPhi->Draw();
  myCanvas->cd(4);
  hTrackerSiEta->Draw();
  myCanvas->cd(5);
  hTrackerSiBToF->Draw();
  myCanvas->cd(6);
  hTrackerSiBR->Draw();
  myCanvas->cd(7);
  hTrackerSiFToF->Draw();
  myCanvas->cd(8);
  hTrackerSiFZ->Draw();
  myCanvas->Print(psfile);

  //Muon info
  TCanvas *Canvas9 = new TCanvas("MuonHits","MuonHits",cWidth,cWidth);
  Canvas9->Divide(2,2);
  myCanvas = Canvas9;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Muon Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hMuon[i]->Draw();
  }
  myCanvas->cd(3);
  hMuonPhi->Draw();
  myCanvas->cd(4);
  hMuonEta->Draw();
  myCanvas->Print(psfile);

  //Muon CSC info
  TCanvas *Canvas10 = new TCanvas("MuonCscHits","MuonCscHits",cWidth,cWidth);
  Canvas10->Divide(2,2);
  myCanvas = Canvas10;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Muon CSC Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hMuonCscToF[i]->Draw();
  }
  myCanvas->cd(3);
  hMuonCscZ->Draw();
  myCanvas->Print(psfile);

  //Muon DT info
  TCanvas *Canvas11 = new TCanvas("MuonDtHits","MuonDtHits",cWidth,cWidth);
  Canvas11->Divide(2,2);
  myCanvas = Canvas11;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Muon DT Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hMuonDtToF[i]->Draw();
  }
  myCanvas->cd(3);
  hMuonDtR->Draw();
  myCanvas->Print(psfile);

  //Muon RPC info
  TCanvas *Canvas12 = new TCanvas("MuonRpcHits","MuonRpcHits",cWidth,cHeight);
  Canvas12->Divide(2,3);
  myCanvas = Canvas12;
  myCanvas->cd(0);
  label->DrawLatex(0.5,1.00,"Muon RPC Information");
  for (Int_t i = 0; i < 2; ++i) {
    myCanvas->cd(i+1);
    hMuonRpcBToF[i]->Draw();
    myCanvas->cd(i+4);
    hMuonRpcFToF[i]->Draw();
  }
  myCanvas->cd(3);
  hMuonRpcBR->Draw();
  myCanvas->cd(6);
  hMuonRpcFZ->Draw();
  myCanvas->Print(psfile);

  // close output ps file
  myCanvas->Print(psfileclose);
  
  // close root files
  f1->Close();
  srcfile->Close();

  return;
}
