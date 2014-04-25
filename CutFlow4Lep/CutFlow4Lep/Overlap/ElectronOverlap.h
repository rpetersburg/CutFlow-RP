#ifndef ELECTRONOVERLAP_H
#define ELECTRONOVERLAP_H

#include "CutFlow4Lep/Overlap/Overlap.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"

#include "D3PDReader/ElectronD3PDObject.h"

#include <TMath.h>

class ElectronOverlap : public Overlap
{
	public:
		ElectronOverlap(vector<Electron*> *tInitElectronVec);
		~ElectronOverlap();

		void removeOverlap();
		void setInitElectronVec(vector<Electron*> *tInitElectronVec) {m_initElectronVec = tInitElectronVec;};
		vector<Electron*> getGoodElectronVec() {return m_goodElectronVec;};

	protected:

	private:
		Bool_t overlapsElectron(Electron *currElectronObj);
		Bool_t overlapsCluster(Electron *currElectronObj);

		Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2);

		vector<Electron*> *m_initElectronVec;
		vector<Electron*> m_goodElectronVec;
};
#endif