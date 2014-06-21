#ifndef ELECTRONMUONTRIGGERMATCH_H
#define ELECTRONMUONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"

using namespace std;
class ElectronMuonTriggerMatch : public TriggerMatch
{
	public:
		ElectronMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf);
		~ElectronMuonTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();
};
#endif