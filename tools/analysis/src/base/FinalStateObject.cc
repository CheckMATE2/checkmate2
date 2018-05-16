#include "FinalStateObject.h"

bool FinalStateObject::sortByPT(FinalStateObject *i, FinalStateObject *j) { return (i->PT > j->PT); }