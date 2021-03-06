#ifndef PARTICLECUTS_H
#define PARTICLECUTS_H

#include "CutFlow4Lep/Cuts/Cuts.h"

using namespace std;
template <class Particle> // Electron, Muon, or Jets
class ParticleCuts : public Cuts
{
	public:
		ParticleCuts(D3PDReader::Event *tEvent, vector<Particle*> *tInitVec) 
			: Cuts(tEvent), m_initParticleVec(tInitVec) {};
		~ParticleCuts() {};

		virtual void executeCut() = 0;
		void setInitParticleVec(vector<Particle*> *tInitVec) 
		{
			m_cutParticleVec.clear();
			m_initParticleVec = tInitVec;
		};
		vector<Particle*> getCutParticleVec() {return m_cutParticleVec;};

	protected:
		vector<Particle*> *m_initParticleVec;
		vector<Particle*> m_cutParticleVec;

	private:
};
#endif