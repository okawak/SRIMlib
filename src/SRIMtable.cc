/*
   @File name     : SRIMtable.cc
   @description   : useful function from SRIM output textfiles
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-26 17:52:55
   @Last modified : 2023-07-27 16:16:55
*/

#include <iostream>
#include <TFile.h>

#include <SRIMtable.h>

SRIMtable::SRIMtable(TString name, Double_t e_min, Double_t e_max, Double_t t_max)
{
   if (!getenv("SRIMLIB_HOME")) {
      std::cerr << "ERROR: load thisSRIMlib.sh" << std::endl;
      exit(1);
   }
   TString workdir = getenv("SRIMLIB_HOME");
   TString rootpath = workdir + "/db/SRIMdata.root";

   min = e_min;
   max = e_max;
   thick_max = t_max;

   TFile *file = new TFile(rootpath, "read");
   if (!file) {
      std::cerr << "ERROR : firstly update command needed" << std::endl;
      exit(1);
   }

   // eloss_e = (TSpline3 *)file->Get(name + "_eloss_e");
   // eloss_n = (TSpline3 *)file->Get(name + "_eloss_n");
   eloss_tot = (TSpline3 *)file->Get(name + "_eloss_tot");
   range = (TSpline3 *)file->Get(name + "_range");
   inv_range = (TSpline3 *)file->Get(name + "_inv_range");
   strg_y = (TSpline3 *)file->Get(name + "_strg_y");
   strg_x = (TSpline3 *)file->Get(name + "_strg_x");

   file->Close();
}

SRIMtable::~SRIMtable() {}

Double_t SRIMtable::GetEloss(Double_t energy)
{
   Int_t flag = EnergyCheck(energy);
   if (flag == 0) {
      return 0.0;
   }
   Double_t result = eloss_tot->Eval(energy);
   return result;
}

Double_t SRIMtable::GetNewE(Double_t energy, Double_t thickness)
{
   Int_t flag = EnergyCheck(energy);
   if (flag == 0) {
      return 0.0;
   }
   Bool_t isNormal = ThicknessCheck(thickness);
   if (!isNormal)
      return 0.0;

   Double_t result = energy;
   Double_t ini_range = range->Eval(energy);
   if (ini_range - thickness < range->Eval(range->GetXmin()))
      result = 0.0;
   else
      result = inv_range->Eval(ini_range - thickness);

   return result;
}

Double_t SRIMtable::GetOldE(Double_t energy, Double_t thickness)
{
   Int_t flag = EnergyCheck(energy);
   if (flag == 0) {
      std::cerr << "out of energy range" << std::endl;
      std::exit(1);
   }
   Bool_t isNormal = ThicknessCheck(thickness);
   if (!isNormal) {
      std::cerr << "out of thickenss range" << std::endl;
      std::exit(1);
   }

   Double_t result = energy;
   Double_t ini_range = range->Eval(energy);
   if (ini_range + thickness > range->Eval(range->GetXmax())) {
      std::cerr << "Warning: out of range" << std::endl;
      result = 0.0;
   } else
      result = inv_range->Eval(ini_range + thickness);

   return result;
}

Double_t SRIMtable::GetRange(Double_t energy)
{
   Int_t flag = EnergyCheck(energy);
   if (flag == 0) {
      return 0.0;
   }
   Double_t result = range->Eval(energy);
   return result;
}

Double_t SRIMtable::GetStrg(Double_t energy)
{
   Int_t flag = EnergyCheck(energy);
   if (flag == 0) {
      return 0.0;
   }
   Double_t result = strg_y->Eval(energy);
   return result;
}

Int_t SRIMtable::EnergyCheck(Double_t energy) // 0: zero 1: normal
{
   if (max < energy) {
      std::cerr << "ERROR: out of range (input energy < " << max << " needed)" << std::endl;
      std::exit(1);
   }

   if (energy < min) {
      return 0;
   }
   return 1;
}

Bool_t SRIMtable::ThicknessCheck(Double_t thickness)
{
   if (thickness > thick_max) {
      return false;
   }
   return true;
}
