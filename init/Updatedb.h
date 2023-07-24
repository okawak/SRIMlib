/*
   @File name     : Updatedb.h
   @description   : update
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-29 11:33:07
   @Last modified : 2023-06-10 16:45:02
*/

#ifndef _UPDATEDB_H_
#define _UPDATEDB_H_

#include <vector>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TSpline.h>

class Updatedb {
public:
   Updatedb(Short_t opt_value);
   ~Updatedb();

   void Initialize(); // main function of make ROOTdb file

private:
   Bool_t doDebug;
   Bool_t doFigure;

   static const Int_t Nfunc = 7;

   TString workdir;
   TString rootpath;
   TString figbase;
   TString txtpath;

   TString funcnames[Nfunc];
   std::vector<TString> filenames;

   std::vector<TString> GetSRIMFileNames(TString input);
   void MakeGraph(TString name, TGraph *eloss[]);
   std::vector<TString> SplitTString(TString str, TString sep);
};

#endif // end of #ifndef _UPDATEDB_H_
