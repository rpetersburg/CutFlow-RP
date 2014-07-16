#include "CutFlow4Lep/EventTypeConverter.h"

EventTypeConverter::EventTypeConverter()
{

}

EventTypeConverter::~EventTypeConverter()
{

}

D3PDReader::Event* EventTypeConverter::xAODtoD3PD(xAOD::TEvent *xAODEvent)
{
	D3PDReader::Event *D3PDEvent = new D3PDReader::Event();

	eventInfoTransfer(D3PDEvent, xAODEvent);

	return D3PDEvent;
}

void EventTypeConverter::eventInfoTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent)
{
	const xAOD::EventInfo *xAODEventInfo = 0;
	xAODEvent->retrieve(xAODEventInfo, "EventInfo");

	D3PDEvent->eventinfo.averageIntPerXing() = xAODEventInfo->averageInteractionsPerCrossing();
	// D3PDEvent->eventinfo.coreFlags() = xAODEventInfo->coreFlags();
	D3PDEvent->eventinfo.EventNumber() = xAODEventInfo->eventNumber();
	D3PDEvent->eventinfo.isSimulation() = xAODEventInfo->eventType(xAOD::EventInfo::EventType::IS_SIMULATION);
	// D3PDEvent->eventinfo.larError() = xAODEventInfo->larError();
	D3PDEvent->eventinfo.lbn() = xAODEventInfo->lumiBlock();
	D3PDEvent->eventinfo.mc_channel_number() = xAODEventInfo->mcChannelNumber();
	D3PDEvent->eventinfo.mc_event_weight() = xAODEventInfo->mcEventWeight();
	D3PDEvent->eventinfo.RunNumber() = xAODEventInfo->runNumber();
	//D3PDEvent->eventinfo.tileError() = xAODEventInfo->tileError();
}

void EventTypeConverter::muonTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent)
{
	const xAOD::MuonContainer *xAODMuons = 0;
	xAODEvent->retrieve(xAODMuons, "Muons");
	Int_t numMuons = xAODMuons->size();

	xAOD::MuonContainer_v1 xAODMuonItr = xAODMuons->begin();

}