#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>

#include <TTree.h>
#include <TChain.h>
#include <TString.h>

#include "../CutFlow/HiggsAnalysis.h"

HiggsAnalysis::HiggsAnalysis()
{
	m_event = new D3PDReader::Event();
}

HiggsAnalysis::HiggsAnalysis(TTree *tPhysicsTree) : m_physicsTree(tPhysicsTree)
{
	m_event = new D3PDReader::Event();
	m_event->ReadFrom(m_physicsTree);	
}

HiggsAnalysis::~HiggsAnalysis()
{
	delete m_physicsTree;
	delete m_event;	
}

void HiggsAnalysis::AnalyzeTree()
{
	Int_t eventNum = 0;
	Int_t numPassed = 0;

	Int_t totNumEvents = m_physicsTree->GetEntries();

	for (Long64_t iEvent = 0; iEvent < totNumEvents; iEvent++)
	{
		Long64_t event = iEvent;
		TChain* chain = dynamic_cast<TChain*> (m_physicsTree);
		if (chain)
		{
			event = chain->LoadTree(iEvent);
		}
		// m_currFileName = chain->GetFile()->GetPath();
		AnalyzeTreeEvent(iEvent);
	}



	cout << "Tree was analyzed..." << endl;
}

void HiggsAnalysis::AnalyzeTreeEvent(Long64_t eventNumber)
{
	if(eventNumber >= 0) m_event->GetEntry(eventNumber);

}

void HiggsAnalysis::InitializeVar()
{
	cout << "Variables were initialized..." << endl;
}

void HiggsAnalysis::SetOutputFilePath(string newFilePath)
{
	m_outputFilePath = newFilePath;
	cout << "Output file set..." << endl;
}