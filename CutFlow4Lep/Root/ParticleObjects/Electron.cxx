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
	m_flavor = Flavor::Muon;
	m_charge = m_electron->charge();
	m_mass = pdgElMass;
	m_covMomErr = -1;

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
	m_type = LeptonType::ElectronGSF;

	if (m_electron->charge() > 0) m_vhLeptonType = VHLeptonType::electronPlus;
	else m_vhLeptonType = VHLeptonType::electronMinus;
	m_truthParentType = VHTruthType::unknown;
}

void Electron::setptCone20Correction()
{
	m_ptCone20Correction = m_electron->trackpt();
}

void Electron::fillCovMatrix()
{
	if (m_elRescale == 0)
	{
		cout << "ChargedLepton: ERROR = Energy rescaler not set" << endl;
		return;
	}

	Double_t E = m_electron->cl_E();
	Double_t Eta = m_electron->cl_eta();
	Double_t res_electron = m_elRescale->resolution(E, Eta, PATCore::ParticleType::Electron, true)*E;

	// Overwrite it with the outside resolution if given
	if (m_resolution != -1) res_electron = m_resolution;
	// Momentum Error Term
	m_covMomErr = pow((res_electron), 2) / (1000 * 1000);
	//covMomErr = res_electron/1000;

	TMatrixD tmp1 = ZMassConstraint::getCovarianceTMatrixDd0z0PhiThetaPElectron(res_electron,
																																							m_electron->trackcov_d0(),
																																							m_electron->trackcov_z0(),
																																							m_electron->trackcov_phi(),
																																							m_electron->trackcov_theta(),
																																							m_electron->trackcov_d0_z0(),
																																							m_electron->trackcov_d0_phi(),
																																							m_electron->trackcov_d0_theta(),
																																							m_electron->trackcov_z0_phi(),
																																							m_electron->trackcov_z0_theta(),
																																							m_electron->trackcov_phi_theta());
	m_covMatrix.ResizeTo(tmp1);
	m_covMatrix = tmp1;

	m_covMatrixHep = ZMassConstraint::getCovarianceMatrixd0z0PhiThetaPElectron(res_electron,
																																						 m_electron->trackcov_d0(),
																																						 m_electron->trackcov_z0(),
																																						 m_electron->trackcov_phi(),
																																						 m_electron->trackcov_theta(),
																																						 m_electron->trackcov_d0_z0(),
																																						 m_electron->trackcov_d0_phi(),
																																						 m_electron->trackcov_d0_theta(),
																																						 m_electron->trackcov_z0_phi(),
																																						 m_electron->trackcov_z0_theta(),
																																						 m_electron->trackcov_phi_theta());
	m_covMatrixME.ResizeTo(tmp1);
	m_covMatrixME = m_covMatrix;
	m_covMatrixHepME = m_covMatrixHep;

	m_covMatrixID.ResizeTo(tmp1);
	m_covMatrixID = m_covMatrix;
	m_covMatrixHepID = m_covMatrixHep;
}