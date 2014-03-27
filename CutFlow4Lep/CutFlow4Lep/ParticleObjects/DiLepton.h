#ifndef DILEPTON_H
#define DILEPTON_H

#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h>

#include "ChargedLepton.h"

using namespace std;

class DiLepton
{
	public:
		DiLepton();
		~DiLepton();

		ChargedLepton getLepton1();
		ChargedLepton getLepton2();


	protected:
		ChargedLepton lepton1;
		ChargedLepton lepton2;

	private:


};

#endif