#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>

#include <TTree.h>
#include <TChain.h>
#include <TString.h>

#include "HiggsAnalysis.h"


HiggsAnalysis::HiggsAnalysis(TTree *tPhysicsTree) : m_physicsTree(tPhysicsTree)
{
	m_event = new D3PDReader::Event();
	m_event->ReadFrom(tPhysicsTree);
	
}

HiggsAnalysis::~HiggsAnalysis()
{
	delete m_physicsTree;
	delete m_event;
	
	
}

void HiggsAnalysis::AnalyzeTree()
{
	cout << "Tree was analyzed..." << endl;
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