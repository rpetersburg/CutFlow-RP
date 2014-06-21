#include "CutFlow4Lep/TriggerMatching/DiMuonTriggerMatch.h"

DiMuonTriggerMatch::DiMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tLeptonVec, tRunNumber_sf), PTTHRESHOLD1(-1000), PTTHRESHOLD2(-1000)
{

}

DiMuonTriggerMatch::~DiMuonTriggerMatch()
{

}

void DiMuonTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH ||
			m_dataPeriod == DataPeriod::run2011_IK || m_dataPeriod == DataPeriod::run2011_LM)
		triggerString[0] = "EF_2mu10_loose";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		triggerString[0] = "EF_2mu13";
		triggerString[1] = "EF_mu18_tight_mu8_EFFS";
	}
}

Bool_t DiMuonTriggerMatch::passedCutThreshold()
{
	Bool_t passCutTrig = false;

	if (m_dataYear == 2011) // From Fabien, apparently a "random fix"
	{
 		m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainerStatus(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_MuonEFInfoStatus());
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainer(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_MuonEFInfo());
	}

	if (!(new DiMuonTrigger(m_event, m_runNumber_sf))->passedTrigger())
		return false;

	vector<ChargedLepton*>::iterator iCurrLeptonItr = m_leptonVec->begin();
	vector<ChargedLepton*>::iterator jCurrLeptonItr = m_leptonVec->begin();

	//To store the result from the matching tool
	pair<bool, bool> passT1, passT2;
	pair<bool, bool> passT3, passT4;

	for ( ; iCurrLeptonItr != m_leptonVec->end(); ++iCurrLeptonItr)
	{
		ChargedLepton *iCurrLepton = *iCurrLeptonItr;

		if (iCurrLepton->getFlavor() != Flavor::Muon) continue; // Don't want an electron...
		if (iCurrLepton->getType() == LeptonType::MuonCalo) continue; // No trigger for Calo Muon

		Muon *iCurrMuon = (Muon*)iCurrLepton; // Recasted for simplicity

		for ( ; jCurrLeptonItr != m_leptonVec->end(); ++jCurrLeptonItr)
		{
			ChargedLepton *jCurrLepton = *jCurrLeptonItr;

			if (iCurrLepton == jCurrLepton) continue;
			if (jCurrLepton->getFlavor() != Flavor::Muon) continue;
			if (jCurrLepton->getType() == LeptonType::MuonCalo) continue;

			Muon *jCurrMuon = (Muon*)jCurrLepton; // Recasted for simplicity

			for (Int_t triggerIndex = 0; triggerIndex < 2; triggerIndex++)
			{
				if (triggerString[triggerIndex] == "") continue;

				Bool_t trigMatch1 = m_muonTriggerMatchTool->matchDimuon(*(iCurrMuon->getMomentumVec()), 
            		                            		 	  						*(jCurrMuon->getMomentumVec()),
                		                         			 							triggerString[triggerIndex].Data(), 
																																passT1, passT2);
				Bool_t trigMatch2 = m_muonTriggerMatchTool->matchDimuon(*(jCurrMuon->getMomentumVec()), 
            		                            		 	  						*(iCurrMuon->getMomentumVec()), 
                		                         			 							triggerString[triggerIndex].Data(), 
																																passT3, passT4);
				if (!(trigMatch1 || trigMatch2))
				{
					cout << "Error: DiMuonTriggerMatch::passedCutThreshold(): Trigger not supported: " << triggerString[triggerIndex] << endl;
					continue;
				}
				else
					Bool_t passCutTrig = (passT1.first && passT2.second) || (passT3.first && passT4.second);

				if (passCutTrig && iCurrMuon->getMuon()->pt() > PTTHRESHOLD1 && jCurrMuon->getMuon()->pt() > PTTHRESHOLD2)
					return true;
			}
		}
	}
}