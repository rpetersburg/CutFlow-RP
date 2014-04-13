#ifndef D0Z0SMEAR_H
#define D0Z0SMEAR_H

#include "CutFlow4Lep/Smear/Smear.h"

#include <TFile.h>
#include <TH2F.h>
#include <TRandom3.h>

using namespace std;
class D0Z0Smear : public Smear
{
	public:
		D0Z0Smear(D3PDReader::Event *tEvent);
		~D0Z0Smear();

		void executeSmear();
		void smearMuon();
		void smearElectron();

	protected:

	private:
		void initializeSmearObj();		
		void smearMuon(D3PDReader::MuonD3PDObject *muon);
		void smearMuonCalo(D3PDReader::MuonD3PDObject *muon);
		void smearMuonStaco(D3PDReader::MuonD3PDObject *muon);

		Double_t getZ0SmearSigma(Int_t nBL, Double_t pT, Double_t eta, Int_t index);
		Double_t getD0SmearSigma(Int_t nBL, Double_t pT, Double_t eta, Int_t index);

		TH2F *m_d0SmearHists[3];
		TAxis *m_d0SmearXAxis;
		TAxis *m_d0SmearYAxis;
		TRandom3 m_d0SmearRandObj;

		TH2F *m_z0SmearHists[3];
		TAxis *m_z0SmearXAxis;
		TAxis *m_z0SmearYAxis;
		TRandom3 m_z0SmearRandObj;

};

#endif