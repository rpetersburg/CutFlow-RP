#include "CutFlow4Lep/Triggers/DiMuonTrigger.h"

DiMuonTrigger::DiMuonTrigger(D3PDReader::Event *tEvent, Int_t tRunNumber)
	: Triggers(tEvent, tRunNumber)
{

}

DiMuonTrigger::~DiMuonTrigger()
{

}

Bool_t DiMuonTrigger::passedTrigger()
{
	if (m_dataPeriod == DataPeriod::run2011_BD ||
			m_dataPeriod == DataPeriod::run2011_EH ||
			m_dataPeriod == DataPeriod::run2011_IK ||
			m_dataPeriod == DataPeriod::run2011_LM)
		return m_event->triggerbits.EF_2mu10_loose();

	else if (m_dataPeriod == DataPeriod::run2012_All)
		return m_event->triggerbits.EF_2mu13() |
					 m_event->triggerbits.EF_mu18_tight_mu8_EFFS();

	else
		return true;
}