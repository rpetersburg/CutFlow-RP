#include "CutFlow4Lep/Triggers/MuonTrigger.h"

MuonTrigger::MuonTrigger(D3PDReader::Event *tEvent, Int_t tDataPeriod, Int_t tRunNumber)
	: Triggers(tEvent)
{

}

MuonTrigger::~MuonTrigger()
{

}

Bool_t MuonTrigger::passedTrigger()
{
	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH)
		return m_event->triggerbits.EF_mu18_MG();
	else if (m_dataPeriod == DataPeriod::run2011_IK)
	{
		if (m_runNumber < 186516)
			return m_event->triggerbits.EF_mu18_MG();
		else
			return m_event->triggerbits.EF_mu18_MG_medium();
	}
	else if (m_dataPeriod == DataPeriod::run2011_LM)
		return m_event->triggerbits.EF_mu18_MG_medium();
	else if (m_dataPeriod == DataPeriod::run2012_All)
		return m_event->triggerbits.EF_mu24i_tight() | m_event->triggerbits.EF_mu36_tight();
	else
		return true;
}