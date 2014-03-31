#include <CutFlow4Lep/Weights/Weights.h>
#include <CutFlow4Lep/Calculations/MCHiggsMass.h>
#include <CutFlow4Lep/StructDef.h>

#include <PileupReweighting/TPileupReweighting.h>

using namespace std;

class PileupWeight : public Weights
{
	public:
		PileupWeight(D3PDReader::Event *tEvent, TString tDataYear, Int_t tMCCollection, Int_t tDataCollection);
		~PileupWeight();

	protected:

	private:
		void initializeReweight();
		void setWeight();

		Root::TPileupReweighting *m_reweight;
		Int_t m_mcCollection;
		Int_t m_dataCollection;
};
