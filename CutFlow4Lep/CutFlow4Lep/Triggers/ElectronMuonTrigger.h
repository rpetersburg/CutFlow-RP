#include "CutFlow4Lep/Triggers/Triggers.h"

class ElectronMuonTrigger : public Triggers
{
	public:
		ElectronMuonTrigger(D3PDReader::Event *tEvent, Int_t tDataPeriod, Int_t tRunNumber);
		~ElectronMuonTrigger();

		Bool_t passedTrigger();

	protected:

	private:

};