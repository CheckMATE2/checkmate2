#include "atlas_1602_09058.h"
#include <vector>
using namespace std;
// AUTHOR: jan schuette-engel
//  EMAIL: schuette@physik.rwth-aachen.de

std::string Atlas_1602_09058::cf_index[7] = {"0","1","2","3","4","5","6"};

void Atlas_1602_09058::initialize() {
  setAnalysisName("atlas_1602_09058");          
  setInformation(""
    "# Search for SUSY at 13 TeV in final stated with jets and two same sign leptons or three leptons with the Atlas detector\n"
  "");
  setLuminosity(3.2*units::INVFB);      
  bookSignalRegions("SR0b3j;SR0b5j;SR1b;SR3b;");
  for (int i=0; i<7; i++)
    bookCutflowRegions("SR0b3j"+cf_index[i]);//+";SR0b5j"+cf_index[i]+";SR1b"+cf_index[i]);

  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1602_09058::analyze() {
  
  //string cf_index[7] = {"0","1","2","3","4","5","6"};

  missingET->addMuons(muonsCombined); 
  double etmiss=missingET->P4().Et();
  
  countCutflowEvent("SR0b3j"+cf_index[0]);
  countCutflowEvent("SR0b5j"+cf_index[0]);
  countCutflowEvent("SR1b"+cf_index[0]);
  countCutflowEvent("SR3b"+cf_index[0]);

  vector<Jet*> original_jets;
  original_jets=jets;//save the original jets befor manipulation the container jets



  //Electron candidates
  //Electron candidates are reconstructed from an isolated electromagnetic calorimeter energy deposit matched
  //to an ID track and are required to have p T > 10 GeV, |η| < 2.47, and to pass a loose likelihood-based
  //identification requirement [55, 56]. The likelihood input variables include measurements of calorimeter shower shapes and measurements of track properties from the ID.
  //Candidates within the transition region between the barrel and endcap electromagnetic calorimeters, 1.37 < |η| < 1.52, are removed.
  //Comment: Die Loose electrons sind subset von electrons! Da wir nicht mit einer 'echten' Detektorsimulation wie GEANT arbeiten, müssen wir die reconstruction gar nicht nochmal ausführen.
  //         Wir 'wenden einfach nur effizienzen an'. In irgendwelche experimentellen papern finden sich Wahrscheinlichkeiten dafür, dass ein elektron als Loose Elektron identifiziert wird.
  //         Wenn diese Wahrscheinlichkeit bei einer bestimmten Engergie 95 % ist, wird unsere Detektorsimulation Delphes nur 95 von 100 echten elektronen in den container LooseElectrons packen
  //Comment: Das removen von electrons im Bereich 1.37 < |η| < 1.52, wird ausgeführt weil wir als argument noch true mitgeben.
  //Question: Checke, ob die implementierten effizienzen für Loose und Tight auch stimmen!?[57,58]
  vector<Electron*> electron_candidates;
  electron_candidates=filterPhaseSpace(electronsLoose, 10, -2.47, 2.47,true);

  //The track matched with the electron must have a significance of the transverse impact
  //parameter with respect to the reconstructed primary vertex, d 0 , of |d 0 |/σ(d 0 ) < 5.
  //Question: Ich denke, dass muss man nur machen kann, wenn man mit GEANT arbeitet, denn was der Satz sagt ist ja, dass der track, den man einem Elektron zuordnet |d_0 |/σ(d_0 ) < 5 erfüllen muss.
  //          Da wir aber von vornherein den Track von jedem Elektron kennen erübrigt sich dieser Schritt. Richtig?

  //Muon candidates
  //Muon candidates are reconstructed in the region |η| < 2.5 from muon spectrometer tracks matching
  //ID tracks. All muons must have p T > 10 GeV and must pass the medium identification requirements
  //defined in Ref. [57], based on selections on the number of hits in the different inner detector and muon
  //spectrometer subsystems, and the significance of the charge to momentum ratio q/p [57].
  //Comment: Der part mit der identifikation über MS und track matching kann ignoriert werden, da das nur gemacht werden muss, falls man echte Detektorsimulation wie GEANT benutzt.
  //Comment: medium identification criteria gibt es eigentlich nur für electronen vordefiniert in Checkmate!? Wahrscheinlich benutzt checkmate hier einen veralterten muon identification algorithmus!?
  //         Ich nehme einfach die muonsLoose( haben fast 100% effizienz) und entferne im Bereich |eta|<0.1 40% aller muonen. Das ist gerechtfertigt, wenn man in das experimentelle paper [59] schaut.
  //         Dort sieht man, dass medium muons bei allen etas fast zu 100% richtig identifiziert werden, nur im Bereich |eta|<0.1 gibt es eine efficiency von 0.6.
  vector<Muon*> muon_candidates;
  muon_candidates=filterPhaseSpace(muonsLoose, 10, -2.5, 2.5,false);

  muon_candidates=filter_candidates_in_eta_range(muon_candidates,-0.1,0.1,0.6);


  //Jets
  //Jets are reconstructed with the anti-k t algorithm [58] with radius parameter R = 0.4 using three-dimensional
  //energy clusters in the calorimeter [59] as input objects.
  //Comment: Wieder werden die jets hier nicht identifiziert indem wir den anti kt algorithmus anwednen, da wir mit Delphes arbeiten. Vielmehr werden wieder die effizienzen des anti kT algorithmuses
  //         angewendet.

  //All jets must have p T > 20 GeV and |η| < 2.8.
  jets=filterPhaseSpace(jets, 20, -2.8, 2.8,false);
  
  //Jets are calibrated as described in Ref. [60].
  //Question: Calibartion braucht man nur, wenn man die jet energy messen will! Aber das Problem gibt es hier nicht. Richtig?

  //In order to reduce the effects of pileup, for jets with p_T < 50 GeV
  //and |η| < 2.4 a significant fraction of the tracks associated with each jet must have an origin compat-
  //ible with the primary vertex, as defined by the jet vertex tagger [61], and the expected average energy
  //contribution from pileup clusters is subtracted according to the jet area [60].
  //Comment: Ignore effects of pileup, because Delphes in Checkmate does not simulate pileup

  //b jets
  //Identification of jets containing b-quarks (b-tagging) is performed with the MV2c20 algorithm, a mul-
  //tivariate discriminant making use of track impact parameters and reconstructed secondary vertices [62–
  //64]. A requirement is chosen corresponding to a 70% average efficiency obtained for b-jets in simulated
  //t t  ̄ events. The rejection factors for light-quark jets, c-quark jets and τ leptons in simulated t t  ̄ events
  //are approximately 440, 8 and 26, respectively [64]. Jets with |η| < 2.5 which satisfy this b-tagging re-
  //quirement are identified as b-jets.
  //Comment: Auszug aus mail von Daniel: Für CheckMATE, ist dieses neurale Netzwerk(MV2c20 algorithm ) völlig egal: a) haben wir gar nicht alle Informationen, die eine richtige Messung haben würde und 
  //         b) wissen wir in unserer Simulation ja, was richtig und was falsch ist. Deswegen brauchen wir CheckMATE nur zu sagen, welcher Punkt auf der ROC-Kurve genommen werden soll, zB in
  //         deinem Fall "70%" und CheckMATE geht dann einfach durch alle Jets, und wenn sie ein "b" enthalten kriegen sie mit 70% einen BTag und wenn sie keins enthalten zu 0.5% einen BTag.
  //         Jetzt kann es passieren, dass ein CheckMATE-Durchlauf mehrere Analysen verwendet, und eine sucht nach B-Tags mit 70% Effizienz und ein anderer mit 50% Effizienz.
  //         In deinem Fall gibt es sogar mehrere Tags in einer Analyse. Dann wird das in CheckMATE entsprechend einzeln abgearbeitet und die Ergebnisse in "BFlags" eingeschrieben.
  //         Die Details sind aber eigentlich für den Analyse-Nutzer uninteressant: Im AnalysisManager wirst du ja gefragt "do you need b-Tags", und falls ja sollst du die nacheinander angeben. 
  //         Wenn du in der analyse() Funktion die Funktion "bool checkBTag(Jet whichJet, int whichTag)" aufrufst, wird das intern schon richtig geregelt und du kriegst ein "true", wenn es ein
  //         getaggter Jet ist und "false" sonst. Dabei ist whichTag=0 der erste Tag und whichTag=1 der zweite Tag, wobei "erster" und "zweiter" sich darauf bezieht, in welcher Reihenfolge 
  //         du die b-Effizienzen im AnalysisManager eingegeben hast. Und das ist eigentlich das einzige, was du für die Analyse brauchst.
  //Question: Bei der funktion checkBTag kann man noch ein zweites argument eingeben.  the first b-efficiency defined in CheckMATE is tested with index 0, the second one with 1 etc. ???????
  //Answer:  Ich brauche nur eins(80%), siehe Text oben in dem erklärt wird wofür man mehrere Argumente braucht.
  //Question: Der algorithmus im source code ist ziemlich kurz. Stelle sicher, dass wirklich der MV2c20 algortimus benutzt wird!???
  //Answer:  Es wird kein MV2c20 algorithmus benutzt. Was gemacht wird, wird oben erklärt!
  //Question: Habe die rejection factors 440,8,26 im AnalysisManager nicht mit angegeben! Kann man das überhaupt? Wahrscheinlich angebbar als zweite und dritte bedingung?
  //Answer:  Das brauche ich auch nicht, da das rejection werte für light quarks, c-quarks und tau sind!
  //Question: In einem Absatz weiter unten werden jet overlaps behandelt! Muss ich das auch mit den b jets machen, die ich hier raussuche? Das wird doch später bei signal b jets wichtig!? 
  //Answer:  Ja im prinzip schon, aber ich werde jetzt erläutern, wie ich vorgehe und dann wird klar sein, wieso die Frage sich erübrigt:
  //         Ich definiere erst bjets_80. Wenn man dem algorithmus mit 80% effizienz alle echten bjets gibt wird dieser 80% davon auch als b jets erkennen. Diese b jets werden dann in bjets_80 
  //         geschrieben. Da Delphes/Checkmate ja die echten bjets kennt wird es einfach 80% random rauspicken und in den container bjets_80 schreiben(es werden auch nicht echte b jets gemäß der background
  //         efficiency in diesen container geschrieben). Der container bjets_70 wird dann so konstruiert,
  //         dass bjets_70 < bjets_80 und dass seine elemente mit 70 % effizienz rausgesucht wurden!
  //         Wenn man jetzt die overlap removals macht, dann wird der jet discarded, wenn er dR(j,e)<0.2 hat und nicht in b_jets_80 ist. Da der jet in diesem Fall auch nicht in bjets_70 ist muss man
  //         auch mit diesem container nix machen! Wenn der jet dR(j,e)<0.2 erfüllt und in bjets_80 ist, wird das elektron verworfen un der container bjets_70 bleibt ebenfalls unversehrt.
  vector<Jet*> bjets_80;

  for( int i=0;i<jets.size();i++){
    if(checkBTag(jets[i],0)){//do the 80% efficiency b jet check! 
       bjets_80.push_back(jets[i]);
    }
  }
  
  vector<Jet*> bjets_70;
  for( int i=0;i<jets.size();i++){
    if(checkBTag(jets[i],1)){//do the 70% efficiency b jet check! 
       bjets_70.push_back(jets[i]);
    }
  }

  checkSubset(bjets_70,bjets_80);//Actually checkmate should work in a way that bjets_70 < b_jets_80 automatically, but I include this to be really sure.

  //Comment: dieses Vorgehen ist nicht ganz korrekt, weil bei 70% efficiency vll eine andere background efficiency ist! Deshalb mache das jetzt so wie oben.
  /*vector<Jet*> bjets_70;
  for(int i=0;i<bjets_80.size();i++){
    double zz=rand()/double(RAND_MAX)*0.8;//random number between 0 and 0.8
    if(zz<0.7)
      bjets_70.push_back(bjets_80[i]);
  }*/

  bjets_70=filterPhaseSpace(bjets_70,0.0,-2.5,2.5,false);
  
  //To compensate for differences between data and MC simulation in the
  //b-tagging efficiencies and mis-tag rates, correction factors derived from data-driven methods are applied
  //to the simulated samples [64].
  //Question: Einbauen? paper nochmal auf diesen Bezug hin lesen!?

  //resolve overlaps
  
  //After object identification, overlaps are resolved. Any jet within a distance ∆R = ∆y 2 + ∆φ 2 = 0.2 of
  //an electron candidate is discarded, unless the jet MV2c20 weight is larger than the value corresponding
  //to approximately an 80% b-tagging efficiency, in which case the electron is discarded since it is likely
  //coming from a semileptonic b-hadron decay
  //Comment: We need ∆R_y here, but with the DeltaR() function below we get ∆R_eta. Therefore the ∆R_y is calculated manually
  
  overlapRemoval_jets_electrons(jets,electron_candidates);
  
  //Any remaining electron within dR_y = 0.4 of a jet is discarded

  overlapRemoval_y(electron_candidates,jets, 0.4);

  //Muons within ∆R_y = 0.4 of a jet are also removed. However, if the jet has fewer than three associated
  //tracks, the muon is kept and the jet is discarded instead to avoid inefficiencies for high-energy muons
  //undergoing significant energy loss in the calorimeter.
  
  overlapRemoval_jets_muons(jets,bjets_70,muon_candidates);
  
  //missing p_T is calculated with the objects we have at this stage!

  //2)
  //for(int i=0;i<tracks.size();i++){
  //  for(int j=0;j<electron_candidates.size();j++){
  //    if(tracks[i]->Particle!= electron_candidates[j]->Particle && tracks[i]->Particle== muon_candidates[j]->Particle && tracks[i]->Particle== jet_candidates[j]->Particle){
  //     
  //    }
  //    else if(tracks[i]->Particle== muon_candidates[j]->Particle){
  //    }  
  //  } 
  //}

  //------------------------------------//
  //                                    //
  //                SIGNAL              //
  //                                    //
  //------------------------------------//

  vector<Electron*> signal_electrons=electron_candidates;
  checkSubset(signal_electrons,electronsTight);//if electron_candidates_i is not in set electronsTight it will be deleted.

  vector<Muon*> signal_muons=muon_candidates;

  vector<Jet*> signal_jets=jets;

  vector<Jet*> signal_bjets=bjets_70;

  //Signal electrons must satisfy a tight likelihood-based identification requirement [55, 56] and have |η| < 2
  //to reduce the impact of electron charge mis-identification.

  signal_electrons=filterPhaseSpace(signal_electrons,0.0,-2.0,2.0,false);

  //Signal muons must fulfil the requirement of |d_0 |/σ(d_0) < 3.

  //Isolation requirements are applied to both signal electrons and muons. The scalar sum of the p_T of tracks within a variable-sized
  //cone around the lepton, excluding its own track, must be less than 6% of the lepton p_T . The track isolation
  //cone radius for electrons (muons) is given by the smaller of ∆R = 10 GeV/p_T and ∆R = 0.2 (0.3), that is,
  //a cone of size 0.2 (0.3) at low p T but narrower for high-p T leptons. In addition, in the case of electrons the
  //energy of calorimeter energy clusters in a cone of ∆R = 0.2 around the electron (excluding the deposition
  //from the electron itself) must be less than 6% of the electron p T . Simulated events are corrected to account
  //for minor differences in the lepton trigger, reconstruction and identification efficiencies between data and MC simulation.
  //Question: Letzteen Satz habe ich unbeachtet gelassen!?

  signal_electrons=Isolate_leptons_with_inverse_track_isolation_cone(signal_electrons,tracks, towers,0.2,10.0,0.2,0.06,0.06,true);

  signal_muons=Isolate_leptons_with_inverse_track_isolation_cone(signal_muons,tracks, towers,0.3,10.0,0.2,0.06,0.06,false);

  //Events are selected using a combination (logical OR) of dilepton and E T miss triggers, the latter being used
 //only for events with E T miss > 250 GeV. The trigger-level requirements on E T miss and the leading and
 //subleading lepton p T are looser than those applied offline to ensure that trigger efficiencies are constant
 //in the relevant phase space.
 //Question: Das event muss mindestens eine trigger requirement erfüllen. Soll heißen entweder ETmiss>250 oder eine der lepton reqirements!

 bool etmisstrigger=false;
 bool leptontrigger=true;//TODO!
 if(etmiss>250.0)
   etmisstrigger=true;//fulfills etmiss trigger

  //Events of interest are selected if they contain at least two signal leptons with
  //p_T > 20 GeV. If the event contains exactly two signal leptons, they are required to have the same electric charge.

  int N_leptons=signal_electrons.size()+signal_muons.size();
  int N_leptons_20=0;//Number of leptons with p_T langer than 20 GeV

  for(int i=0;i<signal_electrons.size();i++){
    if(signal_electrons[i]->PT>20.0)
      N_leptons_20++;
  }
  for(int i=0;i<signal_muons.size();i++){
    if(signal_muons[i]->PT>20.0)
      N_leptons_20++;
  }

  bool leptonrequirement=true;

  if(N_leptons_20<2)
    leptonrequirement=false;//does not fulfill lepton requirement
  if(N_leptons==2){//if one has exactly two signal leptons they must have the same sign
    if(signal_electrons.size()==2){
      if(signal_electrons[0]->Charge!=signal_electrons[1]->Charge)
        leptonrequirement=false;
    }
    else if(signal_electrons.size()==1 && signal_muons.size()==1){
      if(signal_electrons[0]->Charge!=signal_muons[0]->Charge)
        leptonrequirement=false;
    }
    else if(signal_muons.size()==2){
      if(signal_muons[0]->Charge!=signal_muons[1]->Charge)
        leptonrequirement=false;
    }
  }

  double m_eff=etmiss;

  for(int i=0;i<signal_jets.size();i++){
    m_eff=m_eff+signal_jets[i]->PT;
  }  
  for(int i=0;i<signal_electrons.size();i++){
    m_eff=m_eff+signal_electrons[i]->PT;
  }  
  for(int i=0;i<signal_muons.size();i++){
    m_eff=m_eff+signal_muons[i]->PT;
  }  

  //Determine the quantities to specify the signal regions!

  int N_bjets_20=0;
  int N_jets_50=0;

  for(int i=0;i<signal_bjets.size();i++){
    if(signal_bjets[i]->PT>20.0)
      N_bjets_20++;
  }
  for(int i=0;i<signal_jets.size();i++){
    if(signal_jets[i]->PT>50.0)
      N_jets_50++;
  }

  //cutflow
  //SR0b3j
  if(N_leptons_20>=2 && N_leptons>=3){
    countCutflowEvent("SR0b3j"+cf_index[1]);
    if(leptontrigger==true || etmisstrigger==true){
      countCutflowEvent("SR0b3j"+cf_index[2]);
      if(N_bjets_20==0){
        countCutflowEvent("SR0b3j"+cf_index[3]);
        if(N_jets_50>=3){
          countCutflowEvent("SR0b3j"+cf_index[4]);
          if(etmiss>=200){
            countCutflowEvent("SR0b3j"+cf_index[5]);
            if(m_eff>=550){
              countCutflowEvent("SR0b3j"+cf_index[6]);
            }
          }
        }
      }
    }
  } 
  //SR0b5j
  if(N_leptons_20>=2 && leptonrequirement){
    countCutflowEvent("SR0b5j"+cf_index[1]);
    if(leptontrigger==true || etmisstrigger==true){
      countCutflowEvent("SR0b5j"+cf_index[2]);
      if(N_bjets_20==0){
        countCutflowEvent("SR0b5j"+cf_index[3]);
        if(N_jets_50>=5){
          countCutflowEvent("SR0b5j"+cf_index[4]);
          if(etmiss>=125){
            countCutflowEvent("SR0b5j"+cf_index[5]);
            if(m_eff>=650){
              countCutflowEvent("SR0b5j"+cf_index[6]);
            }
          }
        }
      }
    }
  } 
    //SR1b
  if(N_leptons_20>=2 && leptonrequirement){
    countCutflowEvent("SR1b"+cf_index[1]);
    if(leptontrigger==true || etmisstrigger==true){
      countCutflowEvent("SR1b"+cf_index[2]);
      if(N_bjets_20>=1){
        countCutflowEvent("SR1b"+cf_index[3]);
        if(N_jets_50>=4){
          countCutflowEvent("SR1b"+cf_index[4]);
          if(etmiss>=150){
            countCutflowEvent("SR1b"+cf_index[5]);
            if(m_eff>=550){
              countCutflowEvent("SR1b"+cf_index[6]);
            }
          }
        }
      }
    }
  } 

    //SR3b
  if(N_leptons_20>=2 && leptonrequirement){
    countCutflowEvent("SR3b"+cf_index[1]);
    if(leptontrigger==true || etmisstrigger==true){
      countCutflowEvent("SR3b"+cf_index[2]);
      if(N_bjets_20>=3){
        countCutflowEvent("SR3b"+cf_index[3]);
        if(etmiss>=125){
          countCutflowEvent("SR3b"+cf_index[4]);
          if(m_eff>=650){
            countCutflowEvent("SR3b"+cf_index[5]);
          }
        }   
      }
    }
  }

  if(leptontrigger==false && etmisstrigger==false){//If none of the two triggers is fulfilled!
    return;
  }
  if(leptonrequirement==false)
    return;

  //sort event in signal region
  if(N_leptons>=3 && N_bjets_20==0 && N_jets_50 >=3 && etmiss>200 && m_eff>550)
    countSignalEvent("SR0b3j");
  if(N_leptons>=2 && N_bjets_20==0 && N_jets_50 >=5 && etmiss>125 && m_eff>650)
    countSignalEvent("SR0b5j");
  if(N_leptons>=2 && N_bjets_20>=1 && N_jets_50 >=4 && etmiss>150 && m_eff>550)
    countSignalEvent("SR1b");
  if(N_leptons>=2 && N_bjets_20>=3 && etmiss>125 && m_eff>650)
    countSignalEvent("SR3b");

}



void Atlas_1602_09058::overlapRemoval_jets_electrons(std::vector<Jet*>& jets,std::vector<Electron*>& electrons){
  vector<bool> del_jets;
  vector<bool> del_electrons;
  //fill bool vectors:
  for(int i=0;i<jets.size();i++){
    del_jets.push_back(false);
  }
  for(int i=0;i<electrons.size();i++){
    del_electrons.push_back(false);
  }
  for(int i=0;i<jets.size();i++){
    for(int j=0;j<electrons.size();j++){
      double y_jet=jets[i]->P4().Rapidity();
      double y_electron=electrons[j]->P4().Rapidity();
      
      double dR_y=sqrt(pow(y_jet-y_electron,2)+pow(jets[i]->P4().DeltaPhi(electrons[j]->P4()),2));
      if (dR_y < 0.2) {
        //bool isbjet=jetBTags.find(jets[i])->second)[0]
        if(checkBTag(jets[i],0)){//Die Null als argument ist richtig, da ich das als 80% effizienz als einzige btagging Bedingung im Analysis Manager angeben werde!
          del_electrons[j]=true; //discard electron, keep jet
        }
        else{
          del_jets[i]=true;//discard jet, keep electron
        } 
      } 
    }
  }
  //delete the corresponding entries in jets and electron candidates
  vector<Jet*> filtered_jets;
  vector<Electron*> filtered_electrons;
  for(int i=0;i<jets.size();i++){
    if(del_jets[i]==false)
      filtered_jets.push_back(jets[i]);
  }
  for(int i=0;i<electrons.size();i++){
    if(del_electrons[i]==false)
      filtered_electrons.push_back(electrons[i]);
  }
  jets=filtered_jets;
  electrons=filtered_electrons;
}


void Atlas_1602_09058::overlapRemoval_jets_muons(std::vector<Jet*>& jets,std::vector<Jet*>& bjets,std::vector<Muon*>& muons){


  vector<Muon*> filtered_muons;
  vector<Jet*> filtered_jets;
  vector<Jet*> filtered_bjets;
  vector<bool> del_muons;
  vector<bool> del_jets;
  vector<bool> del_bjets;
  //fill bool vectors
  for(int i=0;i< muons.size();i++){
    del_muons.push_back(false);
  }
  for(int i=0;i<jets.size();i++){
    del_jets.push_back(false);
  }
  for(int i=0;i<bjets.size();i++){
    del_bjets.push_back(false);
  }
  for(int i=0;i<jets.size();i++){
    for(int j=0;j<muons.size();j++){
      double y_muon=muons[j]->P4().Rapidity();
      double y_jet=jets[i]->P4().Rapidity();
      double dR_y=sqrt(pow(y_muon-y_jet,2)+pow(jets[i]->P4().DeltaPhi(muons[j]->P4()),2));
      
      if(dR_y <0.4){
        if((jets[i]->Particles).GetEntries()<3.0){ //TODO hier bin ich mir unsicher, ob ich so die oben genannte Bedingung mit den Tracks richtig überprüfe!?
          del_jets[i]=true; //discard jet, keep muon 
        }
        else{
          del_muons[j]=true; //discard muon, keep jet
        }
      }
    }
  }
  for(int i=0;i<jets.size();i++){
    if(del_jets[i]==false){
      filtered_jets.push_back(jets[i]);
    }
    else{//if the jet should be discarded check if it is in bjets_70
      for(int j=0;j<bjets.size();j++){//check if the jet which should be discarded is also in bjets_70
        if(bjets[j]==jets[i]){
          del_bjets[j]=true;
        }
      }
    }
  }
  for(int i=0;i<muons.size();i++){
    if(del_muons[i]==false)
      filtered_muons.push_back(muons[i]);
  }
  for(int i=0;i<bjets.size();i++){
    if(del_bjets[i]==false)
      filtered_bjets.push_back(bjets[i]);
  }
  jets=filtered_jets;
  bjets=filtered_bjets;
  muons=filtered_muons;

}

void Atlas_1602_09058::overlapRemoval_y(std::vector<Electron*>& electrons,std::vector<Jet*> jets, double dR){
  std::vector<bool> del_electrons;
  for(int i=0;i<electrons.size();i++){
    del_electrons.push_back(false);
  }
  for(int i=0;i<electrons.size();i++){
    for(int j=0;j<jets.size();j++){
      double y_electron=electrons[i]->P4().Rapidity();
      double y_jet=jets[j]->P4().Rapidity();
      double dR_y=sqrt(pow(y_electron-y_jet,2)+pow(jets[j]->P4().DeltaPhi(electrons[i]->P4()),2));
      if(dR_y<dR){
        del_electrons[i]=true;
      }
    }
  }
  std::vector<Electron*> filtered_electrons;
  for(int i=0;i<del_electrons.size();i++){
    if(del_electrons[i]==false){
      filtered_electrons.push_back(electrons[i]);
    }
  }
  electrons=filtered_electrons;
}

int Atlas_1602_09058::getNumberOfBjets(std::vector<Jet*> check_jets){
  int Nbjets=0;
  for(int i=0;i<check_jets.size();i++){
    if(checkBTag(check_jets[i],0))
      Nbjets++;
  }
  return Nbjets;
}


void Atlas_1602_09058::finalize() {
  // Whatever should be done after the run goes here
}       
