#include "CutFlow4Lep/Cuts/JetsCut.h"

JetsCut::JetsCut(D3PDReader::Event *tEvent, vector<Jets*> *tInitJetsVec)
	: Cuts(tEvent), m_initJetsVec(tInitJetsVec)
{

}

JetsCut::~JetsCut()
{

}

void JetsCut::setInitJetsVec(vector<Jets*> *tInitJetsVec)
{
	m_initJetsVec = tInitJetsVec;
	m_cutJetsVec.clear();
}

Bool_t JetsCut::passedCut()
{
	Int_t currType;
	D3PDReader::JetD3PDObjectElement *currJets;

	vector<Jets*>::iterator currJetsObj = m_initJetsVec->begin();
	for ( ; currJetsObj != m_initJetsVec->end(); ++currJetsObj)
	{
		currType = (*currJetsObj)->getType();
		currJets = (*currJetsObj)->getJets();
		if ( (currType == JetsType::AntiKt4TopoEM && passedEMCut(currJets)) ||
			   (currType == JetsType::AntiKt4TopoEMTruth && passedEMTruthCut(currJets)) ||
				 (currType == JetsType::AntiKt4TopoEM_Fid && passedEMFidCut(currJets)) ||
				 (currType == JetsType::AntiKt4TopoEMTruth_Fid && passedEMTruthFidCut(currJets)) )
				m_cutJetsVec.push_back(*currJetsObj);
	}

	if (m_cutJetsVec.size() > 0) return true;
	return false;
}

Bool_t JetsCut::passedEMCut(D3PDReader::JetD3PDObjectElement *currJets)
{
	Double_t pT = currJets->pt();
	Double_t eta = currJets->emscale_eta();

	// pT Cut
	if (!passedpTCut(pT)) return false;

	// Eta Cut
	if (!passedEtaCut(pT, eta)) return false;

	// Pileup Removal
	Double_t jvfCut = 0;
	if (m_dataYear == 2011) jvfCut = 0.75;
	else if (m_dataYear == 2011) jvfCut = 0.5;
	if (pT < 50 * 1000 && fabs(eta) < 2.4 && fabs(currJets->jvtxf()) <= jvfCut)
		return false;

	// Jets Cleaning
	if (!passedJetsCleaning(currJets)) return false;

	return true;
}

Bool_t JetsCut::passedEMTruthCut(D3PDReader::JetD3PDObjectElement *currJets)
{
	Double_t pT = currJets->pt();
	Double_t eta = currJets->eta();

	// pT Cut
	if (!passedpTCut(pT)) return false;

	// Eta Cut
	if (!passedEtaCut(pT, eta)) return false;

	return true;
}

Bool_t JetsCut::passedEMFidCut(D3PDReader::JetD3PDObjectElement *currJets)
{
	Double_t pT = currJets->pt();

	// pT Cut
	if (!passedFidpTCut(pT)) return false;

	// Eta Cut
	if (!passedFidEtaCut(pT, currJets->eta(), currJets->phi(), currJets->m()))
		return false;

	// Pileup Removal
	Double_t jvfCut = 0.25;
	if (pT < 50 * 1000 && fabs(currJets->emscale_eta()) < 2.4 && fabs(currJets->jvtxf()) <= jvfCut)
		return false;

	// Jets Cleaning
	if (!passedJetsCleaning(currJets)) return false;

	return true;
}

Bool_t JetsCut::passedEMTruthFidCut(D3PDReader::JetD3PDObjectElement *currJets)
{
	Double_t pT = currJets->pt();

	// pT Cut
	if (!passedFidpTCut(pT)) return false;

	// Eta Cut
	if (!passedFidEtaCut(pT, currJets->eta(), currJets->phi(), currJets->m()))
		return false;

	return true;
}

Bool_t JetsCut::passedJetsCleaning(D3PDReader::JetD3PDObjectElement *currJets)
{
	if (currJets->isBadLooseMinus() != 0) return false;

	Double_t fMax = currJets->fracSamplingMax();
 	Double_t sMax = currJets->SamplingMax();
  Double_t _eta  = currJets->eta();
  Double_t _phi  = currJets->phi();

  Bool_t _etaphi28 = (_eta > -0.2 && _eta < -0.1 && _phi > 2.65 && _phi < 2.75) ? true : false;
  Bool_t affected = (m_runNumber==202660 || m_runNumber==202668 || m_runNumber==202712 ||
										 m_runNumber==202740 || m_runNumber==202965 || m_runNumber==202987 || 
										 m_runNumber==202991 || m_runNumber==203027) ? true : false;

  if (fMax > 0.6 && sMax == 13 && _etaphi28 && affected) return false;
	return true;
}

Bool_t JetsCut::passedpTCut(Double_t pT)
{
	if (pT > 25000) return true;
	return false;
}

Bool_t JetsCut::passedFidpTCut(Double_t pT)
{
	if (pT > 30000) return true;
	return false;
}

Bool_t JetsCut::passedEtaCut(Double_t pT, Double_t eta)
{
	if ( (pT > 25000 && fabs(eta) < 2.4) || (pT > 30000 && fabs(eta) > 2.4 && fabs(eta) < 4.5))
		return true;
	return false;
}

Bool_t JetsCut::passedFidEtaCut(Double_t pT, Double_t eta, Double_t phi, Double_t M)
{
	TLorentzVector momentumVec;
	momentumVec.SetPtEtaPhiM(pT, eta, phi, M);

	if (fabs(momentumVec.Rapidity()) < 4.4 ) return true;
	return false;
}