/*
   @File name     : allinfo.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-26 18:16:08
   @Last modified : 2023-06-07 17:39:27
*/

#include <iostream>
#include <TROOT.h>

#include <SRIMData.h>
#include <SRIMtable.h>

Int_t main()
{
   TString dummy = "dummy";
   Double_t dtmp = 0.0;
   std::cin.exceptions(std::ios::failbit);

   SRIMData *data = new SRIMData();
   data->PrintBeams();
   std::cout << "select the beam particle : ";
   std::cin >> dummy;
   TString beamname = dummy;
   std::cout << std::endl;

   data->PrintTargets(beamname);
   std::cout << "select the target material : ";
   std::cin >> dummy;
   TString targetname = dummy;
   std::cout << std::endl;

   SRIMtable *table = data->GetTable(beamname, targetname);
   std::cout << "input the initial energy (MeV) : ";
   while (true) {
      try {
         std::cin >> dtmp;
      } catch (...) {
         std::cerr << "ERROR: input the double number : ";

         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         continue;
      }
      break;
   }
   Double_t ini_energy = dtmp;
   std::cout << std::endl;

   std::cout << "input the material thickness (mm) : ";
   while (true) {
      try {
         std::cin >> dtmp;
      } catch (...) {
         std::cout << "ERROR: input the double number : ";

         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         continue;
      }
      break;
   }

   Double_t thickness = dtmp;

   Double_t eloss = table->GetEloss(ini_energy);
   Double_t newe = table->GetNewE(ini_energy, dtmp);
   Double_t olde = table->GetOldE(ini_energy, dtmp);
   Double_t range = table->GetRange(ini_energy);
   Double_t strg = table->GetStrg(ini_energy);

   std::cout << "energy loss : " << eloss << " keV/um (MeV/mm)" << std::endl;
   std::cout << "newe        : " << newe << " MeV" << std::endl;
   std::cout << "olde        : " << olde << " MeV" << std::endl;
   std::cout << "Range       : " << range << " mm" << std::endl;
   std::cout << "straggling  : " << strg << " mm" << std::endl;

   return 0;
}