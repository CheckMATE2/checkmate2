// -*- C++ -*-
#ifndef HEPMC_LHEF_H
#define HEPMC_LHEF_H
//
// This is the declaration of the Les Houches Event File classes,
// implementing a simple C++ parser/writer for Les Houches Event files.
// Copyright (C) 2009-2016 Leif Lonnblad
//
// The code is licenced under version 2 of the GPL, see COPYING for
// details.  Please respect the MCnet academic guidelines, see
// http://montecarlonet.org/index.php?p=Publications/Guidelines for
// details.
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <limits>

namespace LHEF {

template <typename T>
struct OAttr {

  OAttr(std::string n, const T & v): name(n), val(v) {}

  std::string name;

  T val;

};

template <typename T>
OAttr<T> oattr(std::string name, const T & value) {
  return OAttr<T>(name, value);
}

template <typename T>
std::ostream & operator<<(std::ostream & os, const OAttr<T> & oa) {
  os << " " << oa.name << "=\"" << oa.val << "\"";
  return os;
}

struct XMLTag {

  typedef std::string::size_type pos_t;

  typedef std::map<std::string,std::string> AttributeMap;

  static const pos_t end = std::string::npos;
  
  XMLTag() {}

  ~XMLTag() {
    for ( int i = 0, N = tags.size(); i < N; ++i ) delete tags[i];
  }

  std::string name;

  AttributeMap attr;

  std::vector<XMLTag*> tags;

  std::string contents;

  bool getattr(std::string n, double & v) const {
    AttributeMap::const_iterator it = attr.find(n);
    if ( it == attr.end() ) return false;
    v = std::atof(it->second.c_str());
    return true;
  }

  bool getattr(std::string n, bool & v) const {
    AttributeMap::const_iterator it = attr.find(n);
    if ( it == attr.end() ) return false;
    if ( it->second == "yes" ) v = true;
    return true;
  }

  bool getattr(std::string n, long & v) const {
    AttributeMap::const_iterator it = attr.find(n);
    if ( it == attr.end() ) return false;
    v = std::atoi(it->second.c_str());
    return true;
  }

  bool getattr(std::string n, int & v) const {
    AttributeMap::const_iterator it = attr.find(n);
    if ( it == attr.end() ) return false;
    v = int(std::atoi(it->second.c_str()));
    return true;
  }

  bool getattr(std::string n, std::string & v) const {
    AttributeMap::const_iterator it = attr.find(n);
    if ( it == attr.end() ) return false;
    v = it->second;
    return true;
  }

  static std::vector<XMLTag*> findXMLTags(std::string str,
                                          std::string * leftover = 0) {
    std::vector<XMLTag*> tags;
    pos_t curr = 0;

    while ( curr != end ) {

      // Find the first tag
      pos_t begin = str.find("<", curr);

      // Check for comments
      if ( str.find("<!--", curr) == begin ) {
        pos_t endcom = str.find("-->", begin);
        tags.push_back(new XMLTag());
        if ( endcom == end ) {
          tags.back()->contents = str.substr(curr);
          if ( leftover ) *leftover += str.substr(curr);
          return tags;
        }
        tags.back()->contents = str.substr(curr, endcom - curr);
        if ( leftover ) *leftover += str.substr(curr, endcom - curr);
        curr = endcom;
        continue;
      }

      if ( begin != curr ) {
        tags.push_back(new XMLTag());
        tags.back()->contents = str.substr(curr, begin - curr);
        if ( leftover ) *leftover += str.substr(curr, begin - curr);
      }
      if ( begin == end || begin > str.length() - 3 || str[begin + 1] == '/' )
        return tags; 

      pos_t close = str.find(">", curr);
      if ( close == end ) return tags;

      // find the tag name.
      curr = str.find_first_of(" \t\n/>", begin);
      tags.push_back(new XMLTag());
      tags.back()->name = str.substr(begin + 1, curr - begin - 1);

      while ( true ) {

        // Now skip some white space to see if we can find an attribute.
        curr = str.find_first_not_of(" \t\n", curr);
        if ( curr == end || curr >= close ) break;

        pos_t tend = str.find_first_of("= \t\n", curr);
        if ( tend == end || tend >= close ) break;

        std::string name = str.substr(curr, tend - curr);
        curr = str.find("=", curr) + 1;

        // OK now find the beginning and end of the atribute.
        curr = str.find_first_of("\"'", curr);
        if ( curr == end || curr >= close ) break;
        char quote = str[curr];
        pos_t bega = ++curr;
        curr = str.find(quote, curr);
        while ( curr != end && str[curr - 1] == '\\' )
          curr = str.find(quote, curr + 1);

        std::string value = str.substr(bega, curr == end? end: curr - bega);

        tags.back()->attr[name] = value;

        ++curr;

      }

      curr = close + 1;
      if ( str[close - 1] == '/' ) continue;

      pos_t endtag = str.find("</" + tags.back()->name + ">", curr);
      if ( endtag == end ) {
        tags.back()->contents = str.substr(curr);
        curr = endtag;
      } else {
        tags.back()->contents = str.substr(curr, endtag - curr);
        curr = endtag + tags.back()->name.length() + 3;
      }

      std::string leftovers;
      tags.back()->tags = findXMLTags(tags.back()->contents, &leftovers);
      if ( leftovers.find_first_not_of(" \t\n") == end ) leftovers="";
      tags.back()->contents = leftovers;

    }
    return tags;

  }

  static void deleteAll(std::vector<XMLTag*> & tags) {
    while ( tags.size() && tags.back() ) {
      delete tags.back();
      tags.pop_back();
    }
  }
  void print(std::ostream & os) const {
    if ( name.empty() ) {
      os << contents;
      return;
    }
    os << "<" << name;
    for ( AttributeMap::const_iterator it = attr.begin();
          it != attr.end(); ++it )
      os << oattr(it->first, it->second);
    if ( contents.empty() && tags.empty() ) {
      os << "/>" << std::endl;
      return;
    }
    os << ">";
    for ( int i = 0, N = tags.size(); i < N; ++i )
      tags[i]->print(os);

    os << contents << "</" << name << ">" << std::endl;
  }

};

inline std::string hashline(std::string s) {
  std::string ret;
  std::istringstream is(s);
  std::string ss;
  while ( getline(is, ss) ) {
    if ( ss.empty() ) continue;
    if ( ss.find_first_not_of(" \t") == std::string::npos ) continue;
    if ( ss.find('#') == std::string::npos ||
         ss.find('#') != ss.find_first_not_of(" \t") ) ss = "# " + ss;
    ret += ss + '\n';
  }
  return ret;
}

struct TagBase {

  typedef XMLTag::AttributeMap AttributeMap;

  TagBase() {}

  TagBase(const AttributeMap & attr, std::string conts = std::string())
    : attributes(attr), contents(conts) {}
 
  bool getattr(std::string n, double & v, bool erase = true) {
    AttributeMap::iterator it = attributes.find(n);
    if ( it == attributes.end() ) return false;
    v = std::atof(it->second.c_str());
    if ( erase) attributes.erase(it);
    return true;
  }

  bool getattr(std::string n, bool & v, bool erase = true) {
    AttributeMap::iterator it = attributes.find(n);
    if ( it == attributes.end() ) return false;
    if ( it->second == "yes" ) v = true;
    if ( erase) attributes.erase(it);
    return true;
  }

