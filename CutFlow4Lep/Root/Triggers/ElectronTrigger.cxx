#include "CutFlow4Lep/Triggers/ElectronTrigger.h"

ElectronTrigger::ElectronTrigger(D3PDReader::Event *tEvent, Int_t tDataPeriod, Int_t tRunNumber)
	: Triggers(tEvent, tDataPeriod, tRunNumber)
{

}

ElectronTrigger::~ElectronTrigger()
{

}

Bool_t ElectronTrigger::passedTrigger()
{
	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		return m_event->triggerbits.EF_e20_medium();
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber < 186873)
			return m_event->triggerbits.EF_e20_medium();
		else
			return m_event->triggerbits.EF_e22_medium();
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		return m_event->triggerbits.EF_e22vh_medium1();
	else if (m_dataPeriod == DataPeriod::run2012_All)
		return m_event->triggerbits.EF_e60_medium1();
	else
		return true;
}