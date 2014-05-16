#ifndef CHARGEDLEPTON_H
#define CHARGEDLEPTON_H

#include <TString.h>

#include "CutFlow4Lep/UsesEvent.h"
#include "CutFlow4Lep/ParticleObjects/ParticleObject.h"

using namespace std;

class ChargedLepton : public ParticleObject, public UsesEvent
{
	public:
		ChargedLepton(D3PDReader::Event *tEvent) : UsesEvent(tEvent) {};
		~ChargedLepton();

		Int_t getDataYear() {return m_dataYear;};

		Int_t getCharge() {return m_charge;};
		Int_t getFlavor() {return m_flavor;};

		Double_t getSmear() {return m_smear;};
		Double_t getEff() {return m_eff;};
		void setSmear(Double_t tSmear) {m_smear = tSmear;};
		void setEff(Double_t tEff) {m_eff = tEff;};

		TLorentzVector* getMomentumBDTVec() {return m_momentumBDT;};
		TLorentzVector* getMomentumMEVec() {return m_momentumME;};
		TLorentzVector* getMomentumIDVec() {return m_momentumID;};

	protected:
		Int_t m_index;
		Double_t m_mass;
		Int_t m_charge;
		Int_t m_flavor;

		Double_t m_smear;
		Double_t m_eff;

		TLorentzVector *m_momentumBDT;
		TLorentzVector *m_momentumME;
		TLorentzVector *m_momentumID;

		Double_t m_ptCone20;
		Double_t m_ptCone20Correction;


		Float_t sysE[DoSys::Nom + 1];
		Float_t nomE;

		Int_t m_vhLeptonType;
		Int_t m_truthParentType;
	private:
};

#endif