  bool getattr(std::string n, long & v, bool erase = true) {
    AttributeMap::iterator it = attributes.find(n);
    if ( it == attributes.end() ) return false;
    v = std::atoi(it->second.c_str());
    if ( erase) attributes.erase(it);
    return true;
  }

  bool getattr(std::string n, int & v, bool erase = true) {
    AttributeMap::iterator it = attributes.find(n);
    if ( it == attributes.end() ) return false;
    v = int(std::atoi(it->second.c_str()));
    if ( erase) attributes.erase(it);
    return true;
  }

  bool getattr(std::string n, std::string & v, bool erase = true) {
    AttributeMap::iterator it = attributes.find(n);
    if ( it == attributes.end() ) return false;
    v = it->second;
    if ( erase) attributes.erase(it);
    return true;
  }

  void printattrs(std::ostream & file) const {
    for ( AttributeMap::const_iterator it = attributes.begin();
          it != attributes.end(); ++ it )
      file << oattr(it->first, it->second);
  }

  void closetag(std::ostream & file, std::string tag) const {
    if ( contents.empty() )
      file << "/>\n";
    else if ( contents.find('\n') != std::string::npos )
      file << ">\n" << contents << "\n</" << tag << ">\n";
    else
      file << ">" << contents << "</" << tag << ">\n";
  }

  XMLTag::AttributeMap attributes;

  std::string contents;

  static std::string yes() { return "yes"; }

};

struct Generator : public TagBase {

  Generator(const XMLTag & tag)
    : TagBase(tag.attr, tag.contents) {
    getattr("name", name);
    getattr("version", version);
  }

  void print(std::ostream & file) const {
    file << "<generator";
    if ( !name.empty() ) file << oattr("name", name);
    if ( !version.empty() ) file << oattr("version", version);
    printattrs(file);
    closetag(file, "generator");
  }

  std::string name;

  std::string version;

};

struct XSecInfo : public TagBase {

  XSecInfo(): neve(-1), totxsec(0.0), maxweight(1.0), meanweight(1.0),
              negweights(false), varweights(false) {}

  XSecInfo(const XMLTag & tag)
    : TagBase(tag.attr, tag.contents), neve(-1), totxsec(0.0),
      maxweight(1.0), meanweight(1.0), negweights(false), varweights(false) {
    if ( !getattr("neve", neve) ) 
      throw std::runtime_error("Found xsecinfo tag without neve attribute "
                               "in Les Houches Event File.");
    if ( !getattr("totxsec", totxsec) ) 
      throw std::runtime_error("Found xsecinfo tag without totxsec "
                               "attribute in Les Houches Event File.");
    getattr("maxweight", maxweight);
    getattr("meanweight", meanweight);
    getattr("negweights", negweights);
    getattr("varweights", varweights);

  }

  void print(std::ostream & file) const {
    file << "<xsecinfo" << oattr("neve", neve) << oattr("totxsec", totxsec)
         << oattr("maxweight", maxweight) << oattr("meanweight", meanweight);
    if ( negweights ) file << oattr("negweights", yes());
    if ( varweights ) file << oattr("varweights", yes());
    printattrs(file);
    closetag(file, "xsecinfo");
  }

  long neve;

  double totxsec;

  double maxweight;

  double meanweight;

  bool negweights;

  bool varweights;


};

struct Cut : public TagBase {

  Cut(): min(-0.99*std::numeric_limits<double>::max()),
         max(0.99*std::numeric_limits<double>::max()) {}

  Cut(const XMLTag & tag,
      const std::map<std::string,std::set<long> >& ptypes)
    : TagBase(tag.attr),
      min(-0.99*std::numeric_limits<double>::max()),
      max(0.99*std::numeric_limits<double>::max()) {
    if ( !getattr("type", type) )
      throw std::runtime_error("Found cut tag without type attribute "
                               "in Les Houches file");
    long tmp;
    if ( tag.getattr("p1", np1) ) {
      if ( ptypes.find(np1) != ptypes.end() ) {
        p1 =  ptypes.find(np1)->second;
        attributes.erase("p1");
      } else {
        getattr("p1", tmp);
        p1.insert(tmp);
        np1 = "";
      }
    }
    if ( tag.getattr("p2", np2) ) {
      if ( ptypes.find(np2) != ptypes.end() ) {
        p2 =  ptypes.find(np2)->second;
        attributes.erase("p2");
      } else {
        getattr("p2", tmp);
        p2.insert(tmp);
        np2 = "";
      }
    }

    std::istringstream iss(tag.contents);
    iss >> min;
    if ( iss >> max ) {
      if ( min >= max )
        min = -0.99*std::numeric_limits<double>::max();
    } else
      max = 0.99*std::numeric_limits<double>::max();
  }

  void print(std::ostream & file) const {
    file << "<cut" << oattr("type", type);
    if ( !np1.empty() )
      file << oattr("p1", np1);
    else
      if ( p1.size() == 1 ) file << oattr("p1", *p1.begin());
    if ( !np2.empty() )
      file << oattr("p2", np2);
    else
      if ( p2.size() == 1 ) file << oattr("p2", *p2.begin());
    printattrs(file);

    file << ">";
    if ( min > -0.9*std::numeric_limits<double>::max() )
      file << min;
    else
      file << max;
    if ( max < 0.9*std::numeric_limits<double>::max() )
      file << " " << max;
    if ( !contents.empty() ) file << std::endl << contents << std::endl;
    file << "</cut>" << std::endl;
  }

  bool match(long id1, long id2 = 0) const {
    std::pair<bool,bool> ret(false, false);
    if ( !id2 ) ret.second = true;
    if ( !id1 ) ret.first = true;
    if ( p1.find(0) != p1.end() ) ret.first = true;
    if ( p1.find(id1) != p1.end() ) ret.first = true;
    if ( p2.find(0) != p2.end() ) ret.second = true;
    if ( p2.find(id2) != p2.end() ) ret.second = true;
    return ret.first && ret.second;
  }

