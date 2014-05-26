#include "CutFlow4Lep/TriggerMatching/MuonTriggerMatch.h"

MuonTriggerMatch::MuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec)
	: TriggerMatch(tEvent, tCurrMCCollection, tLeptonVec)
{

}

MuonTriggerMatch::~MuonTriggerMatch()
{

}

Bool_t MuonTriggerMatch::passedCutThreshold()
{
	if (m_dataYear == 2011) // From Fabien, apparently a "random fix"
	{
 		m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainerStatus(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_eMuonEFInfoStatus());
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainer(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_eMuonEFInfo());
	}


}