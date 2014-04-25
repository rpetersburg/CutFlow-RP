#include "CutFlow4Lep/Overlap/ElectronOverlap.h"

ElectronOverlap::ElectronOverlap(vector<Electron*> *tInitElectronVec)
	: m_initElectronVec(tInitElectronVec)
{

}

ElectronOverlap::~ElectronOverlap()
{

}

void ElectronOverlap::removeOverlap()
{
	vector<Electron*> tempElectronVec;

	vector<Electron*>::iterator currElectronObj = m_initElectronVec->begin();
	for ( ; currElectronObj != m_initElectronVec->end(); ++currElectronObj)
	{
		if (!overlapsElectron(*currElectronObj))
			tempElectronVec.push_back(*currElectronObj);
	}

	currElectronObj = tempElectronVec.begin();
	for ( ; currElectronObj != tempElectronVec.end(); ++currElectronObj)
	{
		if (!overlapsCluster(*currElectronObj))
			m_goodElectronVec.push_back(*currElectronObj);
	}
}

Bool_t ElectronOverlap::overlapsElectron(Electron *currElectronObj)
{
	Int_t dataYear = currElectronObj->getDataYear();
	D3PDReader::ElectronD3PDObjectElement *currElectron = currElectronObj->getElectron();

	Double_t currD0, currZ0, currPhi, currQoverp, currEta;
	if (dataYear == 2011)
	{
		currD0 = currElectron->trackd0();
		currZ0 = currElectron->trackz0();
		currPhi = currElectron->trackphi();
		currQoverp = currElectron->trackqoverp();
		currEta = currElectron->tracketa();
	}
	else if (dataYear == 2012)
	{
		currD0 = currElectron->Unrefittedtrack_d0();
		currZ0 = currElectron->Unrefittedtrack_z0();
		currPhi = currElectron->Unrefittedtrack_phi();
		currQoverp = currElectron->Unrefittedtrack_qoverp();
		currEta = currElectron->Unrefittedtrack_eta();
	}
	Double_t currEt = currElectron->cl_E() / cosh(currEta);

	D3PDReader::ElectronD3PDObjectElement *testElectron;

	vector<Electron*>::iterator testElectronObj = m_initElectronVec->begin();
	for ( ; testElectronObj != m_initElectronVec->end(); ++testElectronObj)
	{
		testElectron = (*testElectronObj)->getElectron();
		if (currElectron == testElectron) continue;

		Double_t testD0, testZ0, testPhi, testQoverp, testEta;
		if (dataYear == 2011)
		{
			testD0 = testElectron->trackd0();
			testZ0 = testElectron->trackz0();
			testPhi = testElectron->trackphi();
			testQoverp = testElectron->trackqoverp();
			testEta = testElectron->tracketa();
		}
		else if (dataYear == 2011)
		{
			testD0 = testElectron->Unrefittedtrack_d0();
			testD0 = testElectron->Unrefittedtrack_z0();
			testPhi = testElectron->Unrefittedtrack_phi();
			testQoverp = testElectron->Unrefittedtrack_qoverp();
			testEta = testElectron->Unrefittedtrack_eta();
		}
		Double_t testEt = testElectron->cl_E() / cosh(testEta);

		// If curr and test share same ID and curr has lower ET, reject
		if (currD0 == testD0 && currZ0 == testZ0 && currPhi == testPhi && 
			  currQoverp == testQoverp && currEt < testEt)
			return true;
		return false;
	}
}

Bool_t ElectronOverlap::overlapsCluster(Electron *currElectronObj)
{
	Int_t dataYear = currElectronObj->getDataYear();
	D3PDReader::ElectronD3PDObjectElement *currElectron = currElectronObj->getElectron();

	Double_t cutPhi, cutEta;
	if (dataYear == 2011)
	{
		cutPhi = 0;
		cutEta = 0;
	}
	else if (dataYear == 2012)
	{
		cutPhi = 5 * 0.025;
		cutEta = 3 * 0.025;
	}

	Double_t currPhi = currElectron->cl_phi();
	Double_t currEta = currElectron->cl_eta();
	Double_t currEt = currElectron->cl_E() / cosh(currElectron->tracketa());

	D3PDReader::ElectronD3PDObjectElement *testElectron;

	vector<Electron*>::iterator testElectronObj = m_initElectronVec->begin();
	for ( ; testElectronObj != m_initElectronVec->end(); ++testElectronObj)
	{
		testElectron = (*testElectronObj)->getElectron();
		if (currElectron == testElectron) continue;

		Double_t deltaEta = fabs(currEta - testElectron->cl_eta());
		Double_t deltaPhi = fabs(currPhi - testElectron->cl_phi());
		deltaPhi = deltaPhi > TMath::Pi() ? 2 * TMath::Pi() - deltaPhi : deltaPhi;
		Double_t testEt = testElectron->cl_E() / cosh(testElectron->tracketa());

		// Rejects electron if cut tresholds are not met and has lower Et
		if (deltaEta < cutEta && deltaPhi < cutPhi && currEt < testEt)
			return true;
		return false;
	}
}