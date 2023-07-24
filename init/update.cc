/*
   @File name     : update.cc
   @description   : initialize or update the ROOT file from SRIM outputs
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-29 11:37:31
   @Last modified : 2023-06-03 16:05:22
*/

#include <iostream>
#include <unistd.h>

#include "Updatedb.h"

Int_t main(Int_t argc, Char_t **argv)
{
   Int_t opt = 0;
   Short_t opt_value = 0;
   const Char_t *optstring = "df";
   opterr = 0;

   while ((opt = getopt(argc, argv, optstring)) != -1) {
      if (opt == 'd') {
         opt_value = 1;
      } else if (opt == 'f') {
         opt_value = 2;
      }
   }

   Updatedb *upd = new Updatedb(opt_value);
   upd->Initialize();

   return 0;
}