#ifndef MUONSTACOSMEAR_H
#define MUONSTACOSMEAR_H

#include "CutFlow4Lep/Smear/MuonSmear4Lep.h"

using namespace std;
class MuonStacoSmear : public MuonSmear4Lep
{
	public:
		MuonStacoSmear(D3PDReader::Event *tEvent, Root::TPileupReweighting *tPileupReweighting);
		~MuonStacoSmear() {};

		void executeSmear();

	protected:

	private:
		void initializeMuonObj();
};
#endif