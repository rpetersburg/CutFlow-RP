#include "CutFlow4Lep/TriggerMatching/ElectronTriggerMatch.h"

ElectronTriggerMatch::ElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, vector<ChargedLepton*> *tLeptonVec, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tLeptonVec, tRunNumber_sf), PTTHRESHOLD(-1000)
{

}

ElectronTriggerMatch::~ElectronTriggerMatch()
{

}

void ElectronTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		m_triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		m_triggerString[0] = "EF_e20_medium";
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber_sf < 186873) m_triggerString[0] = "EF_e20_medium";
		else m_triggerString[0] = "EF_e22_medium";
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		m_triggerString[0] = "EF_e22vh_medium";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		m_triggerString[0] = "EF_e24vhi_medium1";
		m_triggerString[1] = "EF_e60_medium1";
	}
}

Bool_t ElectronTriggerMatch::passedCutThreshold()
{
	if (!(new ElectronTrigger(m_event, m_runNumber_sf))->passedTrigger())
		return false;

	vector<ChargedLepton*>::iterator leptonItr = m_leptonVec->begin();

	for ( ; leptonItr != m_leptonVec->end(); ++leptonItr)
	{
		ChargedLepton *currLepton = *leptonItr;

		if (currLepton->getFlavor() != Flavor::Electron) continue; // Only Electrons...

		Electron *currElectron = (Electron*)currLepton; // For simplicity

		for (Int_t triggerIndex = 0; triggerIndex < 2; triggerIndex++)
		{
			if (m_triggerString[triggerIndex] == "") continue;

			Bool_t triggerMatch = m_electronTriggerMatchTool->match(currElectron->getElectron()->tracketa(),
																															currElectron->getElectron()->trackphi(),
																															m_triggerString[triggerIndex].Data());

			if (triggerMatch && currElectron->getElectron()->pt() > PTTHRESHOLD)
				return true;
		}
	}
}