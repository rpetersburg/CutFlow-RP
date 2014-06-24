#ifndef DIELECTRONTRIGGERMATCH_H
#define DIELECTRONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"
#include "CutFlow4Lep/Triggers/DiElectronTrigger.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

using namespace std;
class DiElectronTriggerMatch : public TriggerMatch
{
	public:
		DiElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf, vector<ChargedLepton*> *tLeptonVec);
		DiElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf);
		~DiElectronTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();

		const Double_t PTTHRESHOLD1, PTTHRESHOLD2;
};
#endif