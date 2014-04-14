#ifndef CROSSSECTIONWEIGHT_H
#define CROSSSECTIONWEIGHT_H

#include "CutFlow4Lep/Weights/Weights.h"
#include "CutFlow4Lep/Calculations/MCHiggsMass.h"
#include "CutFlow4Lep/StructDef.h"

#include <HiggsZZ4lUtils/HiggsCrossSection.h>
#include <HiggsZZ4lUtils/BkgCrossSection.h>
#include <HiggsZZ4lUtils/H4lBrRatio.h>

using namespace std;

class CrossSectionWeight : public Weights
{
	public:
		CrossSectionWeight(D3PDReader::Event *tEvent, Int_t tDataYear, Double_t tHiggsMass, Double_t tLuminosity, Int_t tSampleType);
		~CrossSectionWeight();

	protected:

	private:
		void setWeight();

		HiggsCrossSection *m_higgsCrossSectionObj;
		H4lBrRatio *m_brRatioObj;

		Int_t m_sampleType;
		Double_t m_higgsMass;
		Double_t m_crossSection;
		Double_t m_brCorrection;
		Double_t m_luminosity;
};
#endif
