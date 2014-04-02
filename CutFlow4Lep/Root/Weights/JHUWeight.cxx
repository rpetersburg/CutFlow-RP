#include "CutFlow4Lep\Weights\JHUWeight.h"

JHUWeight::JHUWeight(D3PDReader::Event *tEvent, Int_t tDataYear) 
	:	Weights(tEvent, tDataYear)
{
	initializeReweight();
	setWeight();
}

JHUWeight::~JHUWeight() {}

void JHUWeight::initializeReweight()
{
	if (m_dataYear == 2011)
		m_reweight = new JHUPtReweighting("mc11", "../../../JHUReweighting/share/");
	else if (m_dataYear == 2012)
		m_reweight = new JHUPtReweighting("mc12", "../../../JHUReweighting/share/");
	else
		cout << "JHUWeight::initializeReweight(): Data Year not recognized" << endl;
}

void JHUWeight::setWeight()
{
	// if (isMC);

	// Determining True Higgs transverse momentum
	Double_t trueHiggsPt = 0;
	for (Int_t i = 0; i < m_event->mc.n(); i++)
	{
		Int_t pdgID = TMath::Abs(m_event->mc[i].pdgId());
		Int_t status = m_event->mc[i].status();

		if ( (pdgID == 25 || pdgID == 39) && (status == 1 || status == 10902 || status == 62) )
			trueHiggsPt = m_event->mc[i].pt();
	}

	m_weight = m_reweight->GetJHUWeight(m_event->eventinfo.mc_channel_number(), trueHiggsPt/1000, 0);
}