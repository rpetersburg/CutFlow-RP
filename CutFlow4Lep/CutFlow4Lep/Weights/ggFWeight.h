#ifndef GGFWEIGHT_H
#define GGFWEIGHT_H

#include <CutFlow4Lep/Weights/Weights.h>
#include <CutFlow4Lep/Calculations/MCHiggsMass.h>
#include <CutFlow4Lep/StructDef.h>

#include <ggFReweighting/ggFReweighting.h>

using namespace std;

class ggFWeight : public Weights
{
	public:
		ggFWeight(D3PDReader::Event *tEvent, Double_t tHiggsMass, Int_t tSampleType);
		~ggFWeight();

	protected:

	private:
		void initializeReweight();
		void setWeight();

		ggFReweighting *m_reweight;
		Double_t m_higgsMass;
		Int_t m_sampleType;

};
#endif