  bool passCuts(const std::vector<long> & id,
                const std::vector< std::vector<double> >& p ) const {
    if ( ( type == "m" && !p2.size() ) || type == "kt" || type == "eta" ||
         type == "y" || type == "E" ) {
      for ( int i = 0, N = id.size(); i < N; ++i )
        if ( match(id[i]) ) {
          if ( type == "m" ) {
            double v = p[i][4]*p[i][4] - p[i][3]*p[i][3] - p[i][2]*p[i][2]
              - p[i][1]*p[i][1];
            v = v >= 0.0? std::sqrt(v): -std::sqrt(-v);
            if ( outside(v) ) return false;
          }
          else if ( type == "kt" ) {
            if ( outside(std::sqrt(p[i][2]*p[i][2] + p[i][1]*p[i][1])) )
              return false;
          }
          else if ( type == "E" ) {
            if ( outside(p[i][4]) ) return false;
          }
          else if ( type == "eta" ) {
            if ( outside(eta(p[i])) ) return false;
          }
          else if ( type == "y" ) {
            if ( outside(rap(p[i])) ) return false;
          }
        }
    }
    else if ( type == "m"  || type == "deltaR" ) {
      for ( int i = 1, N = id.size(); i < N; ++i )
        for ( int j = 0; j < i; ++j )
          if ( match(id[i], id[j]) || match(id[j], id[i]) ) {
            if ( type == "m" ) {
              double v = (p[i][4] + p[j][4])*(p[i][4] + p[j][4])
                - (p[i][3] + p[j][3])*(p[i][3] + p[j][3])
                - (p[i][2] + p[j][2])*(p[i][2] + p[j][2])
                - (p[i][1] + p[j][1])*(p[i][1] + p[j][1]);
              v = v >= 0.0? std::sqrt(v): -std::sqrt(-v);
              if ( outside(v) ) return false;
            }
            else if ( type == "deltaR" ) {
              if ( outside(deltaR(p[i], p[j])) ) return false;
            }
          }
    }
    else if ( type == "ETmiss" ) {
      double x = 0.0;
      double y = 0.0;
      for ( int i = 0, N = id.size(); i < N; ++i )
        if ( match(id[i]) && !match(0, id[i]) ) {
          x += p[i][1];
          y += p[i][2];
        }
      if ( outside(std::sqrt(x*x + y*y)) ) return false;
    }
    else if ( type == "HT" ) {
      double pt = 0.0;
      for ( int i = 0, N = id.size(); i < N; ++i )
        if ( match(id[i]) && !match(0, id[i]) )
          pt += std::sqrt(p[i][1]*p[i][1] + p[i][2]*p[i][2]);
      if ( outside(pt) ) return false;
    }
    return true;
  }

  static double eta(const std::vector<double> & p) {
    double pt2 = p[2]*p[2] + p[1]*p[1];
    if ( pt2 != 0.0 ) {
      double dum = std::sqrt(pt2 + p[3]*p[3]) + p[3];
      if ( dum != 0.0 )
        return std::log(dum/std::sqrt(pt2));
    }
    return p[3] < 0.0? -std::numeric_limits<double>::max():
      std::numeric_limits<double>::max();
  }
    
  static double rap(const std::vector<double> & p) {
    double pt2 = p[5]*p[5] + p[2]*p[2] + p[1]*p[1];
    if ( pt2 != 0.0 ) {
      double dum = std::sqrt(pt2 + p[3]*p[3]) + p[3];
      if ( dum != 0.0 )
        return std::log(dum/std::sqrt(pt2));
    }
    return p[3] < 0.0? -std::numeric_limits<double>::max():
      std::numeric_limits<double>::max();
  }
    
  static double deltaR(const std::vector<double> & p1,
                       const std::vector<double> & p2) {
    double deta = eta(p1) - eta(p2);
    double dphi = std::atan2(p1[1], p1[2]) - std::atan2(p2[1], p2[2]);
    if ( dphi > M_PI ) dphi -= 2.0*M_PI;
    if ( dphi < -M_PI ) dphi += 2.0*M_PI;
    return std::sqrt(dphi*dphi + deta*deta);
  }

  bool outside(double value) const {
    return value < min || value >= max;
  }

  std::string type;

  std::set<long> p1;

  std::string np1;

  std::set<long> p2;

  std::string np2;

  double min;
  double max;

};

struct ProcInfo : public TagBase {

  ProcInfo(): iproc(0), loops(0), qcdorder(-1), eworder(-1) {}

  ProcInfo(const XMLTag & tag)
    : TagBase(tag.attr, tag.contents),
      iproc(0), loops(0), qcdorder(-1), eworder(-1) {
    getattr("iproc", iproc);
    getattr("loops", loops);
    getattr("qcdorder", qcdorder);
    getattr("eworder", eworder);
    getattr("rscheme", rscheme);
    getattr("fscheme", fscheme);
    getattr("scheme", scheme);
  }

  void print(std::ostream & file) const {
    file << "<procinfo" << oattr("iproc", iproc);
    if ( loops >= 0 ) file << oattr("loops", loops);
    if ( qcdorder >= 0 ) file << oattr("qcdorder", qcdorder);
    if ( eworder >= 0 ) file<< oattr("eworder", eworder);
    if ( !rscheme.empty() ) file << oattr("rscheme", rscheme);
    if ( !fscheme.empty() ) file << oattr("fscheme", fscheme);
    if ( !scheme.empty() ) file << oattr("scheme", scheme);
    printattrs(file);
    closetag(file, "procinfo");
  }

  int iproc;

  int loops;

  int qcdorder;

  int eworder;

  std::string fscheme;

  std::string rscheme;

  std::string scheme;

};

struct MergeInfo : public TagBase {

  MergeInfo(): iproc(0), mergingscale(0.0), maxmult(false) {}

  MergeInfo(const XMLTag & tag)
    : TagBase(tag.attr, tag.contents),
      iproc(0), mergingscale(0.0), maxmult(false) {
    getattr("iproc", iproc);
    getattr("mergingscale", mergingscale);
    getattr("maxmult", maxmult);
  }

  void print(std::ostream & file) const {
    file << "<mergeinfo" << oattr("iproc", iproc);
    if ( mergingscale > 0.0 ) file << oattr("mergingscale", mergingscale);
    if ( maxmult ) file << oattr("maxmult", yes());
    printattrs(file);
    closetag(file, "mergeinfo");
  }

  int iproc;

  double mergingscale;

  bool maxmult;

};

struct WeightInfo : public TagBase {

  WeightInfo(): inGroup(-1), isrwgt(false),
                muf(1.0), mur(1.0), pdf(0), pdf2(0) {}

  WeightInfo(const XMLTag & tag)
    : TagBase(tag.attr, tag.contents),
      inGroup(-1), isrwgt(tag.name == "weight"),
      muf(1.0), mur(1.0), pdf(0), pdf2(0) {
    getattr("mur", mur);
    getattr("muf", muf);
    getattr("pdf", pdf);
    getattr("pdf2", pdf2);
    if ( isrwgt )
      getattr("id", name);
    else
      getattr("name",  name);
  }

  void print(std::ostream & file) const {

    if ( isrwgt ) 
      file << "<weight" << oattr("id", name);
    else
      file << "<weightinfo" << oattr("name", name);
    if ( mur != 1.0 ) file << oattr("mur", mur);
    if ( muf != 1.0 ) file << oattr("muf", muf);
    if ( pdf != 0 ) file << oattr("pdf", pdf);
    if ( pdf2 != 0 ) file << oattr("pdf2", pdf2);
    printattrs(file);
    if ( isrwgt )
      closetag(file, "weight");
    else
      closetag(file, "weightinfo");
  }

  int inGroup;

  bool isrwgt;

  std::string name;

  double muf;

  double mur;

  long pdf;

  long pdf2;

};

struct WeightGroup : public TagBase {

  WeightGroup() {}

  WeightGroup(const XMLTag & tag, int groupIndex, std::vector<WeightInfo> & wiv)
    : TagBase(tag.attr) {
    getattr("type", type);
    getattr("combine", combine);
    for ( int i = 0, N = tag.tags.size(); i < N; ++i ) {
      if ( tag.tags[i]->name == "weight" ||
           tag.tags[i]->name == "weightinfo" ) {
        WeightInfo wi(*tag.tags[i]);
        wi.inGroup = groupIndex;
        wiv.push_back(wi);
      }
    }
  }

  std::string type;

