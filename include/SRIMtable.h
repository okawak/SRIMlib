/*
   @File name     : SRIMtable.h
   @description   : useful function from SRIM output textfiles
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-26 17:34:19
   @Last modified : 2023-07-27 16:14:44
*/

#ifndef _SRIMTABLE_H_
#define _SRIMTABLE_H_

#include <TSpline.h>

class SRIMtable {
public:
   SRIMtable(TString name, Double_t e_min, Double_t e_max, Double_t t_max);
   ~SRIMtable();

   Double_t GetEloss(Double_t energy);
   Double_t GetNewE(Double_t energy, Double_t thickness);
   Double_t GetOldE(Double_t energy, Double_t thickness);
   Double_t GetRange(Double_t energy);
   Double_t GetStrg(Double_t energy);

private:
   Double_t min;
   Double_t max;
   Double_t thick_max;

   // TSpline3 *eloss_e;
   // TSpline3 *eloss_n;
   TSpline3 *eloss_tot;
   TSpline3 *range;
   TSpline3 *inv_range;
   TSpline3 *strg_y;
   TSpline3 *strg_x;

   Int_t EnergyCheck(Double_t energy);
   Bool_t ThicknessCheck(Double_t thickness);
};

#endif // end of #ifndef _SRIMTABLE_H_
