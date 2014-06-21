#ifndef ELECTRONTRIGGERMATCH_H
#define ELECTRONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"

using namespace std;
class ElectronTriggerMatch : public TriggerMatch
{
	public:
		ElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf);
		~ElectronTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();
};
#endif