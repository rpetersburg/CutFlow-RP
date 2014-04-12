#ifndef MUONCALOSMEAR_H
#define MUONCALOSMEAR_H

#include "CutFlow4Lep/Smear/MuonSmear4Lep.h"

using namespace std;
class MuonCaloSmear : public MuonSmear4Lep
{
	public:
		MuonCaloSmear(D3PDReader::Event *tEvent, Root::TPileupReweighting *tPileupReweighting);
		~MuonCaloSmear() {};

		void executeSmear();

	protected:

	private:
		void initializeMuonObj();
};
#endif