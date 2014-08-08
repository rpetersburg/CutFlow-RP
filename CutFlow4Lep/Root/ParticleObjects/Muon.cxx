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
	m_momentumMS = new TLorentzVector();
	m_momentumMS->SetPtEtaPhiM(m_muon->me_pt, m_muon->me_eta, m_muon->me_phi(), m_mass);
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

void Muon::fillCovMatrix()
{
	TMatrixD tmp1 = ZMassConstraint::getCovarianceTMatrixDd0z0PhiThetaPMuon(m_momentum->P(),
																																					m_muon->cov_d0_exPV(),
																																					m_muon->cov_z0_exPV(),
																																					m_muon->cov_phi_exPV(),
																																					m_muon->cov_theta_exPV(),
																																					m_muon->cov_qoverp_exPV(),
																																					m_muon->cov_d0_z0_exPV(),
																																					m_muon->cov_d0_phi_exPV(),
																																					m_muon->cov_d0_theta_exPV(),
																																					m_muon->cov_d0_qoverp_exPV(),
																																					m_muon->cov_z0_phi_exPV(),
																																					m_muon->cov_z0_theta_exPV(),
																																					m_muon->cov_z0_qoverp_exPV(),		
																																					m_muon->cov_phi_theta_exPV(),
																																					m_muon->cov_phi_qoverp_exPV(),
																																					m_muon->cov_theta_qoverp_exPV());
	m_covMatrix.ResizeTo(tmp1);
	m_covMatrix = tmp1;

	// Momentum Error Term
	m_covMomErr = (m_muon->cov_qoverp_exPV()) * 1000 * 1000;

	m_covMatrixHep = ZMassConstraint::getCovarianceMatrixd0z0PhiThetaPMuon(m_momentum->P(),
																																				 m_muon->cov_d0_exPV(),
																																				 m_muon->cov_z0_exPV(),
																																				 m_muon->cov_phi_exPV(),
																																				 m_muon->cov_theta_exPV(),
																																				 m_muon->cov_qoverp_exPV(),
																																				 m_muon->cov_d0_z0_exPV(),
																																				 m_muon->cov_d0_phi_exPV(),
																																				 m_muon->cov_d0_theta_exPV(),
																																				 m_muon->cov_d0_qoverp_exPV(),
																																				 m_muon->cov_z0_phi_exPV(),
																																				 m_muon->cov_z0_theta_exPV(),
																																				 m_muon->cov_z0_qoverp_exPV(),
																																				 m_muon->cov_phi_theta_exPV(),
																																				 m_muon->cov_phi_qoverp_exPV(),
																																				 m_muon->cov_theta_qoverp_exPV());

	// For MS
	TMatrixD tmp1MS = ZMassConstraint::getCovarianceTMatrixDd0z0PhiThetaPMuon(m_momentumMS->P(),
																																						m_muon->me_cov_d0_exPV(),
																																						m_muon->me_cov_z0_exPV(),
																																						m_muon->me_cov_phi_exPV(),
																																						m_muon->me_cov_theta_exPV(),
																																						m_muon->me_cov_qoverp_exPV(),
																																						m_muon->me_cov_d0_z0_exPV(),
																																						m_muon->me_cov_d0_phi_exPV(),
																																						m_muon->me_cov_d0_theta_exPV(),
																																						m_muon->me_cov_d0_qoverp_exPV(),
																																						m_muon->me_cov_z0_phi_exPV(),
																																						m_muon->me_cov_z0_theta_exPV(),
																																						m_muon->me_cov_z0_qoverp_exPV(),
																																						m_muon->me_cov_phi_theta_exPV(),
																																						m_muon->me_cov_phi_qoverp_exPV(),
																																						m_muon->me_cov_theta_qoverp_exPV());
	m_covMatrixME.ResizeTo(tmp1MS);
	m_covMatrixMS = tmp1MS;


	m_covMatrixHepMS = ZMassConstraint::getCovarianceMatrixd0z0PhiThetaPMuon(m_momentumMS->P(),
																																					 m_muon->me_cov_d0_exPV(),
																																					 m_muon->me_cov_z0_exPV(),
																																					 m_muon->me_cov_phi_exPV(),
																																					 m_muon->me_cov_theta_exPV(),
																																					 m_muon->me_cov_qoverp_exPV(),
																																					 m_muon->me_cov_d0_z0_exPV(),
																																					 m_muon->me_cov_d0_phi_exPV(),
																																					 m_muon->me_cov_d0_theta_exPV(),
																																					 m_muon->me_cov_d0_qoverp_exPV(),
																																					 m_muon->me_cov_z0_phi_exPV(),
																																					 m_muon->me_cov_z0_theta_exPV(),
																																					 m_muon->me_cov_z0_qoverp_exPV(),
																																					 m_muon->me_cov_phi_theta_exPV(),
																																					 m_muon->me_cov_phi_qoverp_exPV(),
																																					 m_muon->me_cov_theta_qoverp_exPV());

	// For ID 
	TMatrixD tmp1ID = ZMassConstraint::getCovarianceTMatrixDd0z0PhiThetaPMuon(m_momentumID->P(),
																																						m_muon->id_cov_d0_exPV(),
																																						m_muon->id_cov_z0_exPV(),
																																						m_muon->id_cov_phi_exPV(),
																																						m_muon->id_cov_theta_exPV(),
																																						m_muon->id_cov_qoverp_exPV(),
																																						m_muon->id_cov_d0_z0_exPV(),
																																						m_muon->id_cov_d0_phi_exPV(),
																																						m_muon->id_cov_d0_theta_exPV(),
																																						m_muon->id_cov_d0_qoverp_exPV(),
																																						m_muon->id_cov_z0_phi_exPV(),
																																						m_muon->id_cov_z0_theta_exPV(),
																																						m_muon->id_cov_z0_qoverp_exPV(),
																																						m_muon->id_cov_phi_theta_exPV(),
																																						m_muon->id_cov_phi_qoverp_exPV(),
																																						m_muon->id_cov_theta_qoverp_exPV());
	m_covMatrixID.ResizeTo(tmp1ID);
	m_covMatrixID = tmp1ID;


	m_covMatrixHepID = ZMassConstraint::getCovarianceMatrixd0z0PhiThetaPMuon(m_momentumID->P(),
																																					 m_muon->id_cov_d0_exPV(),
																																					 m_muon->id_cov_z0_exPV(),
																																					 m_muon->id_cov_phi_exPV(),
																																					 m_muon->id_cov_theta_exPV(),
																																					 m_muon->id_cov_qoverp_exPV(),
																																					 m_muon->id_cov_d0_z0_exPV(),
																																					 m_muon->id_cov_d0_phi_exPV(),
																																					 m_muon->id_cov_d0_theta_exPV(),
																																					 m_muon->id_cov_d0_qoverp_exPV(),
																																					 m_muon->id_cov_z0_phi_exPV(),
																																					 m_muon->id_cov_z0_theta_exPV(),
																																					 m_muon->id_cov_z0_qoverp_exPV(),
																																					 m_muon->id_cov_phi_theta_exPV(),
																																					 m_muon->id_cov_phi_qoverp_exPV(),
																																					 m_muon->id_cov_theta_qoverp_exPV());

	if (m_muon->isStandAloneMuon())
	{
		m_covMatrixHepID = m_covMatrixHep;
		m_covMatrixID = m_covMatrix;
	}
	if (m_muon->isSegmentTaggedMuon() || m_muon->isCaloMuonId())
	{
		m_covMatrixHepMS = m_covMatrixHep;
		m_covMatrixMS = m_covMatrix;
	}
}