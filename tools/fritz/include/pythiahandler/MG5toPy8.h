#ifndef Pythia8_LHAMadgraph_H
#define Pythia8_LHAMadgraph_H

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/GeneratorInput.h"
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

namespace Pythia8 {

//==========================================================================

// A derived class from LHAup which generates events with MadGraph 5.

// This class automatically generates hard processes with MadGraph 5
// and aMC@NLO, reads in the LHEF file output, and performs matching
// (if requested). For tree-level generation MLM matching is performed
// while FxFx is used for aMC@NLO generation.

// The user can send commands to MadGraph via the readString
// method. Any string begining with "configure " is used for the initial
// MadGraph configuration with "configure " stripped from the
// begining. In general, only the process and run settings need to be
// provided. Run settings must begin with " set" (note the leading
// space). The output and launch commands, random seed, and shower
// choice are automatically handled. For example, the following will
// produce di-muon events from 13 TeV proton proton collisions at NLO
// in QCD:

//    readString("generate p p > mu+ mu- [QCD]")

// The number of events generated per MadGraph run is controlled by
// setEvents, while the random seed is controlled by setSeed. The
// maximum number of jets produced by MadGraph (needed for matching)
// is automatically determined but can be manually specified with
// setJets. In general these methods should not be needed; for further
// details see the method documentation.

// Events are generated with MadGraph utilizing the "gridpack" method
// for MadGraph 5:
//
//    https://cp3.irmp.ucl.ac.be/projects/madgraph/wiki/GridDevelopment
//
// and an eqivalent method for aMC@NLO:
//
//    https://answers.launchpad.net/mg5amcnlo/+question/243268
//
// Consequently the run directory ("madgraphrun" by default) does not
// need to be deleted between independent runs with the same
// configuration (excluding random seeds). Indeed, keeping the
// directory significantly speeds the generation process, particularly
// for NLO generation with aMC@NLO as the grid initialization
// can be skipped after the initial run.

class LHAupMadgraph : public LHAup {

public:

  // Types of MadGraph stages.
    enum Stage{Auto, Configure, Generate, Launch};

  // Constructor.
  LHAupMadgraph(Pythia* pythiaIn, bool matchIn = true,
                string dirIn = "madgraphrun", string exeIn = "mg5_aMC");

  // Destructor.
  ~LHAupMadgraph();

  // Read a MadGraph command string.
  bool readString(string line, Stage stage = Auto);

  // Add a MadGraph configuration card to be used.
  void addCard(string src, string dst);

  // Set the number of events to generate per run.
  void setEvents(int eventsIn);

  // Set the random seed and maximum runs.
  bool setSeed(int seedIn, int runsIn = 30081);

  // Set the initialization information.
  bool setInit();

  // Set the event information.
  bool setEvent(int = 0);

  // Daniel Dercks: Needed for LHE Killer
  inline double getXS() {return xSecSumSave;};

protected:

  // Execute a system command.
  bool execute(string line);

  // Write the MadGraph configuration.
  bool configure();

  // Run the generate stage of MadGraph.
  bool generate();

  // Run the launch stage of MadGraph.
  bool launch();

  // Run MadGraph.
  bool run(int eventsIn, int seedIn = -1);

  // Create the LHEF reader.
  bool reader(bool init);

  // The PYTHIA object and LHEF file reader and matching hook.
  Pythia *pythia;
  LHAupLHEF *lhef;

  // Stored members.
  int events, seed, runs, nRuns, jets;
  bool match, amcatnlo;
  string dir, exe, lhegz;
  double sigWgt, wgt;
  vector< pair<string, string> > cards;

  // The MadGraph commands for the config, generate, and launch stages.
  vector<string> configureLines, generateLines, launchLines;

