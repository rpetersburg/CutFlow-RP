#ifndef QUADLEPTON_H
#define QUADLEPTON_H

#include <TLorentzVector.h>
#include "CutFlow4Lep/ParticleObjects/ParticleObject.h"
#include "CutFlow4Lep/ParticleObjects/ChargedLepton.h"
#include "CutFlow4Lep/ParticleObjects/DiLepton.h"

#include "PATCore/PATCoreEnums.h"

using namespace std;
class QuadLepton : public ParticleObject
{
	public:
		QuadLepton(DiLepton *tZ1, DiLepton *tZ2);
		~QuadLepton();

		void setZBosons(DiLepton *tZ1, DiLepton *tZ2);
		void setMass(Double_t tMass) {m_mass = tMass;};
		void setFSRType(Int_t tFSRType) {m_fsrType = tFSRType;};
		void setElRescale(AtlasRoot::egammaEnergyCorrectionTool *telRescale);

		void fillFSRCorrection(TLorentzVector fsrMomentum, Bool_t isZ1, Bool_t isZ2, PATCore::ParticleType::Type particleType);

		DiLepton* getZ1() {return m_z1;};
		DiLepton* getZ2() {return m_z2;};
		vector<ChargedLepton*> getLeptons() {return m_leptonVec;};

		vector<TLorentzVector> getLeptonLorentzVec() {return m_leptonLorentzVec;};
		vector<TLorentzVector> getLeptonLorentzMEVec() {return m_leptonLorentzMEVec;};
		vector<TLorentzVector> getLeptonLorentzIDVec() {return m_leptonLorentzIDVec;};

		Int_t getType() {return m_type;};
		Int_t getDataYear() {return m_z1->getDataYear();};

		void clearTrackIsoVecs();
		void trackIsoPushBack(Bool_t boolean) {m_trackIso.push_back(boolean);};
		void trackIsoValPushBack(Double_t value) {m_trackIsoVal.push_back(value);};

		void clearCaloIsoVecs();
		void caloIsoPushBack(Bool_t boolean) {m_caloIso.push_back(boolean);};
		void caloIsoValPushBack(Double_t value) {m_caloIsoVal.push_back(value);};

		void clearD0SigVecs();
		void d0SigPushBack(Bool_t boolean) {m_d0Sig.push_back(boolean);};
		void d0SigValPushBack(Double_t value) {m_d0SigVal.push_back(value);};

	protected:

	private:
		Int_t m_type;
		Double_t m_mass;
		Int_t m_fsrType;

		DiLepton *m_z1;
		DiLepton *m_z2;

		vector<ChargedLepton*> m_leptonVec;
		vector<TLorentzVector> m_leptonLorentzVec;
		vector<TLorentzVector> m_leptonLorentzMEVec;
		vector<TLorentzVector> m_leptonLorentzIDVec;

		vector<TLorentzVector> m_fsrLorentzVec;
		vector<TLorentzVector> m_fsrLorentzMEVec;
		vector<TLorentzVector> m_fsrLorentzIDVec;

		vector<Bool_t> m_trackIso;
		vector<Double_t> m_trackIsoVal;

		vector<Bool_t > m_caloIso;
		vector<Double_t> m_caloIsoVal;

		vector<Bool_t> m_d0Sig;
		vector<Double_t> m_d0SigVal;

};
#endif
