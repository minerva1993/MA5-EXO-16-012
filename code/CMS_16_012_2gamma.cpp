#include "SampleAnalyzer/User/Analyzer/CMS_16_012_2gamma.h"
using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool CMS_16_012_2gamma::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters)
{
  cout << "BEGIN Initialization" << endl;
  // initialize variables, histos
  cout << "END   Initialization" << endl;
  Manager()->AddRegionSelection("higgs_aa");
  Manager()->AddCut("120<dip_mass<130","higgs_aa");
  return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void CMS_16_012_2gamma::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files)
{
  cout << "BEGIN Finalization" << endl;
  // saving histos
  cout << "END   Finalization" << endl;
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------
bool CMS_16_012_2gamma::Execute(SampleFormat& sample, const EventFormat& event)
{
  double myEventWeight;
  if(Configuration().IsNoEventWeight()) myEventWeight=1.;
  else if(event.mc()->weight()!=0.) myEventWeight=event.mc()->weight();
  else
  {
    WARNING << "Found one event with a zero weight. Skipping..." << endmsg;
    return false;
  }

  if (event.rec()==0) {return true;}
       EventFormat myEvent;
       myEvent = event;
  Manager()->InitializeForNewEvent(myEventWeight);
  if (event.rec()!=0)









  MALorentzVector pTmiss = MALorentzVector();
  MALorentzVector pTmiss = event.rec()->MET().momentum();

  double MissET = pTmiss.Pt();
//  double MET = pTmiss.Pt();
//  double MissET = event.rec()->MET().momentum().Pt();




   vector<const RecPhotonFormat*> SignalPhotons;
  for(int ii=event.rec()->photons().size()-1;ii>=0;ii--)
  {
    const RecPhotonFormat * myPhoton = &(event.rec()->photons()[ii]);
    double mypt=myPhoton->pt();  double ph_eta= fabs(myPhoton->eta());
    double chargept=PHYSICS->Isol->eflow->sumIsolation(myPhoton,event.rec(),0.3,0.,IsolationEFlow::TRACK_COMPONENT);
    double neutralpt=PHYSICS->Isol->eflow->sumIsolation(myPhoton,event.rec(),0.3,0.,IsolationEFlow::NEUTRAL_COMPONENT);
    double photonpt=PHYSICS->Isol->eflow->sumIsolation(myPhoton,event.rec(),0.3,0.,IsolationEFlow::PHOTON_COMPONENT);
    double photonplt=photonpt-mypt;
    double photonID=myPhoton->HEoverEE();
    if(mypt > 20. &&( ph_eta<1.44 || (ph_eta>1.56 && ph_eta<2.5) )   ){ 
      if(!(chargept > 1.5||neutralpt > (1.0+0.04*mypt)||photonplt > (0.7+0.005*mypt)||photonID > 0.1))
       SignalPhotons.push_back(myPhoton);
    }
  }


   for(int ii=SignalPhotons.size()-1;ii>=0;ii--){
      const RecPhotonFormat * myPhoton = SignalPhotons[ii];
      double ph_pt=myPhoton->pt(); double ph_eta= fabs(myPhoton->eta());
      if(ph_pt < 20. || (ph_eta>1.44 && ph_eta<1.56) || ph_eta>2.5) 
      SignalPhotons.erase(SignalPhotons.begin()+ii);
   }



 vector<const RecPhotonFormat*> v_signalPhotons;
  for(unsigned int i=0; i<SignalPhotons.size(); ++i){
    const RecPhotonFormat * myphoton1 = &(event.rec()->photons()[i]);
    MALorentzVector photon1;
    photon1.SetPtEtaPhiE(myphoton1->pt(), myphoton1->eta(), myphoton1->phi(), myphoton1->e());
    for(unsigned int j=0; j<event.rec()->photons().size(); ++j){
      const RecPhotonFormat * myphoton2 = &(event.rec()->photons()[j]);
      MALorentzVector photon2;
      photon2.SetPtEtaPhiE(myphoton2->pt(), myphoton2->eta(), myphoton2->phi(), myphoton2->e());
      if( (photon1 + photon2).M() > 95.0 ){
        v_signalPhotons.push_back(myphoton1);
	    v_signalPhotons.push_back(myphoton2);
      }
    }
  }




  double pt_1=v_signalPhotons[0]->pt(),pt_2=v_signalPhotons[1]->pt();

  MALorentzVector diPhoton;
  diPhoton.SetPtEtaPhiE(v_signalPhotons[0]->pt()+v_signalPhotons[1]->pt(),
      v_signalPhotons[0]->eta()+v_signalPhotons[1]->eta(),
      v_signalPhotons[0]->phi()+v_signalPhotons[1]->phi(),
      v_signalPhotons[0]->e()+v_signalPhotons[1]->e());


   double Photon_PT_Sum=diPhoton.Pt(); double dip_mass=diPhoton.M(); 
   if( diPhoton.DeltaPhi(pTmiss) < 2.1)  return true;





 vector<const RecJetFormat*> SignalJets;
 vector<const RecLeptonFormat*> SignalElectrons;
 for(unsigned int ij=0;ij<event.rec()->jets().size();ij++){
    const RecJetFormat * CurrentJet = &(event.rec()->jets()[ij]);
    double pt_jet=CurrentJet->pt(); double pt_eta=fabs(CurrentJet->eta());
    if(pt_jet > 50. && pt_eta < 4.7)
    SignalJets.push_back(CurrentJet);
 }






  for(unsigned int ij=0;ij<SignalJets.size();ij++){
    if( SignalJets[ij]->dphi_0_pi(pTmiss) < 0.5)  return true;
  }






  int NB=0, Nmu=0;
  for(unsigned int ie=0; ie<event.rec()->electrons().size(); ie++)
  {
    const RecLeptonFormat * CurrentElectron = &(event.rec()->electrons()[ie]);
    if(CurrentElectron->pt()>10.0) NB++;
  }
  if(NB>=2) return true;



  for(unsigned int ie=0; ie<event.rec()->muons().size(); ie++)
  {
    const RecLeptonFormat * CurrentMuon = &(event.rec()->muons()[ie]);
    if(CurrentMuon->pt()>10.0) Nmu++;
  }
  if(Nmu > 0) return true;









  if(MissET < 105.|| Photon_PT_Sum < 90. || (pt_1/dip_mass < 0.5) || (pt_2/dip_mass < 0.25) ) return true;

  if( !Manager()->ApplyCut(dip_mass>120. && dip_mass<130.,"120<dip_mass<130")) return true;

  








//  double mydphi = SignalJets[i]->dphi_0_pi(pTmiss);


  return true;
}

/*
 double CMS_16_012_2gamma::Invariant_Mass(const RecPhotonFormat* jet1, const RecPhotonFormat* jet2) {
      double M1=jet1->momentum().M(), M2=jet2->momentum().M();
      double Et_1=sqrt(pow(jet1->pt(),2)+pow(M1,2));double Et_2=sqrt(pow(jet2->pt(),2)+pow(M2,2));
      double y_jet1=log((sqrt(pow(M1,2)+pow(jet1->pt() *cosh(jet1->eta()),2))+jet1->pt()*sinh(jet1->eta()))/sqrt(pow(M1,2)+pow(jet1->pt(),2)));
      double y_jet2=log((sqrt(pow(M2,2)+pow(jet2->pt() *cosh(jet2->eta()),2))+jet2->pt()*sinh(jet2->eta()))/sqrt(pow(M2,2)+pow(jet2->pt(),2)));
      double dR_y=sqrt(pow(y_jet1-y_jet2,2)+pow(jet1->dphi_0_pi(jet2),2));
      double Inv_mass = sqrt(pow(M1,2)+pow(M2,2)+2*(Et_1*Et_2*cosh(dR_y)-(jet1->pt()*jet2->pt())) );
      return Inv_mass;
 }

*/
