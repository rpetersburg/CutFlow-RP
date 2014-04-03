#include "CutFlow4Lep/Triggers/Triggers.h"

class MuonTrigger : public Triggers
{
	public:
		MuonTrigger(D3PDReader::Event *tEvent, Int_t tDataPeriod, Int_t tRunNumber);
		~MuonTrigger();

		Bool_t passedTrigger();

	protected:

	private:

};