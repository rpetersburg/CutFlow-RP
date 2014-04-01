#include "CutFlow4Lep\Weights\ggFWeight.h"

ggFWeight::ggFWeight(D3PDReader::Event *tEvent, Int_t tDataYear, Double_t tHiggsMass, Int_t tSampleType) 
	: Weights(tEvent, tDataYear), m_higgsMass(tHiggsMass), m_reweight(0), m_sampleType(tSampleType)
{
	initializeReweight();
	setWeight();
}

ggFWeight::~ggFWeight() {}

void ggFWeight::initializeReweight()
{
	// Possible PowHeg masses
	Double_t powhegMass[] = { 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155,
                         160, 165, 170, 175, 180, 185, 190, 195, 200, 210, 220, 240,
                         260, 280, 300, 320, 340, 360, 380, 400, 420, 440, 460, 480,
                         500, 520, 540, 560, 580, 600, 650, 700, 750, 800, 850, 900 };
	// Minimizing the delta mass to obtain approx PowHeg mass
	Double_t minDeltaMass = 9999;
	Double_t tmpDeltaMass = 0;
	Double_t ggFHiggsMass = m_higgsMass;
	for (Int_t i = 0; i < (Int_t) sizeof(powhegMass)/sizeof(powhegMass[0]); i++)
	{
		tmpDeltaMass = powhegMass[i] - m_higgsMass;
		if ( fabs(tmpDeltaMass) < fabs(minDeltaMass) )
		{
			minDeltaMass = tmpDeltaMass;
			ggFHiggsMass = powhegMass[i];
		}
	}
	// Instantiating the reweight object based on the Higgs Mass
	m_reweight = new ggFReweighting("PowHeg", ggFHiggsMass, "Mean", "../../../ggFReweighting/share/", "mc11");
}

void ggFWeight::setWeight()
{
	// if (isMC);

	// Determining True Higgs transverse momentum
	Double_t trueHiggsPt = 0;
	for (Int_t i = 0; i < m_event->mc.n(); i++)
	{
		Int_t pdgID = TMath::Abs(m_event->mc[i].pdgId());
		Int_t status = m_event->mc[i].status();

		if (pdgID == 25 && (status == 2 || status == 10902 || status == 62))
			trueHiggsPt = m_event->mc[i].pt()/1000;
	}
	
	// Using ggF Reweighting to return proper weight
	pair<double, double> result;
	if (m_sampleType == SampleType::ggF || m_sampleType == SampleType::ggF_ZpZp)
		m_weight = m_reweight->getWeightAndStatError(trueHiggsPt).first;
}