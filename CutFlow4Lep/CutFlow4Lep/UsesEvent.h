#ifndef USESEVENT_H
#define USESEVENT_H

#include <D3PDReader/Event.h>

using namespace std;
class UsesEvent
{
	public:
		UsesEvent(D3PDReader::Event *tEvent);
		~UsesEvent();

	protected:
		D3PDReader::Event *m_event;
		Int_t m_dataYear;
		Int_t m_dataPeriod;
		Int_t m_runNumber;
		Int_t m_eventNumber;
		Bool_t m_isMC;

		void initializeAll();
		void setEvent(D3PDReader::Event *tEvent);
		void setDataYear();
		void setDataPeriod();
		void setRunNumber();
		void setEventNumber();
		void setIsMC();

	private:

};
#endif