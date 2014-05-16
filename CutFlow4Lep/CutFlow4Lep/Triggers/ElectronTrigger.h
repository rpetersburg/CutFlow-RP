#ifndef ELECTRONTRIGGER_H
#define ELECTRONTRIGGER_H

#include "CutFlow4Lep/Triggers/Triggers.h"

class ElectronTrigger : public Triggers
{
	public:
		ElectronTrigger(D3PDReader::Event *tEvent, Int_t tRunNumber);
		~ElectronTrigger();

		Bool_t passedTrigger();

	protected:

	private:

};
#endif