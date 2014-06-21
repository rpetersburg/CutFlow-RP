#include "CutFlow4Lep/TriggerMatching/ElectronMuonTriggerMatch.h"

ElectronMuonTriggerMatch::ElectronMuonTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tLeptonVec, tRunNumber_sf)
{

}

ElectronMuonTriggerMatch::~ElectronMuonTriggerMatch()
{

}

void ElectronMuonTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD ||	m_dataPeriod == DataPeriod::run2011_EH ||
			m_dataPeriod == DataPeriod::run2011_IK ||	m_dataPeriod == DataPeriod::run2011_LM)
		triggerString[0] = "EF_e10_medium_mu6";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		triggerString[0] = "EF_e12Tvh_medium1_mu8";
		triggerString[1] = "EF_e24vhi_loose1_mu8";
	}
}

Bool_t ElectronMuonTriggerMatch::passedCutThreshold()
{
}