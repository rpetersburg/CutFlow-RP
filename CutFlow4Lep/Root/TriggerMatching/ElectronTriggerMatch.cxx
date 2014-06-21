#include "CutFlow4Lep/TriggerMatching/ElectronTriggerMatch.h"

ElectronTriggerMatch::ElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tLeptonVec, tRunNumber_sf)
{

}

ElectronTriggerMatch::~ElectronTriggerMatch()
{

}

void ElectronTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		triggerString[0] = "EF_e20_medium";
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber_sf < 186873) triggerString[0] = "EF_e20_medium";
		else triggerString[0] = "EF_e22_medium";
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		triggerString[0] = "EF_e22vh_medium";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		triggerString[0] = "EF_e24vhi_medium1";
		triggerString[1] = "EF_e60_medium1";
	}
}

Bool_t ElectronTriggerMatch::passedCutThreshold()
{

}