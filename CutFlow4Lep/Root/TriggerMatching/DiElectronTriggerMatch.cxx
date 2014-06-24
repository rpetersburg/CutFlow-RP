#include "CutFlow4Lep/TriggerMatching/DiElectronTriggerMatch.h"

DiElectronTriggerMatch::DiElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf)
	: TriggerMatch(tEvent, tCurrMCCollection, tRunNumber_sf), PTTHRESHOLD1(-1000), PTTHRESHOLD2(-1000)
{

}

DiElectronTriggerMatch::DiElectronTriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf, vector<ChargedLepton*> *tLeptonVec)
	: TriggerMatch(tEvent, tCurrMCCollection, tRunNumber_sf), PTTHRESHOLD1(-1000), PTTHRESHOLD2(-1000)
{
	setLeptonVec(tLeptonVec);
}

DiElectronTriggerMatch::~DiElectronTriggerMatch()
{

}

void DiElectronTriggerMatch::initTriggerString()
{
	for (Int_t i = 0; i < 2; i++) // Cleaning the String
		m_triggerString[i] = "";

	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		m_triggerString[0] = "EF_2e12_medium";
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber_sf < 186873) m_triggerString[0] = "EF_2e12_medium";
		else m_triggerString[0] = "EF_2e12T_medium";
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		m_triggerString[0] = "EF_2e12Tvh_medium";
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		m_triggerString[0] = "EF_2e12Tvh_loose1";
		if (!m_isMC) m_triggerString[1] = "EF_2e12Tvh_loose1_L2StarB";
	}
}

Bool_t DiElectronTriggerMatch::passedCutThreshold()
{
	if (!(new DiElectronTrigger(m_event, m_runNumber_sf))->passedTrigger())
		return false;

	vector<ChargedLepton*>::iterator iLeptonItr = m_leptonVec->begin();
	vector<ChargedLepton*>::iterator jLeptonItr = iLeptonItr + 1;

	Bool_t passT1, passT2; // To store result from matching tool

	for ( ; iLeptonItr != m_leptonVec->end(); ++iLeptonItr)
	{
		ChargedLepton *iCurrLepton = *iLeptonItr;

		if (iCurrLepton->getFlavor() != Flavor::Electron) continue; // Only Electrons...

		Electron *iCurrElectron = (Electron*)iCurrLepton;

		for ( ; jLeptonItr != m_leptonVec->end(); ++jLeptonItr)
		{
			ChargedLepton *jCurrLepton = *jLeptonItr;

			if (jCurrLepton->getFlavor() != Flavor::Electron) continue; // Only Electrons...

			Electron *jCurrElectron = (Electron*)jCurrLepton;

			for (Int_t triggerIndex = 0; triggerIndex < 2; triggerIndex++)
			{
				if (m_triggerString[triggerIndex] == "") continue;

				Bool_t triggerMatch = m_electronTriggerMatchTool->matchDielectron(&(iCurrElectron->getMomentumVec()),
																																					&(jCurrElectron->getMomentumVec()),
																																					m_triggerString[triggerIndex].Data(),
																																					passT1, passT2);

				if (!triggerMatch) 
					cout << "Error: DiElectronTriggerMatch::passCutThreshold(): Trigger not suppored: " << m_triggerString[triggerIndex] << endl;
				else if (passT1 && passT2 &&
								 iCurrElectron->getElectron()->pt() > PTTHRESHOLD1 &&
								 jCurrElectron->getElectron()->pt() > PTTHRESHOLD2)
					return true;
			}
		}
	}
}