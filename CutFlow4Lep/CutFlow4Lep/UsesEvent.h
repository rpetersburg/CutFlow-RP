#ifndef USESEVENT_H
#define USESEVENT_H

#include <iostream>

#include <D3PDReader/Event.h>

#include "CutFlow4Lep/StructDef.h"

using namespace std;
class UsesEvent
{
	public:
		UsesEvent();
		UsesEvent(D3PDReader::Event *tEvent);
		~UsesEvent();

	protected:
		D3PDReader::Event *m_event;
		Int_t m_dataYear;
		Int_t m_dataPeriod;
		Int_t m_runNumber;
		Int_t m_eventNumber;
		Int_t m_electronCollection;
		Int_t m_dataCalibration;
		Bool_t m_isMC;

		void initializeAll();
		void setEvent(D3PDReader::Event *tEvent);
		void setDataYear();
		void setDataPeriod();
		void setRunNumber();
		void setEventNumber();
		void setIsMC();
		void setElectronCollection();
		void setDataCalibration();

		Int_t getNumVertex(Int_t threshold);

	private:

};
#endif