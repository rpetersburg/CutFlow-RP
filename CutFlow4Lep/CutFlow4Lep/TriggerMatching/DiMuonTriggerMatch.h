#ifndef DIMUONTRIGGERMATCH_H
#define DIMUONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"
#include "CutFlow4Lep/Triggers/DiMuonTrigger.h"

using namespace std;
class DiMuonTriggerMatch : public TriggerMatch
{
	public:
		DiMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf);
		~DiMuonTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();

		const Double_t PTTHRESHOLD1;
		const Double_t PTTHRESHOLD2;
};
#endif