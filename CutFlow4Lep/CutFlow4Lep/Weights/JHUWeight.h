#ifndef JHUWEIGHT_H
#define JHUWEIGHT_H

#include <CutFlow4Lep/Weights/Weights.h>
#include <CutFlow4Lep/Calculations/MCHiggsMass.h>

#include <JHUReweighting/JHUPtReweighting.h>

using namespace std;

class JHUWeight : public Weights
{
	public:
		JHUWeight(D3PDReader::Event *tEvent);
		~JHUWeight();

	protected:

	private:
		void initializeReweight();
		void setWeight();

		JHUPtReweighting *m_reweight;
};
#endif
