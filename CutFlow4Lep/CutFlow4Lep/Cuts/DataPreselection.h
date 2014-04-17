#ifndef DATAPRESELECTION_H
#define DATAPRESELECTION_H

#include "CutFlow4Lep/Cuts/Cuts.h"

#include <TileTripReader/TTileTripReader.h>
#include <GoodRunsLists/TGoodRunsList.h>
#include <GoodRunsLists/TGoodRunsListReader.h>

using namespace std;
class DataPreselection : public Cuts
{
	public:
		DataPreselection(D3PDReader::Event *tEvent);
		~DataPreselection() {};

		Bool_t passedCut();

	protected:

	private:
		void initializeTileTripReader();
		void initializeGoodRunsList();

		Root::TTileTripReader *m_tileTripReader;
		Root::TGoodRunsList *m_goodRunsList;
};
#endif