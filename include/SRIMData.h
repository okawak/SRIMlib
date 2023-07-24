/*
   @File name     : SRIMData.h
   @description   : include db information from SRIM outputs
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-26 15:47:16
   @Last modified : 2023-06-07 14:24:55
*/

#ifndef _SRIMDATA_H_
#define _SRIMDATA_H_

#include <vector>
#include <TROOT.h>

#include "SRIMtable.h"

class SRIMData {
public:
   SRIMData();
   ~SRIMData();

   SRIMtable *GetTable(TString beam, TString target);
   void PrintBeams();
   void PrintTargets(TString beam);

   void PrintAll();

private:
   std::vector<TString> funcnames;
   std::vector<TString> beams;
   std::vector<TString> targets;
   std::vector<Double_t> vec_min_e;
   std::vector<Double_t> vec_max_e;
   std::vector<Double_t> vec_max_thick;
};

#endif // end of #ifndef _SRIMDATA_H_
