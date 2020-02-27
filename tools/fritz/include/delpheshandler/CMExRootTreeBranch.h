#ifndef CMExRootTreeBranch_h
#define CMExRootTreeBranch_h

/** \class ExRootTreeBranch
 *
 *  Class handling object creation.
 *  It is also used for output ROOT tree branches
 *
 *  \author P. Demin - UCL, Louvain-la-Neuve
 *
 */

#include "Rtypes.h"

class TTree;
class TClonesArray;

class CMExRootTreeBranch
{
public:

  CMExRootTreeBranch(const char *name, TClass *cl, TTree *tree = 0);
  ~CMExRootTreeBranch();
  
  inline TClonesArray *GetData() {return fData;};
  TObject *NewEntry();
  void Clear();

private:

  Int_t fSize, fCapacity; //!
  TClonesArray *fData; //!
};

#endif /* CMExRootTreeBranch */

