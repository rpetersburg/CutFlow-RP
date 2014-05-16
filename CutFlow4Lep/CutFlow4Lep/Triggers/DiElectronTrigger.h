#ifndef DIELECTRONTRIGGER_H
#define DIELECTRONTRIGGER_H

#include "CutFlow4Lep/Triggers/Triggers.h"

class DiElectronTrigger : public Triggers
{
	public:
		DiElectronTrigger(D3PDReader::Event *tEvent, Int_t tRunNumber);
		~DiElectronTrigger();

		Bool_t passedTrigger();

	protected:

	private:

};
#endif