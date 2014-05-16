#include "CutFlow4Lep/Triggers/ElectronMuonTrigger.h"

ElectronMuonTrigger::ElectronMuonTrigger(D3PDReader::Event *tEvent, Int_t tRunNumber)
	: Triggers(tEvent, tRunNumber)
{

}

ElectronMuonTrigger::~ElectronMuonTrigger()
{

}

Bool_t ElectronMuonTrigger::passedTrigger()
{
	if (m_dataPeriod == DataPeriod::run2011_BD || m_dataPeriod == DataPeriod::run2011_EH || m_dataPeriod == DataPeriod::run2011_IK || m_dataPeriod == DataPeriod::run2011_LM)
		return m_event->triggerbits.EF_e10_medium_mu6();
	else if (m_dataPeriod == DataPeriod::run2012_All)
		return m_event->triggerbits.EF_e12Tvh_medium1_mu8() | m_event->triggerbits.EF_e24vhi_loose1_mu8();
	else
		return true;
}