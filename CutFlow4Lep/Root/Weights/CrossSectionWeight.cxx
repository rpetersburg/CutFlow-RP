#include "CutFlow4Lep\Weights\CrossSectionWeight.h"

CrossSectionWeight::CrossSectionWeight(D3PDReader::Event *tEvent, Int_t tDataYear, Double_t tHiggsMass, Double_t tLuminosity, Int_t tSampleType) 
	:	Weights(tEvent, tDataYear), m_sampleType(tSampleType), m_higgsMass(tHiggsMass), m_luminosity(tLuminosity), m_crossSection(1), m_brCorrection(1)
{
	setWeight();
}

CrossSectionWeight::~CrossSectionWeight() {}

void CrossSectionWeight::setWeight()
{
	Int_t runNumber = m_event->eventinfo.mc_channel_number();
	
	CrossSections::LHCEnergy energyLHC;
	if (m_dataYear == 2011) energyLHC = CrossSections::SevenTeV;
	else energyLHC = CrossSections::EightTeV;

	m_brCorrection = m_brRatioObj->get4eBrRatio(m_higgsMass) * 9.;

	switch (m_sampleType)
	{
		case SampleType::ggF:
			m_crossSection = m_higgsCrossSectionObj->higgsprodxsecGGF(m_higgsMass, energyLHC);
		case SampleType::VBF:
			m_crossSection = m_higgsCrossSectionObj->higgsprodxsecVBF(m_higgsMass, energyLHC);
		case SampleType::WH:
			m_crossSection = m_higgsCrossSectionObj->higgsprodxsecWH(m_higgsMass, energyLHC);
		case SampleType::ZH:
			m_crossSection = m_higgsCrossSectionObj->higgsprodxsecZH(m_higgsMass, energyLHC);
		case SampleType::ttH:
			m_crossSection = m_higgsCrossSectionObj->higgsprodxsecttH(m_higgsMass, energyLHC);
		case SampleType::qqF:
			m_crossSection = m_higgsCrossSectionObj->higgsprodxsecGGF(m_higgsMass, energyLHC);
		case SampleType::Background:
			switch (energyLHC)
			{
				case CrossSections::SevenTeV: m_crossSection = CrossSections::GetBkgCrossSection7TeV(runNumber);
				case CrossSections::EightTeV: m_crossSection = CrossSections::GetBkgCrossSection8TeV(runNumber);
			}
	}

	m_weight = m_crossSection * m_brCorrection * m_luminosity;
}