  std::string combine;

};


struct Weight : public TagBase {

  Weight(): iswgt(false), born(0.0), sudakov(0.0) {}

  Weight(const XMLTag & tag)
    : TagBase(tag.attr, tag.contents),
      iswgt(tag.name == "wgt"), born(0.0), sudakov(0.0) {
    if ( iswgt )
      getattr("id", name);
    else
      getattr("name", name);
    getattr("born", born);
    getattr("sudakov", sudakov);
    std::istringstream iss(tag.contents);
    double w;
    while ( iss >> w ) weights.push_back(w);
    indices.resize(weights.size(), 0.0);
  }

  void print(std::ostream & file) const {
    if ( iswgt )
      file << "<wgt" << oattr("id", name);
    else {
      file << "<weight";
      if ( !name.empty() ) file << oattr("name", name);
    }
    if ( born != 0.0 ) file << oattr("born", born);
    if ( sudakov != 0.0 ) file << oattr("sudakov", sudakov);
    file << ">";
    for ( int j = 0, M = weights.size(); j < M; ++j ) file << " " << weights[j];
    file << "</weight>" << std::endl;
  }

  std::string name;

  bool iswgt;

  double born;

  double sudakov;

  mutable std::vector<double> weights;

  std::vector<int> indices;

};

struct Clus : public TagBase {

  Clus(): scale(-1.0), alphas(-1.0) {}

  Clus(const XMLTag & tag)
    : TagBase(tag.attr, tag.contents), scale(-1.0), alphas(-1.0) {
    getattr("scale", scale);
    getattr("alphas", alphas);
    std::istringstream iss(tag.contents);
    iss >> p1 >> p2;
    if ( !( iss >> p0 ) ) p0 = p1;
  }

  void print(std::ostream & file) const {
    file << "<clus";
    if ( scale > 0.0 ) file << oattr("scale", scale);
    if ( alphas > 0.0 ) file << oattr("alphas", alphas);
    file << ">" << p1 << " " << p2;
    if ( p1 != p0 ) file << " " << p0;
    file << "</clus>" << std::endl;
  }

  int p1;

  int p2;

  int p0;

  double scale;

  double alphas;

};

struct Scales : public TagBase {

  Scales(double defscale = -1.0)
  : muf(defscale), mur(defscale), mups(defscale), SCALUP(defscale) {}

  Scales(const XMLTag & tag, double defscale = -1.0)
    : TagBase(tag.attr, tag.contents),
      muf(defscale), mur(defscale), mups(defscale), SCALUP(defscale) {
    getattr("muf", muf);
    getattr("mur", mur);
    getattr("mups", mups);
  }

  void print(std::ostream & file) const {
    if ( muf == SCALUP && mur == SCALUP && mups == SCALUP ) return;
    file << "<scales";
    if ( muf != SCALUP ) file << oattr("muf", muf);
    if ( mur != SCALUP ) file << oattr("mur", mur);
    if ( mups != SCALUP ) file << oattr("mups", mups);
    printattrs(file);
    closetag(file, "scales");
  }

  double muf;

  double mur;

  double mups;

  double SCALUP;

};

struct PDFInfo : public TagBase {

  PDFInfo(double defscale = -1.0): p1(0), p2(0), x1(-1.0), x2(-1.0),
             xf1(-1.0),  xf2(-1.0), scale(defscale), SCALUP(defscale) {}

  PDFInfo(const XMLTag & tag, double defscale = -1.0)
    : TagBase(tag.attr, tag.contents),
      p1(0), p2(0), x1(-1.0), x2(-1.0), xf1(-1.0),  xf2(-1.0),
      scale(defscale), SCALUP(defscale) {
    getattr("scale", scale);
    getattr("p1", p1);
    getattr("p2", p2);
    getattr("x1", x1);
    getattr("x2", x2);
  }

  void print(std::ostream & file) const {
    if ( xf1 <= 0 ) return;
    file << "<pdfinfo";
    if ( p1 != 0 ) file << oattr("p1", p1);
    if ( p2 != 0 ) file << oattr("p2", p2);
    if ( x1 > 0 ) file << oattr("x1", x1);
    if ( x2 > 0 ) file << oattr("x2", x2);
    if ( scale != SCALUP ) file << oattr("scale", scale);
    printattrs(file);
    file << ">" << xf1 << " " << xf2 << "</pdfinfo>" << std::endl;
  }

  long p1;

  long p2;

  double x1;

  double x2;

  double xf1;

  double xf2;

  double scale;

  double SCALUP;

};

class HEPRUP : public TagBase {

public:

  HEPRUP()
    : IDWTUP(0), NPRUP(0), version(3) {}

  HEPRUP & operator=(const HEPRUP & x) {
    attributes = x.attributes;
    contents = x.contents;
    IDBMUP = x.IDBMUP;
    EBMUP = x.EBMUP;
    PDFGUP = x.PDFGUP;
    PDFSUP = x.PDFSUP;
    IDWTUP = x.IDWTUP;
    NPRUP = x.NPRUP;
    XSECUP = x.XSECUP;
    XERRUP = x.XERRUP;
    XMAXUP = x.XMAXUP;
    LPRUP = x.LPRUP;
    xsecinfo = x.xsecinfo;
    cuts = x.cuts;
    ptypes = x.ptypes;
    procinfo = x.procinfo;
    mergeinfo = x.mergeinfo;
    generators = x.generators;
    weightgroup = x.weightgroup;
    weightinfo = x.weightinfo;
    junk = x.junk;
    version = x.version;
    weightmap = x.weightmap;
    return *this;
  }

  HEPRUP(const XMLTag & tag, int versin)
    : TagBase(tag.attr, tag.contents), version(versin) {

    std::vector<XMLTag*> tags = tag.tags;
 
    // The first (anonymous) tag should just be the init block.
    std::istringstream iss(tags[0]->contents);
    if ( !( iss >> IDBMUP.first >> IDBMUP.second >> EBMUP.first >> EBMUP.second
            >> PDFGUP.first >> PDFGUP.second >> PDFSUP.first >> PDFSUP.second
            >> IDWTUP >> NPRUP ) ) {
      throw std::runtime_error("Could not parse init block "
                               "in Les Houches Event File.");
    }
    resize();

    for ( int i = 0; i < NPRUP; ++i ) {
      if ( !( iss >> XSECUP[i] >> XERRUP[i] >> XMAXUP[i] >> LPRUP[i] ) ) {
      throw std::runtime_error("Could not parse processes in init block "
                               "in Les Houches Event File.");
      }
    }

    for ( int i = 1, N = tags.size(); i < N; ++i ) {
      const XMLTag & tag = *tags[i];

      if ( tag.name.empty() ) junk += tag.contents;

      if ( tag.name == "initrwgt" ) {
        for ( int j = 0, M = tag.tags.size(); j < M; ++j ) {
          if ( tag.tags[j]->name == "weightgroup" )
            weightgroup.push_back(WeightGroup(*tag.tags[j], weightgroup.size(),
                                              weightinfo));
          if ( tag.tags[j]->name == "weight" )
            weightinfo.push_back(WeightInfo(*tag.tags[j]));
          
        }
      }
      if ( tag.name == "weightinfo" ) {
        weightinfo.push_back(WeightInfo(tag));
      }
      if ( tag.name == "weightgroup" ) {
        weightgroup.push_back(WeightGroup(tag, weightgroup.size(),
                                          weightinfo));
      }
      if ( tag.name == "xsecinfo" ) {
        xsecinfo = XSecInfo(tag);
      }
      if ( tag.name == "generator" ) {
        generators.push_back(Generator(tag));
      }
      else if ( tag.name == "cutsinfo" ) {
        for ( int j = 0, M = tag.tags.size(); j < M; ++j ) {
          XMLTag & ctag = *tag.tags[j];
          
          if ( ctag.name == "ptype" ) {
            std::string tname = ctag.attr["name"];
            long id;
            std::istringstream iss(ctag.contents);
            while ( iss >> id ) ptypes[tname].insert(id);
          }
          else if ( ctag.name == "cut" )
            cuts.push_back(Cut(ctag, ptypes));
        }
      }
      else if ( tag.name == "procinfo" ) {
        ProcInfo proc(tag);
        procinfo[proc.iproc] = proc;
      }
      else if ( tag.name == "mergeinfo" ) {
        MergeInfo merge(tag);
        mergeinfo[merge.iproc] = merge;
      }

    }

    weightmap.clear();
    for ( int i = 0, N = weightinfo.size(); i < N; ++i )
      weightmap[weightinfo[i].name] = i + 1;

  }

