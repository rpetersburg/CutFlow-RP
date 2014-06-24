#include "CutFlow4Lep/TriggerMatching/ElectronMuonTriggerMatch.h"

ElectronMuonTriggerMatch::ElectronMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tRunNumber_sf), PTTHRESHOLD1(-1000), PTTHRESHOLD2(-1000)
{

}

ElectronMuonTriggerMatch::ElectronMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf, vector<ChargedLepton*> *tLeptonVec)
	: TriggerMatch(tEvent, tCurrMCCollection, tRunNumber_sf), PTTHRESHOLD1(-1000), PTTHRESHOLD2(-1000)
{
	setLeptonVec(tLeptonVec);
}

ElectronMuonTriggerMatch::~ElectronMuonTriggerMatch()
{

}

void ElectronMuonTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		m_triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD ||	m_dataPeriod == DataPeriod::run2011_EH ||
			m_dataPeriod == DataPeriod::run2011_IK ||	m_dataPeriod == DataPeriod::run2011_LM)
		m_triggerString[0] = "EF_e10_medium_mu6";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		m_triggerString[0] = "EF_e12Tvh_medium1_mu8";
		m_triggerString[1] = "EF_e24vhi_loose1_mu8";
	}
}

Bool_t ElectronMuonTriggerMatch::passedCutThreshold()
{
	if (m_dataYear == 2011) // From Fabien, apparently a "random fix"
	{
 		m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainerStatus(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_MuonEFInfoStatus());
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainer(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_MuonEFInfo());
	}

	vector<ChargedLepton*>::iterator iLeptonItr = m_leptonVec->begin();

	for ( ; iLeptonItr != m_leptonVec->end(); ++iLeptonItr)
	{
		ChargedLepton *iCurrLepton = *iLeptonItr;

		if (iCurrLepton->getFlavor() != Flavor::Electron) continue; // First get Electron

		Electron *currElectron = (Electron*)iCurrLepton;

		vector<ChargedLepton*>::iterator jLeptonItr = m_leptonVec->begin();
		for ( ; jLeptonItr != m_leptonVec->end(); ++jLeptonItr)
		{
			ChargedLepton *jCurrLepton = *jLeptonItr;

			if (jCurrLepton = iCurrLepton) continue; // Need different particles
			if (jCurrLepton->getFlavor() != Flavor::Muon) continue; // Second get Muon

			Muon *currMuon = (Muon*)jCurrLepton;

			for (Int_t triggerIndex = 0; triggerIndex < 2; triggerIndex++)
			{
				if (m_triggerString[triggerIndex] == "") continue;

				Bool_t triggerMatch = m_electronTriggerMatchTool->matchElectronMuon(&(currElectron->getMomentumVec()),
																																						&(currMuon->getMomentumVec()),
																																						m_triggerString[triggerIndex].Data());

				if (triggerMatch &&
						currElectron->getElectron()->pt() > PTTHRESHOLD1 &&
						currMuon->getMuon()->pt() > PTTHRESHOLD2)
					return true;
			}
		}		
	}
}