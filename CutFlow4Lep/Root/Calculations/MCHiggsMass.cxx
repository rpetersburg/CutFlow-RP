#include "CutFlow4Lep/Calculations/MCHiggsMass.h"

MCHiggsMass::MCHiggsMass(D3PDReader::Event *tEvent, vector<TString> tFileNameVec) : m_event(tEvent), m_fileNameVec(tFileNameVec)
{
	setMass();
}

MCHiggsMass::~MCHiggsMass() {}

void MCHiggsMass::setMass()
{
	for (Int_t i = 0; i < m_fileNameVec.size(); i++)
	{
		// Find proper part of file name to begin
		if (m_fileNameVec[i].Contains("mc11_7TeV") || m_fileNameVec[i].Contains("mc12_8TeV"))
		{
			Int_t sampleIndex = i + 2;
			Int_t massStartIndex = 0;
			TString sampleName = m_fileNameVec[sampleIndex];

			if (sampleName.Contains("ggH"))				massStartIndex = sampleName.Index("ggH") + 3;
			else if (sampleName.Contains("VBHF"))	massStartIndex = sampleName.Index("VBHF") + 4;
			else if (sampleName.Contains("VBF"))	massStartIndex = sampleName.Index("VBF") + 3;
			else if (sampleName.Contains("WH"))		massStartIndex = sampleName.Index("WH") + 2;
			else if (sampleName.Contains("ZH"))		massStartIndex = sampleName.Index("ZH") + 2;
			else if (sampleName.Contains("ttH"))	massStartIndex = sampleName.Index("ttH") + 3;
			else if (sampleName.Contains("qqH"))	massStartIndex = sampleName.Index("qqH") + 3;
			else																	{m_mass = -1; return;}

			// Find pre-decimal higgs mass
			TString higgsMassStr = findNumInString(sampleName, massStartIndex);
			Double_t higgsMass = higgsMassStr.Atoi();
			Int_t massEndIndex = massStartIndex + higgsMassStr.Length();

			// Find post-decimal higgs mass
			Double_t decimal = 0;
			if (sampleName[massEndIndex] == 'p')
			{
				TString decimalStr = findNumInString(sampleName, massEndIndex+1);
				decimal = decimalStr.Atoi();
				decimal = decimal/pow(10.0, decimalStr.Length());
			}

			// Find total mass value
			m_mass = higgsMass + decimal;
		}
	}
}

Double_t MCHiggsMass::getMass()
{
	return m_mass;
}

TString findNumInString(TString sampleName, Int_t startIndex)
{
	Int_t endIndex = startIndex;
	Int_t length = 0;
	TString character;
	do {
		endIndex++;
		length++;
		character = sampleName[endIndex];
	} while (character.IsDec());
	return sampleName(startIndex, length);
}