  // Vector of whether a command stage has been overridden by the user.
  vector<bool> override;

};

// Constructor.

LHAupMadgraph::LHAupMadgraph(Pythia *pythiaIn, bool matchIn, string dirIn,
                             string exeIn) :
  pythia(pythiaIn), lhef(0), events(10000), seed(-1), runs(30081),
  nRuns(0), jets(-1), match(matchIn), dir(dirIn), exe(exeIn),
  lhegz(dirIn + "/Events/run/unweighted_events.lhe"), override(vector<bool>(3, false)) {
  mkdir(dir.c_str(), 0777);
  if (pythia) pythia->readString("Beams:frameType = 5");
}

//--------------------------------------------------------------------------

// Destructor.

LHAupMadgraph::~LHAupMadgraph() {if (lhef) delete lhef;}

//--------------------------------------------------------------------------

// Read a MadGraph command string.

// If the stage is set to Auto, commands beginning with " set" are
// used in the launch stage (these must begin with a single space to
// differentiate from generate stage set commands), commands begining
// with "configure" are used in the configuration stage, and all
// remaining commands (excluding output and launch) are used in the
// generate stage. Output, launch, seed, and shower commands are
// automatically handled. If the user wishes to override commands,
// then the stage can be specified. This will prevent any
// automatically generated commands from being used for that
// stage. This should only be done if the user understands what
// additional commands are needed.

// The system MadGraph configuration will be used if the configuration
// stage is overridden by the user and only blank commands have been
// passed. This is accomplished via
// readString("", LHAupMadgraph::Configure).

bool LHAupMadgraph::readString(string line, Stage stage) {
  if (stage == Auto) {
    if (line.substr(0, 4) == " set") launchLines.push_back(line);
    else if (line.substr(0, 10) == "configure ")
      configureLines.push_back(line.substr(10));
    else if (line.substr(0, 6) != "output" && line.substr(0, 6) != "launch")
      generateLines.push_back(line);
    else return false;
  } else if (stage == Configure) {
    override[Configure] = true; if (line != "") configureLines.push_back(line);
  } else if (stage == Generate) {
    override[Generate] = true; generateLines.push_back(line);
  } else if (stage == Launch) {
    override[Launch] = true; launchLines.push_back(line);
  } else return false;
  return true;
}

//--------------------------------------------------------------------------

// Add a MadGraph configuration card to be used.

// In general, MadGraph should be configured via the readString
// method. However, there are some cases where the user might wish to
// provide an entire configuration card, e.g. setting BSM model
// space. This method allows the user to provide a source card, src,
// which is then copied to <MadGraph run directory>/Cards/dst. These
// cards are copied before any MadGraph processes are launched.

void LHAupMadgraph::addCard(string src, string dst) {
  cards.push_back(make_pair(src, dst));
}

//--------------------------------------------------------------------------

// Set the random seed and maximum allowed runs.

// If the random seed is negative (default of -1), then the MadGraph
// seed is taken as the Pythia parameter "Random:seed", which must be
// greater than 0. If the maximum number of allowed runs is exceeded
// (default of 30081) an error is thrown. The seed for a MadGraph run
// is set as:

//    (random seed - 1) * (maximum runs) + (number of runs) + 1

// MadGraph can only handle random seeds up to 30081 * 30081. So, with
// this strategy, one can generate Pythia jobs with seeds from 1 to
// 30081, with each job running MadGraph less than 30081 times, and
// ensure a fully statistically independent sample. If more than 30081
// jobs are needed, then the maximum allowed runs can be lowered
// accordingly, and if need be, setEvents can be used to increase the
// number of events generated per run.

bool LHAupMadgraph::setSeed(int seedIn, int runsIn) {

  if (!pythia) return false;
  seed = seedIn;
  if (seed < 0) {
    seed = pythia->settings.mode("Random:seed");
    if (seed < 1) {
      pythia->info.errorMsg("Error from LHAupMadgraph::setSeed: the given "
                            "Pythia seed is less than 1."); return false;}
  }
  runs = runsIn;
  if (seed * runs > 30081 * 30081) {
    pythia->info.errorMsg("Error from LHAupMadgraph::setSeed: the given seed "
                          "exceeds the MadGraph limit."); return false;}
  nRuns = 0;
  return true;

}

//--------------------------------------------------------------------------

// Set the number of events to generate per MadGraph run, the default is 10000.

void LHAupMadgraph::setEvents(int eventsIn) {events = eventsIn;}

//--------------------------------------------------------------------------

// Execute a system command.

bool LHAupMadgraph::execute(string line) {return system(line.c_str()) != -1;}

//--------------------------------------------------------------------------

// Write the MadGraph configuration.

// If not overridden, the MadGraph configuration is set to prevent the
// output from being opened in a web-browser and stop MadGraph
// updates.

bool LHAupMadgraph::configure() {

  // if (override[Configure] && configureLines.size() == 0) return true;
  mkdir((dir + "/.mg5").c_str(), 0777);
  fstream config((dir + "/.mg5/mg5_configuration.txt").c_str(), ios::out);
  config << "automatic_html_opening = False\n"
	 << "auto_update = 0\n";
  for (int iLine = 0; iLine < (int)configureLines.size(); ++iLine)
    config << configureLines[iLine] << "\n";
  config.close();
  return true;

}

//--------------------------------------------------------------------------

// Run the generate stage of MadGraph.

// The final command of "output <dir> -f -nojpeg\n" is automatically
// set, if not overridden. MadGraph is then run and the output is
// checked. Finally, the configuration is updated and the type of run,
// MadGraph or aMC@NLO, is determined.

bool LHAupMadgraph::generate() {

  // Write the user settings to the generate configuration file.
  if (!pythia) return false;
  fstream config((dir + "/generate.py").c_str(), ios::out);
  for (int iLine = 0; iLine < (int)generateLines.size(); ++iLine)
    config << generateLines[iLine] << "\n";
  if (!override[Generate]) config << "output " << dir << " -f -nojpeg\n";
  config.close();
  
  // Run MadGraph and check output.
  fstream orig((dir + "/.mg5/mg5_configuration.txt").c_str(), ios::in);
  char *home = getenv("HOME");
  setenv("HOME", dir.c_str(), 1);
  bool success = execute(exe + " " + dir + "/generate.py");
  setenv("HOME", home, 1);
  if (!success) {orig.close(); return false;}
  else if (access((dir + "/Cards/run_card.dat").c_str(), F_OK) == -1) {
    pythia->info.errorMsg("Error from LHAupMadgraph::generate: MadGraph "
                          "failed to produce run_card.dat");
    orig.close(); return false;
  }

  orig.close();

  // Copy over any user provided configuration cards.
  for (int iCard = 0; iCard < (int)cards.size(); ++iCard) {
    ifstream src((cards[iCard].first).c_str(), ios::binary);
    ofstream dst((dir + "/Cards/" + cards[iCard].second).c_str(), ios::binary);
    dst << src.rdbuf();
  }
  return true;

}

//--------------------------------------------------------------------------

// Run the launch stage of MadGraph.

// The first command "launch ..." is automatically set, and depends on
// whether aMC@NLO is being used.

bool LHAupMadgraph::launch() {

  // Open the launch configuration file and write the settings.
  if (!pythia) return false;
  fstream config((dir + "/launch.py").c_str(), ios::out);
  if (!override[Launch]) {
    config << "launch " << dir << " -n run \n";
  }

  // Write the user settings.
  for (int iLine = 0; iLine < (int)launchLines.size(); ++iLine)
    config << launchLines[iLine] << "\n";
  if (!override[Launch]) config << "done\n";
  config.close();

  // BY DANIEL DERCKS: Edit /dir/bin/madevent_interface.py
  //  and make sure that, if defined, random seed is also used in python code to ensure
  //  deterministic results
  if (seed != 0) {
    stringstream ss;
    ss << seed;
    string seedstr = ss.str();
    cout << "Forcing random seed " << seed << " into madevent" << endl;
    if (!execute("sed -i 's/import random/import random\\nrandom.seed("+seedstr+")/' " + dir + "/bin/internal/madevent_interface.py"))
        return false;
  }
  

  
  // Run MadGraph and create run scripts.
  if (!execute(exe + " " + dir + "/launch.py")) return false;
  cout << "Unzipping events" << endl;
  if (!execute("cd " + dir +"/Events/run; gunzip unweighted_events.lhe.gz"))
    return false;
  
  /* fstream script((dir + "/run.sh").c_str(), ios::out); */
  /* script << "#!/usr/bin/env bash\n" */
  /* 	 << "sed -i \"s/.*= *nevents/$1 = nevents/g\" ./Cards/run_card.dat\n" */
  /* 	 << "sed -i \"s/.*= *iseed/$2 = iseed/g\" ./Cards/run_card.dat\n" */
  /* 	 << "./bin/generate_events --parton --nocompile --only_generation " */
  /* 	 << "--force --name run\n"; */
  /* script.close(); execute("chmod 755 " + dir + "/run.sh"); */

  return true;

}

//--------------------------------------------------------------------------

// Run MadGraph.

bool LHAupMadgraph::run(int eventsIn, int seedIn) {

  if (!pythia) return false;
  if (nRuns >= runs) {
    pythia->info.errorMsg("Error from LHAupMadgraph::run: maximum number "
                          "of allowed runs exceeded."); return false;}

  if (access((dir + "/run.sh").c_str(), F_OK) == -1){
    pythia->info.errorMsg("Error from LHAupMadgraph::run: "
                          "Cannot find run.sh."); return false;}

  if (seed < 0 && !setSeed(seed, runs)) {
    pythia->info.errorMsg("Error from LHAupMadgraph::run: "
                          "Cannot set seed."); return false;}
  
  if (seedIn < 0) seedIn = (seed - 1) * runs + nRuns + 1;
  stringstream line;
  line << "cd " + dir + "; ./run.sh " << eventsIn << " " << seedIn;
  if (!execute(line.str())) return false;
  if (access(lhegz.c_str(), F_OK) == -1) {

    pythia->info.errorMsg("Error from LHAupMadgraph::run: "
                          "could not read LHEfile.");
    return false;
  }
  
  ++nRuns;
  return true;

}

//--------------------------------------------------------------------------

// Create the LHEF reader.

bool LHAupMadgraph::reader(bool init) {

  // Check valid LHE file.
  if (!pythia) return false;
  if (lhef) delete lhef;
  bool setScales(pythia->settings.flag("Beams:setProductionScalesFromLHEF"));

  lhef = new LHAupLHEF(&pythia->info, lhegz.c_str(), NULL, false, setScales);
  if (!lhef->setInit()) {
    pythia->info.errorMsg("Error from LHAupMadgraph::reader: failed to "
                          "initialize the LHEF reader"); return false;}
  if (lhef->sizeProc() != 1) {
    pythia->info.errorMsg("Error from LHAupMadgraph::reader: number of "
                          "processes is not 1"); return false;}

  if (init) {

    // Determine the cross-section (if needed).
    double sig(lhef->xSec(0)), err(lhef->xErr(0));

    // Set the info.
    setBeamA(lhef->idBeamA(), lhef->eBeamA(), lhef->pdfGroupBeamA(),
             lhef->pdfSetBeamA());
    setBeamB(lhef->idBeamB(), lhef->eBeamB(), lhef->pdfGroupBeamB(),
             lhef->pdfSetBeamB());
    setStrategy(lhef->strategy());
    addProcess(lhef->idProcess(0), sig, err, lhef->xMax(0));
    xSecSumSave = sig; xErrSumSave = err;
  }
  return true;

}

//--------------------------------------------------------------------------

// Set the initialization information.

bool LHAupMadgraph::setInit() {

  // Initialize MadGraph.
  if (!pythia) return false;
  //  if (access((dir + "/run.sh").c_str(), F_OK) == -1) {
  if (!configure()) {
    pythia->info.errorMsg("Error from LHAupMadgraph::setInit: failed to "
			  "create the MadGraph configuration"); return false;}
  if (!generate()) {
    pythia->info.errorMsg("Error from LHAupMadgraph::setInit: failed to "
			  "generate the MadGraph process"); return false;}
  if (!launch()) {
    pythia->info.errorMsg("Error from LHAupMadgraph::setInit: failed to "
                            "launch the MadGraph process"); return false;}
  //}
  
  // Create the LHEF LHAup object and run setInit.
  //if (!run(events)) return false;
  if (!reader(true)) return false;
  //listInit();
  return true;

}

//--------------------------------------------------------------------------

// Set the event information.

bool LHAupMadgraph::setEvent(int) {

  // Run setEvent from the LHEF object and launch MadGraph if failed.
  if (!pythia) return false;
  if (!lhef) {
    pythia->info.errorMsg("Error from LHAupMadgraph::setEvent: LHAupLHEF "
                          "object not correctly initialized"); return false;}
  if (!lhef->fileFound()) {
    pythia->info.errorMsg("Error from LHAupMadgraph::setEvent: LHEF "
                          "event file was not found"); return false;}
  if (!lhef->setEvent()) {
    if (!run(events)) return false;
    if (!reader(false)) return false;
    lhef->setEvent();
  }

  // Read the event from the LHEF object.
  setProcess(lhef->idProcess(), lhef->weight(), lhef->scale(),
    lhef->alphaQED(), lhef->alphaQCD());
  for (int ip = 1; ip < lhef->sizePart(); ++ip)
    addParticle(lhef->id(ip), lhef->status(ip), lhef->mother1(ip),
                lhef->mother2(ip), lhef->col1(ip), lhef->col2(ip),
                lhef->px(ip), lhef->py(ip), lhef->pz(ip), lhef->e(ip),
                lhef->m(ip), lhef->tau(ip), lhef->spin(ip), lhef->scale(ip));
  setIdX(lhef->id1(), lhef->id2(), lhef->x1(), lhef->x2());
  setPdf(lhef->id1pdf(), lhef->id2pdf(), lhef->x1pdf(), lhef->x2pdf(),
         lhef->scalePDF(), lhef->pdf1(), lhef->pdf2(), lhef->pdfIsSet());
  return true;

}

//==========================================================================

} // end namespace Pythia8

#endif // Pythia8_LHAMadgraph_H
