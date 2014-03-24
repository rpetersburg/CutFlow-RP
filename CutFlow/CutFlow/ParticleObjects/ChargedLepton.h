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
		Double_t lepEff;
		Double_t lepMass;
		Double_t smearVar;


	private:


}

#endif
