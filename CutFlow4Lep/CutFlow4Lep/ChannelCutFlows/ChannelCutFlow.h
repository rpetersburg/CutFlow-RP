#ifndef CHANNELCUTFLOW_H
#define CHANNELCUTFLOW_H

#include <TROOT.h>

#include "CutFlow4Lep/ParticleObjects/QuadLepton.h"

#include "CutFlow4Lep/TriggerMatching/MuonTriggerMatch.h"
#include "CutFlow4Lep/TriggerMatching/DiMuonTriggerMatch.h"
#include "CutFlow4Lep/TriggerMatching/ElectronTriggerMatch.h"
#include "CutFlow4Lep/TriggerMatching/DiElectronTriggerMatch.h"
#include "CutFlow4Lep/TriggerMatching/ElectronMuonTriggerMatch.h"

using namespace std;
class ChannelCutFlow : public UsesEvent
{
	public:
		ChannelCutFlow(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf);
		~ChannelCutFlow();

		virtual Bool_t passedCut() = 0;

	protected:
		template<class Lepton> vector<DiLepton*> getDiLeptons(vector<Lepton*> *leptonVec);
		vector<QuadLepton*> getQuadLeptons(vector<DiLepton*> *diLeptons);
		vector<QuadLepton*> getQuadLeptons(vector<DiLepton*> *iDiLeptons, vector<DiLepton*> *jDiLeptons);

		QuadLepton* getQuadEvent(vector<QuadLepton*> *quadLeptonVec, Bool_t doZ4lAnalysis);

		Int_t getNumCaloAndStandAlone(DiLepton *diLepton);

		Int_t m_currMCCollection;
		Int_t m_runNumber_sf;

		MuonTriggerMatch *m_muonTriggerMatch;
		DiMuonTriggerMatch *m_diMuonTriggerMatch;
		ElectronTriggerMatch *m_electronTriggerMatch;
		DiElectronTriggerMatch *m_diElectronTriggerMatch;
		ElectronMuonTriggerMatch *m_electronMuonTriggerMatch;

	private:
		void setLeptonVecsForTriggerMatch(vector<ChargedLepton*> currLeptonVec);
};
#endif