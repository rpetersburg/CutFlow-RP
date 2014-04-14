#include "CutFlow4Lep/Smear/EGammaSmear.h"

EGammaSmear::EGammaSmear(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tCurrDataCalibration, Int_t tRunNumber_sf) 
	: Smear(tEvent), m_currMCCollection(tCurrMCCollection), m_currDataCalibration(tCurrDataCalibration), m_runNumber_sf(tRunNumber_sf)
{
	initializeSmearObj();
}

EGammaSmear::~EGammaSmear()
{

}

void EGammaSmear::initializeSmearObj()
{
	m_smearTool = new AtlasRoot::egammaEnergyCorrectionTool();

	if (m_dataYear == 2011)
	{
		m_smearTool->setFileName("../../../ElectronPhotonFourMomentumCorrection/data/egammaEnergyCorrectionData.root");

		if (m_currMCCollection == MCCollection::MC11c ||
				m_currDataCalibration == DataCalibType::y2011c)
			m_smearTool->setESModel(egEnergyCorr::es2011c);
		else if (m_currMCCollection == MCCollection::MC11d ||
						 m_currDataCalibration == DataCalibType::y2011d)
			m_smearTool->setESModel(egEnergyCorr::es2011d);
	}
	else if (m_dataYear == 2012)
	{
		m_smearTool->setFileName("../../../ElectronPhotonFourMomentumCorrection/data/egammaEnergyCorrectionData.root");

		if (m_currMCCollection == MCCollection::MC12a ||
				m_currMCCollection == MCCollection::MC12b)
			m_smearTool->setESModel(egEnergyCorr::es2012a);
		else if (m_currMCCollection == MCCollection::MC12c)
			m_smearTool->setESModel(egEnergyCorr::es2012c);
		else
		{
			if (m_currDataCalibration == DataCalibType::y2012ab)
				m_smearTool->setESModel(egEnergyCorr::es2012a);
			else if (m_currDataCalibration == DataCalibType::y2012c)
				m_smearTool->setESModel(egEnergyCorr::es2012c);
			m_smearTool->useIntermoduleCorrection(false);
			m_smearTool->usePhiUniformCorrection(false);
			m_smearTool->useGainCorrection(false);
		}
	}
	m_smearTool->initialize();
}