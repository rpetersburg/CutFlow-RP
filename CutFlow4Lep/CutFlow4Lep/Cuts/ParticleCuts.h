#ifndef PARTICLECUTS_H
#define PARTICLECUTS_H

#include "CutFlow4Lep/Cuts/Cuts.h"

using namespace std;
template <class Particle>
class ParticleCuts : public Cuts
{
	public:
		ParticleCuts(D3PDReader::Event *tEvent, vector<Particle*> *tInitVec) 
			: Cuts(tEVent), m_initVec(tInitVec) {};
		~ParticleCuts() {};

		virtual void executeCut() = 0;
		void setInitVec(vector<Particle*> *tInitVec) {m_initVec = tInitVec;};
		vector<Particle*> getCutVec() {return m_cutVec;};

	protected:
		vector<Particle*> *m_initVec;
		vector<Particle*> m_cutVec;

	private:
}
#endif