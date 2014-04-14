#ifndef CORRECTION_H
#define CORRECTION_H

#include "CutFlow4Lep/UsesEvent.h"

class Correction : public UsesEvent
{
	public:
		Correction(D3PDReader::Event *tEvent) : UsesEvent(tEvent) {};
		~Correction();

		virtual void executeCorrection() = 0;

	protected:
		virtual void initializeCorrectionTool() = 0;

	private:

};


#endif