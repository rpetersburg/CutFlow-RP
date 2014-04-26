#ifndef JetsElectronOverlap_H
#define JetsElectronOverlap_H

#include "CutFlow4Lep/Overlap/Overlap.h"
#include "CutFlow4Lep/ParticleObjects/Electron.h"
#include "CutFlow4Lep/ParticleObjects/Jets.h"

using namespace std;
class JetsElectronOverlap : public Overlap<Jets>
{
	public:
		JetsElectronOverlap(vector<Jets*> *tInitJetsVec, vector<Electron*> *tElectronVec);
		~JetsElectronOverlap();

		void removeOverlap();

	protected:

	private:
		Bool_t overlapsElectron(Jets *currJetsObj);

		vector<Electron*> *m_electronVec;
};

#endif