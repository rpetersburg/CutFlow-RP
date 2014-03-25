#ifndef CHARGEDLEPTON_H
#define CHARGEDLEPTON_H

#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h>

using namespace CLHEP;

class ChargedLepton
{
	public:
		ChargedLepton();
		~ChargedLepton();


	protected:
		Int_t flavor;
		Int_t type;
		Int_t charge;
		Int_t index;

		TLorentzVector *m_momentum_main;
		TLorentzVector m_momentum;

		Double_t lepEff;
		Double_t lepMass;
		Double_t smearVar;


	private:


};

#endif
