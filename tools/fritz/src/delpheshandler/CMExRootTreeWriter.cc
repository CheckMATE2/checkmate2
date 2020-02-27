
/** \class CMExRootTreeWriter
 *
 *  Class handling output ROOT tree
 *
 *  \author P. Demin - UCL, Louvain-la-Neuve
 *
 */

#include "CMExRootTreeWriter.h"
#include "CMExRootTreeBranch.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

CMExRootTreeWriter::CMExRootTreeWriter(TFile *file, const char *treeName) :
  fFile(file), fTree(0), fTreeName(treeName)
{
}

//------------------------------------------------------------------------------

CMExRootTreeWriter::~CMExRootTreeWriter()
{
  set<CMExRootTreeBranch*>::iterator itBranches;
  for(itBranches = fBranches.begin(); itBranches != fBranches.end(); ++itBranches)
  {
    delete (*itBranches);
  }

  if(fTree) delete fTree;
}

//------------------------------------------------------------------------------

CMExRootTreeBranch *CMExRootTreeWriter::NewBranch(const char *name, TClass *cl)
{
  if(!fTree) fTree = NewTree();
  CMExRootTreeBranch *branch = new CMExRootTreeBranch(name, cl, fTree);
  fBranches.insert(branch);
  return branch;
}

//------------------------------------------------------------------------------

void CMExRootTreeWriter::Fill()
{
  if(fTree) fTree->Fill();
}

//------------------------------------------------------------------------------

void CMExRootTreeWriter::Write()
{
  fFile = fTree ? fTree->GetCurrentFile() : 0;
  if(fFile) fFile->Write();
}

//------------------------------------------------------------------------------

void CMExRootTreeWriter::Clear()
{
  set<CMExRootTreeBranch*>::iterator itBranches;
  for(itBranches = fBranches.begin(); itBranches != fBranches.end(); ++itBranches)
  {
    (*itBranches)->Clear();
  }
}

//------------------------------------------------------------------------------

TTree *CMExRootTreeWriter::NewTree()
{
  if(!fFile) return 0;

  TTree *tree = 0;
  TDirectory *dir = gDirectory;

  fFile->cd();
  tree = new TTree(fTreeName, "Analysis tree");
  dir->cd();

  if(!tree)
  {
    throw runtime_error("can't create output ROOT tree");
  }

  tree->SetDirectory(fFile);
  tree->SetAutoSave(10000000);  // autosave when 10 MB written

  return tree;
}
