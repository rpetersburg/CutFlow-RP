#ifndef EVENTTYPECONVERTER_H
#define EVENTTYPECONVERTER_H

#include "D3PDReader/Event.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODMuon/MuonContainer.h"

class EventTypeConverter
{
public:
	EventTypeConverter();
	~EventTypeConverter();

	D3PDReader::Event* xAODtoD3PD(xAOD::TEvent *xAODEvent);

protected:

private:
	void eventInfoTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);

	void muonTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);
	void electronTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);
	void jetsTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);
	void photonTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);

	void trigTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);
	void vxpTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);
	void mcTransfer(D3PDReader::Event *D3PDEvent, xAOD::TEvent *xAODEvent);
};
#endif