#include "CutFlow4Lep/Cuts/CaloIsoCut.h"

CaloIsoCut::CaloIsoCut(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton)
	: QuadLeptonCuts(tEvent, tQuadLepton)
{

}

CaloIsoCut::~CaloIsoCut()
{

}

Bool_t CaloIsoCut::passedCut()
{
	Bool_t passedCut = true;

	Double_t muonCut = 0.30;
	Double_t muonStandAloneCut = 0.15;

	Double_t electronCut2011 = 0.30;
	Double_t electronCut2012 = 0.20;

	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();
	vector<Double_t> caloET = getCaloET();
	m_quadLepton->clearCaloIsoVecs();

	for (UInt_t i = 0; i < leptonVec.size(); i++)
	{
		ChargedLepton *currLepton = leptonVec[i];

		Double_t dataCut = caloET[i] / currLepton->getMomentumVec->Pt();
		m_quadLepton->caloIsoValPushBack(dataCut);

		Bool_t cutLepton = true;
		if (currLepton->getFlavor() == Flavor::Electron)
		{
			if (m_quadLepton->getDataYear() == 2011 && dataCut > electronCut2011)
				cutLepton = false;
			else if (m_quadLepton->getDataYear() == 2012 && dataCut > electronCut2012)
				cutLepton = false;
		}
		else if (currLepton->getFlavor() == Flavor::Muon)
		{
			if (currLepton->getType() != LeptonType::MuonStandAlone && dataCut > muonCut)
				cutLepton = false;
			else if (dataCut > muonStandAloneCut)
				cutLepton = false;
		}
		m_quadLepton->caloIsoPushBack(cutLepton);
		if (!cutLepton) passedCut = false; // If any lepton fails cut, will return false
	}
	return passedCut;
}

vector<Double_t> CaloIsoCut::getCaloET()
{
	vector<Double_t> caloET; // Container
	vector<ChargedLepton*> leptonVec = m_quadLepton->getLeptons();

	Double_t EtOverlap[4];
	Double_t EtCone20CorrectedIso[4];
	Double_t deltaRCut = 0.18;

	vector<ChargedLepton*>::iterator iLeptonItr;
	vector<ChargedLepton*>::iterator jLeptonItr;

	// Filling the containers for Overlap and CorrectedIso
	for (UInt_t i = 0; i < leptonVec.size(); i++)
	{
		ChargedLepton *currLepton = leptonVec[i];

		if (currLepton->getFlavor() == Flavor::Electron)
		{
			D3PDReader::ElectronD3PDObjectElement *currElectron = currLepton->getElectron();

			EtOverlap[i] = currElectron->rawcl_E() / cosh(currElectron->tracketa());

			if (m_quadLepton->getDataYear() == 2011)
				EtCone20CorrectedIso[i] = currElectron->Etcone20();
			else if (m_quadLepton->getDataYear() == 2012)
				EtCone20CorrectedIso[i] = currElectron->topoEtcone20();
			EtCone20CorrectedIso[i] = getCaloIsoCorrection(currElectron); // if (doCaloIsolationCorr), but always true...?

		}
		else if (currLepton->getFlavor() == Flavor::Muon)
		{
			EtOverlap[i] = currLepton->getMuon()->pt();
			EtOverlap[i] = 0; // Not sure why this is here, since it overrides previous line

			EtCone20CorrectedIso[i] = currLepton->getMuon()->etcone20();
		}
	}

	// Subtraction from EtCone20CorrectedIso for overlapping leptons
	Int_t i = -1;
	for (iLeptonItr = leptonVec.begin(); iLeptonItr != leptonVec.end(); ++iLeptonItr)
	{
		i++; // To keep track of loop
		ChargedLepton *iLepton = *iLeptonItr;

		Int_t j = -1;
		for (jLeptonItr = leptonVec.begin(); jLeptonItr != leptonVec.end(); ++jLeptonItr)
		{
			j++; // To keep track of loop
			ChargedLepton *jLepton = *jLeptonItr;
			if (iLepton = jLepton) continue; // Don't want to compare same lepton

			if (deltaRCaloIso(iLepton, jLepton) < deltaRCut)
				EtCone20CorrectedIso[i] = EtCone20CorrectedIso[i] - EtOverlap[j];
		}
		caloET.push_back(EtCone20CorrectedIso[i]);
	}
	return caloET;
}

Double_t CaloIsoCut::getCaloIsoCorrection(D3PDReader::ElectronD3PDObjectElement *currElectron)
{
	UsesEvent *eventObj = m_quadLepton->getZ1()->getPosLepton(); // Just to access certain functions
	if (m_quadLepton->getDataYear() == 2011)
		return CaloIsoCorrection::GetNPVCorrectedIsolation(eventObj->getNumVertex(2),
																											 currElectron->etas2(),
																											 20,
																											 eventObj->getIsMC(),
																											 currElectron->Etcone20(),
																											 CaloIsoCorrection::ELECTRON,
																											 CaloIsoCorrection::REL17);	
	else if (m_quadLepton->getDataYear() == 2012)
		return CaloIsoCorrection::GetPtEDCorrectedTopoIsolation(currElectron->ED_median(),
																														currElectron->cl_E_unsmeared,
																														currElectron->weta2(),
																														currElectron->etap(),
																														currElectron->cl_eta(),
																														20,
																														eventObj->getIsMC(),
																														currElectron->topoEtcone20(),
																														false,
																														CaloIsoCorrection::ELECTRON,
																														CaloIsoCorrection::REL17_2);
}

Double_t CaloIsoCut::deltaRCaloIso(ChargedLepton *iLepton, ChargedLepton *jLepton)
{
	ChargedLepton *lepton[2]; // For looping
	lepton[0] = iLepton;
	lepton[2] = jLepton;
	// Variable containers to calculate DeltaR
	Double_t eta[2];
	Double_t phi[2];

	for (Int_t i = 0; i < 2; i++)
	{
		if (lepton[i]->getFlavor() == Flavor::Electron)
		{
			eta[i] = lepton[i]->getElectron()->etas2();
			phi[i] = lepton[i]->getElectron()->phis2();
		}
		else if (lepton[i]->getFlavor() == Flavor::Muon)
		{
			eta[i] = lepton[i]->getMomentumVec()->Eta();
			phi[i] = lepton[i]->getMomentumVec()->Phi();
		}
	}

	return deltaR(eta[0], phi[0], eta[1], phi[1]);
}

Double_t CaloIsoCut::deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2)
{
	Double_t deltaEta = (eta1 - eta2);
	Double_t deltaPhi = (fabs(phi1 - phi2) > TMath::Pi()) ? 2*TMath::Pi() - fabs(phi1-phi2) : fabs(phi1 - phi2);

	return sqrt(deltaEta * deltaEta + deltaPhi * deltaPhi);
}