#ifndef ELECTRONTRIGGERMATCH_H
#define ELECTRONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"
#include "CutFlow4Lep/Triggers/ElectronTrigger.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

using namespace std;
class ElectronTriggerMatch : public TriggerMatch
{
	public:
		ElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf, vector<ChargedLepton*> *tLeptonVec);
		ElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf);
		~ElectronTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();
		
		const Double_t PTTHRESHOLD;
};
#endif