#ifndef _CMS_1303_2985
#define _CMS_1303_2985

#include "AnalysisBase.h"

class Cms_1303_2985 : public AnalysisBase {
  public:
    Cms_1303_2985() : AnalysisBase() {};
    ~Cms_1303_2985() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    bool cut325, cutNorm;
    bool triggerPass275, triggerPass325, triggerPassNorm;
    double HT275, HT325, HT, mHT275, mHT325, mHTNorm;
    double alphaT275, alphaT325, alphaT;
    int bJet275, bJet325, bJetNorm;
    static std::string nj[2];
    static std::string nbj[5];
    static std::string binHT[8];
    
    struct fabs_less { 
      bool operator()(const double x, const double y) const { 
        return fabs(x) < fabs(y); 
      } 
    };
};


class Cms_1303_2985_CR : public AnalysisBase {
  public:
    Cms_1303_2985_CR() : AnalysisBase() {};
    ~Cms_1303_2985_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:        
    bool cut325, cutNorm;
    bool triggerPass275, triggerPass325, triggerPassNorm;
    double HT275, HT325, HT, mHT275, mHT325, mHTNorm;
    double alphaT275, alphaT325, alphaT;
    double triggerRand;
    int bJet275, bJet325, bJetNorm;
    bool crMu, crDiMu, crY;
    static std::string nj [2];
    static std::string nbj [5];
    static std::string binHT [8];
    static std::string binCr [3];
    
    struct fabs_less { 
      bool operator()(const double x, const double y) const { 
        return fabs(x) < fabs(y); 
      } 
    };
};

#endif
