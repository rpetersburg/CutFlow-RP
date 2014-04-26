#ifndef ELECTRONMUONOVERLAP_H
#define ELECTRONMUONOVERLAP_H

#include "CutFlow4Lep/Overlap/Overlap.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

using namespace std;
class ElectronMuonOverlap : public Overlap<Electron>
{
	public:
		ElectronMuonOverlap(vector<Electron*> *tInitElectronVec, vector<Muon*> *tMuonCaloVec, vector<Muon*> *tMuonStacoVec);
		~ElectronMuonOverlap();

		void removeOverlap();

	protected:

	private:
		Bool_t overlapsMuon(Electron *currElectronObj);

		vector<Muon*> *m_muonCaloVec;
		vector<Muon*> *m_muonStacoVec;
};

#endif