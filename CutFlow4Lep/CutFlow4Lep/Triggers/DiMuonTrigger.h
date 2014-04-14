#ifndef DIMUONTRIGGER_H
#define DIMUONTRIGGER_H

#include "CutFlow4Lep/Triggers/Triggers.h"

class DiMuonTrigger : public Triggers
{
	public:
		DiMuonTrigger(D3PDReader::Event *tEvent, Int_t tDataPeriod, Int_t tRunNumber);
		~DiMuonTrigger();

		Bool_t passedTrigger();

	protected:

	private:

};
#endif