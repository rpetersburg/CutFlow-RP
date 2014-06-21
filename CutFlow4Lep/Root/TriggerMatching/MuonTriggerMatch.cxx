#include "CutFlow4Lep/TriggerMatching/MuonTriggerMatch.h"

MuonTriggerMatch::MuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tLeptonVec, tRunNumber_sf), PTTHRESHOLD(-1000)
{

}

MuonTriggerMatch::~MuonTriggerMatch()
{

}

void MuonTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		triggerString[0] = "EF_mu18_MG";
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber_sf < 186516) triggerString[0] = "EF_mu18_MG";
		else triggerString[0] = "EF_mu18_MG_medium";
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		triggerString[0] = "EF_mu18_MG_medium";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		triggerString[0] = "EF_mu24i_tight";
		triggerString[1] = "EF_mu36_tight";
	}
}

Bool_t MuonTriggerMatch::passedCutThreshold()
{
	if (m_dataYear == 2011) // From Fabien, apparently a "random fix"
	{
 		m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainerStatus(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_eMuonEFInfoStatus());
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainer(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_eMuonEFInfo());
	}

	if (!(new MuonTrigger(m_event, m_runNumber_sf))->passedTrigger())
		return false;

	vector<ChargedLepton*>::iterator leptonItr = m_leptonVec->begin();

	for ( ; leptonItr != m_leptonVec->end(); ++leptonItr)
	{
		ChargedLepton *currLepton = *leptonItr;

		// Don't want an Electron...
		if (currLepton->getFlavor() != Flavor::Muon) continue;
		// No Trigger for Calo Muon
		if (currLepton->getFlavor() == LeptonType::MuonCalo) continue;

		Muon *currMuon = (Muon*)currLepton; // Casting for easier use

		for (Int_t triggerIndex = 0; triggerIndex < 2; triggerIndex++)
		{
			// If the second trigger is not initialized
			if (triggerString[triggerIndex] == "") continue;

			Bool_t triggerMatch = m_muonTriggerMatchTool->match(currMuon->getMuon()->eta(),
																													currMuon->getMuon()->phi(),
																													triggerString[triggerIndex].Data());

			if (triggerMatch && currMuon->getMuon()->pt() > PTTHRESHOLD) return true;
		}
	}
	return false;
}