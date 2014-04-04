#include "CutFlow4Lep/Cuts/Cuts.h"

using namespace std;
class AllPreselection : public Cuts
{
	public:
		AllPreselection(D3PDReader::Event *tEvent);
		~AllPreselection() {};

		Bool_t passedCut();

	protected:

	private:
		Int_t m_dataYear;
};