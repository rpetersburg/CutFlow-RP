#ifndef QUADLEPTON_H
#define QUADLEPTON_H

#include <TLorentzVector.h>
#include "CutFlow4Lep/ParticleObjects/ParticleObject.h"
#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"
#include "CutFlow4Lep/ParticleObjects/DiLepton.h"
#include "CutFlow4Lep/ParticleObjects/Jets.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

#include "PATCore/PATCoreEnums.h"

#include "egammaFourMomentumError/GeneralUtils.h"

using namespace std;
class QuadLepton : public ParticleObject
{
	public:
		QuadLepton(DiLepton *tZ1, DiLepton *tZ2);
		~QuadLepton();

		void setZBosons(DiLepton *tZ1, DiLepton *tZ2);

		void setMass(Double_t tMass) { m_mass = tMass; };
		void setMassID(Double_t tMassID) { m_massID = tMassID; };
		void setMassMS(Double_t tMassMS) { m_massMS = tMassMS; };
		void setMassErr(Double_t tMassErr) { m_massErr = tMassErr; };
		void setMassIDErr(Double_t tMassIDErr) { m_massIDErr = tMassIDErr; };
		void setMassMSErr(Double_t tMassMSErr) { m_massMSErr = tMassMSErr; };
		void setMassUnconstrainedSum(TLorentzVector tSum) { m_massUnconstrainedSum = tSum; };
		TLorentzVector getMassUnconstrainedSum() { return m_massUnconstrainedSum; };

		// From FSR Correction
		void setFSRType(Int_t tFSRType) { m_fsrType = tFSRType; };
		void setFSRMass(Double_t tFSRMass) { m_fsrMass = tFSRMass; };
		void setFSRMassError(Double_t tFSRMassError) { m_fsrMassError = tFSRMassError; };
		void setFSRZ1Mass(Double_t tFSRZ1Mass) { m_fsrZ1Mass = tFSRZ1Mass; };
		void setFSRZ2Mass(Double_t tFSRZ2Mass) { m_fsrZ2Mass = tFSRZ2Mass; };
		void setFSRSum(TLorentzVector tFSRSum) { m_fsrSum = tFSRSum; };

		TLorentzVector getFSRMomentum() { return m_fsrMomentum; };
		TLorentzVector getFSRSum() { return m_fsrSum; };

		// From Z Mass Constraint Correction
		void setZMassLorentzVec(vector<TLorentzVector> tZMassLorentzVec) { m_zMassLorentzVec = tZMassLorentzVec; };
		void setZMassCovMatrixVec(vector<TMatrixD> tZMassCovMatrixVec) { m_zMassCovMatrixVec = tZMassCovMatrixVec; };
		void setZMass(Double_t tZMass, Int_t muonType);
		void setZMassErr(Double_t tZMassErr, Int_t muonType);
		void setZ1Mass(Double_t tZ1Mass, Int_t muonType);
		void setZ2Mass(Double_t tZ2Mass, Int_t muonType);
		void setZMassConstrainedSum(TLorentzVector tSum) { m_zMassConstrainedSum = tSum; };

		vector<TLorentzVector> getZMassLorentzVec() { return m_zMassLorentzVec; };
		TLorentzVector getZMassConstrainedSum() { return m_zMassConstrainedSum; };

		void setElRescale(AtlasRoot::egammaEnergyCorrectionTool *telRescale);

		void fillFSRCorrection(TLorentzVector fsrMomentum, Bool_t isZ1, Bool_t isZ2, PATCore::ParticleType::Type particleType);
		void fillMasses();
		void fillCovMatrix();
		void fillMassErrors();
		void fillProductionChannel(vector<Jets*> jetsVec, vector<Muon*> muonVec, vector<Electron*> electronVec);

		DiLepton* getZ1() {return m_z1;};
		DiLepton* getZ2() {return m_z2;};
		vector<ChargedLepton*> getLeptons() { return m_leptonVec; };

		vector<TLorentzVector> getLeptonLorentzVec() { return m_leptonLorentzVec; };
		vector<TLorentzVector> getLeptonLorentzMSVec() { return m_leptonLorentzMSVec; };
		vector<TLorentzVector> getLeptonLorentzIDVec() { return m_leptonLorentzIDVec; };

		vector<TMatrixD> getCovMatrixVec() { return m_covMatrixVec; };
		vector<TMatrixD> getCovMatrixIDVec() { return m_covMatrixIDVec; };
		vector<TMatrixD> getCovMatrixMSVec() { return m_covMatrixMSVec; };

		vector<CLHEP::HepMatrix> getCovMatrixHepVec() { return m_covMatrixHepVec; };
		vector<CLHEP::HepMatrix> getCovMatrixIDHepVec() { return m_covMatrixIDHepVec; };
		vector<CLHEP::HepMatrix> getCovMatrixMSHepVec() { return m_covMatrixMSHepVec; };

