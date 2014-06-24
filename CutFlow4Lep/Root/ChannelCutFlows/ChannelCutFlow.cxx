#include "CutFlow4Lep/ChannelCutFlows/ChannelCutFlow.h"

ChannelCutFlow::ChannelCutFlow(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf)
	: UsesEvent(tEvent), m_currMCCollection(tCurrMCCollection), m_runNumber_sf(tRunNumber_sf)
{
	m_muonTriggerMatch = new MuonTriggerMatch(m_event, m_currMCCollection, m_runNumber_sf);
	m_diMuonTriggerMatch = new DiMuonTriggerMatch(m_event, m_currMCCollection, m_runNumber_sf);
	m_electronTriggerMatch = new ElectronTriggerMatch(m_event, m_currMCCollection, m_runNumber_sf);	
	m_diElectronTriggerMatch = new DiElectronTriggerMatch(m_event, m_currMCCollection, m_runNumber_sf);
	m_electronMuonTriggerMatch = new ElectronMuonTriggerMatch(m_event, m_currMCCollection, m_runNumber_sf);
}

ChannelCutFlow::~ChannelCutFlow()
{
	delete m_muonTriggerMatch;
	delete m_diMuonTriggerMatch;
	delete m_electronTriggerMatch;
	delete m_diElectronTriggerMatch;
	delete m_electronMuonTriggerMatch;
}

template<class Lepton>
vector<DiLepton*> ChannelCutFlow::getDiLeptons(vector<Lepton*> *leptonVec)
{
	vector<DiLepton*> diLeptonVec;

	ChargedLepton *iLepton, *jLepton;
	// Iterate through leptons
	vector<Lepton*>::iterator iLeptonItr = leptonVec->begin();
	for ( ; iLeptonItr != leptonVec->end(); ++iLeptonItr)
	{
		iLepton = *iLeptonItr;

		// Iterate through later leptons in the vector
		vector<Lepton*>::iterator jLeptonItr = iLeptonItr + 1;
		for ( ; jLeptonItr != leptonVec->end(); ++jLeptonItr)
		{
			jLepton = *jLeptonItr;

			// Two leptons must be oppositely charged
			if (iLepton != jLepton && iLepton->getCharge() * jLepton->getCharge() < 0)
				diLeptonVec.push_back(new DiLepton(iLepton, jLepton));
		}
	}
	return diLeptonVec;
}

vector<QuadLepton*> ChannelCutFlow::getQuadLeptons(vector<DiLepton*> *diLeptons)
{
	vector<QuadLepton*> quadLeptonVec;

	DiLepton *iDiLepton, *jDiLepton;
	// Iterate through dileptons
	vector<DiLepton*>::iterator iDiLeptonItr = diLeptons->begin();
	for ( ; iDiLeptonItr != diLeptons->end(); ++iDiLeptonItr)
	{
		iDiLepton = *iDiLeptonItr;

		// Counting Calo and StandAlone Muons in first DiLepton
		Int_t iNumCaloAndStandAlone = getNumCaloAndStandAlone(iDiLepton);

		// Iterate through later dileptons in the vector
		vector<DiLepton*>::iterator jDiLeptonItr = iDiLeptonItr + 1;
		for ( ; jDiLeptonItr != diLeptons->end(); ++jDiLeptonItr)
		{
			jDiLepton = *jDiLeptonItr;

			// Preventing identical DiLeptons from forming quadruplet
			if (jDiLepton->isEqual(iDiLepton)) continue;

			// Counting Calo and StandAlone Muon in second DiLepton
			Int_t jNumCaloAndStandAlone = getNumCaloAndStandAlone(jDiLepton);

			// At most one muon can be either Calo or Standalone
			if (iNumCaloAndStandAlone + jNumCaloAndStandAlone > 1) continue;

			// Ordering of Z1 and Z2 depends on Z mass difference from pdgZMass
			Double_t iDeltaZMass = fabs(iDiLepton->getMomentumVec()->M() - pdgZMass);
			Double_t jDeltaZMass = fabs(jDiLepton->getMomentumVec()->M() - pdgZMass);
			if (iDeltaZMass < jDeltaZMass)
				quadLeptonVec.push_back(new QuadLepton(iDiLepton, jDiLepton));
			else
				quadLeptonVec.push_back(new QuadLepton(jDiLepton, iDiLepton));
		}
	}
	return quadLeptonVec;
}

vector<QuadLepton*> ChannelCutFlow::getQuadLeptons(vector<DiLepton*> *iDiLeptons, vector<DiLepton*> *jDiLeptons)
{
	vector<QuadLepton*> quadLeptonVec;

	DiLepton *iDiLepton, *jDiLepton;
	// Iterate through dileptons
	vector<DiLepton*>::iterator iDiLeptonItr = iDiLeptons->begin();
	for ( ; iDiLeptonItr != iDiLeptons->end(); ++iDiLeptonItr)
	{
		iDiLepton = *iDiLeptonItr;

		// Counting Calo and StandAlone Muons in first DiLepton
		Int_t iNumCaloAndStandAlone = getNumCaloAndStandAlone(iDiLepton);

		// Iterate through later dileptons in the vector
		vector<DiLepton*>::iterator jDiLeptonItr = jDiLeptons->begin();
		for ( ; jDiLeptonItr != jDiLeptons->end(); ++jDiLeptonItr)
		{
			jDiLepton = *jDiLeptonItr;

			// Preventing identical DiLeptons from forming quadruplet
			if (jDiLepton->isEqual(iDiLepton)) continue;

			// Counting Calo and StandAlone Muon in second DiLepton
			Int_t jNumCaloAndStandAlone = getNumCaloAndStandAlone(jDiLepton);

			// At most one muon can be either Calo or Standalone
			if (iNumCaloAndStandAlone + jNumCaloAndStandAlone > 1) continue;

			// Ordering of Z1 and Z2 depends on Z mass difference from pdgZMass
			Double_t iDeltaZMass = fabs(iDiLepton->getMomentumVec()->M() - pdgZMass);
			Double_t jDeltaZMass = fabs(jDiLepton->getMomentumVec()->M() - pdgZMass);
			if (iDeltaZMass < jDeltaZMass)
				quadLeptonVec.push_back(new QuadLepton(iDiLepton, jDiLepton));
			else
				quadLeptonVec.push_back(new QuadLepton(jDiLepton, iDiLepton));
		}
	}
	return quadLeptonVec;
}

