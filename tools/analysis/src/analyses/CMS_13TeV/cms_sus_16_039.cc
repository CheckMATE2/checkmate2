#include "cms_sus_16_039.h"
// AUTHOR: Junjie Cao, Liangliang Shang, Jinmin Yang, Yuanfang Yue, Yang Zhang, Pengxuan Zhu;
//  EMAIL: junjiec@itp.ac.cn, shlwell1988@gmail.com, jmyang@itp.ac.cn, yuanfang405@gmail.com, zhangyang@itp.ac.cn, zhupengxuan326@hotmail.com    

void Cms_sus_16_039::initialize() {
  setAnalysisName("cms_sus_16_039");          
  setInformation(""
		 "# cms\n"
		 "# cms pss sus-16-039\n"
                 "# 3 leptons\n"
                 "# sqrt(s) = 13 TeV\n"
                 "# int(L) = 35.9fb^-1\n"		 
  "");
  setLuminosity(35.9*units::INVFB);      

  bookSignalRegions("SS01;SS02;SS03;SS04;SS05;SS06;SS07;SS08;SS09;SS10;\
SS11;SS12;SS13;SS14;SS15;SS16;SS17;SS18;SS19;SS20;\
SS21;SS22;SS23;SS24;SS25;SS26;SS27;SS28;SS29;SS30;\
SR_A01;SR_A02;SR_A03;SR_A04;SR_A05;SR_A06;SR_A07;SR_A08;SR_A09;SR_A10;\
SR_A11;SR_A12;SR_A13;SR_A14;SR_A15;SR_A16;SR_A17;SR_A18;SR_A19;SR_A20;\
SR_A21;SR_A22;SR_A23;SR_A24;SR_A25;SR_A26;SR_A27;SR_A28;SR_A29;SR_A30;\
SR_A31;SR_A32;SR_A33;SR_A34;SR_A35;SR_A36;SR_A37;SR_A38;SR_A39;SR_A40;\
SR_A41;SR_A42;SR_A43;SR_A44;\
SR_B01;SR_B02;SR_B03;SR_B04;SR_B05;SR_B06;\
SR_C01;SR_C02;SR_C03;SR_C04;SR_C05;SR_C06;SR_C07;SR_C08;SR_C09;SR_C10;\
SR_C11;SR_C12;SR_C13;SR_C14;SR_C15;SR_C16;SR_C17;SR_C18;\
SR_D01;SR_D02;SR_D03;SR_D04;SR_D05;SR_D06;SR_D07;SR_D08;SR_D09;SR_D10;\
SR_D11;SR_D12;SR_D13;SR_D14;SR_D15;SR_D16;\
SR_E01;SR_E02;SR_E03;SR_E04;SR_E05;SR_E06;SR_E07;SR_E08;SR_E09;SR_E10;\
SR_E11;SR_E12;\
SR_F01;SR_F02;SR_F03;SR_F04;SR_F05;SR_F06;SR_F07;SR_F08;SR_F09;SR_F10;\
SR_F11;SR_F12;\
SR_G01;SR_G02;SR_G03;SR_G04;SR_G05;\
SR_H01;SR_H02;SR_H03;SR_H04;\
SR_I01;SR_I02;SR_I03;SR_I04;\
SR_J01;SR_J02;SR_J03;SR_J04;\
SR_K01;SR_K02;SR_K03");

  
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_sus_16_039::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  countCutflowEvent("CutFlow_0_Start");

  //missing et
  missingET->addMuons(muons); // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  double met = missingET->P4().Et();

  //construct electrons and muons
  std::vector<Electron *> electrons_sel;
  std::vector<Muon *> muons_sel;
  electrons_sel = filterPhaseSpace(electronsLoose, 10., -2.5, 2.5, false);
  muons_sel = filterPhaseSpace(muons, 10., -2.4, 2.4, false);

  electrons_sel = isolateLeptons(electrons_sel, 0.4);
  muons_sel = isolateLeptons(muons_sel, 0.4);

  //construct tau
  std::vector<Jet *> taus;
  for (int i = 0; i < jets.size(); i++)
  {
    //if (checkTauTag(jets[i], "tight")) {
    //if (checkTauTag(jets[i], "medium")) {
    if (checkTauTag(jets[i], "loose") and fabs(jets[i]->Charge) == 1)
    {
      taus.push_back(jets[i]);
    }
  }

  taus = filterPhaseSpace(taus, 20., -2.3, 2.3);
  taus = overlapRemoval(taus, electrons_sel, 0.5);
  taus = overlapRemoval(taus, muons_sel, 0.5);

  //construct jets
  jets = filterPhaseSpace(jets, 25., -2.4, 2.4);
  jets = overlapRemoval(jets, electrons_sel, 0.4);
  jets = overlapRemoval(jets, muons_sel, 0.4);

  electrons_sel = overlapRemoval(electrons_sel, 0.1);

  //construct b-jet
  std::vector<Jet *> bjets;
  for (int i = 0; i < jets.size(); i++)
  {
    if (checkBTag(jets[i]))
    {
      bjets.push_back(jets[i]);
    }
  }

  //define light leptons
  std::vector<FinalStateObject *> leptons;
  for (int i = 0; i < electrons_sel.size(); i++)
  {
    FinalStateObject *lep = newFinalStateObject(electrons_sel[i]);
    leptons.push_back(lep);
  }
  for (int i = 0; i < muons_sel.size(); i++)
  {
    FinalStateObject *lep = newFinalStateObject(muons_sel[i]);
    leptons.push_back(lep);
  }
  std::sort(leptons.begin(), leptons.end(), FinalStateObject::sortByPT);

  //printf("leptons.size()=%d\n", leptons.size());

  // Trigger
  bool trigger = false;
  // Dilepton trigger
  if (leptons.size() >= 2)
  {
    double leading_cut = (leptons[0]->Type == "electron") ? 25. : 17.;
    double subleading_cut = (leptons[1]->Type == "electron") ? 12. : 8.;
    trigger = ((leptons[0]->PT > leading_cut) and (leptons[1]->PT > leading_cut));
  }
  else if (leptons.size() == 1 and taus.size() == 2)
  {
    double leading_cut = (leptons[0]->Type == "electron") ? 27. : 24.;
    trigger = (leptons[0]->PT > leading_cut);
  }

  if (!trigger)
    return;

  //two leptons same-sign category (labeled as CA)
  bool CA_preSelection = false;
  if (leptons.size() >= 2)
  {
    bool tight_lepton = true;
    if (electrons_sel.size() > 0 and electrons_sel[0]->PT < 25.){tight_lepton = false;}
    if (electrons_sel.size() > 1 and electrons_sel[1]->PT < 15.){tight_lepton = false;}
    if (muons_sel.size() > 0 and muons_sel[0]->PT < 20.){tight_lepton = false;}
    if (muons_sel.size() > 1 and muons_sel[1]->PT < 10.){tight_lepton = false;}

    // double leading_cut = (leptons[0]->Type == "electron") ? 25. : 20.;
    // double subleading_cut = (leptons[1]->Type == "electron") ? 15. : 10.;
    // if ((leptons[0]->PT > leading_cut) and (leptons[1]->PT > subleading_cut))
    if (tight_lepton)
    {
      countCutflowEvent("CutFlow_2l_a_2tight_e_mu");
      if (leptons[0]->Charge * leptons[1]->Charge > 0.)
      {
        countCutflowEvent("CutFlow_2l_b_same_sign");
        bool CA_3th_l = false;
        if (leptons.size() > 2)
        {
          CA_3th_l = leptons[2]->PT > ((leptons[0]->Type == "muon") ? 10. : 20.);
        }
        if (taus.size() > 0)
        {
          CA_3th_l = taus[0]->PT > 20;
        }
        if (!CA_3th_l)
        {
          countCutflowEvent("CutFlow_2l_c_3th_l_veto");

          bool CA_low_mass = false;
          for (int i = 0; i < leptons.size(); i++)
          {
            for (int j = i + 1; j < leptons.size(); j++)
            {
              if (leptons[i]->Type == leptons[1]->Type and leptons[j]->Charge * leptons[1]->Charge < 0)
              {
                double mll = (leptons[i]->P4() + leptons[j]->P4()).M();
                CA_low_mass = (fabs(mll - mZ) < 15.) or (mll < 12.);
              }
            }
          }
          if (taus.size() >= 2)
          {
            for (int i = 0; i < taus.size(); i++)
            {
              for (int j = i + 1; j < taus.size(); j++)
              {
                if (taus[i]->Charge * taus[j]->Charge < 0 and (taus[i]->P4() + taus[j]->P4()).M() < 12.)
                {
                  CA_low_mass = true;
                }
              }
            }
          }

          if (!CA_low_mass)
          {
            countCutflowEvent("CutFlow_2l_d_low_mass_veto");
            if (bjets.size() == 0)
            {
              countCutflowEvent("CutFlow_2l_e_bj_veto");
              if (met > 60.)
              {
                countCutflowEvent("CutFlow_2l_f_MET_60");
                CA_preSelection = true;
              }
            }
          }
        }
      }
    }
  }
  if (CA_preSelection)
  {
    int njet = 0;
    for (int i = 0; i < jets.size(); i++)
    {
      if (jets[i]->PT > 40.)
      {
        njet += 1;
      }
    }

    double mt1 = mT(leptons[0]->P4(), missingET->P4());
    double mt2 = mT(leptons[1]->P4(), missingET->P4());
    double mt = mt1 < mt2 ? mt1 : mt2;

    double ptll = (leptons[0]->P4() + leptons[1]->P4()).Pt();

    int charge;
    if (leptons[0]->Charge * leptons[1]->Charge > 0)
    {
      if (leptons[0]->Charge == 1)
      {
        charge = 1;
      }
      else
      {
        charge = -1;
      }
    }

    if (njet == 0)
    {
      countCutflowEvent("CutFlow_2l_g_ISR_jet");
      if (mt < 100.)
      {
        countCutflowEvent("CutFlow_2l_h_MT_100");
        if (ptll > 100.)
        {
          countCutflowEvent("CutFlow_2l_i_PTll_100");
        }
        if (met < 100.)
        { //end met
          if (ptll < 50.)
          {
            countSignalEvent("SS01");
          }
          else
          {
            countSignalEvent("SS06");
          }
        }
        else if (met < 150.)
        { //end met
          if (ptll < 50.)
          {
            if (charge == 1)
            {
              countSignalEvent("SS02");
            }
            else
            {
              countSignalEvent("SS03");
            }
          }
          else
          {
            if (charge == 1)
            {
              countSignalEvent("SS07");
            }
            else
            {
              countSignalEvent("SS08");
            }
          }
        }
        else if (met < 200.)
        { //end met
          if (ptll < 50.)
          {
            countSignalEvent("SS04");
          }
          else
          {
            countSignalEvent("SS09");
          }
        }
        else
        { //end met
          if (ptll < 50.)
          {
            countSignalEvent("SS05");
          }
          else
          {
            countSignalEvent("SS10");
          }
        } //end met
      }
      else
      { //end mt
        if (met < 100.)
        { //end met
          countSignalEvent("SS11");
        }
        else if (met < 150.)
        { //end met
          if (charge == 1)
          {
            countSignalEvent("SS12");
          }
          else
          {
            countSignalEvent("SS13");
          }
        }
        else if (met < 200.)
        { //end met
          countSignalEvent("SS14");
        }
        else
        { //end met
          countSignalEvent("SS15");
        } //end met
      }   //end mt
    }
    else if (njet == 1)
    { //end njet
      countCutflowEvent("CutFlow_2l_g_ISR_jet");
      if (mt < 100.)
      {
        countCutflowEvent("CutFlow_2l_h_MT_100");
        if (ptll > 100.)
        {
          countCutflowEvent("CutFlow_2l_i_PTll_100");
        }
        if (met < 100.)
        { //end met
          if (ptll < 50.)
          {
            countSignalEvent("SS16");
          }
          else
          {
            countSignalEvent("SS21");
          }
        }
        else if (met < 150.)
        { //end met
          if (ptll < 50.)
          {
            if (charge == 1)
            {
              countSignalEvent("SS17");
            }
            else
            {
              countSignalEvent("SS18");
            }
          }
          else
          {
            if (charge == 1)
            {
              countSignalEvent("SS22");
            }
            else
            {
              countSignalEvent("SS23");
            }
          }
        }
        else if (met < 200.)
        { //end met
          if (ptll < 50.)
          {
            countSignalEvent("SS19");
          }
          else
          {
            countSignalEvent("SS24");
          }
        }
        else
        { //end met
          if (ptll < 50.)
          {
            countSignalEvent("SS20");
          }
          else
          {
            countSignalEvent("SS25");
          }
        } //end met
      }
      else
      { //end mt
        if (met < 100.)
        { //end met
          countSignalEvent("SS26");
        }
        else if (met < 150.)
        { //end met
          if (charge == 1)
          {
            countSignalEvent("SS27");
          }
          else
          {
            countSignalEvent("SS28");
          }
        }
        else if (met < 200.)
        { //end met
          countSignalEvent("SS29");
        }
        else
        { //end met
          countSignalEvent("SS30");
        } //end met
      }   //end mt
    }     //end njet
  }       // end CA

  //three or more leptons involving taus category (labeled as CB)
  //define lepton_all
  std::vector<FinalStateObject *> lepton_all;
  for (int i = 0; i < electrons_sel.size(); i++)
  {
    FinalStateObject *lep = newFinalStateObject(electrons_sel[i]);
    lepton_all.push_back(lep);
  }
  for (int i = 0; i < muons_sel.size(); i++)
  {
    FinalStateObject *lep = newFinalStateObject(muons_sel[i]);
    lepton_all.push_back(lep);
  }
  for (int i = 0; i < taus.size(); i++)
  {
    FinalStateObject *lep = newFinalStateObject(taus[i]);
    lepton_all.push_back(lep);
  }
  std::sort(lepton_all.begin(), lepton_all.end(), FinalStateObject::sortByPT);

  // at least 3 leptons with a maximum of two taus
  bool Three_leptons = false;
  bool low_ossf_mass = false;
  if (lepton_all.size() >= 3 and taus.size() <= 2)
  {
    // Leading e(mu) PT>25(20), subleading e(mu) PT>15(10)
    if (leptons[0]->PT > ((leptons[0]->Type == "electron") ? 25. : 20.))
    {
      Three_leptons = true;
      if (leptons.size() > 1)
      {
        Three_leptons = (leptons[1]->PT > ((leptons[1]->Type == "electron") ? 15. : 10.));
      }
      // If leading lepton is muon and others are not, PT>25
      if ((lepton_all[0]->Type == "muon") and (lepton_all[1]->Type != "muon") and (lepton_all[2]->Type != "muon"))
      {
        if (lepton_all[0]->PT < 25.)
          Three_leptons = false;
      }
      // For events with two tau, |eta|<2.1 and e(mu) PT>30(25)
      if (leptons.size() == 1 and taus.size() == 2)
      {
        for (int i = 0; i < lepton_all.size(); i++)
        {
          if (lepton_all[i]->Eta > 2.1)
            Three_leptons = false;
        }
        if (leptons[0]->PT < ((leptons[0]->Type == "electron") ? 30. : 25.))
        {
          Three_leptons = false;
        }
      }
    }
  }

  bool CB_preSelection = false;
  bool CB_tau = (taus.size() == 0);
  if (Three_leptons)
  {
    if (CB_tau)
    {
      countCutflowEvent("CutFlow_t1_3l_a_3tight_l");
    }
    else
    {
      countCutflowEvent("CutFlow_t2_3l_a_3tight_l");
    }
    if (lepton_all.size() == 3)
    {
      if (CB_tau)
      {
        countCutflowEvent("CutFlow_t1_3l_b_4th_l_veto");
      }
      else
      {
        countCutflowEvent("CutFlow_t2_3l_b_4th_l_veto");
      }
      bool low_mass_veto = false;
      for (int i = 0; i < lepton_all.size(); i++)
      {
        for (int j = i + 1; j < lepton_all.size(); j++)
        {
          if (lepton_all[i]->Type == lepton_all[j]->Type and lepton_all[i]->Charge * lepton_all[j]->Charge < 0)
          {
            low_mass_veto = (lepton_all[i]->P4() + lepton_all[j]->P4()).M() < 12.;
            if (lepton_all[i]->Type != "tau" and fabs((lepton_all[0]->P4() + lepton_all[1]->P4() + lepton_all[2]->P4()).M() - mZ) < 15.)
            {
              low_mass_veto = true;
            }
          }
        }
      }
      if (!low_mass_veto)
      {
        if (CB_tau)
        {
          countCutflowEvent("CutFlow_t1_3l_c_conversions_low_mass_veto");
        }
        else
        {
          countCutflowEvent("CutFlow_t2_3l_c_conversions_low_mass_veto");
        }
        if (bjets.size() == 0)
        {
          if (CB_tau)
          {
            countCutflowEvent("CutFlow_t1_3l_d_bjet_veto");
          }
          else
          {
            countCutflowEvent("CutFlow_t2_3l_d_bjet_veto");
          }
          if (met > 50.)
          {
            if (CB_tau)
            {
              countCutflowEvent("CutFlow_t1_3l_e_MET_50");
            }
            else
            {
              countCutflowEvent("CutFlow_t2_3l_e_MET_50");
            }
            CB_preSelection = true;
          }
        }
      }
    }
  }

  if (CB_preSelection)
  {

    //SRA and SRB
    double mll;
    if (leptons.size() == 3)
    {
      bool ossf = false;
      double mll_0, mll_z, mll_z_min = 1.E5;
      double mt;

      for (int i = 0; i < 3; i++)
      {
        for (int j = i + 1; j < 3; j++)
        {
          if (leptons[i]->Type == leptons[j]->Type and leptons[i]->Charge * leptons[j]->Charge < 0)
          {
            ossf = true;
            mll = (leptons[i]->P4() + leptons[j]->P4()).M();
            mll_z = fabs(mll - mZ);
            if (mll_z < mll_z_min)
            {
              mll_z_min = mll_z;
              mll_0 = mll;
              mt = mT(leptons[3 - i - j]->P4(), missingET->P4());
            }
          }
        }
      }

      if (ossf)
      {
        //SRA
        if (mt > 100.)
        {
          countCutflowEvent("CutFlow_t1_3l_f_MT_100");
          if (mll_0 > 75)
            countCutflowEvent("CutFlow_t1_3l_g_Mll_GT_75");
        }

        if (mt > 0. and mt < 100.)
        {
          if (mll_0 < 75.)
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A01");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A02");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A03");
            }
            else if (met < 250.)
            {
              countSignalEvent("SR_A04");
            }
            else
            {
              countSignalEvent("SR_A05");
            }
          }
          else if (mll_0 < 105.)
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A15");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A16");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A17");
            }
            else if (met < 250.)
            {
              countSignalEvent("SR_A18");
            }
            else if (met < 400.)
            {
              countSignalEvent("SR_A19");
            }
            else if (met < 550.)
            {
              countSignalEvent("SR_A20");
            }
            else
            {
              countSignalEvent("SR_A21");
            }
          }
          else
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A32");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A33");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A34");
            }
            else if (met < 250.)
            {
              countSignalEvent("SR_A35");
            }
            else
            {
              countSignalEvent("SR_A36");
            }
          }
        }
        else if (mt < 160.)
        { //end mt
          if (mll_0 < 75.)
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A06");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A07");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A08");
            }
            else
            {
              countSignalEvent("SR_A09");
            }
          }
          else if (mll_0 < 105.)
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A22");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A23");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A24");
            }
            else
            {
              countSignalEvent("SR_A25");
            }
          }
          else
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A37");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A38");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A39");
            }
            else
            {
              countSignalEvent("SR_A40");
            }
          }
        }
        else
        { //end mt
          if (mll_0 < 75.)
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A10");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A11");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A12");
            }
            else if (met < 250.)
            {
              countSignalEvent("SR_A13");
            }
            else
            {
              countSignalEvent("SR_A14");
            }
          }
          else if (mll_0 < 105.)
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A26");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A27");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A28");
            }
            else if (met < 250.)
            {
              countSignalEvent("SR_A29");
            }
            else if (met < 400.)
            {
              countSignalEvent("SR_A30");
            }
            else
            {
              countSignalEvent("SR_A31");
            }
          }
          else
          {
            if (met < 100.)
            {
              countSignalEvent("SR_A41");
            }
            else if (met < 150.)
            {
              countSignalEvent("SR_A42");
            }
            else if (met < 200.)
            {
              countSignalEvent("SR_A43");
            }
            else
            {
              countSignalEvent("SR_A44");
            }
          }
        } // end mt
      }
      else
      { // end SRA, start of SRB

        double mll_min, mcut = 50., mt_min = 1.E5;
        mll_z_min = 1.E5;
        for (int i = 0; i < 3; i++)
        {
          mt = mT(leptons[i]->P4(), missingET->P4());
          if (mt < mt_min)
          {
            mt_min = mt;
          }
          for (int j = i + 1; j < 3; j++)
          {
            if (leptons[i]->Charge * leptons[j]->Charge < 0)
            {
              mll = (leptons[i]->P4() + leptons[j]->P4()).M();
              mll_z = fabs(mll - mcut);
              if (mll_z < mll_z_min)
              {
                mll_z_min = mll_z;
                mll_min = mll;
              }
            }
          }
        }

        if (mt_min > 100.)
        {
          countCutflowEvent("CutFlow_t1_3l_f_MT_100");
          if (mll_min > 75)
            countCutflowEvent("CutFlow_t1_3l_g_Mll_GT_75");
        }

        if (mt_min < 0.)
        { //mt_min
          ;
        }
        else if (mt_min < 120.)
        { //end mt_min
          if (mll_min < 100.)
          { //mll_min
            if (met < 100.)
            {
              countSignalEvent("SR_B01");
            }
            else
            {
              countSignalEvent("SR_B02");
            }
          }
          else
          { //end mll_min
            if (met < 100.)
            {
              countSignalEvent("SR_B04");
            }
            else
            {
              countSignalEvent("SR_B05");
            }
          } //end mll_min
        }
        else
        { //end mt_min
          if (mll_min < 100.)
          { //mll_min
            countSignalEvent("SR_B03");
          }
          else
          { //end mll_min
            countSignalEvent("SR_B06");
          } //end mll_min
        }   //end mt_min
      }     //end SRB
    }       // End SRA and SRB

    //signal region SRC, SRD, SRE, SRF
    if ((leptons.size() + taus.size()) == 3 and taus.size() >= 1)
    {
      double mll, mt2;
      // countCutflowEvent("CutFlow_3l_f");
      //SRC
      if (true)
      {
        if (leptons.size() == 2 and leptons[0]->Type == leptons[1]->Type and leptons[0]->Charge * leptons[1]->Charge < 0)
        {
          // countCutflowEvent("CutFlow_3l_f_SRC");

          //"tight" for matching CMS result
          // if (checkTauTag(taus[0], "tight")) {
          if (true)
          {
            mll = (leptons[0]->P4() + leptons[1]->P4()).M();
            mt2 = mT2(leptons[0]->P4(), leptons[1]->P4(), 0.);

            if (mt2 < 100)
            {
              countCutflowEvent("CutFlow_t2_3l_f_MT2_100");
              if (mll < 75)
                countCutflowEvent("CutFlow_t2_3l_g_Mll_LT_75");
            }

            if (mll < 75.)
            {
              if (mt2 < 0.)
              {
                ;
              }
              else if (mt2 < 100.)
              { //end mt2
                if (met < 100.)
                {
                  countSignalEvent("SR_C01");
                }
                else if (met < 150.)
                {
                  countSignalEvent("SR_C02");
                }
                else if (met < 200.)
                {
                  countSignalEvent("SR_C03");
                }
                else if (met < 250.)
                {
                  countSignalEvent("SR_C04");
                }
                else
                {
                  countSignalEvent("SR_C05");
                }
              }
              else
              { //end mt2
                if (met < 200.)
                {
                  if ((double)rand() / (double)RAND_MAX < 1)
                  {
                    countSignalEvent("SR_C17");
                  }
                }
                else
                {
                  countSignalEvent("SR_C18");
                }
              } //end mt2 < 0.
            }
            else if (mll < 105)
            { //end mll
              if (met < 100.)
              {
                countSignalEvent("SR_C06");
              }
              else if (met < 150.)
              {
                countSignalEvent("SR_C07");
              }
              else if (met < 200.)
              {
                countSignalEvent("SR_C08");
              }
              else if (met < 300.)
              {
                countSignalEvent("SR_C09");
              }
              else if (met < 400.)
              {
                countSignalEvent("SR_C10");
              }
              else
              {
                countSignalEvent("SR_C11");
              }
            }
            else
            { //end mll
              if (mt2 < 0.)
              {
                ;
              }
              else if (mt2 < 100.)
              { //end mt2
                if (met < 100.)
                {
                  countSignalEvent("SR_C12");
                }
                else if (met < 150.)
                {
                  countSignalEvent("SR_C13");
                }
                else if (met < 200.)
                {
                  countSignalEvent("SR_C14");
                }
                else if (met < 250.)
                {
                  countSignalEvent("SR_C15");
                }
                else
                {
                  countSignalEvent("SR_C16");
                }
              }
              else
              { //end mt2
                if (met < 200.)
                {
                  if ((double)rand() / (double)RAND_MAX < 1)
                  {
                    countSignalEvent("SR_C17");
                  }
                }
                else
                {
                  countSignalEvent("SR_C18");
                }
              } //end mt2 < 0.
            }   //end mll
          }     //end "tight" criteria
        }       //end ossf
      }         //end SRC

      //SRD
      if (true)
      {
        if (leptons.size() == 2 and leptons[0]->Type != leptons[1]->Type and leptons[0]->Charge * leptons[1]->Charge < 0)
        {
          double mll_tau = fabs((leptons[0]->P4() + taus[0]->P4()).M() - 60.) < fabs((leptons[1]->P4() + taus[0]->P4()).M() - 60.) ? (leptons[0]->P4() + taus[0]->P4()).M() : (leptons[1]->P4() + taus[0]->P4()).M();
          mll = fabs((leptons[0]->P4() + leptons[1]->P4()).M() - 50.) < fabs(mll_tau - 60.) ? (leptons[0]->P4() + leptons[1]->P4()).M() : mll_tau;
          //printf("mll in srd = %f\n", mll);

          mt2 = mT2(leptons[0]->P4(), leptons[1]->P4(), 0.);

          if (mt2 < 100)
          { //Buding Test2059
            countCutflowEvent("CutFlow_t2_3l_f_MT2_100");
            if (mll < 75)
              countCutflowEvent("CutFlow_t2_3l_g_Mll_LT_75");
          }

          if (mt2 < 0.)
          {
            ;
          }
          else if (mt2 < 100.)
          { //end mt2
            if (mll < 60.)
            {
              if (met < 100.)
              {
                countSignalEvent("SR_D01");
              }
              else if (met < 150.)
              {
                countSignalEvent("SR_D02");
              }
              else if (met < 200.)
              {
                countSignalEvent("SR_D03");
              }
              else if (met < 250.)
              {
                countSignalEvent("SR_D04");
              }
              else
              {
                countSignalEvent("SR_D05");
              }
            }
            else if (mll < 100.)
            { // end mll
              if (met < 100.)
              {
                countSignalEvent("SR_D06");
              }
              else if (met < 150.)
              {
                countSignalEvent("SR_D07");
              }
              else if (met < 200.)
              {
                countSignalEvent("SR_D08");
              }
              else if (met < 250.)
              {
                countSignalEvent("SR_D09");
              }
              else
              {
                countSignalEvent("SR_D10");
              }
            }
            else
            { // endl mll
              if (met < 100.)
              {
                countSignalEvent("SR_D11");
              }
              else if (met < 150.)
              {
                countSignalEvent("SR_D12");
              }
              else if (met < 200.)
              {
                countSignalEvent("SR_D13");
              }
              else
              {
                countSignalEvent("SR_D14");
              }
            } // end mll
          }
          else
          { //end mt2
            if (met < 200.)
            {
              countSignalEvent("SR_D15");
            }
            else
            {
              countSignalEvent("SR_D16");
            }
          } // end mt2
        }   // end o-sign o-flavor
      }     // end SRD

      //SRE
      if (true)
      {
        // if (leptons.size() == 2 and leptons[0]->Type == leptons[1]->Type and leptons[0]->Charge * leptons[1]->Charge > 0)      // Budign Test 01141559
        if (leptons.size() == 2 and leptons[0]->Charge * leptons[1]->Charge > 0)
        {
          double mll_0 = (lepton_all[0]->P4() + lepton_all[2]->P4()).M() < (lepton_all[1]->P4() + lepton_all[2]->P4()).M() ? (lepton_all[0]->P4() + lepton_all[2]->P4()).M() : (lepton_all[1]->P4() + lepton_all[2]->P4()).M();
          mll = mll_0 < (lepton_all[0]->P4() + lepton_all[1]->P4()).M() ? mll_0 : (lepton_all[0]->P4() + lepton_all[1]->P4()).M();
          mt2 = mT2(leptons[0]->P4(), taus[0]->P4(), 0.);

          if (mt2 < 100)
          { // Buding Test2056
            countCutflowEvent("CutFlow_t2_3l_f_MT2_100");
            if (mll < 75)
              countCutflowEvent("CutFlow_t2_3l_g_Mll_LT_75");
          }

          if (mt2 < 0.)
          {
            ;
          }
          else if (mt2 < 100.)
          { //end mt2
            if (mll < 60.)
            {
              if (met < 100.)
              {
                countSignalEvent("SR_E01");
              }
              else if (met < 150.)
              {
                countSignalEvent("SR_E02");
              }
              else if (met < 200.)
              {
                countSignalEvent("SR_E03");
              }
              else if (met < 250.)
              {
                countSignalEvent("SR_E04");
              }
              else
              {
                countSignalEvent("SR_E05");
              }
            }
            else if (mll < 100.)
            { // end mll
              if (met < 100.)
              {
                countSignalEvent("SR_E06");
              }
              else if (met < 150.)
              {
                countSignalEvent("SR_E07");
              }
              else if (met < 200.)
              {
                countSignalEvent("SR_E08");
              }
              else if (met < 250.)
              {
                countSignalEvent("SR_E09");
              }
              else
              {
                countSignalEvent("SR_E10");
              }
            }
            else
            { // endl mll
              countSignalEvent("SR_E11");
            } // end mll
          }
          else
          { //end mt2
            countSignalEvent("SR_E12");
          } // end mt2
        }   // end s-sign s-flavor
      }     // end SRE

      //SRF
      if (true)
      {
        if (leptons.size() >= 1 and taus.size() == 2)
        {
          if (true)
          {
            double mll_3 = (taus[0]->P4() + taus[1]->P4()).M();
            double mll_12 = (fabs((taus[0]->P4() + leptons[0]->P4()).M() - 60.) < fabs((taus[1]->P4() + leptons[0]->P4()).M() - 60.) ? (taus[0]->P4() + leptons[0]->P4()).M() : (taus[1]->P4() + leptons[0]->P4()).M());
            mll = (fabs(mll_12 - 50. < fabs((taus[0]->P4() + taus[1]->P4()).M() - 91.2) ? mll_12 : (taus[0]->P4() + taus[1]->P4()).M()));

            mt2 = mT2(leptons[0]->P4(), taus[0]->P4(), 0.);

            if (mt2 < 100)
            { //Buding Test2054
              countCutflowEvent("CutFlow_t2_3l_f_MT2_100");
              if (mll < 75)
                countCutflowEvent("CutFlow_t2_3l_g_Mll_LT_75");
            }

            if (mt2 < 0.)
            {
              ;
            }
            else if (mt2 < 100.)
            { //end mt2
              if (mll < 100.)
              {
                if (met < 100.)
                {
                  countSignalEvent("SR_F01");
                }
                else if (met < 150.)
                {
                  countSignalEvent("SR_F02");
                }
                else if (met < 200.)
                {
                  countSignalEvent("SR_F03");
                }
                else if (met < 250.)
                {
                  countSignalEvent("SR_F04");
                }
                else if (met < 300.)
                {
                  countSignalEvent("SR_F05");
                }
                else
                {
                  countSignalEvent("SR_F06");
                }
              }
              else
              { // end mll
                if (met < 100.)
                {
                  countSignalEvent("SR_F07");
                }
                else if (met < 150.)
                {
                  countSignalEvent("SR_F08");
                }
                else if (met < 200.)
                {
                  countSignalEvent("SR_F09");
                }
                else
                {
                  countSignalEvent("SR_F10");
                }
              } // end mll
            }
            else
            { //end mt2
              if (met < 200.)
              {
                countSignalEvent("SR_F11");
              }
              else
              {
                countSignalEvent("SR_F12");
              }
            } // end mt2
          }   // opposite sign taus
        }     // end one lepton and two taus
      }       // end SRF

    } // end signal region SR C-F

  } //end CB

  bool CC_preSelection = false;
  if (Three_leptons and leptons.size()+taus.size()>3)
  {
    countCutflowEvent("CutFlow_4l_a_4tight_l");
    bool low_mass_veto = false;
    for (int i = 0; i < lepton_all.size(); i++)
    {
      for (int j = i + 1; j < lepton_all.size(); j++)
      {
        if (lepton_all[i]->Type == lepton_all[j]->Type and lepton_all[i]->Charge * lepton_all[j]->Charge < 0)
        {
          low_mass_veto = ((lepton_all[i]->P4() + lepton_all[j]->P4()).M() < 12.);
        }
      }
    }
    if (!low_mass_veto)
    {
      countCutflowEvent("CutFlow_4l_b_low_mass_veto");
      if (bjets.size() == 0)
      {
        countCutflowEvent("CutFlow_4l_c_bjet_veto");
        CC_preSelection = true;
        if (met > 100.)
        {
          countCutflowEvent("CutFlow_4l_e_MET_100");
        }
      }
    }
  }

  if (CC_preSelection)
  {
    //signal region SRG, SRH, SRI, SRJ, SRK
    if (true)
    {

      int ntaus = taus.size();
      int nossf = 0;

      for (int i = 0; i < leptons.size(); i++)
      {
        for (int j = 0; j < leptons.size(); j++)
        {
          if (i > j)
          {
            if (leptons[i]->Type == leptons[j]->Type and leptons[i]->Charge * leptons[j]->Charge < 0)
            {
              nossf += 1;
            }
          }
        }
      }

      if (ntaus == 0.)
      {
        if (nossf >= 2)
        {
          if (met < 50.)
          {
            countSignalEvent("SR_G01");
          }
          else if (met < 100.)
          {
            countSignalEvent("SR_G02");
          }
          else if (met < 150.)
          {
            countSignalEvent("SR_G03");
          }
          else if (met < 200.)
          {
            countSignalEvent("SR_G04");
          }
          else
          {
            countSignalEvent("SR_G05");
          }
        }
        else
        { //end nossf
          if (met < 50.)
          {
            countSignalEvent("SR_H01");
          }
          else if (met < 100.)
          {
            countSignalEvent("SR_H02");
          }
          else if (met < 150.)
          {
            countSignalEvent("SR_H03");
          }
          else
          {
            countSignalEvent("SR_H04");
          }
        } //end nossf
      }
      else if (ntaus == 1.)
      { //end ntaus
        if (met < 50.)
        {
          countSignalEvent("SR_I01");
        }
        else if (met < 100.)
        {
          countSignalEvent("SR_I02");
        }
        else if (met < 150.)
        {
          countSignalEvent("SR_I03");
        }
        else
        {
          countSignalEvent("SR_I04");
        }
      }
      else if (ntaus == 2.)
      { //end ntaus
        if (nossf >= 2)
        {
          if (met < 50.)
          {
            countSignalEvent("SR_J01");
          }
          else if (met < 100.)
          {
            countSignalEvent("SR_J02");
          }
          else if (met < 150.)
          {
            countSignalEvent("SR_J03");
          }
          else
          {
            countSignalEvent("SR_J04");
          }
        }
        else
        { //end nossf
          if (met < 50.)
          {
            countSignalEvent("SR_K01");
          }
          else if (met < 100.)
          {
            countSignalEvent("SR_K02");
          }
          else
          {
            countSignalEvent("SR_K03");
          }
        } //end nossf
      }   // end ntaus
    }     // end SR G-K

  } //end CC

  
}

