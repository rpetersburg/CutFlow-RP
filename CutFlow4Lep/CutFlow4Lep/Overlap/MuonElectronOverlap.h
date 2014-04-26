#ifndef MUONELECTRONOVERLAP_H
#define MUONELECTRONOVERLAP_H

#include "CutFlow4Lep/Overlap/Overlap.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"
#include "CutFlow4Lep/ParticleObjects/Muon.h"

using namespace std;
class MuonElectronOverlap : public Overlap<Muon>
{
	public:
		MuonElectronOverlap(vector<Muon*> *tInitMuonVec, vector<Electron*> *tElectronVec);
		~MuonElectronOverlap();

		void removeOverlap();

	protected:

	private:
		Bool_t overlapsElectron(Muon *currMuonObj);

		vector<Electron*> *m_electronVec;
};

#endif