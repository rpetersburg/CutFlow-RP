#include "CutFlow4Lep/Corrections/ZMassConstraintCorrection.h"

ZMassConstraintCorrection::ZMassConstraintCorrection(D3PDReader::Event *tEvent, QuadLepton *tQuadLepton)
	: Correction(tEvent), m_quadLepton(tQuadLepton)
{
	initializeCorrectionTool();
}

ZMassConstraintCorrection::~ZMassConstraintCorrection()
{
	delete m_constraintFitTool;
}

void ZMassConstraintCorrection::executeCorrection()
{
	// Vector containers
	vector<TLorentzVector> z1Momentum;
	vector<TLorentzVector> z2Momentum;

	ZMassConstraint::ConstraintFitInput inputZMassConstraint;
	fillInputWithZBoson(inputZMassConstraint, m_quadLepton->getZ1());

	correctZBoson(m_quadLepton->getZ1(), z1Momentum);

	if (m_quadLepton->getMomentumVec()->M() > 190 * 1000)
		correctZBoson(m_quadLepton->getZ2(), z2Momentum);
	else
	{
		// If no Z mass constraints for Z2, add just the normal 4Vector and Cov Matrix 
		m_lepMomentum.push_back(*m_quadLepton->getZ2()->getPosLepton()->getMomentumVec(m_muonType));
		m_lepMomentum.push_back(*m_quadLepton->getZ2()->getNegLepton()->getMomentumVec(m_muonType));
		m_lepError.push_back(m_quadLepton->getZ2()->getPosLepton()->getCovMatrix(m_muonType));
		m_lepError.push_back(m_quadLepton->getZ2()->getNegLepton()->getCovMatrix(m_muonType));
		z2Momentum.push_back(*m_quadLepton->getZ2()->getPosLepton()->getMomentumVec(m_muonType));
		z2Momentum.push_back(*m_quadLepton->getZ2()->getNegLepton()->getMomentumVec(m_muonType));

		// Add the fsr if it is there as well
		if(m_quadLepton->getZ2()->getHasFSR())
		{
			m_lepMomentum.push_back(m_quadLepton->getZ2()->getFSRMomentum());
			m_lepError.push_back(m_quadLepton->getZ2()->getFSRError());
			z2Momentum.push_back(m_quadLepton->getZ2()->getFSRMomentum());
		}
	}

	if (m_muonType == MuonType::CB)
	{
		m_quadLepton->setZMassLorentzVec(m_lepMomentum);
		m_quadLepton->setZMassCovMatrixVec(m_lepError);
	}

	m_quadLepton->setZMass(InvariantMass::invMassCalc(m_lepMomentum), m_muonType);
	m_quadLepton->setZMassErr(MassError::sigmaMassCalc_d0z0PhiThetaP(m_lepMomentum, m_lepError), m_muonType);
	m_quadLepton->setZ1Mass(InvariantMass::invMassCalc(z1Momentum), m_muonType);
	m_quadLepton->setZ2Mass(InvariantMass::invMassCalc(z2Momentum), m_muonType);

	vector<TLorentzVector> zMassLorentzVec = m_quadLepton->getZMassLorentzVec();
	for (Int_t i = 0; i < (Int_t)zMassLorentzVec.size(); i++)
	{
		if (m_muonType == MuonType::CB)
			m_quadLepton->setZMassConstrainedSum(m_quadLepton->getZMassConstrainedSum() + zMassLorentzVec[i]);
	}
}

void ZMassConstraintCorrection::correctZBoson(DiLepton *diLepton, vector<TLorentzVector> zMomentum)
{
	ZMassConstraint::ConstraintFitInput inputZMassConstraint;
	fillInputWithZBoson(inputZMassConstraint, diLepton);

	// Perform the Z Mass constraint on Z1
	m_constraintFitTool->massFitInterface(inputZMassConstraint);
	ZMassConstraint::ConstraintFitOutput massFitResult = m_constraintFitTool->massFitRun(-1.);

	// Storing the Results
	m_lepMomentum.push_back(massFitResult.getConstituentFourVector(0));
	m_lepMomentum.push_back(massFitResult.getConstituentFourVector(1));
	m_lepError.push_back(massFitResult.getConstituentCovarianced0z0PhiThetaP_TMatrix(0));
	m_lepError.push_back(massFitResult.getConstituentCovarianced0z0PhiThetaP_TMatrix(1));

	zMomentum.push_back(massFitResult.getConstituentFourVector(0));
	zMomentum.push_back(massFitResult.getConstituentFourVector(1));

	// Getting the FSR Photon
	if (diLepton->getHasFSR())
	{
		if (massFitResult.getNConstituents() > 2)
		{
			m_lepMomentum.push_back(massFitResult.getConstituentFourVector(2));
			m_lepError.push_back(massFitResult.getConstituentCovarianced0z0PhiThetaP_TMatrix(2));

			zMomentum.push_back(massFitResult.getConstituentFourVector(2));
		}
		else
		{
			cout << "Error: ZMassConstraintCorrection::correctZBoson(): Z FSR Event but no Photon correction from ZMass Constraints" << endl;
			cout << "Has FSR: " << diLepton->getHasFSR() << " n ZMass vector: " << massFitResult.getNConstituents() << endl;
		}
	}
}

void ZMassConstraintCorrection::initializeCorrectionTool()
{
	Bool_t hasWidth = true;
	m_constraintFitTool = new ZMassConstraint::ConstraintFit(pdgZMass, hasWidth, 2.4952e3);
}

void ZMassConstraintCorrection::fillInputWithZBoson(ZMassConstraint::ConstraintFitInput input, DiLepton *diLepton)
{
	input.addConstituent_FourVector_d0z0PhiThetaP(*diLepton->getPosLepton()->getMomentumVec(m_muonType),
																								diLepton->getPosLepton()->getCovMatrixHep(m_muonType));
	input.addConstituent_FourVector_d0z0PhiThetaP(*diLepton->getNegLepton()->getMomentumVec(m_muonType),
																								diLepton->getNegLepton()->getCovMatrixHep(m_muonType));

	if (m_quadLepton->getZ1()->getHasFSR())
		input.addConstituent_FourVector_d0z0PhiThetaP(diLepton->getFSRMomentum(),
																									diLepton->getHepFSRError());
}