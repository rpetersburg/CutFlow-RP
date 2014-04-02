#include "CutFlow4Lep/Weights/PileupWeight.h"

PileupWeight::PileupWeight(D3PDReader::Event *tEvent, Int_t tDataYear, Root::TPileupReweighting *tPileupReweight) 
	: Weights(tEvent, tDataYear), m_reweight(tPileupReweight)
{
	initializeReweight();
	setWeight();
}

PileupWeight::~PileupWeight() {}

void PileupWeight::initializeReweight()
{
	// No initialization necessary since the reweight is included in the constructor
	return;
}

void PileupWeight::setWeight()
{
	// if (isMC);

	// Finding value mu
	Double_t mu = m_event->eventinfo.averageIntPerXing();
	if (m_dataYear == 2012) mu = (m_event->eventinfo.lbn() == 1 && int(mu+0.5) == 1) ? 0. : mu;

	// Setting weight based on mu
	m_weight = m_reweight->GetCombinedWeight(m_event->eventinfo.RunNumber(), m_event->eventinfo.mc_channel_number(), mu);
}