#ifndef ELECTRONOVERLAP_H
#define ELECTRONOVERLAP_H

#include "CutFlow4Lep/Overlap/Overlap.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

#include "D3PDReader/ElectronD3PDObject.h"

#include <TMath.h>

class ElectronOverlap : public Overlap<Electron>
{
	public:
		ElectronOverlap(vector<Electron*> *tInitElectronVec);
		~ElectronOverlap();

		void removeOverlap();

	protected:

	private:
		Bool_t overlapsElectron(Electron *currElectronObj);
		Bool_t overlapsCluster(Electron *currElectronObj);

		Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2);
};
#endif