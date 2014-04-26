#include "CutFlow4Lep/ParticleObjects/Jets.h"

Jets::Jets(D3PDReader::Event *tEvent, D3PDReader::JetD3PDObjectElement *tJets, Int_t tType)
	: ChargedLepton(tEvent), m_jets(tJets), m_type(tType)
{
	init();
}

Jets::~Jets()
{
	delete m_momentum;
	delete m_momentumMain;
	delete m_momentumBDT;
}

void Jets::init()
{
	m_ptCone20 = -1;
	m_mass = -1;
	setMomentumLorentzVectors();
	setTypes();	
}

void Jets::setMomentumLorentzVectors()
{
	Double_t pT = m_jets->pt();
	Double_t eta = m_jets->eta();
	Double_t phi = m_jets->phi();
	Double_t M = m_jets->m();

	m_momentumMain = new TLorentzVector();
	m_momentumMain->SetPtEtaPhiM(pT, eta, phi, M);
	m_momentum = new TLorentzVector();
	m_momentum->SetPtEtaPhiM(pT, eta, phi, M);
	m_momentumBDT = new TLorentzVector();
	m_momentumBDT->SetPtEtaPhiM(pT/1000, eta, phi, M/1000);
}

void Jets::setTypes()
{
	m_vhLeptonType = VHLeptonType::unknown;
	m_truthParentType = VHTruthType::unknown;
}