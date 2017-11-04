#include <iostream>
#include <vector>

#include "SampleAnalyzer/User/Analyzer/MyAnalysis.h"

using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool MyAnalysis::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters){
  cout << "BEGIN Initialization" << endl;
  // initialize variables, histos
  cout << "END   Initialization" << endl;

  nPhoton = 0;
  nEvent = 0;
  nSignal = 0;

  c = new TCanvas("c","c",800,800);
  diPhotonMass = new TH1F("diphoton mass", "Diphoton Mass", 40, 105, 185);
  missingET = new TH1F("MET", "MET", 70, 0, 350);

  //Declaring signal area
  Manager()->AddRegionSelection("Hgg");

  /* 1. Asymmetric Transverse Energy Threshold */
  Manager()->AddCut("Photon1.pt() > 30 GeV, Photon2.pt() > 18 GeV", "Hgg");
  /* 2. Loose Photon ID (H/E < 0.1)  */
  Manager()->AddCut("Hadronic/Electronic < 0.1","Hgg");
  /* 3. Loose Photon Isolation (?) */
  Manager()->AddCut("isolation","Hgg");
  /* 4. EB(ECAL Barrel) :|eta| < 1.44, EE(ECAL Endcap) : 1.566 < |eta| < 2.5 */
  Manager()->AddCut("reject 1.44 < |eta| < 1.566", "Hgg");
  /* 5. |dPhi(Diphoton, MET)| >= 2.1  */
  Manager()->AddCut("dPhi(Diphoton,MET) > 2.1","Hgg");
  /* 6. min(|dPhi(jet, MET)|) > 0.5 */
  Manager()->AddCut("min(dPhi(jet,MET)) > 0.5", "Hgg");
  /* 7. Photon1.pT/Diphoton.M > 0.5, Photon2.pT/Diphoton.M > 0.25 */
  Manager()->AddCut("photon1.pT/diphoton.m > 0.5 && photon2.pT/diphoton.m > 0.25");
  /* 8. Diphoton.pT > 90 GeV , MET > 105 GeV */
  Manager()->AddCut("diphoton.pT > 90 GeV && MET > 105 GeV","Hgg");
  /* 9. 120 GeV < Diphoton.m < 130 GeV */
  Manager()->AddCut("120 GeV < Diphoton.m() < 130 GeV","Hgg");
  
  return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void MyAnalysis::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files){
  cout << "BEGIN Finalization" << endl;
  //cout << "PASS 1.44 < |eta| << 1.566 : " << nPhoton << endl;
  cout << "NUMBER OF DIPHOTON EVENT : " << nEvent << endl;
  cout << " NUMBER OF SIGNAL : " << nSignal << endl;
  // saving histos
  TFile * outFile = new TFile("../Output/histo.root", "recreate");
  outFile->cd();
  c->cd();
  c->SetLogy();
  //diPhotonMass->Draw();
  diPhotonMass->Write();
  //missingET->Draw();
  missingET->Write();
  outFile->Close();
  cout << "END   Finalization" << endl;
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------
bool MyAnalysis::Execute(SampleFormat& sample, const EventFormat& event){
  // ***************************************************************************
  // Example of analysis with reconstructed objects
  // Concerned samples : 
  //   - LHCO samples
  //   - LHE/STDHEP/HEPMC samples after applying jet-clustering algorithm
  // ***************************************************************************

  if (event.rec()==0) return true;
  //cout << "---------------NEW EVENT-------------------" << endl;

  vector<const RecPhotonFormat *> v_signalPhotons;
  vector<const RecJetFormat *> v_signalJets;
  vector<const RecLeptonFormat *> v_signalLeptons;

  MALorentzVector pTmiss = event.rec()->MET().momentum();
  double MET = pTmiss.Pt();

  for(unsigned int i=0; i<event.rec()->jets().size(); ++i){
    const RecJetFormat * jet = &(event.rec()->jets()[i]);
    v_signalJets.push_back(jet);
  }

  //Diphoton selection
  /* 1. Asymmetric transverse energy threshold */
  //if( event.rec()->photons().size() != 0)
  //  cout << "PHOTON SIZE : " << event.rec()->photons().size() << endl;
  //if( event.rec()->photons().size() == 1) ++nPhoton;
  if( event.rec()->photons().size() >= 2) ++nEvent;
  
  for(unsigned int i=0; i<event.rec()->photons().size(); ++i){
    const RecPhotonFormat * myphoton1 = &(event.rec()->photons()[i]);
    MALorentzVector photon1;
    if(myphoton1->pt() < 30) return true;
    photon1.SetPtEtaPhiE(myphoton1->pt(), myphoton1->eta(), myphoton1->phi(), myphoton1->e());
    for(unsigned int j=0; j<event.rec()->photons().size(); ++j){
      if( i == j ) continue;
      const RecPhotonFormat * myphoton2 = &(event.rec()->photons()[j]);
      MALorentzVector photon2;
      if(myphoton2->pt() < 20) return true;
      photon2.SetPtEtaPhiE(myphoton2->pt(), myphoton2->eta(), myphoton2->phi(), myphoton2->e());
      if( (photon1 + photon2).M() > 95.0 ){
        v_signalPhotons.push_back(myphoton1);
        v_signalPhotons.push_back(myphoton2);
      }
    }
  }

  //std::sort(v_signalPhotons.begin(),v_signalPhotons.end());
  //vector<const RecPhotonFormat *>::iterator v_remove = std::unique(v_signalPhotons.begin(),v_signalPhotons.end());
  //if( v_remove != v_signalPhotons.end() )
  //  v_signalPhotons.erase(v_remove, v_signalPhotons.end());

  /* 2. Loose Photon ID */
  /* 3. Loose Photon Isolation*/
  for(int i = v_signalPhotons.size()-1 ; i>=0; --i){
    const RecPhotonFormat * photon = v_signalPhotons[i];
    double myPhotonPt = photon->pt();
    double chargePt = PHYSICS->Isol->eflow->sumIsolation(photon, event.rec(), 0.3, 0., IsolationEFlow::TRACK_COMPONENT);
    double neutralPt = PHYSICS->Isol->eflow->sumIsolation(photon, event.rec(), 0.3, 0., IsolationEFlow::NEUTRAL_COMPONENT);
    double photonPt = PHYSICS->Isol->eflow->sumIsolation(photon, event.rec(), 0.3, 0., IsolationEFlow::PHOTON_COMPONENT);
    double photonplt = photonPt-myPhotonPt;
    double photonID = photon->HEoverEE();
    
    //barrelcap
    if( photon->eta() < 1.44 ) {
      if(chargePt > 3.32 || neutralPt > (1.920+0.14*myPhotonPt+0.000019*myPhotonPt*myPhotonPt) || photonplt > (0.81+0.0053*myPhotonPt) || photonID > 0.05 ){
        v_signalPhotons.erase(v_signalPhotons.begin()+i);
      }
    }
    //endcap
    else if( photon->eta() > 1.566 && photon->eta() < 2.4 ){
      if(chargePt > 1.97 || neutralPt > (11.86+0.0139*myPhotonPt+0.000025*myPhotonPt*myPhotonPt) || photonplt > (0.83+0.0034*myPhotonPt) || photonID > 0.05 ){
        v_signalPhotons.erase(v_signalPhotons.begin()+i);
      }
    }
    else return true;
  }
  //if( !Manager()->ApplyCut(v_signalPhotons.size() > 0, "isolation")) return true;

  /* 4. EB(ECAL Barrel) :|eta| < 1.44, EE(ECAL Endcap) : 1.566 < |eta| < 2.5 */
  //for(vector<const RecPhotonFormat *>::iterator v_itr = v_signalPhotons.begin(); v_itr != v_signalPhotons.end(); ++v_itr){
  //  const RecPhotonFormat * photon = *v_itr;
  //  if( photon->eta() > 1.44 && photon->eta() < 1.566 ) return true;
    //if( !Manager()->ApplyCut(photon->eta() > 1.44 && photon->eta() < 1.566, "reject 1.44 < |eta| < 1.566")) return true;
  //}

  MALorentzVector diPhoton = {0.0,0.0,0.0,0.0};
  if( v_signalPhotons.size() >= 2){
    diPhoton.SetPxPyPzE(v_signalPhotons[0]->px()+v_signalPhotons[1]->px(),
                        v_signalPhotons[0]->py()+v_signalPhotons[1]->py(),
                        v_signalPhotons[0]->pz()+v_signalPhotons[1]->pz(),
                        v_signalPhotons[0]->e()+v_signalPhotons[1]->e());
  }
  else return true;

  /* 5. |dPhi(Diphoton, MET)| >= 2.1 */
  if( abs(diPhoton.DeltaPhi(pTmiss)) < 2.1 ) return true;
  //if( !Manager()->ApplyCut(diPhoton.DeltaPhi(pTmiss) > 2.1, "dPhi(Diphoton,MET) > 2.1")) return true;

  /* 6. min(|dPhi(Jet, MET)|) > 0.5 */
  if( v_signalJets.size() != 0 ){
    std::sort(v_signalJets.begin(),v_signalJets.end());
    for(vector<const RecJetFormat *>::iterator v_itr = v_signalJets.begin(); v_itr != v_signalJets.end(); ++v_itr){
      const RecJetFormat * myjet = *v_itr;
      //if( myjet->pt() < 30 ) return true;
      //if( myjet->eta() > 4.7 ) return true;
      if( myjet->pt() > 50 && myjet->dphi_0_pi(pTmiss) < 0.5) return true;
    }
  }
  
  /* 7. Photon1.pT/Diphoton.M > 0.5 Photon2.pT/Diphoton.M > 0.25 */
  if( v_signalPhotons[0]->pt()/diPhoton.M() < 0.5 || v_signalPhotons[1]->pt()/diPhoton.M() < 0.25 ) return true;
  //if( !Manager()->ApplyCut((v_signalPhotons[0]->pt()/diPhoton.M()) > 0.5 && (v_signalPhotons[1]->pt()/diPhoton.M()) > 0.25, "photon1.pT/diphoton.m > 0.5 && photon2.pT/diphoton.m > 0.25")) return true;
  
  /* 8. Diphoton.pT > 90 GeV, MET > 105 GeV */
  if( diPhoton.Pt() < 90.0 || MET < 105.0 ) return true;//comment out MET part when you draw met plot!!
  //if( !Manager()->ApplyCut(diPhoton.Pt() > 90.0 && MET > 105.0, "diphoton.pT > 90 GeV && MET > 105 GeV")) return true;

  diPhotonMass->Fill(diPhoton.M());  
  /* 9. 120 GeV < Diphoton.m < 130 GeV */
  if( diPhoton.M() < 120.0 || diPhoton.M() > 130.0 ) return true;
  //if( !Manager()->ApplyCut(diPhoton.M() > 120.0 && diPhoton.M() < 130.0, "120 GeV < Diphoton.m() < 130 GeV")) return true;

  missingET->Fill(MET);
  ++nSignal;
  return true;
}