QuadLepton* ChannelCutFlow::getQuadEvent(vector<QuadLepton*> *quadLeptonVec, Bool_t doZ4lAnalysis)
{
	QuadLepton *quadEvent;
	Double_t diffZ1Mass = 99999999;
	Double_t diffZ2Mass = 99999999;

	// Cut Values
	Double_t pTCut1 = 20 * 1000;
	Double_t pTCut2 = 15 * 1000;
	Double_t pTCut3 = 10 * 1000;
	Double_t pTCut3Electron = 10 * 1000;
	Double_t pTCut3Muon = 8 * 1000;

	// Counting variables
	Int_t pTCount1, pTCount2, pTCount3;
	Double_t pTStorage[4] = {0};

	// Loop over all possible QuadLeptons to store pT values
	vector<QuadLepton*>::iterator quadLeptonItr = quadLeptonVec->begin();
	for ( ; quadLeptonItr != quadLeptonVec->end(); ++quadLeptonItr)
	{
		int pTCount1, pTCount2, pTCount3;

		QuadLepton *currQuadLepton = *quadLeptonItr;

		vector<ChargedLepton*> currLeptonVec = currQuadLepton->getLeptons();
		vector<ChargedLepton*>::iterator currLeptonItr = currLeptonVec.begin();
		Int_t i = 0; // Simple counter for pT storage
		for ( ; currLeptonItr != currLeptonVec.end(); ++currLeptonItr)
		{
			ChargedLepton *currLepton = *currLeptonItr;
			Double_t currpT = currLepton->getMomentumVec()->Pt();
			pTStorage[i] = currpT;
			i++;

			if (currpT > pTCut1) pTCount1++;
			if (currpT > pTCut2) pTCount2++;

			// For Z->4l analysis and pTCount3
			if (doZ4lAnalysis && currLepton->getFlavor() == Flavor::Electron && currpT > pTCut3Electron)
				pTCount3++;
			else if (doZ4lAnalysis && currLepton->getFlavor() == Flavor::Muon && currpT > pTCut3Muon)
				pTCount3++;
			else if (currpT > pTCut3)
				pTCount3++;
		}

		// Cuts
		if (pTCount1 < 1 || pTCount2 < 2 || pTCount3 <3) continue;

		// Implement TriggerMatching
		setLeptonVecsForTriggerMatch(currLeptonVec);
		Bool_t muTrigger = (m_muonTriggerMatch->passedCutThreshold() || m_diMuonTriggerMatch->passedCutThreshold());
		Bool_t elTrigger = (m_electronTriggerMatch->passedCutThreshold() || m_diElectronTriggerMatch->passedCutThreshold());
		Bool_t elMuTrigger = (m_electronMuonTriggerMatch->passedCutThreshold());
		// No need for analysis discrimination since each trigger match class confirms proper particle types
		if (!(muTrigger || elTrigger || elMuTrigger)) continue;

		// Find the QuadLepton with closest z mass
		Double_t tempDiff1 = fabs(currQuadLepton->getZ1()->getMomentumVec()->M() - pdgZMass);
		Double_t tempDiff2 = fabs(currQuadLepton->getZ1()->getMomentumVec()->M() - pdgZMass);

		if (tempDiff1 < diffZ1Mass)
		{
			diffZ1Mass = tempDiff1;
			diffZ2Mass = tempDiff2;
			quadEvent = currQuadLepton;
		}
		else if (tempDiff1 == diffZ1Mass && tempDiff2 < diffZ2Mass)
		{
			diffZ2Mass == tempDiff2;
			quadEvent = currQuadLepton;
		}		
	}
	return quadEvent;
}

void ChannelCutFlow::setLeptonVecsForTriggerMatch(vector<ChargedLepton*> currLeptonVec)
{
	m_muonTriggerMatch->setLeptonVec(&currLeptonVec);
	m_diMuonTriggerMatch->setLeptonVec(&currLeptonVec);
	m_electronTriggerMatch->setLeptonVec(&currLeptonVec);
	m_diElectronTriggerMatch->setLeptonVec(&currLeptonVec);
	m_electronMuonTriggerMatch->setLeptonVec(&currLeptonVec);
}

Int_t ChannelCutFlow::getNumCaloAndStandAlone(DiLepton *diLepton)
{
	Int_t numCalo, numStandAlone;
	if (diLepton->getPosLepton()->getType() == LeptonType::MuonCalo) numCalo++;
	else if (diLepton->getPosLepton()->getType() == LeptonType::MuonStandAlone) numStandAlone++;
	if (diLepton->getNegLepton()->getType() == LeptonType::MuonCalo) numCalo++;
	else if (diLepton->getNegLepton()->getType() == LeptonType::MuonStandAlone) numStandAlone++;

	return numCalo + numStandAlone;
}