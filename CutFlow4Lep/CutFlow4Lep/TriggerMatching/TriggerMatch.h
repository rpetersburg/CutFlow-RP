#ifndef TRIGGERMATCH_H
#define TRIGGERMATCH_H

#include "CutFlow4Lep/UsesEvent.h"
#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"

#include "TrigMuonEfficiency/TriggerNavigationVariables.h"
#include "TrigMuonEfficiency/MuonTriggerMatching.h"
#include "TrigMuonEfficiency/ElectronTriggerMatching.h"
#include "TrigMuonEfficiency/LeptonTriggerSF.h"

using namespace std;
class TriggerMatch : public UsesEvent
{
	public:
		TriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec);
		~TriggerMatch();

		virtual Bool_t passedCutThreshold() = 0;

	protected:
		void initTriggerNavigationVariables();
		void initTriggerMatchTools();
		void initLeptonTriggerSFTool();

		Int_t m_currMCCollection;

		vector<ChargedLepton*> *m_leptonVec;

		TriggerNavigationVariables *m_triggerNavigationVariables;
		MuonTriggerMatching *m_muonTriggerMatchTool;
		ElectronTriggerMatching *m_electronTriggerMatchTool;
		LeptonTriggerSF *m_leptonTriggerSFTool;

	private:
};
#endif