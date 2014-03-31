#ifndef STRUCTDEF_H
#define STRUCTEDEF_H

// Definition of macro for period
#define period2011_BD "period2011_BD"
#define period2011_EH "period2011_EH"
#define period2011_IK "period2011_IK"
#define period2011_I "period2011_I"
#define period2011_J "period2011_J"
#define period2011_K "period2011_K"
#define period2011_LM "period2011_LM"

#define period2012_All "period2012_All"

// Z mass
#define pdgZMass  91187.6 
#define pdgMuMass 105.6583715
#define pdgElMass 0.510998928

struct DoAnalysis{
	enum {
		StdHZZllll,
		trigeff4l,
		Zllll
	};
};

struct  MCCollection{
	enum {
		MC11c,
		MC11d,
		MC12a,
		MC12b,
		MC12c
	};
};

struct CutFlow {
	enum {
		Total,
		DataPreselection,
		Preselection,
		Trigger,
		Trigger4Mu,
		Trigger4e
	};
};

struct CutFlowCH {
	enum {
		Total,
		Trigger,
		Lepton,
		SFOS,
		Kinematics,
		TriggerMatch,
		Z1Mass,
		Z2Mass,
		DeltaR,
		TrackIso,
		CaloIso,
		D0Sig,
		Final
	};
};

struct CutElFlow {
	enum {
		Total,
		DataPreselection,
		Preselection,
		Trigger,
		Author,
		Loose,
		Eta,
		Et,
		ObjectQuality,
		Z0,
		OverLapElEl,
		OverLapClElEl,
		OverLap
	};
};

struct CutMuFlow {
	enum {
		Total,
		DataPreselection,
		Preselection,
		Trigger,
		Author,
		Pt,
		Eta,
		BLayer,
		Pix,
		SCT,
		Holes,
		TRT,
		D0,
		OverLap
	};
};

struct CutJetsFlow {
	enum {
		Total,
		DataPreselection,
		Preselection,
		Trigger,
		Pt,
		Eta,
		Pileup,
		Clean,
		OverLap
	};
};

struct Flavor {
	enum {
		Muon,
		Electron,
		Jet
	};
};

struct LeptonType{
	enum {
		MuonStaco,
		MuonCalo,
		MuonStandAlone,
		ElectronGSF
	};
};

struct JetsType{
	enum {
		AntiKt4TopoEM,
		AntiKt4TopoEMTruth,
		AntiKt4TopoEM_Fid,
		AntiKt4TopoEMTruth_Fid
	};
};

struct QuadType{
	enum {
		Mu4,
		El4,
		Mu2El2,
		El2Mu2
	};
};

struct AnalysisType{
	enum {
		Mu4,
		El4,
		Mu2El2,
		El2Mu2
	};
};

struct ElectronCollection{
	enum {
		LoosePlusPlus,
		MultiLepton,
		Likelihood
	};
};

struct MuonCollection{
	enum {
		Loose
	};
};

struct ProductionChannel{
	enum {
		VBF,
		VHLep,
		VHHad,		
		ggF,
		VH		
	};
};

struct SampleType{
	enum {
		ggF,
		VBF,
		WH,
		ZH,
		ttH,
		qqF,
		Background,
		ggF_ZpZp
	};
};

struct StreamContainer{
	enum {
		eGamma,
		Muon,
		Other
	};
};

struct TruthTypeQuad{
	enum {
		_4mu,
		_2mu2e,
		_4e,
		_4tau,
		_2tau2mu,
		_2tau2e,
		noStatus
	};
};

struct LeptonIDType{
	enum {
		// what we use
		mu_staco_cb,
		mu_staco_st,
		mu_staco_sa,
		// what we used to use
		mu_muid_cb,
		mu_muid_st,
		mu_muid_sa,
		// what we use
		mu_calomuon,
		// For future
		mu_muon_cb,
		mu_muon_st,
		mu_muon_sa,
		mu_muon_calomuon,
		// Electrons 2011
		el_loosepp_H4l,
		el_loosepp_H4l_Ep_comb,
		el_loosepp_H4l_relax,
		// Electrons 2012
		el_multilepton,
		el_multilepton_Ep_comb,
		el_multilepton_relax,
		el_likelihood_loose,
		el_likelihood_loose_Ep_comb,
		el_likelihood_loose_relax,
		unknown
	};
};

struct MCGeneratorName{
	enum {
		Pythia,
		other
	};
};

struct CalibrationType{
	enum {
		stdCalib,
		stdCalibEp,
		MvaCalib,
		MvaCalibEp,
		noCalib
	};
};

struct FSRType{
	enum {
		collFSRZ1mumu,
		farFSRZ1,
		farFSRZ2, 
		noFSR
	};
};

struct DataCalibType{
	enum {
		y2011c,
		y2011d,
		y2012ab,
		y2012c
	};
};

struct DiLeptonType{
	enum {
		_2e, 
		_2mu
	};
};

struct MuonType{
	enum
	{
		CB, 
		MS,
		ID
	};
};

struct DoSys{
	enum
	{
		ZeeStatUp,
		ZeeStatDown, 
		ZeeSystUp,
		ZeeSystDown,
		ZeeAllUp,
		ZeeAllDown,
		PSUp,
		PSDown,
		S12Up,
		S12Down,
		MatIDUp,
		MatIDDown,
		MatCryoUp,
		MatCryoDown,
		MatCaloUp,
		MatCaloDown,
		LArCalibUp,
		LArCalibDown,
		LArUnconvCalibUp,
		LArUnconvCalibDown,
		LArElecUnconvUp,
		LArElecUnconvDown,
		LArElecCalibUp,
		LArElecCalibDown,
		GainUp,
		GainDown,
		G4Up,
		G4Down,
		MomentumUp,
		MomentumDown,
		ZSmearingUp,
		ZSmearingDown,
		SamplingTermUp,
		SamplingTermDown,
		MaterialIDUp,
		MaterialIDDown,
		MaterialCaloUp,
		MaterialCaloDown,
		MaterialGapUp,
		MaterialGapDown,
		MaterialCryoUp,
		MaterialCryoDown,
		PileUpUp,
		PileUpDown,
		Nom
	};
};

// For category
struct VHLeptonType{
	enum {
		electronPlus,
		electronMinus,
		muonPlus,
		muonMinus,
		unknown
	};
};

struct VHTruthType{
	enum {
		ZnoHiggs,
		WnoHiggs,
		Z1fromHiggs,
		Z2fromHiggs,
		unknown
	};
};

#endif