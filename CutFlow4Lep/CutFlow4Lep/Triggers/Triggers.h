#include <D3PDReader/Event.h>
#include <PileupReweighting/TPileupReweighting.h>
#include "CutFlow4Lep/StructDef.h"

using namespace std;
class Triggers
{
	public:
		Triggers(D3PDReader::Event *tEvent, Int_t tDataPeriod, Int_t tRunNumber) 
			: m_event(tEvent), m_dataPeriod(tDataPeriod), m_runNumber(tRunNumber) {};
		~Triggers() {};

		virtual Bool_t passedTrigger() = 0;

	protected:
		D3PDReader::Event *m_event;
		Int_t m_dataPeriod;
		Int_t m_runNumber;

	private:

};