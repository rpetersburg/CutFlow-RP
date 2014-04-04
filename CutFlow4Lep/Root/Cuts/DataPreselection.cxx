#include "CutFlow4Lep/Cuts/DataPreselection.h"

DataPreselection::DataPreselection(D3PDReader::Event *tEvent) : Cuts(tEvent)
{
	initializeTileTripReader();
	initializeGoodRunsList();
}

DataPreselection::~DataPreselection() {}

Bool_t DataPreselection::passedCut()
{
	if (m_event->eventinfo.larError() == 2) return false;
	if (m_event->eventinfo.tileError() == 2) return false;
	if ((m_event->eventinfo.coreFlags() & 0x40000) != 0) return false;
	if (!(m_tileTripReader->checkEvent(m_event->eventinfo.RunNumber(),
																		 m_event->eventinfo.lbn(),
																		 m_event->eventinfo.EventNumber()))) return false;
	//Int_t reinitializedTool = m_goodRunsList->HasRunLumiBlock(m_event->eventinfo.RunNumber(), -1);
	if (!(m_goodRunsList->HasRunLumiBlock(m_event->eventinfo.RunNumber(), m_event->eventinfo.lbn())))
		return false;
	return true;
}

void DataPreselection::initializeTileTripReader()
{
	m_tileTripReader = new Root::TTileTripReader("TripReader");
	m_tileTripReader->setTripFile("../../TileTripReader/data/CompleteTripList_2011-2012.root");
}

void DataPreselection::initializeGoodRunsList()
{
	TString goodRunsListName;

	if (m_dataYear == 2011) goodRunsListName = "InputFile/GRL/data11_7TeV.periodAllYear_DetStatus-v36-pro10-02_CoolRunQuery-00-04-08_All_Good.xml";
	else if (m_dataYear == 2012) goodRunsListName = "InputFile/GRL/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml";
	else cout << "Error: DataPreselection::initializeGoodRunList(): dataYear not recognized" << endl;

	Root::TGoodRunsListReader *goodRunsListReader = new Root::TGoodRunsListReader();
	goodRunsListReader->AddXMLFile(goodRunsListName);
	goodRunsListReader->Interpret();

	*m_goodRunsList = goodRunsListReader->GetMergedGoodRunsList();
}