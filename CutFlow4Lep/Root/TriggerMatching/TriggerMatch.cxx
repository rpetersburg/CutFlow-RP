#include "CutFlow4Lep/TriggerMatching/TriggerMatch.h"

TriggerMatch::TriggerMatch(D3PDReader::Event *tEvent, Int_t tCurrMCCollection, Int_t tRunNumber_sf)
	: UsesEvent(tEvent), m_currMCCollection(tCurrMCCollection), m_runNumber_sf(tRunNumber_sf)
{
	init();
}

TriggerMatch::~TriggerMatch()
{
	delete m_triggerNavigationVariables;
	delete m_muonTriggerMatchTool;
	delete m_electronTriggerMatchTool;
	delete m_leptonTriggerSFTool;
}

void TriggerMatch::init()
{
	initTriggerNavigationVariables();
	initTriggerMatchTools();
	initLeptonTriggerSFTool();
	initTriggerString();
}

void TriggerMatch::initTriggerMatchTools()
{
	m_muonTriggerMatchTool = new MuonTriggerMatching(m_triggerNavigationVariables);
	m_electronTriggerMatchTool = new ElectronTriggerMatching(m_triggerNavigationVariables);

	m_muonTriggerMatchTool->setDeltaR(0.15);
	m_electronTriggerMatchTool->setDeltaR(0.15);
}

void TriggerMatch::initLeptonTriggerSFTool()
{
	if (m_dataYear == 2011)
	{
		m_leptonTriggerSFTool = new LeptonTriggerSF(2011, "../../TrigMuonEfficiency/share",
																								"muon_trigger_sf_mc11c.root",	
																								"../../ElectronEfficiencyCorrection/data/",	
																								"rel17p0.v02");
	}
	else if (m_dataYear == 2012)
	{
		if (m_currMCCollection == MCCollection::MC12a ||
				m_currMCCollection == MCCollection::MC12b ||
				m_dataCalibration == DataCalibType::y2012ab)
		{
			m_leptonTriggerSFTool = new LeptonTriggerSF(2012, 
																									"../../TrigMuonEfficiency/share", 
																									"muon_trigger_sf_2012_AtoL.p1328.root",
																									"../../ElectronEfficiencyCorrection/data/",
																									"rel17p2.GEO20.v08");
		}
		else if (m_currMCCollection == MCCollection::MC12c ||
						 m_dataCalibration == DataCalibType::y2012c)
		{
			m_leptonTriggerSFTool = new LeptonTriggerSF(2012, 
																									"../../TrigMuonEfficiency/share", 
																									"muon_trigger_sf_2012_AtoL.p1328.root",
																									"../../ElectronEfficiencyCorrection/data/",
																									"rel17p2.GEO21.v01");
		}
	}
}

