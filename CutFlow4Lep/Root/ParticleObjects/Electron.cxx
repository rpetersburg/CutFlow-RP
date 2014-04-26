#include "CutFlow4Lep/ParticleObjects/Electron.h"

Electron::Electron(D3PDReader::Event *tEvent, D3PDReader::ElectronD3PDObjectElement *tElectron)
	: ChargedLepton(tEvent), m_electron(tElectron)
{
	init();
}

Electron::Electron(D3PDReader::Event *tEvent, D3PDReader::ElectronD3PDObjectElement *tElectron, Double_t tElectronEff, Double_t tElectronSmear)
	: ChargedLepton(tEvent), m_electron(tElectron)
{
	m_eff = tElectronEff;
	m_smear = tElectronSmear;
	init();
}

Electron::Electron(D3PDReader::Event *tEvent, D3PDReader::ElectronD3PDObjectElement *tElectron, Double_t tElectronEff, Double_t tElectronSmear, Double_t tElectronResolution, Double_t tElectronClusterpT)
	: ChargedLepton(tEvent), m_electron(tElectron)
{
	fill(tElectronEff, tElectronSmear, tElectronResolution, tElectronClusterpT);
	init();
}

Electron::~Electron()
{

}

void Electron::fill(Double_t tElectronEff, Double_t tElectronSmear, Double_t tElectronResolution, Double_t tElectronClusterpT)
{
	m_eff = tElectronEff;
	m_smear = tElectronSmear;
	m_resolution = tElectronResolution;
	m_clusterpT = tElectronClusterpT;
}

void Electron::init()
{
	m_mass = pdgElMass;
	setMomentumLorentzVectors();
	setptCone20Correction();
	setSysEnergy();
	setTypes();
}

void Electron::setMomentumLorentzVectors()
{
	Double_t trackPhi = m_electron->trackphi();
	Double_t trackEta = m_electron->tracketa();
	Double_t pT = m_electron->cl_E() / cosh(trackEta);

	m_momentumMain = new TLorentzVector();
	m_momentumMain->SetPtEtaPhiM(pT, trackEta, trackPhi, m_mass);
	m_momentum = new TLorentzVector();
	m_momentum->SetPtEtaPhiM(pT, trackEta, trackPhi, m_mass);
	m_momentumBDT = new TLorentzVector();
	m_momentumBDT->SetPtEtaPhiM(pT/1000, trackEta, trackPhi, m_mass/1000);
	m_momentumME = m_momentum;
	m_momentumID = m_momentum;
}

void Electron::setSysEnergy()
{
	for (Int_t i = 0; i < DoSys::Nom + 1; i++)
	{
		sysE[i] = m_electron->E_sysVar[i];
	}
}

void Electron::setTypes()
{
	if (m_electron->charge() > 0) m_vhLeptonType = VHLeptonType::electronPlus;
	else m_vhLeptonType = VHLeptonType::electronMinus;
	m_truthParentType = VHTruthType::unknown;
}

void Electron::setptCone20Correction()
{
	m_ptCone20Correction = m_electron->trackpt();
}