  ~HEPRUP() {}

public:

  std::string weightNameHepMC(int i) const {
    std::string name;
    if ( i < 0 || i >= (int)weightinfo.size() ) return name;
    if ( weightinfo[i].inGroup >= 0 )
      name = weightgroup[weightinfo[i].inGroup].type + "/"
        +  weightgroup[weightinfo[i].inGroup].combine + "/";
    name += weightinfo[i].name;
    return name;
  }


  void print(std::ostream & file) const {

    using std::setw;

    file << "<init>\n"
         << " " << setw(8) << IDBMUP.first
         << " " << setw(8) << IDBMUP.second
         << " " << setw(14) << EBMUP.first
         << " " << setw(14) << EBMUP.second
         << " " << setw(4) << PDFGUP.first
         << " " << setw(4) << PDFGUP.second
         << " " << setw(4) << PDFSUP.first
         << " " << setw(4) << PDFSUP.second
         << " " << setw(4) << IDWTUP
         << " " << setw(4) << NPRUP << std::endl;

    for ( int i = 0; i < NPRUP; ++i )
      file << " " << setw(14) << XSECUP[i]
           << " " << setw(14) << XERRUP[i]
           << " " << setw(14) << XMAXUP[i]
           << " " << setw(6) << LPRUP[i] << std::endl;

    for ( int i = 0, N = generators.size(); i < N; ++i )
      generators[i].print(file);

    if ( xsecinfo.neve > 0 ) xsecinfo.print(file);

    if ( cuts.size() > 0 ) {
      file << "<cutsinfo>" << std::endl;
    
      for ( std::map<std::string, std::set<long> >::const_iterator ptit =
              ptypes.begin(); ptit !=  ptypes.end(); ++ptit ) {
        file << "<ptype" << oattr("name", ptit->first) << ">";
        for ( std::set<long>::const_iterator it = ptit->second.begin();
              it != ptit->second.end(); ++it )
          file << " " << *it;
        file << "</ptype>" << std::endl;
      }

      for ( int i = 0, N = cuts.size(); i < N; ++i )
        cuts[i].print(file);
      file << "</cutsinfo>" << std::endl;
    }

    for ( std::map<long,ProcInfo>::const_iterator it = procinfo.begin();
          it != procinfo.end(); ++it )
      it->second.print(file);

    for ( std::map<long,MergeInfo>::const_iterator it = mergeinfo.begin();
          it != mergeinfo.end(); ++it )
      it->second.print(file);

    bool isrwgt = false;
    int ingroup = -1;
    for ( int i = 0, N = weightinfo.size(); i < N; ++i ) {
      if ( weightinfo[i].isrwgt ) {
        if ( !isrwgt ) file << "<initrwgt>\n";
        isrwgt = true;
      } else {
        if ( isrwgt ) file << "</initrwgt>\n";
        isrwgt = false;
      }
      int group = weightinfo[i].inGroup;
      if ( group != ingroup ) {
        if ( ingroup != -1 ) file << "</weightgroup>\n";
        if ( group != -1 ) {
          file << "<weightgroup"
               << oattr("type", weightgroup[group].type);
          if ( !weightgroup[group].combine.empty() )
            file << oattr("combine", weightgroup[group].combine);
          file << ">\n";
        }
        ingroup = group;
      }
      weightinfo[i].print(file);
    }
    if ( ingroup != -1 ) file << "</weightgroup>\n";
    if ( isrwgt ) file << "</initrwgt>\n";


    file << hashline(junk) << "</init>" << std::endl;

  }

  void clear() {
    procinfo.clear();
    mergeinfo.clear();
    weightinfo.clear();
    weightgroup.clear();
    cuts.clear();
    ptypes.clear();
    junk.clear();
  }

  void resize(int nrup) {
    NPRUP = nrup;
    resize();
  }

  void resize() {
    XSECUP.resize(NPRUP);
    XERRUP.resize(NPRUP);
    XMAXUP.resize(NPRUP);
    LPRUP.resize(NPRUP);
  }

  int weightIndex(std::string name) const {
    std::map<std::string, int>::const_iterator it = weightmap.find(name);
    if ( it != weightmap.end() ) return it->second;
    return 0;
  }

  int nWeights() const {
    return weightmap.size() + 1;
  }

public:

  std::pair<long,long> IDBMUP;

  std::pair<double,double> EBMUP;

  std::pair<int,int> PDFGUP;

  std::pair<int,int> PDFSUP;

  int IDWTUP;

  int NPRUP;

  std::vector<double> XSECUP;

  std::vector<double> XERRUP;

  std::vector<double> XMAXUP;

  std::vector<int> LPRUP;

  XSecInfo xsecinfo;

  std::vector<Cut> cuts;

  std::map<std::string, std::set<long> > ptypes;

  std::map<long,ProcInfo> procinfo;

  std::map<long,MergeInfo> mergeinfo;

  std::vector<Generator> generators;

  std::vector<WeightInfo> weightinfo;

  std::map<std::string,int> weightmap;

  std::vector<WeightGroup> weightgroup;

  std::string junk;

  int version;

};

class HEPEUP;

struct EventGroup: public std::vector<HEPEUP*> {

  inline EventGroup(): nreal(-1), ncounter(-1) {}

  inline EventGroup(const EventGroup &);

  inline EventGroup & operator=(const EventGroup &);

  inline void clear();

  inline ~EventGroup();

  int nreal;

  int ncounter;

};


class HEPEUP : public TagBase {

public:

  HEPEUP()
    : NUP(0), IDPRUP(0), XWGTUP(0.0), XPDWUP(0.0, 0.0),
      SCALUP(0.0), AQEDUP(0.0), AQCDUP(0.0), heprup(0), currentWeight(0),
      isGroup(false) {}

  HEPEUP(const HEPEUP & x)
    : TagBase(x), isGroup(false) {
    operator=(x);
  }

