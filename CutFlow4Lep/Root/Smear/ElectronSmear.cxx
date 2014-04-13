#include "CutFlow4Lep/Smear/ElectronSmear.h"

ElectronSmear::ElectronSmear(D3PDReader::Event *tEvent, Int_t tCurrMCCollection,
														 Int_t tCurrDataCalibration, Int_t tRunNumber_sf)
	: Smear(tEvent), m_currMCCollection(tCurrMCCollection),
	  m_currDataCalibration(tCurrDataCalibration), m_runNumber_sf(tRunNumber_sf)
{
	initializeSmearObj();
	initializeEfficiencyTool();
	initializeMomentumErrorTool();
}

ElectronSmear::~ElectronSmear()
{

}

// Needs further work to separate and clean
void ElectronSmear::executeSmear()
{
	D3PDReader::ElectronD3PDObject *electron;
	if (m_dataYear == 2011) electron = &(m_event->el_GSF);
	else if (m_dataYear == 2012) electron = &(m_event->el);

	// Executes smearing for each system variable
	for (Int_t sysVar; sysVar <= DoSys::Nom; sysVar++)
	{

		for (Int_t i = 0; i < electron->n(); i++)
		{
			D3PDReader::ElectronD3PDObjectElement currElectron = (*electron)[i];

			// Initial smearing using energy correction tool
			Double_t eta = currElectron.cl_eta();
			Double_t trackEta = currElectron.tracketa();
			Double_t E = currElectron.E();
			Double_t clpT = currElectron.cl_pt();
			Double_t ET = E / cosh(trackEta);

			Double_t correctionFactor = 1.;

			// Saving the unsmeared energy
			currElectron.cl_E_unsmeared = E;

			TLorentzVector trackSysLorentzVec;

			// Setting the necessary enum values (from egammaEnergyCorrectionTool)
			PATCore::ParticleDataType::DataType dataType; 
			if(!m_isMC)
		 		dataType=PATCore::ParticleDataType::Data; 
			else
				dataType=PATCore::ParticleDataType::Full;

			PATCore::ParticleType::Type ptype = PATCore::ParticleType::Electron;
			egEnergyCorr::Resolution::resolutionType resType = egEnergyCorr::Resolution::SigmaEff90;

			egEnergyCorr::Scale::Variation scaleVar = egEnergyCorr::Scale::Nominal;
			egEnergyCorr::Resolution::Variation res = egEnergyCorr::Resolution::Nominal;
			switch (sysVar)
			{
				case DoSys::ZeeStatUp:					scaleVar = egEnergyCorr::Scale::ZeeStatUp; break;
				case DoSys::ZeeStatDown:				scaleVar = egEnergyCorr::Scale::ZeeStatDown; break;
				case DoSys::ZeeSystUp:					scaleVar = egEnergyCorr::Scale::ZeeSystUp; break;
				case DoSys::ZeeSystDown:				scaleVar = egEnergyCorr::Scale::ZeeSystDown; break;
				case DoSys::ZeeAllUp:						scaleVar = egEnergyCorr::Scale::ZeeAllUp; break;
				case DoSys::ZeeAllDown:					scaleVar = egEnergyCorr::Scale::ZeeAllDown; break;
				case DoSys::PSUp:								scaleVar = egEnergyCorr::Scale::PSUp; break;
				case DoSys::PSDown:							scaleVar = egEnergyCorr::Scale::PSDown; break;
				case DoSys::S12Up:							scaleVar = egEnergyCorr::Scale::S12Up; break;
				case DoSys::S12Down:						scaleVar = egEnergyCorr::Scale::S12Down; break;
				case DoSys::MatIDUp:						scaleVar = egEnergyCorr::Scale::MatIDUp; break;
				case DoSys::MatIDDown:					scaleVar = egEnergyCorr::Scale::MatIDDown; break;
				case DoSys::MatCryoUp:					scaleVar = egEnergyCorr::Scale::MatCryoUp; break;
				case DoSys::MatCryoDown:				scaleVar = egEnergyCorr::Scale::MatCryoDown; break;
				case DoSys::MatCaloUp:					scaleVar = egEnergyCorr::Scale::MatCaloUp; break;
				case DoSys::MatCaloDown:				scaleVar = egEnergyCorr::Scale::MatCaloDown; break;
				case DoSys::LArCalibUp:					scaleVar = egEnergyCorr::Scale::LArCalibUp; break;
				case DoSys::LArCalibDown:				scaleVar = egEnergyCorr::Scale::LArCalibDown; break;
				case DoSys::LArUnconvCalibUp:		scaleVar = egEnergyCorr::Scale::LArUnconvCalibUp; break;
				case DoSys::LArUnconvCalibDown:	scaleVar = egEnergyCorr::Scale::LArUnconvCalibDown; break;
				case DoSys::LArElecUnconvUp:		scaleVar = egEnergyCorr::Scale::LArElecUnconvUp; break;
				case DoSys::LArElecUnconvDown:	scaleVar = egEnergyCorr::Scale::LArElecUnconvDown; break;
				case DoSys::LArElecCalibUp:			scaleVar = egEnergyCorr::Scale::LArElecCalibUp; break;
				case DoSys::LArElecCalibDown:		scaleVar = egEnergyCorr::Scale::LArElecCalibDown; break;
				case DoSys::GainUp:							scaleVar = egEnergyCorr::Scale::GainUp; break;
				case DoSys::GainDown:						scaleVar = egEnergyCorr::Scale::GainDown; break;
				case DoSys::G4Up:								scaleVar = egEnergyCorr::Scale::G4Up; break;
				case DoSys::G4Down:							scaleVar = egEnergyCorr::Scale::G4Down; break;
				case DoSys::MomentumUp:					scaleVar = egEnergyCorr::Scale::MomentumUp; break;
				case DoSys::MomentumDown:				scaleVar = egEnergyCorr::Scale::MomentumDown; break;
				case DoSys::ZSmearingUp:				res = egEnergyCorr::Resolution::ZSmearingUp; break;
				case DoSys::ZSmearingDown:			res = egEnergyCorr::Resolution::ZSmearingDown; break;
				case DoSys::SamplingTermUp:			res = egEnergyCorr::Resolution::SamplingTermUp; break;
				case DoSys::SamplingTermDown:		res = egEnergyCorr::Resolution::SamplingTermDown; break;
				case DoSys::MaterialIDUp:				res = egEnergyCorr::Resolution::MaterialIDUp; break;
				case DoSys::MaterialIDDown:			res = egEnergyCorr::Resolution::MaterialIDDown; break;
				case DoSys::MaterialCaloUp:			res = egEnergyCorr::Resolution::MaterialCaloUp; break;
				case DoSys::MaterialCaloDown:		res = egEnergyCorr::Resolution::MaterialCaloDown; break;
				case DoSys::MaterialGapUp:			res = egEnergyCorr::Resolution::MaterialGapUp; break;
				case DoSys::MaterialGapDown:		res = egEnergyCorr::Resolution::MaterialGapDown; break;
				case DoSys::MaterialCryoUp:			res = egEnergyCorr::Resolution::MaterialCryoUp; break;
				case DoSys::MaterialCryoDown:		res = egEnergyCorr::Resolution::MaterialCryoDown; break;
				case DoSys::PileUpUp:						res = egEnergyCorr::Resolution::PileUpUp; break;
				case DoSys::PileUpDown:					res = egEnergyCorr::Resolution::PileUpDown; break;
			}
			m_smearTool->setRandomSeed(m_eventNumber + 100 * i);

			double correctedE = E;
			if (currElectron.author() == 1 || currElectron.author() == 3)
			{
				AtlasRoot::egammaEnergyCorrectionTool::ParticleInformation particleInfo = 
					AtlasRoot::egammaEnergyCorrectionTool::ParticleInformation((currElectron.rawcl_Es0)(),
																																		 (currElectron.rawcl_Es1)(),
																																		 (currElectron.rawcl_Es2)(),
																																		 (currElectron.rawcl_Es3)(),
																																		 eta,
																																		 (currElectron.cl_phi)(),
																																		 trackEta,
																																		 (currElectron.cl_E)(),
																																		 (currElectron.cl_etaCalo)(),
																																		 (currElectron.cl_phiCalo)());
				correctedE = m_smearTool->getCorrectedEnergy(m_runNumber_sf, dataType, particleInfo, scaleVar, res, resType, 1.0);
				if (sysVar == DoSys::MomentumUp || sysVar == DoSys::MomentumDown)
				{
					m_smearTool->setRandomSeed(m_eventNumber + 100 * i);
					trackSysLorentzVec.SetPtEtaPhiE(currElectron.trackpt(), currElectron.tracketa(), currElectron.trackphi(), fabs(1./currElectron.trackqoverp()));
					double momentum = m_smearTool->getCorrectedMomentum(dataType, ptype, trackSysLorentzVec.P(), trackSysLorentzVec.Eta(), scaleVar);
          trackSysLorentzVec.SetPtEtaPhiM(momentum/cosh(trackSysLorentzVec.Eta()), trackSysLorentzVec.Eta(), trackSysLorentzVec.Phi(), pdgElMass);
				}
			}
			Double_t correctedET = correctedE / cosh(trackEta);
			Double_t ratio = correctedET / ET;
			correctionFactor *= ratio;
			E = correctedE;
			ET = correctedET;
			clpT *= ratio;

			// Storing cluster pT for later work
			m_bfEP_cl_pT.push_back(clpT);
			m_bfEP_cl_ET.push_back(ET);
			m_momentumErrorVec.push_back(E * m_smearTool->resolution(E, currElectron.cl_eta(), PATCore::ParticleType::Electron, true));

			currElectron.bfEP_pT = ET;

			// Four momentum error correction
			Double_t correctedE = E;
			if (currElectron.author() == 1 || currElectron.author() == 3)
			{
				TLorentzVector trackLorentzVec, clusterLorentzVec, combinedLorentzVec;
				double combinedEnergyError(0.);

				if (sysVar == DoSys::MomentumUp || sysVar == DoSys::MomentumDown)
					trackLorentzVec = trackSysLorentzVec;
				else trackLorentzVec.SetPtEtaPhiE(currElectron.trackpt(), currElectron.tracketa(), currElectron.trackphi(), fabs(1./currElectron.trackqoverp()));

				clusterLorentzVec.SetPtEtaPhiE(E/cosh(eta), currElectron.cl_eta(), currElectron.cl_phi(), E);

				Double_t el_qoverp_LM = currElectron.trackqoverp();
				for (Int_t j = 0; j < currElectron.refittedTrack_LMqoverp().size(); ++j)
				{
					if (currElectron.refittedTrack_author().at(j) == 4)
						el_qoverp_LM = currElectron.refittedTrack_LMqoverp().at(j);
				}

				Double_t cl_error = E * m_smearTool->resolution(E, currElectron.cl_eta(), PATCore::ParticleType::Electron, true);
				Bool_t momentumCorrectionDone = m_momentumErrorTool->buildfourmom(trackLorentzVec, clusterLorentzVec, el_qoverp_LM, currElectron.trackcov_qoverp(), currElectron.charge(), cl_error, combinedLorentzVec, combinedEnergyError);

				m_momentumErrorVec[i] = combinedEnergyError;
				correctedE = combinedLorentzVec.E();
			}

			Double_t correctedET = correctedE / cosh(trackEta);
			Double_t ratio = correctedET / ET;
			correctionFactor *= ratio;
			E = correctedE;
			ET = correctedET;
			clpT *= ratio;


			// ID and Reco Efficiency
			if (m_isMC)
			{
				Double_t electronEff = 1;
				Double_t clET = m_bfEP_cl_ET[i];
				if (fabs(eta) <= 2.47 && clET > 7*1000)
				{
					const Root::TResult &sf_ID = m_idEffTool->calculate(PATCore::ParticleDataType::Full, m_runNumber_sf, eta, clET);
					const Root::TResult &sf_Reco = m_recoEffTool->calculate(PATCore::ParticleDataType::Full, m_runNumber_sf, eta, clET);

					electronEff = sf_Reco.getScaleFactor() * sf_ID.getScaleFactor();
				}
				m_electronEff.push_back(electronEff);
			}
			// Setting the actual smear values
			m_smear.push_back(correctionFactor);
		}
		// Need to delete all except last versions which will be used in other places...
		if (sysVar != DoSys::Nom)
		{
			m_bfEP_cl_ET.clear();
			m_bfEP_cl_pT.clear();
			m_momentumErrorVec.clear();
		}
	}
}

