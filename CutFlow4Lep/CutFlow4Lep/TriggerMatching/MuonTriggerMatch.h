#ifndef MUONTRIGGERMATCH_H
#define MUONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"
#include "CutFlow4Lep/Triggers/MuonTrigger.h"

using namespace std;
class MuonTriggerMatch : public TriggerMatch
{
	const Double_t PTTHRESHOLD;

	public:
		MuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf, vector<ChargedLepton*> *tLeptonVec);
		MuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf);
		~MuonTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();
};
#endif