  HEPEUP & setEvent(const HEPEUP & x) {
    NUP = x.NUP;
    IDPRUP = x.IDPRUP;
    XWGTUP = x.XWGTUP;
    XPDWUP = x.XPDWUP;
    SCALUP = x.SCALUP;
    AQEDUP = x.AQEDUP;
    AQCDUP = x.AQCDUP;
    IDUP = x.IDUP;
    ISTUP = x.ISTUP;
    MOTHUP = x.MOTHUP;
    ICOLUP = x.ICOLUP;
    PUP = x.PUP;
    VTIMUP = x.VTIMUP;
    SPINUP = x.SPINUP;
    heprup = x.heprup;
    namedweights = x.namedweights;
    weights = x.weights;
    pdfinfo = x.pdfinfo;
    PDFGUPsave = x.PDFGUPsave;
    PDFSUPsave = x.PDFSUPsave;
    clustering = x.clustering;
    scales = x.scales;
    junk = x.junk;
    currentWeight = x.currentWeight;
    return *this;
  }

  HEPEUP & operator=(const HEPEUP & x) {
    clear();
    setEvent(x);
    subevents = x.subevents;
    isGroup = x.isGroup;
    return *this;
  }

  ~HEPEUP() {
    clear();
  };

public:


  HEPEUP(const XMLTag & tag, HEPRUP & heprupin)
    : TagBase(tag.attr), NUP(0), IDPRUP(0), XWGTUP(0.0), XPDWUP(0.0, 0.0),
      SCALUP(0.0), AQEDUP(0.0), AQCDUP(0.0), heprup(&heprupin),
      currentWeight(0), isGroup(tag.name == "eventgroup") {

    if ( heprup->NPRUP < 0 )
      throw std::runtime_error("Tried to read events but no processes defined "
                               "in init block of Les Houches file.");

    std::vector<XMLTag*> tags = tag.tags;

    if ( isGroup ) {
      getattr("nreal", subevents.nreal);
      getattr("ncounter", subevents.ncounter);
      for ( int i = 0, N = tags.size(); i < N; ++i )
        if ( tags[i]->name == "event" )
          subevents.push_back(new HEPEUP(*tags[i], heprupin));
      return;
    }
      


    // The event information should be in the first (anonymous) tag
    std::istringstream iss(tags[0]->contents);
    if ( !( iss >> NUP >> IDPRUP >> XWGTUP >> SCALUP >> AQEDUP >> AQCDUP ) )
      throw std::runtime_error("Failed to parse event in Les Houches file.");

    resize();

    // Read all particle lines.
    for ( int i = 0; i < NUP; ++i ) {
      if ( !( iss >> IDUP[i] >> ISTUP[i] >> MOTHUP[i].first >> MOTHUP[i].second
                  >> ICOLUP[i].first >> ICOLUP[i].second
                  >> PUP[i][0] >> PUP[i][1] >> PUP[i][2]
                  >> PUP[i][3] >> PUP[i][4]
                  >> VTIMUP[i] >> SPINUP[i] ) )
        throw std::runtime_error("Failed to parse event in Les Houches file.");
    }

    junk.clear();
    std::string ss;
    while ( getline(iss, ss) ) junk += ss + '\n';
    
    scales = Scales(SCALUP);
    pdfinfo = PDFInfo(SCALUP);
    namedweights.clear();
    weights.clear();
    weights.resize(heprup->nWeights(),
                   std::make_pair(XWGTUP, (WeightInfo*)(0)));
    weights.front().first = XWGTUP;
    for ( int i = 1, N = weights.size(); i < N; ++i ) 
      weights[i].second =  &heprup->weightinfo[i - 1];

    for ( int i = 1, N = tags.size(); i < N; ++i ) {
      XMLTag & tag = *tags[i];

      if ( tag.name.empty() ) junk += tag.contents;

      if ( tag.name == "weights" ) {
        weights.resize(heprup->nWeights(),
                       std::make_pair(XWGTUP, (WeightInfo*)(0)));
        weights.front().first = XWGTUP;
        for ( int i = 1, N = weights.size(); i < N; ++i ) 
          weights[i].second =  &heprup->weightinfo[i - 1];
        double w = 0.0;
        int i = 0;
        std::istringstream iss(tag.contents);
        while ( iss >> w )
          if ( ++i < int(weights.size()) )
            weights[i].first = w;
          else
            weights.push_back(std::make_pair(w, (WeightInfo*)(0)));
      }
      if ( tag.name == "weight" ) {
        namedweights.push_back(Weight(tag));
      }
      if ( tag.name == "rwgt" ) {
        for ( int j = 0, M = tag.tags.size(); j < M; ++j ) {
          if ( tag.tags[j]->name == "wgt" ) {
            namedweights.push_back(Weight(*tag.tags[j]));
          }
        }
      }
      else if ( tag.name == "clustering" ) {
        for ( int j = 0, M= tag.tags.size(); j < M; ++j ) {
          if ( tag.tags[j]->name == "clus" )
            clustering.push_back(Clus(*tag.tags[j]));
        }
      }
      else if ( tag.name == "pdfinfo" ) {
        pdfinfo = PDFInfo(tag, SCALUP);
      }
      else if ( tag.name == "scales" ) {
        scales = Scales(tag, SCALUP);
      }

    }

    for ( int i = 0, N = namedweights.size(); i < N; ++i ) {
      int indx = heprup->weightIndex(namedweights[i].name);
      if ( indx > 0 ) {
        weights[indx].first = namedweights[i].weights[0];
        namedweights[i].indices[0] = indx;
      } else {
        weights.push_back(std::make_pair(namedweights[i].weights[0],
                                         (WeightInfo*)(0)));
        namedweights[i].indices[0] = weights.size() - 1;
      }
      for ( int j = 1, M = namedweights[i].weights.size(); j < M; ++j ) {
        weights.push_back(std::make_pair(namedweights[i].weights[j],
                                         (WeightInfo*)(0)));
        namedweights[i].indices[j] = weights.size() - 1;
      }
    }

  }

