#ifndef OVERLAP_H
#define OVERLAP_H

using namespace std;
template <class Particle>
class Overlap
{
	public:
		Overlap(vector<Particle*> *tInitParticleVec) : m_initParticleVec(tInitParticleVec) {};
		~Overlap() {};

		virtual void removeOverlap() = 0;
		void setInitParticleVec(vector<Particle*> *tInitParticleVec)
		{
			m_goodParticleVec.clear();
			m_initParticleVec = tInitParticleVec;
		};
		vector<Particle*> getGoodParticleVec() {return m_goodParticleVec;};

	protected:
		vector<Particle*> *m_initParticleVec;
		vector<Particle*> m_goodParticleVec;

		Int_t m_dataYear;

		Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2)
		{
			Double_t deltaEta = (eta1 - eta2);
			Double_t deltaPhi = (fabs(phi1 - phi2) > TMath::Pi()) ? 2*TMath::Pi() - fabs(phi1-phi2) : fabs(phi1 - phi2);

			return sqrt(deltaEta * deltaEta + deltaPhi * deltaPhi);
		}

	private:		
};
#endif