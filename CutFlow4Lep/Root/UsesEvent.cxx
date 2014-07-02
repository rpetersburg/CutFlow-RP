#include "CutFlow4Lep/UsesEvent.h"

UsesEvent::UsesEvent()
{

}

UsesEvent::UsesEvent(D3PDReader::Event *tEvent) : m_event(tEvent)
{
	init();
}

UsesEvent::UsesEvent(xAOD::TEvent *tEvent) : m_event_xAOD(tEvent)
{
	init_xAOD();
}

UsesEvent::~UsesEvent()
{

}

void UsesEvent::setEvent(D3PDReader::Event *tEvent)
{
	m_event = tEvent;
	init();
}

void UsesEvent::setEvent(xAOD::TEvent *tEvent)
{
	m_event_xAOD = tEvent;
	init_xAOD();
}

void UsesEvent::init()
{
	initIsMC();
	initRunNumber(); // Must be before DataYear and DataPeriod
	initDataYear();
	initDataPeriod();
	initEventNumber();
	initElectronCollection();
	initDataCalibration();
	if (m_isMC) initMCChannelNumber();
}

void UsesEvent::init_xAOD()
{
	m_event_xAOD->retrieve(m_eventInfo, "EventInfo");

	initIsMC_xAOD();
	initRunNumber_xAOD();
	initDataYear();
	initDataPeriod();
	initEventNumber_xAOD();
	initElectronCollection();
	initDataCalibration();
	if (m_isMC) initMCChannelNumber_xAOD();
}

void UsesEvent::initIsMC()
{
	if (m_event->eventinfo.isSimulation()) m_isMC = true;
	else m_isMC = false;
}

void UsesEvent::initIsMC_xAOD()
{
	if (m_eventInfo->eventType(xAOD::EventInfo::EventType::IS_SIMULATION))
		m_isMC = true;
	else m_isMC = false;
}

void UsesEvent::initRunNumber()
{
	m_runNumber = m_event->eventinfo.RunNumber();
}

void UsesEvent::initRunNumber_xAOD()
{
	m_runNumber = m_eventInfo->runNumber();
}

void UsesEvent::initDataYear()
{
	if ( (m_runNumber >= 177531) && (m_runNumber <= 191933) )
		m_dataYear = 2011;
	else if (m_runNumber > 191933 )
		m_dataYear = 2012;
}

void UsesEvent::initDataPeriod()
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

void UsesEvent::initEventNumber()
{
	m_eventNumber = m_event->eventinfo.EventNumber();
}

void UsesEvent::initEventNumber_xAOD()
{
	m_eventNumber = m_eventInfo->eventNumber();
}

void UsesEvent::initElectronCollection()
{
	if (m_dataYear == 2011) m_electronCollection = ElectronCollection::LoosePlusPlus;
	else if (m_dataYear == 2012) m_electronCollection = ElectronCollection::Likelihood;
	// Below are options in case Moriond Config is active... don't need for now
	//else if(m_dataYear == 2012 && !useLikelihood) m_electronCollection = ElectronCollection::MultiLepton;
	//else if(m_dataYear == 2012 && useLikelihood) m_electronCollection = ElectronCollection::Likelihood;
}

void UsesEvent::initDataCalibration()
{
	if (m_dataYear == 2011) m_dataCalibration = DataCalibType::y2011d;
	else if (m_dataYear == 2012) m_dataCalibration = DataCalibType::y2012c;
}

void UsesEvent::initMCChannelNumber()
{
	m_mcChannelNumber = m_event->eventinfo.mc_channel_number();
}

void UsesEvent::initMCChannelNumber_xAOD()
{
	m_mcChannelNumber = m_eventInfo->mcChannelNumber();
}

Int_t UsesEvent::getNumVertex(Int_t threshold)
{
	Int_t nVertex = 0;
	for (Int_t i = 0; i < m_event->vxp.n(); i++)
		if (m_event->vxp[i].trk_n() >= threshold) nVertex++;
	return nVertex;
}