  void print(std::ostream & file) const {

    using std::setw;

    if ( isGroup ) {
      file << "<eventgroup";
      if ( subevents.nreal > 0 )
        file << oattr("nreal", subevents.nreal);
      if ( subevents.ncounter > 0 )
        file << oattr("ncounter", subevents.ncounter);
      printattrs(file);
      file << ">\n";
      for ( int i = 0, N = subevents.size(); i < N; ++i )
        subevents[i]->print(file);
      file << "</eventgroup>\n";
      return;
    }

    file << "<event";
    printattrs(file);
    file << ">\n";
    file << " " << setw(4) << NUP
         << " " << setw(6) << IDPRUP
         << " " << setw(14) << XWGTUP
         << " " << setw(14) << SCALUP
         << " " << setw(14) << AQEDUP
         << " " << setw(14) << AQCDUP << "\n";

    for ( int i = 0; i < NUP; ++i )
      file << " " << setw(8) << IDUP[i]
           << " " << setw(2) << ISTUP[i]
           << " " << setw(4) << MOTHUP[i].first
           << " " << setw(4) << MOTHUP[i].second
           << " " << setw(4) << ICOLUP[i].first
           << " " << setw(4) << ICOLUP[i].second
           << " " << setw(14) << PUP[i][0]
           << " " << setw(14) << PUP[i][1]
           << " " << setw(14) << PUP[i][2]
           << " " << setw(14) << PUP[i][3]
           << " " << setw(14) << PUP[i][4]
           << " " << setw(1) << VTIMUP[i]
           << " " << setw(1) << SPINUP[i] << std::endl;

      if ( weights.size() > 0 ) {
        file << "<weights>";
        for ( int i = 1, N = weights.size(); i < N; ++i )
          file << " " << weights[i].first;
        file << "</weights>\n";
      }

      bool iswgt = false;
      for ( int i = 0, N = namedweights.size(); i < N; ++i ) {
        if ( namedweights[i].iswgt ) {
          if ( !iswgt ) file << "<rwgt>\n";
          iswgt = true;
        } else {
          if ( iswgt ) file << "</rwgt>\n";
          iswgt = false;
        }
        for ( int j = 0, M = namedweights[i].indices.size(); j < M; ++j )
          namedweights[i].weights[j] = weight(namedweights[i].indices[j]);
        namedweights[i].print(file);
      }
      if ( iswgt ) file << "</rwgt>\n";

      if ( !clustering.empty() ) {
        file << "<clustering>" << std::endl;
        for ( int i = 0, N = clustering.size(); i < N; ++i )
          clustering[i].print(file);
        file << "</clustering>" << std::endl;   
      }

      pdfinfo.print(file);
      scales.print(file);

      //    }

    file << hashline(junk) << "</event>\n";

  }

  void reset() {
    setWeightInfo(0);
    NUP = 0;
    clustering.clear();
    weights.clear();
  }

  void clear() {
    reset();
    subevents.clear();
  }

  void resize(int nup) {
    NUP = nup;
    resize();
  }

  double totalWeight(int i = 0) const {
    if ( subevents.empty() ) return weight(i);
    double w = 0.0;
    for ( int i = 0, N = subevents.size(); i < N; ++i )
      w += subevents[i]->weight(i);
    return w;
  }

  double totalWeight(std::string name) const {
    return totalWeight(heprup->weightIndex(name));
  }

  double weight(int i = 0) const {
    return weights[i].first;
  }

  double weight(std::string name) const {
    return weight(heprup->weightIndex(name));
  }

  void setWeight(int i, double w) {
    weights[i].first = w;
  }
  bool setWeight(std::string name, double w) {
    int i = heprup->weightIndex(name);
    if ( i >= int(weights.size()) ) return false;
    setWeight(i, w);
    return true;
  }


  void resize() {
    IDUP.resize(NUP);
    ISTUP.resize(NUP);
    MOTHUP.resize(NUP);
    ICOLUP.resize(NUP);
    PUP.resize(NUP, std::vector<double>(5));
    VTIMUP.resize(NUP);
    SPINUP.resize(NUP);
  }

  bool setWeightInfo(unsigned int i) {
    if ( i >= weights.size() ) return false;
    if ( currentWeight ) {
      scales.mur /= currentWeight->mur;
      scales.muf /= currentWeight->muf;
      heprup->PDFGUP = PDFGUPsave;
      heprup->PDFSUP = PDFSUPsave;
    }
    XWGTUP = weights[i].first;
    currentWeight = weights[i].second;
    if ( currentWeight ) {
      scales.mur *= currentWeight->mur;
      scales.muf *= currentWeight->muf;
      PDFGUPsave = heprup->PDFGUP;
      PDFSUPsave = heprup->PDFSUP;
      if ( currentWeight->pdf ) {
        heprup->PDFGUP.first =  heprup->PDFGUP.second = 0;
        heprup->PDFSUP.first =  heprup->PDFSUP.second = currentWeight->pdf;
      }
      if ( currentWeight->pdf2 ) {
        heprup->PDFSUP.second = currentWeight->pdf2;
      }

    }
    return true;
  }

  bool setSubEvent(unsigned int i) {
    if ( i > subevents.size() || subevents.empty() ) return false;
    if ( i == 0 ) {
      reset();
      weights = subevents[0]->weights;
      for ( int i = 1, N = subevents.size(); i < N; ++i )
        for ( int j = 0, M = weights.size(); j < M; ++j )
          weights[j].first += subevents[i]->weights[j].first;
      currentWeight = 0;
    } else {
      setEvent(*subevents[i - 1]);
    }
    return true;
  }

public:

  int NUP;

  int IDPRUP;

  double XWGTUP;

  std::pair<double,double> XPDWUP;

  double SCALUP;

  double AQEDUP;

  double AQCDUP;

  std::vector<long> IDUP;

  std::vector<int> ISTUP;

  std::vector< std::pair<int,int> > MOTHUP;

  std::vector< std::pair<int,int> > ICOLUP;

  std::vector< std::vector<double> > PUP;

  std::vector<double> VTIMUP;

  std::vector<double> SPINUP;

  HEPRUP * heprup;

  const WeightInfo * currentWeight;

  std::vector<Weight> namedweights;

  std::vector< std::pair<double, const WeightInfo *> > weights;

  std::vector<Clus> clustering;

  PDFInfo pdfinfo;

  std::pair<int,int> PDFGUPsave;

  std::pair<int,int> PDFSUPsave;
  

  Scales scales;

  bool isGroup;

  EventGroup subevents;

  std::string junk;

};


// Destructor implemented here.

inline void EventGroup::clear() {
  while ( size() > 0 ) {
    delete back();
    pop_back();
  }
}

inline EventGroup::~EventGroup() {
  clear();
}

inline EventGroup::EventGroup(const EventGroup & eg)
  : std::vector<HEPEUP*>(eg.size()) {
  for ( int i = 0, N = eg.size(); i < N; ++i ) at(i) = new HEPEUP(*eg.at(i));
}

inline EventGroup & EventGroup::operator=(const EventGroup & x) {
  if ( &x == this ) return *this;
  clear();
  nreal = x.nreal;
  ncounter = x.ncounter;
  for ( int i = 0, N = x.size(); i < N; ++i ) push_back(new HEPEUP(*x.at(i)));
  return *this;
}


class Reader {

public:

  Reader(std::istream & is)
    : file(is) {
    init();
  }

  Reader(std::string filename)
    : intstream(filename.c_str()), file(intstream) {
    init();
  }

private:

  void init() {

    bool readingHeader = false;
    bool readingInit = false;

    // Make sure we are reading a LHEF file:
    getline();
    if ( !currentFind("<LesHouchesEvents") )
      throw std::runtime_error
        ("Tried to read a file which does not start with the "
         "LesHouchesEvents tag.");
    version = 1;
    if ( currentFind("version=\"3" ) )
      version = 3;
    else if ( currentFind("version=\"2" ) )
      version = 2;
    else if ( !currentFind("version=\"1" ) )
      throw std::runtime_error
        ("Tried to read a LesHouchesEvents file which is above version 3.");

    // Loop over all lines until we hit the </init> tag.
    while ( getline() && !currentFind("</init>") ) {
      if ( currentFind("<header") ) {
        // We have hit the header block, so we should dump this and
        // all following lines to headerBlock until we hit the end of
        // it.
        readingHeader = true;
        headerBlock = currentLine + "\n";
      }
      else if ( currentFind("<init>") ) {
        // We have hit the init block
        readingInit = true;
        initComments = currentLine + "\n";
      }
      else if ( currentFind("</header>") ) {
        // The end of the header block. Dump this line as well to the
        // headerBlock and we're done.
        readingHeader = false;
        headerBlock += currentLine + "\n";
      }
      else if ( readingHeader ) {
        // We are in the process of reading the header block. Dump the
        // line to haderBlock.
        headerBlock += currentLine + "\n";
      }
      else if ( readingInit ) {
        // Here we found a comment line. Dump it to initComments.
        initComments += currentLine + "\n";
      }
      else {
        // We found some other stuff outside the standard tags.
        outsideBlock += currentLine + "\n";
      }
    }
    if ( !currentFind("</init>") )
        throw std::runtime_error("Found incomplete init tag in "
                                 "Les Houches file.");
    initComments += currentLine + "\n";
    std::vector<XMLTag*> tags = XMLTag::findXMLTags(initComments);
    for ( int i = 0, N = tags.size(); i < N; ++i )
      if ( tags[i]->name == "init" ) {
        heprup = HEPRUP(*tags[i], version);
        break;
      }
    XMLTag::deleteAll(tags);
 
  }

public:

