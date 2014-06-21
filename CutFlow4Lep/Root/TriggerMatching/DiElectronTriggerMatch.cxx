#include "CutFlow4Lep/TriggerMatching/DiElectronTriggerMatch.h"

DiElectronTriggerMatch::DiElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tLeptonVec, tRunNumber_sf)
{

}

DiElectronTriggerMatch::~DiElectronTriggerMatch()
{

}

void DiElectronTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		triggerString[0] = "EF_2e12_medium";
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber_sf < 186873) triggerString[0] = "EF_2e12_medium";
		else triggerString[0] = "EF_2e12T_medium";
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		triggerString[0] = "EF_2e12Tvh_medium";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		triggerString[0] = "EF_2e12Tvh_loose1";
		if (!m_isMC) triggerString[1] = "EF_2e12Tvh_loose1_L2StarB";
	}
}

Bool_t DiElectronTriggerMatch::passedCutThreshold()
{
}