		vector<TLorentzVector> getFSRLorentzVec() { return m_fsrLorentzVec; };
		vector<TLorentzVector> getFSRLorentzIDVec() { return m_fsrLorentzIDVec; };
		vector<TLorentzVector> getFSRLorentzMSVec() { return m_fsrLorentzMSVec; };

		vector<TMatrixD> getFSRCovMatrixVec() { return m_fsrCovMatrixVec; };
		vector<TMatrixD> getFSRCovMatrixIDVec() { return m_fsrCovMatrixIDVec; };
		vector<TMatrixD> getFSRCovMatrixMSVec() { return m_fsrCovMatrixMSVec; };

		vector<CLHEP::HepMatrix> getFSRCovMatrixHepVec() { return m_fsrCovMatrixHepVec; };
		vector<CLHEP::HepMatrix> getFSRCovMatrixIDHepVec() { return m_fsrCovMatrixIDHepVec; };
		vector<CLHEP::HepMatrix> getFSRCovMatrixMSHepVec() { return m_fsrCovMatrixMSHepVec; };

		Int_t getType() { return m_type; };
		Int_t getDataYear() { return m_z1->getDataYear(); };
		
		void clearTrackIsoVecs();
		void trackIsoPushBack(Bool_t boolean) { m_trackIso.push_back(boolean); };
		void trackIsoValPushBack(Double_t value) { m_trackIsoVal.push_back(value); };

		void clearCaloIsoVecs();
		void caloIsoPushBack(Bool_t boolean) { m_caloIso.push_back(boolean); };
		void caloIsoValPushBack(Double_t value) { m_caloIsoVal.push_back(value); };

		void clearD0SigVecs();
		void d0SigPushBack(Bool_t boolean) { m_d0Sig.push_back(boolean); };
		void d0SigValPushBack(Double_t value) { m_d0SigVal.push_back(value); };

	protected:

	private:
		Int_t m_type;

		Double_t m_mass;		
		Double_t m_massID;
		Double_t m_massMS;
		Double_t m_massErr;
		Double_t m_massIDErr;
		Double_t m_massMSErr;
		TLorentzVector m_massUnconstrainedSum;

		// From FSR Correction
		TLorentzVector m_fsrMomentum;
		TLorentzVector m_fsrSum;
		Int_t m_fsrType;
		Double_t m_fsrMass;
		Double_t m_fsrMassError;
		Double_t m_fsrZ1Mass;
		Double_t m_fsrZ2Mass;

		// From Z Mass Constraint Correction
		vector<TLorentzVector> m_zMassLorentzVec;
		vector<TMatrixD> m_zMassCovMatrixVec;
		Double_t m_zMass;
		Double_t m_zMassID;
		Double_t m_zMassMS;
		Double_t m_zMassErr;
		Double_t m_zMassErrID;
		Double_t m_zMassErrMS;
		Double_t m_z1Mass;
		Double_t m_z2Mass;
		TLorentzVector m_zMassConstrainedSum;

		DiLepton *m_z1;
		DiLepton *m_z2;

		vector<ChargedLepton*> m_leptonVec;

		vector<TLorentzVector> m_leptonLorentzVec;
		vector<TLorentzVector> m_leptonLorentzMSVec;
		vector<TLorentzVector> m_leptonLorentzIDVec;

		vector<TMatrixD> m_covMatrixVec;
		vector<TMatrixD> m_covMatrixMSVec;
		vector<TMatrixD> m_covMatrixIDVec;

		vector<CLHEP::HepMatrix> m_covMatrixHepVec;
		vector<CLHEP::HepMatrix> m_covMatrixMSHepVec;
		vector<CLHEP::HepMatrix> m_covMatrixIDHepVec;

		vector<TLorentzVector> m_fsrLorentzVec;
		vector<TLorentzVector> m_fsrLorentzMSVec;
		vector<TLorentzVector> m_fsrLorentzIDVec;

		vector<TMatrixD> m_fsrCovMatrixVec;
		vector<TMatrixD> m_fsrCovMatrixIDVec;
		vector<TMatrixD> m_fsrCovMatrixMSVec;

		vector<CLHEP::HepMatrix> m_fsrCovMatrixHepVec;
		vector<CLHEP::HepMatrix> m_fsrCovMatrixIDHepVec;
		vector<CLHEP::HepMatrix> m_fsrCovMatrixMSHepVec;

		vector<Bool_t> m_trackIso;
		vector<Double_t> m_trackIsoVal;

		vector<Bool_t > m_caloIso;
		vector<Double_t> m_caloIsoVal;

		vector<Bool_t> m_d0Sig;
		vector<Double_t> m_d0SigVal;

		Jets *m_leadingJet;
		Jets *m_subleadingJet;
		Jets *m_thirdJet;
		Int_t m_numJets;

		Float_t m_dijet_invmass;
		Float_t m_dijet_deltaeta;
		Float_t m_leading_jet_pt;
		Float_t m_leading_jet_eta;
		Float_t m_subleading_jet_pt;

		Float_t m_BDT_discriminant_VBF;
		Float_t m_BDT_discriminant_HadVH;

		Int_t m_productionChannel;
};
#endif
