#include "CutFlow4Lep/Smear/PhotonSmear.h"

PhotonSmear::PhotonSmear(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tCurrDataCalibration, Int_t tRunNumber_sf)
	: EGammaSmear(tEvent, tCurrMCCollection, tCurrDataCalibration, tRunNumber_sf)
{

}

PhotonSmear::~PhotonSmear()
{

}

void PhotonSmear::executeSmear()
{
	D3PDReader::PhotonD3PDObject *photon = &(m_event->ph);

	for (Int_t i = 0; i < photon->n(); i++)
	{
		D3PDReader::PhotonD3PDObjectElement currPhoton = (*photon)[i];

		// Saving the unsmeared value
		currPhoton.cl_E_unsmeared = currPhoton.cl_E();

		// Setting the necessary enum values (from egammaEnergyCorrectionTool)
		PATCore::ParticleDataType::DataType dataType; 
		if(!m_isMC)
		 	dataType=PATCore::ParticleDataType::Data; 
		else
			dataType=PATCore::ParticleDataType::Full;
		PATCore::ParticleType::Type ptype = PATCore::ParticleType::Photon;
		egEnergyCorr::Resolution::resolutionType resType = egEnergyCorr::Resolution::SigmaEff90;
		egEnergyCorr::Scale::Variation scaleVar = egEnergyCorr::Scale::Nominal;
		egEnergyCorr::Resolution::Variation res = egEnergyCorr::Resolution::Nominal;

		m_smearTool->setRandomSeed(m_eventNumber + 100*i);

		Double_t correctedEnergy = currPhoton.E();
		if (currPhoton.author() == 4 || currPhoton.author() == 16)
		{
			AtlasRoot::egammaEnergyCorrectionTool::ParticleInformation particleInfo = 
				AtlasRoot::egammaEnergyCorrectionTool::ParticleInformation((currPhoton.rawcl_Es0)(),
                                                                   (currPhoton.rawcl_Es1)(),
																																	 (currPhoton.rawcl_Es2)(),
                                                                   (currPhoton.rawcl_Es3)(),
                                                                   (currPhoton.cl_eta)(),
                                                                   (currPhoton.cl_phi)(),
                                                                   (currPhoton.cl_E)(),
                                                                   (currPhoton.cl_etaCalo)(),
                                                                   (currPhoton.cl_phiCalo)(),
                                                                   (currPhoton.ptconv)(),
                                                                   (currPhoton.pt1conv)(),
                                                                   (currPhoton.pt2conv)(),
                                                                   (currPhoton.convtrk1nPixHits)(),
                                                                   (currPhoton.convtrk1nSCTHits)(),
                                                                   (currPhoton.convtrk2nPixHits)(),
                                                                   (currPhoton.convtrk2nSCTHits)(),
                                                                   (currPhoton.Rconv)());
			correctedEnergy = m_smearTool->getCorrectedEnergy(m_runNumber_sf, dataType, particleInfo, scaleVar, res, resType, 1.0);


		}
		// The smearing value for this photon
		Double_t correctionFactor = correctedEnergy / currPhoton.E();

		currPhoton.E() *= correctionFactor;
		currPhoton.cl_E() *= correctionFactor;
		currPhoton.Et() *= correctionFactor;

		m_smear.push_back(correctionFactor);
	}
}