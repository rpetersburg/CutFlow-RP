#include "CutFlow4Lep/Triggers/DiElectronTrigger.h"

DiElectronTrigger::DiElectronTrigger(D3PDReader::Event *tEvent, Int_t tDataPeriod, Int_t tRunNumber)
	: Triggers(tEvent, tDataPeriod, tRunNumber)
{

}

DiElectronTrigger::~DiElectronTrigger()
{

}

Bool_t DiElectronTrigger::passedTrigger()
{
	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		return m_event->triggerbits.EF_2e12_medium();
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber < 186873)
			return m_event->triggerbits.EF_2e12_medium();
		else
			return m_event->triggerbits.EF_2e12T_medium();
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		return m_event->triggerbits.EF_2e12Tvh_medium();
	else if (m_dataPeriod == DataPeriod::run2012_All)
	{
		if (m_event->eventinfo.isSimulation())
			return m_event->triggerbits.EF_2e12Tvh_loose1() | m_event->triggerbits.EF_2e12Tvh_loose1_L2StarB();
		else
			return m_event->triggerbits.EF_2e12Tvh_loose1();
	}
	else
		return true;
}