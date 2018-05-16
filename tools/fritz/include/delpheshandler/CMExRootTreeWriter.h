#ifndef CMExRootTreeWriter_h
#define CMExRootTreeWriter_h

/** \class ExRootTreeWriter
 *
 *  Class handling output ROOT tree
 *
 *  \author P. Demin - UCL, Louvain-la-Neuve
 *
 */

#include "TNamed.h"
#include "CMExRootTreeBranch.h"

#include <set>

class TFile;
class TTree;
class TClass;
class ExRootTreeBranch;

class CMExRootTreeWriter : public TNamed
{
public:

  CMExRootTreeWriter(TFile *file = 0, const char *treeName = "Analysis");
  ~CMExRootTreeWriter();

  void SetTreeFile(TFile *file) { fFile = file; }
  void SetTreeName(const char *name) { fTreeName = name; }

  CMExRootTreeBranch *NewBranch(const char *name, TClass *cl);
  inline std::set<CMExRootTreeBranch*> GetBranches() {return fBranches;};

  void Clear();
  void Fill();
  void Write();

private:

  TTree *NewTree();

  TFile *fFile; //!
  TTree *fTree; //!

  TString fTreeName; //!

  std::set<CMExRootTreeBranch*> fBranches; //!

};

#endif /* CMExRootTreeWriter */

