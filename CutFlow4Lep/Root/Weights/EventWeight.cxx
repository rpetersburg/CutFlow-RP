#include "CutFlow4Lep/Weights/EventWeight.h"

EventWeight::EventWeight(D3PDReader::Event *tEvent, Double_t tHiggsMass,
												 Int_t tSampleType, Root::TPileupReweighting *tPileupReweight)
	:	Weights(tEvent)
{
	m_ggFWeight = (new ggFWeight(m_event, tHiggsMass, tSampleType))->getWeight();
	m_jhuWeight = (new JHUWeight(m_event))->getWeight();
	m_pileupWeight = (new PileupWeight(m_event, tPileupReweight))->getWeight();
	m_zVertexWeight = (new ZVertexWeight(m_event))->getWeight();

	setWeight();
}

EventWeight::~EventWeight() {}

void EventWeight::setWeight()
{
	m_weight *= m_event->eventinfo.mc_event_weight() * m_zVertexWeight * m_pileupWeight * m_ggFWeight * m_jhuWeight;
}