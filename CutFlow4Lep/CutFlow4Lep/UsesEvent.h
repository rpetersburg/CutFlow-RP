#ifndef USESEVENT_H
#define USESEVENT_H

#include <iostream>

#include "D3PDReader/Event.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"

#include "CutFlow4Lep/StructDef.h"

using namespace std;
class UsesEvent
{
	public:
		UsesEvent();
		UsesEvent(D3PDReader::Event *tEvent);
		UsesEvent(xAOD::TEvent *tEvent);
		~UsesEvent();

		void setEvent(D3PDReader::Event *tEvent);
		void setEvent(xAOD::TEvent *tEvent);

		Int_t getNumVertex(Int_t threshold);
		Bool_t getIsMC() {return m_isMC;};

	protected:
		D3PDReader::Event *m_event;
		xAOD::TEvent *m_event_xAOD;
		const xAOD::EventInfo *m_eventInfo;
		Bool_t m_isMC;
		Int_t m_dataYear;
		Int_t m_dataPeriod;
		Int_t m_runNumber;
		Int_t m_eventNumber;
		Int_t m_electronCollection;
		Int_t m_dataCalibration;
		Int_t m_mcChannelNumber;		

		// Normal initialization functions for D3PD
		void init();
		void initIsMC();
		void initDataYear();
		void initDataPeriod();
		void initRunNumber();
		void initEventNumber();
		void initElectronCollection();
		void initDataCalibration();
		void initMCChannelNumber();
		// Added initialization functions for xAOD
		void init_xAOD();
		void initIsMC_xAOD();
		void initRunNumber_xAOD();
		void initEventNumber_xAOD();
		void initMCChannelNumber_xAOD();

	private:
};
#endif