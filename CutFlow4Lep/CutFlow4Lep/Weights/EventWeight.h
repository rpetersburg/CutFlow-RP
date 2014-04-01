#include <CutFlow4Lep/Weights/Weights.h>
#include <CutFlow4Lep/Weights/ggFWeight.h>
#include <CutFlow4Lep/Weights/JHUWeight.h>
#include <CutFlow4Lep/Weights/PileupWeight.h>
#include <CutFlow4Lep/Weights/ZVertexWeight.h>

#include <CutFlow4Lep/Calculations/MCHiggsMass.h>
#include <CutFlow4Lep/StructDef.h>

using namespace std;

class EventWeight : public Weights
{
	public:
		EventWeight(D3PDReader::Event *tEvent, Int_t tDataYear, Double_t tHiggsMass,
								Int_t tSampleType, Int_t tMCCollection, Int_t tDataCollection);
		~EventWeight();

		Double_t getggFWeight() {return m_ggFWeight;};
		Double_t getJHUWeight() {return m_jhuWeight;};
		Double_t getPileupWeight() {return m_pileupWeight;};
		Double_t getZVertexWeight() {return m_zVertexWeight;};

	protected:

	private:
		void initializeReweight();
		void setWeight();

		Double_t m_ggFWeight;
		Double_t m_jhuWeight;
		Double_t m_pileupWeight;
		Double_t m_zVertexWeight;

		VertexPositionReweightingTool *m_reweight;
};
