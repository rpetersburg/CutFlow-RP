#include "CutFlow4Lep/ParticleObjects/Muon.h"

Muon::Muon(D3PDReader::Event *tEvent, D3PDReader::MuonD3PDObjectElement *tMuon)
	: ChargedLepton(tEvent), m_muon(tMuon)
{
	init();
}

Muon::Muon(D3PDReader::Event *tEvent, D3PDReader::MuonD3PDObjectElement *tMuon, Double_t tMuonEff, Double_t tMuonSmear) 
	: ChargedLepton(tEvent), m_muon(tMuon)
{
	init();
	setEff(tMuonEff);
	setSmear(tMuonSmear);	
}

Muon::~Muon()
{

}

void Muon::init()
{
	m_flavor = Flavor::Electron;
	m_charge = m_muon->charge();
	m_mass = pdgMuMass;
	setMomentumLorentzVectors();
	setptCone20Correction();
	setSysEnergy();
	setTypes();
}

void Muon::setMomentumLorentzVectors()
{
	m_momentumMain = new TLorentzVector();
	m_momentumMain->SetPtEtaPhiM(m_muon->pt(), m_muon->eta(), m_muon->phi(), m_mass);
	m_momentumBDT = new TLorentzVector();
	m_momentumBDT->SetPtEtaPhiM(m_muon->pt()/1000, m_muon->eta(), m_muon->phi(), m_mass/1000);
	m_momentumME = new TLorentzVector();
	m_momentumME->SetPtEtaPhiM(m_muon->me_pt, m_muon->me_eta, m_muon->me_phi(), m_mass);
	m_momentumID = new TLorentzVector();
	m_momentumID->SetPtEtaPhiM(m_muon->id_pt, m_muon->id_eta, m_muon->id_phi(), m_mass);
}

void Muon::setSysEnergy()
{
	for (Int_t i = 0; i < DoSys::Nom + 1; i++)
		sysE[i] = m_muon->E();
	nomE = m_muon->E();
}

void Muon::setptCone20Correction()
{
	if (m_muon->isStandAloneMuon()) m_ptCone20Correction = 0;
	else m_ptCone20Correction = sin(m_muon->id_theta()) / fabs(m_muon->id_qoverp());
}

void Muon::setTypes()
{
	if (m_muon->isCaloMuonId()) m_type = LeptonType::MuonCalo;
	else if (m_muon->isStandAloneMuon()) m_type = LeptonType::MuonStandAlone;
	else m_type = LeptonType::MuonStaco;

	if (m_muon->charge() > 0) m_vhLeptonType = VHLeptonType::muonPlus;
	else m_vhLeptonType = VHLeptonType::muonMinus;
	m_truthParentType = VHTruthType::unknown;
}