#include <PileupReweighting/TPileupReweighting.h>

#include "CutFlow4Lep/UsesEvent.h"
#include "CutFlow4Lep/StructDef.h"

using namespace std;
class Triggers : public UsesEvent
{
	public:
		Triggers(D3PDReader::Event *tEvent) : UsesEvent(tEvent) {};
		~Triggers() {};

		virtual Bool_t passedTrigger() = 0;

	protected:

	private:

};