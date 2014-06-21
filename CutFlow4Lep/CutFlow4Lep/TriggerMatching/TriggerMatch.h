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
		TriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf);
		~TriggerMatch();

		virtual Bool_t passedCutThreshold() = 0;

	protected:
		void init();
		void initTriggerNavigationVariables();
		void initTriggerMatchTools();
		void initLeptonTriggerSFTool();
		virtual void initTriggerString() = 0;

		Int_t m_currMCCollection;
		Int_t m_runNumber_sf;

		vector<ChargedLepton*> *m_leptonVec;

		TString m_triggerString[2];

		TriggerNavigationVariables *m_triggerNavigationVariables;
		MuonTriggerMatching *m_muonTriggerMatchTool;
		ElectronTriggerMatching *m_electronTriggerMatchTool;
		LeptonTriggerSF *m_leptonTriggerSFTool;

	private:
};
#endif