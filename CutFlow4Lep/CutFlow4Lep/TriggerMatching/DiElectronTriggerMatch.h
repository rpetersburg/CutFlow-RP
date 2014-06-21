#ifndef DIELECTRONTRIGGERMATCH_H
#define DIELECTRONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"

using namespace std;
class DiElectronTriggerMatch : public TriggerMatch
{
	public:
		DiElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf);
		~DiElectronTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();
};
#endif