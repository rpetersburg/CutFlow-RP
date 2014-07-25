#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"

ChargedLepton::ChargedLepton(D3PDReader::Event *tEvent)
	: UsesEvent(tEvent)
{

}

ChargedLepton::~ChargedLepton()
{

}

TLorentzVector* ChargedLepton::getMomentumVec(Int_t muonType = MuonType::CB)
{
	if (muonType == MuonType::CB)
		return m_momentum;
	else if (muonType == MuonType::ID)
		return m_momentumID;
	else if (muonType == MuonType::MS)
		return m_momentumME;
}

CLHEP::HepMatrix ChargedLepton::getCovMatrixHep(Int_t muonType = MuonType::CB)
{
	if (muonType == MuonType::CB)
		return m_covMatrixHep;
	else if (muonType == MuonType::ID)
		return m_covMatrixHepID;
	else if (muonType == MuonType::MS)
		return m_covMatrixHepME;
}

TMatrixD ChargedLepton::getCovMatrix(Int_t muonType = MuonType::CB)
{
	if (muonType == MuonType::CB)
		return m_covMatrix;
	else if (muonType == MuonType::ID)
		return m_covMatrixID;
	else if (muonType == MuonType::MS)
		return m_covMatrixME;
}