  bool readEvent() {

    // Check if the initialization was successful. Otherwise we will
    // not read any events.
    if ( heprup.NPRUP < 0 ) return false;

    std::string eventLines;
    int inEvent = 0;;

    // Keep reading lines until we hit the end of an event or event group.
    while ( getline() ) {
      if ( inEvent ) {
        eventLines += currentLine + "\n";
        if ( inEvent == 1 && currentFind("</event>") ) break;
        if ( inEvent == 2 && currentFind("</eventgroup>") ) break;
      }
      else if ( currentFind("<eventgroup") ) {
        eventLines += currentLine + "\n";
        inEvent = 2;
      }
      else if ( currentFind("<event") ) {
        eventLines += currentLine + "\n";
        inEvent = 1;
      }
      else {
        outsideBlock += currentLine + "\n";
      }
    }
    if ( inEvent == 1 && !currentFind("</event>") ) return false;
    if ( inEvent == 2 && !currentFind("</eventgroup>") ) return false;

    std::vector<XMLTag*> tags = XMLTag::findXMLTags(eventLines);

    for ( int i = 0, N = tags.size(); i < N ; ++i ) {
      if ( tags[i]->name == "event" || tags[i]->name == "eventgroup" ) {
        hepeup = HEPEUP(*tags[i], heprup);
        XMLTag::deleteAll(tags);
        return true;
      }
    }

    XMLTag::deleteAll(tags);
    return false;

  }

protected:

  bool getline() {
    return ( (bool)std::getline(file, currentLine) );
  }

  bool currentFind(std::string str) const {
    return currentLine.find(str) != std::string::npos;
  }

protected:

  std::ifstream intstream;

  std::istream & file;

  std::string currentLine;

public:

  int version;

  std::string outsideBlock;

  std::string headerBlock;

  HEPRUP heprup;

  std::string initComments;

  HEPEUP hepeup;

  std::string eventComments;

private:

  Reader();

  Reader(const Reader &);

  Reader & operator=(const Reader &);

};

class Writer {

public:

  Writer(std::ostream & os)
    : file(os) {  }

  Writer(std::string filename)
    : intstream(filename.c_str()), file(intstream) {}

  ~Writer() {
    file << "</LesHouchesEvents>" << std::endl;
  }

  std::ostream & headerBlock() {
    return headerStream;
  }

  std::ostream & initComments() {
    return initStream;
  }

  std::ostream & eventComments() {
    return eventStream;
  }

  void init() {

    // Write out the standard XML tag for the event file.
    if ( heprup.version == 3 )
      file << "<LesHouchesEvents version=\"3.0\">\n";
    else if ( heprup.version == 2 )
      file << "<LesHouchesEvents version=\"2.0\">\n";
    else
      file << "<LesHouchesEvents version=\"1.0\">\n";


    file << std::setprecision(8);

    using std::setw;

    std::string headerBlock = headerStream.str();
    if ( headerBlock.length() ) {
      if ( headerBlock.find("<header>") == std::string::npos )
        file << "<header>\n";
      if ( headerBlock[headerBlock.length() - 1] != '\n' )
        headerBlock += '\n';
      file << headerBlock;
      if ( headerBlock.find("</header>") == std::string::npos )
        file << "</header>\n";
    }

    heprup.print(file);

  }

  void writeEvent() {
    hepeup.print(file);
  }
      
protected:

  std::ofstream intstream;

  std::ostream & file;

public:

  std::ostringstream headerStream;

  HEPRUP heprup;

  std::ostringstream initStream;

  HEPEUP hepeup;

  std::ostringstream eventStream;

private:

  Writer();

  Writer(const Writer &);

  Writer & operator=(const Writer &);

};

}

/* \example LHEFCat.cc This is a main function which simply reads a
    Les Houches Event File from the standard input and writes it again
    to the standard output. 
    This file can be downloaded from
    <A HREF="http://www.thep.lu.se/~leif/LHEF/LHEFCat.cc">here</A>. 
    There are also two sample event files,
    <A HREF="http://www.thep.lu.se/~leif/LHEF/ttV_unweighted_events.lhe">ttV_unweighted_events.lhe</A> and <A HREF="http://www.thep.lu.se/~leif/LHEF/testlhef3.lhe">testlhef3.lhe</A>
    to try it on.
*/

/* \mainpage Les Houches Event File

Here are some example classes for reading and writing Les Houches
Event Files according to the
<A HREF="http://www.thep.lu.se/~torbjorn/lhef/lhafile2.pdf">proposal</A>
by Torbj&ouml;rn Sj&ouml;strand discussed at the
<A HREF="http://mc4lhc06.web.cern.ch/mc4lhc06/">MC4LHC</A>
workshop at CERN 2006.

The classes has now been updated to handle the suggested version 3 of
this file standard as discussed at the <a href="http://phystev.in2p3.fr/wiki/2013:groups:tools:lhef3">Les Houches workshop 2013</a> (The previous suggested version 2.0 was discussed at the <a href="http://www.lpthe.jussieu.fr/LesHouches09Wiki/index.php/LHEF_for_Matching">Les Houches workshop 2009</a>).

There is a whole set of classes available in a single header file
called <A
HREF="http://www.thep.lu.se/~leif/LHEF/LHEF.h">LHEF.h</A>. The idea is
that matrix element or parton shower generators will implement their
own wrapper using these classes as containers.

The two classes LHEF::HEPRUP and LHEF::HEPEUP are simple container
classes which contain the same information as the Les Houches standard
Fortran common blocks with the same names. They also contain the extra
information defined in version 3 in the standard. The other two main
classes are called LHEF::Reader and LHEF::Writer and are used to read
and write Les Houches Event Files

Here are a few <A HREF="examples.html">examples</A> of how to use the
classes:

\namespace LHEF The LHEF namespace contains some example classes for reading and writing Les Houches
Event Files according to the
<A HREF="http://www.thep.lu.se/~torbjorn/lhef/lhafile2.pdf">proposal</A>
by Torbj&ouml;rn Sj&ouml;strand discussed at the
<A HREF="http://mc4lhc06.web.cern.ch/mc4lhc06/">MC4LHC</A>
workshop at CERN 2006.



 */


#endif /* HEPMC_LHEF_H */