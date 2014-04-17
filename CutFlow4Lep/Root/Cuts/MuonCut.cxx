#include "CutFlow4Lep/Cuts/MuonCut.h"

MuonCut::MuonCut(D3PDReader::Event *tEvent) : Cuts(tEvent)
{
	initializeMuonObj();
}

MuonCut::~MuonCut()
{

}

void MuonCut::initializeMuonObj()
{
	m_muonCalo = &(m_event->mu_calo);
	m_muonStaco = &(m_event->mu_staco);
	m_d0Cut = 1.;
	m_z0Cut = 10.;
}

Bool_t MuonCut::passedCut()
{
	vector<D3PDReader::MuonD3PDObjectElement*> muonPassedCutWithOverlap;

	// Use only muon elements that pass specific cuts
	D3PDReader::MuonD3PDObjectElement *currMuon;
	for (Int_t i = 0; i < m_muonCalo->n(); i++) // Loop through calo events
	{
		currMuon = &(*m_muonCalo)[i];
		if (passedCaloCut(currMuon))
			muonPassedCutWithOverlap.push_back(currMuon);
	}
	for (Int_t i = 0; i < m_muonStaco->n(); i++) // Loop through staco events
	{
		currMuon = &(*m_muonStaco)[i];
		if (currMuon->isStandAloneMuon())
		{
			if (passedStandAloneCut(currMuon))
				muonPassedCutWithOverlap.push_back(currMuon);
		}
		else
		{
			if (passedStacoCut(currMuon))
				muonPassedCutWithOverlap.push_back(currMuon);
		}
	}
	// Use muon overlap tool to remove overlap of muons that passed the cuts
	MuonOverlap *overlapTool = new MuonOverlap(muonPassedCutWithOverlap);
	overlapTool->removeOverlap();
	// Muons pass cut only if some remain after overlap removal
	if (overlapTool->getGoodMuonVec().size() > 0) return true;
	else return false;
}

Bool_t MuonCut::passedCaloCut(D3PDReader::MuonD3PDObjectElement *currMuon)
{
	// Author Cut
	if (currMuon->author() != 16 || (currMuon->caloMuonIdTag() <= 10 && currMuon->caloLRLikelihood() <= 0.9))
		return false;

	// Kinematic Cut
	if (currMuon->pt() <= 15000)
		return false;

	// Eta Cut
	if (fabs(currMuon->eta()) >= 0.1)
		return false;

	// ID Cuts
	Int_t nBLayer, nPix, nSCT, nHoles;
	Double_t nEta;
	if (m_dataYear == 2011)
	{
		nBLayer = 0;
		nPix = 1;
		nSCT = 5;
		nHoles = 3;
		nEta = 0.1;
	}
	else if (m_dataYear == 2012)
	{
		nBLayer = 0;
		nPix = 0;
		nSCT = 4;
		nHoles = 3;
		nEta = 0.1;
	}
	// Blayer
	if (m_dataYear == 2011 && currMuon->expectBLayerHit() && currMuon->nBLHits() <= nBLayer)
		return false;
	// Pix
	if (currMuon->nPixHits() + currMuon->nPixelDeadSensors() <= nPix)
		return false;
	// STC
	if (currMuon->nSCTHits() + currMuon->nSCTDeadSensors() <= nSCT)
		return false;
	// Holes
	if (currMuon->nPixHoles() + currMuon->nSCTHoles() >= nHoles)
		return false;

	// TRT
	Int_t nTRT = currMuon->nTRTHits() + currMuon->nTRTOutliers();
	Double_t dataEta = fabs(currMuon->eta());
	if (m_dataYear == 2011 && dataEta < 0.1)
		if (nTRT >= 6 && currMuon->nTRTOutliers() >= nTRT*0.9) return false;

	// D0 and Z0
	if (fabs(currMuon->trackd0pvunbiased()) >= m_d0Cut && fabs(currMuon->trackz0pvunbiased()) >= m_z0Cut)
		return false;

	return true; // Muon passed all cuts
}

Bool_t MuonCut::passedStacoCut(D3PDReader::MuonD3PDObjectElement *currMuon)
{
	// Author Cut
	if (currMuon->author() != 6 && currMuon->author() != 7)
		return false;

	// Kinematic Cut
	Double_t pTCut = 6000; // loosepTCut = 4000;
	if (currMuon->pt() <= pTCut)
		return false;

	// Eta Cut
	if (fabs(currMuon->eta()) >= 2.7)
		return false;

	// ID Cuts
	Int_t nBLayer, nPix, nSCT, nHoles;
	Double_t lwrEta, uprEta;
	if (m_dataYear == 2011)
	{
		nBLayer = 0;
		nPix = 1;
		nSCT = 5;
		nHoles = 3;
		lwrEta = 0;
		uprEta = 1.9;
	}
	else if (m_dataYear == 2012)
	{
		nBLayer = 0;
		nPix = 0;
		nSCT = 4;
		nHoles = 3;
		lwrEta = 0.1;
		uprEta = 1.9;
	}
	// Blayer
	if (m_dataYear == 2011 && (currMuon->expectBLayerHit() || currMuon->nBLHits() <= nBLayer))
		return false;
	// Pix
	if (currMuon->nPixHits() + currMuon->nPixelDeadSensors() <= nPix)
		return false;
	// STC
	if (currMuon->nSCTHits() + currMuon->nSCTDeadSensors() <= nSCT)
		return false;
	// Holes
	if (currMuon->nPixHoles() + currMuon->nSCTHoles() >= nHoles)
		return false;
	//TRT
	Int_t nTRT = currMuon->nTRTHits() + currMuon->nTRTOutliers();
	Double_t dataEta = fabs(currMuon->eta());
	if (dataEta > lwrEta && dataEta < uprEta)
	{
		if (nTRT <= 5 && currMuon->nTRTOutliers() >= nTRT*0.9)
			return false;
	}
	else if (m_dataYear == 2011)
	{
		if (nTRT > 5 && currMuon->nTRTOutliers() >= nTRT*0.9)
			return false;
	}

	//D0 and Z0 Cuts
	if (fabs(currMuon->trackd0pvunbiased()) >= m_d0Cut && fabs(currMuon->trackz0pvunbiased()) >= m_z0Cut)
		return false;

	return true; // Muon passed all cuts
}

Bool_t MuonCut::passedStandAloneCut(D3PDReader::MuonD3PDObjectElement *currMuon)
{
	// Author Cut
	if (currMuon->author() != 6)
		return false;

	// Kinematic Cut
	Double_t pTCut = 6000; // lossePTCut = 4000;
	if (currMuon->pt() <= pTCut)
		return false;

	// Eta Cut
	if (fabs(currMuon->eta()) >= 2.7 || fabs(currMuon->eta()) <= 2.5)
		return false;

	//ID Cut
	if (currMuon->nCSCEtaHits() + currMuon->nCSCPhiHits() <= 0 || currMuon->nMDTEMHits() <=0 || currMuon->nMDTEOHits() <= 0)
		return false;
	
	return true; // Muon passed all cuts
}