void Cms_sus_16_039::finalize() {
  // Whatever should be done after the run goes here
}


template <class T>
bool isolateLepton(T *lepton, std::vector<Track *> tracks, double maxPT, double deltaR)
{
  double pTsum = 0;
  double pTmin = 0.2;

  for (std::vector<Track *>::iterator it = tracks.begin(); it != tracks.end(); it++)
  {
    if (lepton->Particle == (*it)->Particle)
    {
      continue;
    }
    if ((*it)->P4().DeltaR(lepton->P4()) > deltaR)
    {
      continue;
    }
    if ((*it)->PT < pTmin)
    {
      continue;
    }
    pTsum += (*it)->PT;
  }
  return pTsum < maxPT;
}

template <class T>
std::vector<T *> Cms_sus_16_039::isolateLeptons(std::vector<T *> leptons, double maxpTrel)
{
  std::vector<T *> isolated;
  for (typename std::vector<T *>::iterator it = leptons.begin(); it != leptons.end(); it++)
  {
    if ((*it)->PT < 50)
    {
      double dR = 0.2;
      if (isolateLepton(*it, tracks, maxpTrel * ((*it)->PT), dR))
      {
        isolated.push_back(*it);
      }
    }
    else if ((*it)->PT > 50 && (*it)->PT < 200)
    {
      double dR = (double)(10 / ((*it)->PT));
      if (isolateLepton(*it, tracks, maxpTrel * ((*it)->PT), dR))
      {
        isolated.push_back(*it);
      }
    }
    else
    {
      double dR = 0.05;
      if (isolateLepton(*it, tracks, maxpTrel * ((*it)->PT), dR))
      {
        isolated.push_back(*it);
      }
    }
  }
  return isolated;
}
