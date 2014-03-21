#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>

#include <TTree.h>
#include <TChain.h>
#include <TString.h>


HiggsAnalysis::HiggsAnalysis(TTree *initPhysicsTree) : m_physicsTree(initPhysicsTree)
{
	//m_event = new D3PDReader::Event();
	//m_event->ReadFrom(phyObject);
	
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

void HiggsAnalysis::SetOutputFilePath(TString newFilePath)
{
	m_outputFilePath = newFilePath;
	cout << "Output file set..." << endl;
}