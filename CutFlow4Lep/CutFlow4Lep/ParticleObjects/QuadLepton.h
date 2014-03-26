#ifndef QUADLEPTON_H
#define QUADLEPTON_H

#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h>

#include "DiLepton.h"

using namespace std;

class QuadLepton
{
	public:
		QuadLepton();
		~QuadLepton();

		DiLepton getDiLepton1() {return diLepton1};
		DiLepton getDiLepton2() {return diLepton2};


	protected:
		DiLepton diLepton1;
		DiLepton diLepton2;


	private:


};

#endif
