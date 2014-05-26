#ifndef MUONTRIGGERMATCH_H
#define MUONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"

using namespace std;
class MuonTriggerMatch : public TriggerMatch
{
	public:
		MuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec);
		~MuonTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
};
#endif