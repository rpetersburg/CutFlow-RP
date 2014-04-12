#include "CutFlow4Lep/Smear/D0Z0Smear.h"

D0Z0Smear::D0Z0Smear(D3PDReader::Event *tEvent) : Smear(tEvent)
{

}

D0Z0Smear::~D0Z0Smear()
{

}

void D0Z0Smear::executeSmear()
{
	smearMuon();
	smearElectron();
}

void D0Z0Smear::smearMuon()
{
	D3PDReader::MuonD3PDObject *muonCalo = &(m_event->mu_calo);
	smearMuonCalo(muonCalo);

	D3PDReader::MuonD3PDObject *muonStaco = &(m_event->mu_staco);
	smearMuonStaco(muonStaco);
}

void D0Z0Smear::smearMuon(D3PDReader::MuonD3PDObject *muon)
{
	for (Int_t i = 0; i < muon->n(); i++)
	{
		D3PDReader::MuonD3PDObjectElement currMuon = (*muon)[i];
		if (!currMuon.isStandAloneMuon());
		{
			Double_t eta = -log(tan(currMuon.id_theta()/2));
			Double_t pT = sin(currMuon.id_theta()) / fabs(currMuon.id_qoverp());

			currMuon.trackd0pvunbiased() += -2.e-3 + getD0SmearSigma(currMuon.nBLHits(), pT, eta, i);
			currMuon.trackz0pvunbiased() += getZ0SmearSigma(currMuon.nBLHits(), pT, eta, i);
		}
	}
}

void D0Z0Smear::smearMuonCalo(D3PDReader::MuonD3PDObject *muon)
{
	for (Int_t i = 0; i < muon->n(); i++)
	{
		D3PDReader::MuonD3PDObjectElement currMuon = (*muon)[i];
		if (!currMuon.isStandAloneMuon());
		{
			Double_t eta = -log(tan(currMuon.id_theta()/2));
			Double_t pT = sin(currMuon.id_theta()) / fabs(currMuon.id_qoverp());

			currMuon.trackd0pvunbiased() += -2.e-3 + getD0SmearSigma(currMuon.nBLHits(), pT, eta, i);
			currMuon.trackz0pvunbiased() += getZ0SmearSigma(currMuon.nBLHits(), pT, eta, i);
		}
	}
}

void D0Z0Smear::smearMuonStaco(D3PDReader::MuonD3PDObject *muon)
{
	for (Int_t i = 0; i < muon->n(); i++)
	{
		D3PDReader::MuonD3PDObjectElement currMuon = (*muon)[i];
		if (!currMuon.isStandAloneMuon())
		{
			Double_t eta = -log(tan(currMuon.id_theta()/2));
			Double_t pT = sin(currMuon.id_theta()) / fabs(currMuon.id_qoverp());

			currMuon.trackd0pvunbiased() += -2.e-3 + getD0SmearSigma(currMuon.nBLHits(), pT, eta, i);
			currMuon.trackz0pvunbiased() += getZ0SmearSigma(currMuon.nBLHits(), pT, eta, i);
		}
	}
}

void D0Z0Smear::smearElectron()
{
	D3PDReader::ElectronD3PDObject *electron;
	if (m_dataYear == 2011) electron = &(m_event->el_GSF);
	else if (m_dataYear == 2012) electron = &(m_event->el);

	for (Int_t i = 0; i < electron->n(); i++)
	{
		D3PDReader::ElectronD3PDObjectElement currElectron = (*electron)[i];

		currElectron.trackd0pvunbiased() += -2.e-3 + getD0SmearSigma(currElectron.nBLHits(), currElectron.trackpt(), currElectron.tracketa(), i);
		currElectron.trackz0pvunbiased() += getZ0SmearSigma(currElectron.nBLHits(), currElectron.trackpt(), currElectron.tracketa(), i);
	}
}

void D0Z0Smear::initializeSmearObj()
{
	TFile *smearFile = new TFile("../../InputFile/SmearD0/impact_parameter_smearing.root");
	
	for (Int_t i = 0; i < 3; i++)
	{
		m_d0SmearHists[i] = (TH2F*)smearFile->Get("smearD0_" + TString::Itoa(i,10));
		m_d0SmearHists[i]->SetDirectory(0);

		m_z0SmearHists[i] = (TH2F*)smearFile->Get("smearZ0_" + TString::Itoa(i,10));
		m_z0SmearHists[i]->SetDirectory(0);
	}

	smearFile->Close();
}

Double_t D0Z0Smear::getZ0SmearSigma(Int_t nBL, Double_t pT, Double_t eta, Int_t index)
{
	m_z0SmearRandObj.SetSeed(m_eventNumber + 100 * index);
	Double_t z0SmearVal = 0;
	if (nBL >= 0)
	{
		if (nBL > 2) nBL = 2;
		Double_t p_quant = 1. / sqrt(pT*pT/cosh(eta)) / 1000.;
		Int_t xBin = m_z0SmearXAxis->FindFixBin(eta);
		Int_t yBin = m_z0SmearYAxis->FindFixBin(p_quant);
		Double_t sigma = m_z0SmearHists[nBL]->GetBinContent(xBin, yBin);

		z0SmearVal = m_z0SmearRandObj.Gaus(0,sigma);
	}
	return z0SmearVal;
}

Double_t D0Z0Smear::getD0SmearSigma(Int_t nBL, Double_t pT, Double_t eta, Int_t index)
{
	m_d0SmearRandObj.SetSeed(m_eventNumber + 100 * index);
	Double_t d0SmearVal = 0;
	if (nBL >= 0)
	{
		if (nBL > 2) nBL = 2;
		Double_t p_quant = 1. / sqrt(pT*pT/cosh(eta)) / 1000.;
		Int_t xBin = m_d0SmearXAxis->FindFixBin(eta);
		Int_t yBin = m_d0SmearYAxis->FindFixBin(p_quant);
		Double_t sigma = m_d0SmearHists[nBL]->GetBinContent(xBin, yBin);

		d0SmearVal = m_d0SmearRandObj.Gaus(0,sigma);
	}
	return d0SmearVal;
}