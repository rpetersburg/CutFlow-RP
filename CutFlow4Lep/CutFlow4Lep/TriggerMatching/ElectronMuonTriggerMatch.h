#ifndef ELECTRONMUONTRIGGERMATCH_H
#define ELECTRONMUONTRIGGERMATCH_H

#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"
#include "CutFlow4Lep/Triggers/ElectronMuonTrigger.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

using namespace std;
class ElectronMuonTriggerMatch : public TriggerMatch
{
	public:
		ElectronMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf, vector<ChargedLepton*> *tLeptonVec);
		ElectronMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf);
		~ElectronMuonTriggerMatch();

		Bool_t passedCutThreshold();

	protected:

	private:
		void initTriggerString();
		
		const Double_t PTTHRESHOLD1, PTTHRESHOLD2;
};
#endif