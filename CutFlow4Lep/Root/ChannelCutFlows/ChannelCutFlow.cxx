#include "CutFlow4Lep/ChannelCutFlows/ChannelCutFlow.h"

ChannelCutFlow::ChannelCutFlow()
{

}

ChannelCutFlow::~ChannelCutFlow()
{

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

Int_t ChannelCutFlow::getNumCaloAndStandAlone(DiLepton *diLepton)
{
	Int_t numCalo, numStandAlone;
	if (diLepton->getPosLepton()->getType() == LeptonType::MuonCalo) numCalo++;
	else if (diLepton->getPosLepton()->getType() == LeptonType::MuonStandAlone) numStandAlone++;
	if (diLepton->getNegLepton()->getType() == LeptonType::MuonCalo) numCalo++;
	else if (diLepton->getNegLepton()->getType() == LeptonType::MuonStandAlone) numStandAlone++;

	return numCalo + numStandAlone;
}