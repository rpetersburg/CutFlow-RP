#include <CutFlow4Lep/Weights/Weights.h>
#include <CutFlow4Lep/Calculations/MCHiggsMass.h>

#include <ggFReweighting/ggFReweighting.h>

using namespace std;

class ggFWeight : public Weights
{
	public:
		ggFWeight(D3PDReader::Event *tEvent, Int_t tSampleType, TString tDataYear, Double_t tHiggsMass);
		~ggFWeight();

		void setWeight();

	protected:

	private:
		void initializeReweight();

		ggFReweighting *m_reweight;
		Double_t m_higgsMass;

};
