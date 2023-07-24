/*
   @File name     : example.cc
   @description   : example code
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-26 14:26:47
   @Last modified : 2023-06-08 09:58:16
*/

#include <iostream>

// in your project, these two header files need to be included
#include <SRIMData.h>
#include <SRIMtable.h>

int main()
{
   // initialize the SRIM instance
   SRIMData *data = new SRIMData();

   // print avalable particles
   // data->PrintAll();

   // define beam particle and target material
   SRIMtable *table = data->GetTable("26Si14", "He150torr");
   std::cout << "beam particle : 26Si14 -> target material : He150Torr" << std::endl;


   Double_t ini_energy = 52.0; // MeV
   Double_t thickness = 10.0;  // mm
   std::cout << "ini_energy : " << ini_energy << " MeV, target thickness : " << thickness << " mm" << std::endl;

   // eloss calculation (MeV)
   Double_t eloss = table->GetEloss(ini_energy);
   std::cout << "eloss : " << eloss << " keV/um (MeV/mm) " << std::endl;

   // newe (enewz) calculation (MeV, mm)
   Double_t newe = table->GetNewE(ini_energy, thickness);
   std::cout << "newe  : " << newe << " MeV" << std::endl;

   // olde (eoldz) calculation (MeV, mm)
   Double_t olde = table->GetOldE(ini_energy, thickness);
   std::cout << "olde  : " << olde << " MeV" << std::endl;

   // range calculation (mm)
   Double_t ran = table->GetRange(ini_energy);
   std::cout << "range : " << ran << " mm" << std::endl;

   // straggling calculation (mm)
   Double_t strg = table->GetStrg(ini_energy);
   std::cout << "strg  : " << strg << " mm" << std::endl;

   return 0;
}
