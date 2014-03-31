#include "CutFlow4Lep/Weights/PileupWeight.h"

PileupWeight::PileupWeight(D3PDReader::Event *tEvent, TString tDataYear, Int_t tMCCollection, Int_t tDataCollection) 
	:	Weights(tEvent, tDataYear), m_mcCollection(tMCCollection), m_dataCollection(tDataCollection)
{
	initializeReweight();
	setWeight();
}

PileupWeight::~PileupWeight() {}

void PileupWeight::initializeReweight()
{
	m_reweight = new Root::TPileupReweighting("PileupReweightingTool");

	m_reweight->SetUnrepresentedDataAction(2);

	if (m_dataYear == 2011)
	{		
		m_reweight->AddConfigFile("InputFile/PileupConfigFile/MC11c.prw.root");
		m_reweight->AddLumiCalcFile("InputFile/PileupConfigFile/ilumicalc_2011_AllYear_All_Good.root");
		m_reweight->SetDefaultChannel(109292);
		m_reweight->UsePeriodConfig("MC11c");
		m_reweight->Initialize();
	}
	else if (m_dataYear == 2012)
	{
		string configFileName = "";
		if (m_mcCollection == MCCollection::MC12a || m_dataCollection != -1)
		{
			configFileName = "InputFile/PileupConfigFile/MC12a.prw.root";
			m_reweight->SetDefaultChannel(160156);
		}
		else
		{
			configFileName = "InputFile/PileupConfigFile/MC12b.prw.root";
			m_reweight->SetDefaultChannel(181341);
		}
		m_reweight->AddConfigFile(configFileName);
		m_reweight->AddLumiCalcFile("InputFile/PileupConfigFile/ilumicalc_2012_AllYear_All_Good.root");
		m_reweight->Initialize();
	}
	else cout << "Error: PileupWeight::initializeReweight(): Data Year not recognized" << endl;
}

void PileupWeight::setWeight()
{
	// if (isMC);

	// Finding value mu
	Double_t mu = m_event->eventinfo.averageIntPerXing();
	if (m_dataYear == 2012) mu = (m_event->eventinfo.lbn() == 1 && int(mu+0.5) == 1) ? 0. : mu;

	// Setting weight based on mu
	m_weight = m_reweight->GetCombinedWeight(m_event->eventinfo.RunNumber(), m_event->eventinfo.mc_channel_number(), mu);
}