void ElectronSmear::initializeSmearObj()
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

void ElectronSmear::initializeEfficiencyTool()
{
	m_idEffTool = new Root::TElectronEfficiencyCorrectionTool();
	m_recoEffTool = new Root::TElectronEfficiencyCorrectionTool();

	if (m_dataYear == 2011)
	{
		if (false) //if (useMoriond)
			m_recoEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.RecoTrk.2011.7TeV.rel17p0.v01.root");
		else
			m_recoEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.RecoTrk.2011.7TeV.rel17p0.v03.root");

		if (m_electronCollection == ElectronCollection::LoosePlusPlus) // True when m_dataYear == 2011......
		{
			if (false) //if (useMoriond)
				m_idEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.Loose.2011.7TeV.rel17p0.v01.root");
			else
				m_idEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.Loose.2011.7TeV.rel17p0.v03.root");
		}
	}
	else if (m_dataYear == 2012)
	{
		if (m_electronCollection == ElectronCollection::MultiLepton) // if (useMoriond)
		{
			m_idEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.Multilepton.2012.8TeV.rel17p2.v02.root");
			m_recoEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.v02.root");
		}
		else if (m_electronCollection == ElectronCollection::Likelihood) // if (!useMoriond)
		{
			if (m_currMCCollection == MCCollection::MC12a || 
					m_currMCCollection == MCCollection::MC12b ||
					m_currDataCalibration == DataCalibType::y2012ab)
			{
				m_idEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.LooseLLH.2012.8TeV.rel17p2.v07.root");
				m_recoEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.GEO20.v08.root");
			}
			else if (m_currMCCollection == MCCollection::MC12c ||
							 m_currDataCalibration == DataCalibType::y2012c)
			{
				m_idEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.LooseLLH.2012.8TeV.rel17p2.GEO21.v01.root");
				m_recoEffTool->addFileName("../../../ElectronEfficiencyCorrection/data/efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.GEO21.v01.root");
			}
		}
	}

	if (!m_idEffTool->initialize())
		cout << "Egamma ID SF initialization failed ..." << endl;
	if (!m_recoEffTool->initialize())
		cout << "Egamma Reco SF initialization failed ..." << endl;
}

void ElectronSmear::initializeMomentumErrorTool()
{
	if (m_dataYear == 2012)
	{
		if (m_currMCCollection == MCCollection::MC11c ||
				m_currDataCalibration == DataCalibType::y2011c)
			m_momentumErrorTool = new egammaFourMomentumError("2011,likelihood,MC11c");
		else if (m_currMCCollection == MCCollection::MC11d ||
						 m_currDataCalibration == DataCalibType::y2011d)
			m_momentumErrorTool = new egammaFourMomentumError("2011,likelihood,MC11d");
	}
	else if (m_dataYear == 2012)
	{
		if (m_currMCCollection == MCCollection::MC12a ||
				m_currMCCollection == MCCollection::MC12b ||
				m_currDataCalibration == DataCalibType::y2012ab)
			m_momentumErrorTool = new egammaFourMomentumError("2012,likelihood,MC12a");
		else if (m_currMCCollection == MCCollection::MC12c ||
						 m_currDataCalibration == DataCalibType::y2012c)
			m_momentumErrorTool = new egammaFourMomentumError("2012,likelihood,MC12c");
	}
}