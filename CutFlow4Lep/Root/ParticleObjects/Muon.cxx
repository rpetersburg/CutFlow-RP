#include "CutFlow4Lep/ParticleObjects/Muon.h"

Muon::Muon(D3PDReader::MuonD3PDObjectElement *tMuon, Double_t tMuonEff, Double_t tMuonSmear) 
	: m_muon(tMuon)
{
	m_eff = tMuonEff;
	m_smear = tMuonSmear;
	m_mass = pdgMuMass;

	m_momentumMain = new TLorentzVector();
	m_momentumMain->SetPtEtaPhiM(m_muon->pt(), m_muon->eta(), m_muon->phi(), m_mass);
	m_momentumBDT = new TLorentzVector();
	m_momentumBDT->SetPtEtaPhiM(m_muon->pt()/1000, m_muon->eta(), m_muon->phi(), m_mass/1000);
	m_momentumME = new TLorentzVector();
	m_momentumME->SetPtEtaPhiM(m_muon->me_pt, m_muon->me_eta, m_muon->me_phi(), m_mass);
	m_momentumID = new TLorentzVector();
	m_momentumID->SetPtEtaPhiM(m_muon->id_pt, m_muon->id_eta, m_muon->id_phi(), m_mass);

	if (m_muon->isStandAloneMuon()) m_ptCone20Correction = 0;
	else m_ptCone20Correction = sin(m_muon->id_theta()) / fabs(m_muon->id_qoverp());

	for (Int_t i = 0; i < DoSys::Nom + 1; i++)
		sysE[i] = m_muon->E();
	nomE = m_muon->E();

	if (m_muon->charge() > 0) m_vhLeptonType = VHLeptonType::muonPlus;
	else m_vhLeptonType = VHLeptonType::muonMinus;
	m_truthParentType = VHTruthType::unknown;
}

Muon::~Muon()
{

}