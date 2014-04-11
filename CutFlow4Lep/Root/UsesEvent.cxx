#include "CutFlow4Lep/UsesEvent.h"
#include "CutFlow4Lep/StructDef.h"

UsesEvent::UsesEvent(D3PDReader::Event *tEvent) : m_event(tEvent)
{
	initializeAll();
}

UsesEvent::~UsesEvent()
{

}

void UsesEvent::initializeAll()
{
	setIsMC();
	setRunNumber();
	setDataYear();
	setDataPeriod();
	setEventNumber();
}

void UsesEvent::setEvent(D3PDReader::Event *tEvent)
{
	m_event = tEvent;
	initializeAll();
}

void UsesEvent::setDataYear()
{
	if ( (m_runNumber >= 177531) && (m_runNumber <= 191933) )
		m_dataYear = 2011;
	else if (m_runNumber > 191933 )
		m_dataYear = 2012;
}

void UsesEvent::setDataPeriod()
{
	if (m_runNumber >= 177986 && m_runNumber <= 180481) 
		m_dataPeriod = DataPeriod::run2011_BD;
	else if (m_runNumber >= 180614 && m_runNumber <= 184169)
		m_dataPeriod = DataPeriod::run2011_EH;
	else if (m_runNumber >= 185353 && m_runNumber <= 187815)
		m_dataPeriod = DataPeriod::run2011_IK;
	else if (m_runNumber >= 188902 && m_runNumber <= 191933)
		m_dataPeriod = DataPeriod::run2011_LM;
	else if (m_runNumber >= 195847 && m_runNumber <= 999999)
		m_dataPeriod = DataPeriod::run2012_All;
}

void UsesEvent::setRunNumber()
{
	m_runNumber = m_event->eventinfo.RunNumber();
}

void UsesEvent::setIsMC()
{
	if(m_event->eventinfo.isSimulation()) m_isMC = true;
	else m_isMC = false;
}

void UsesEvent::setEventNumber()
{
	m_eventNumber = m_event->eventinfo.EventNumber();
}