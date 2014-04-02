#include <CutFlow4Lep/Weights/Weights.h>
#include <CutFlow4Lep/Calculations/MCHiggsMass.h>
#include <CutFlow4Lep/StructDef.h>

#include <PileupReweighting/TPileupReweighting.h>

using namespace std;

class PileupWeight : public Weights
{
	public:
		PileupWeight(D3PDReader::Event *tEvent, Int_t tDataYear, Root::TPileupReweighting *tPileupReweight);
		~PileupWeight();

		Root::TPileupReweighting* getReweight() {return m_reweight;};

	protected:

	private:
		void initializeReweight();
		void setWeight();

		Root::TPileupReweighting *m_reweight;
		Int_t m_mcCollection;
		Int_t m_dataCollection;
};