void TriggerMatch::initTriggerNavigationVariables() // Copied from Haider
{
	m_triggerNavigationVariables = new TriggerNavigationVariables();

	m_triggerNavigationVariables->set_trig_DB_SMK(m_event->trig_DB.SMK());
  m_triggerNavigationVariables->set_trig_Nav_n(m_event->trig_Nav.n());
  m_triggerNavigationVariables->set_trig_Nav_chain_ChainId(m_event->trig_Nav.chain_ChainId());
  m_triggerNavigationVariables->set_trig_Nav_chain_RoIType(m_event->trig_Nav.chain_RoIType());
  m_triggerNavigationVariables->set_trig_Nav_chain_RoIIndex(m_event->trig_Nav.chain_RoIIndex());

  // Electron 
  m_triggerNavigationVariables->set_trig_RoI_EF_e_egammaContainer_egamma_Electrons(m_event->trig_RoI_EF_e.egammaContainer_egamma_Electrons());
  m_triggerNavigationVariables->set_trig_RoI_EF_e_egammaContainer_egamma_ElectronsStatus(m_event->trig_RoI_EF_e.egammaContainer_egamma_ElectronsStatus());
  m_triggerNavigationVariables->set_trig_EF_el_n(m_event->trig_EF_el.n());
  m_triggerNavigationVariables->set_trig_EF_el_eta(m_event->trig_EF_el.eta());
  m_triggerNavigationVariables->set_trig_EF_el_phi(m_event->trig_EF_el.phi());

  // Muon 
  m_triggerNavigationVariables->set_trig_RoI_EF_mu_Muon_ROI(m_event->trig_RoI_EF_mu.Muon_ROI());
  if (m_dataYear == 2012) // Previously !D3PDTriggerDev which is equivalent
	{
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainer(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer());
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainerStatus(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainerStatus());
  }
	else
	{
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainerStatus(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_eMuonEFInfoStatus());
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFInfoContainer(m_event->trig_RoI_EF_mu.TrigMuonEFInfoContainer_eMuonEFInfo());
  }

  if (m_dataYear == 2012) // For 2012 isolated trigger
	{
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFIsolationContainer(m_event->trig_RoI_EF_mu.TrigMuonEFIsolationContainer());
    m_triggerNavigationVariables->set_trig_RoI_EF_mu_TrigMuonEFIsolationContainerStatus(m_event->trig_RoI_EF_mu.TrigMuonEFIsolationContainerStatus());
	}

  m_triggerNavigationVariables->set_trig_RoI_L2_mu_CombinedMuonFeature(m_event->trig_RoI_L2_mu.CombinedMuonFeature());
  m_triggerNavigationVariables->set_trig_RoI_L2_mu_CombinedMuonFeatureStatus(m_event->trig_RoI_L2_mu.CombinedMuonFeatureStatus());
  m_triggerNavigationVariables->set_trig_RoI_L2_mu_MuonFeature(m_event->trig_RoI_L2_mu.MuonFeature());
  m_triggerNavigationVariables->set_trig_RoI_L2_mu_Muon_ROI(m_event->trig_RoI_L2_mu.Muon_ROI());
  m_triggerNavigationVariables->set_trig_EF_trigmuonef_track_MuonType(m_event->trig_EF_trigmuonef.track_MuonType());
  m_triggerNavigationVariables->set_trig_EF_trigmuonef_track_CB_pt(m_event->trig_EF_trigmuonef.track_CB_pt());
  m_triggerNavigationVariables->set_trig_EF_trigmuonef_track_CB_eta(m_event->trig_EF_trigmuonef.track_CB_eta());  
  m_triggerNavigationVariables->set_trig_EF_trigmuonef_track_CB_phi(m_event->trig_EF_trigmuonef.track_CB_phi());
  m_triggerNavigationVariables->set_trig_EF_trigmuonef_track_SA_pt(m_event->trig_EF_trigmuonef.track_SA_pt());
  m_triggerNavigationVariables->set_trig_EF_trigmuonef_track_SA_eta(m_event->trig_EF_trigmuonef.track_SA_eta());
  m_triggerNavigationVariables->set_trig_EF_trigmuonef_track_SA_phi(m_event->trig_EF_trigmuonef.track_SA_phi());
  m_triggerNavigationVariables->set_trig_EF_trigmugirl_track_CB_pt(m_event->trig_EF_trigmugirl.track_CB_pt());
  m_triggerNavigationVariables->set_trig_EF_trigmugirl_track_CB_eta(m_event->trig_EF_trigmugirl.track_CB_eta());
  m_triggerNavigationVariables->set_trig_EF_trigmugirl_track_CB_phi(m_event->trig_EF_trigmugirl.track_CB_phi());
  m_triggerNavigationVariables->set_trig_L2_combmuonfeature_eta(m_event->trig_L2_combmuonfeature.eta());
  m_triggerNavigationVariables->set_trig_L2_combmuonfeature_phi(m_event->trig_L2_combmuonfeature.phi());
  m_triggerNavigationVariables->set_trig_L2_muonfeature_eta(m_event->trig_L2_muonfeature.eta());
  m_triggerNavigationVariables->set_trig_L2_muonfeature_phi(m_event->trig_L2_muonfeature.phi());
  m_triggerNavigationVariables->set_trig_L1_mu_eta(m_event->trig_L1_mu.eta());
  m_triggerNavigationVariables->set_trig_L1_mu_phi(m_event->trig_L1_mu.phi());
  m_triggerNavigationVariables->set_trig_L1_mu_thrName(m_event->trig_L1_mu.thrName());
    
	m_triggerNavigationVariables->set_trig_EF_el_Et(m_event->trig_EF_el.Et());
}