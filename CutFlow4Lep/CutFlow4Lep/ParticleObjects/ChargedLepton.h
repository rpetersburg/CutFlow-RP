#ifndef CHARGEDLEPTON_H
#define CHARGEDLEPTON_H

#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h>

#include "CutFlow4Lep/UsesEvent.h"

using namespace std;

class ChargedLepton : public UsesEvent
{
	public:
		ChargedLepton(D3PDReader::Event *tEvent) : UsesEvent(tEvent) {};
		~ChargedLepton();

		Double_t getSmear() {return m_smear;};
		Double_t getEff() {return m_eff;};
		void setSmear(Double_t tSmear) {m_smear = tSmear;};
		void setEff(Double_t tEff) {m_eff = tEff;};

		TLorentzVector* getMomentum() {return m_momentum;};
		TLorentzVector* getMomentumMain() {return m_momentumMain;};
		TLorentzVector* getMomentumBDT() {return m_momentumBDT;};
		TLorentzVector* getMomentumME() {return m_momentumME;};
		TLorentzVector* getMomentumID() {return m_momentumID;};

		Int_t getDataYear() {return m_dataYear;};

	protected:
		Int_t m_index;
		Double_t m_mass;	

		Double_t m_smear;
		Double_t m_eff;

		TLorentzVector *m_momentum;
		TLorentzVector *m_momentumMain;
		TLorentzVector *m_momentumBDT;
		TLorentzVector *m_momentumME;
		TLorentzVector *m_momentumID;

		Double_t m_ptCone20Correction;


		Float_t sysE[DoSys::Nom + 1];
		Float_t nomE;

		Int_t m_vhLeptonType;
		Int_t m_truthParentType;
	private:
};

#endif
