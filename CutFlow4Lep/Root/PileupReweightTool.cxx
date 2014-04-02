#include "CutFlow4Lep/PileupReweightTool.h"

PileupReweightTool::PileupReweightTool(Int_t tDataYear, Int_t tMCCollection, Int_t tDataCollection) 
	: m_dataYear(tDataYear), m_mcCollection(tMCCollection), m_dataCollection(tDataCollection)
{
	initializeTool();
}

PileupReweightTool::~PileupReweightTool()
{

}

void PileupReweightTool::initializeTool()
{
	m_tool = new Root::TPileupReweighting("PileupReweightingTool");

	m_tool->SetUnrepresentedDataAction(2);

	if (m_dataYear == 2011)
	{		
		m_tool->AddConfigFile("InputFile/PileupConfigFile/MC11c.prw.root");
		m_tool->AddLumiCalcFile("InputFile/PileupConfigFile/ilumicalc_2011_AllYear_All_Good.root");
		m_tool->SetDefaultChannel(109292);
		m_tool->UsePeriodConfig("MC11c");
		m_tool->Initialize();
	}
	else if (m_dataYear == 2012)
	{
		string configFileName = "";
		if (m_mcCollection == MCCollection::MC12a || m_dataCollection != -1)
		{
			configFileName = "InputFile/PileupConfigFile/MC12a.prw.root";
			m_tool->SetDefaultChannel(160156);
		}
		else
		{
			configFileName = "InputFile/PileupConfigFile/MC12b.prw.root";
			m_tool->SetDefaultChannel(181341);
		}
		m_tool->AddConfigFile(configFileName);
		m_tool->AddLumiCalcFile("InputFile/PileupConfigFile/ilumicalc_2012_AllYear_All_Good.root");
		m_tool->Initialize();
	}
	else cout << "Error: PileupWeight::initializeReweight(): Data Year not recognized" << endl;
}