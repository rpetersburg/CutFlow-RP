#include "CutFlow4Lep/UsesEvent.h"

using namespace std;
class Cuts : public UsesEvent
{
	public:
		Cuts(D3PDReader::Event *tEvent) : UsesEvent(tEvent) {};
		~Cuts() {};

		virtual Bool_t passedCut() = 0;

	protected:

	private:

};