/*
   @File name     : Updatedb.cc
   @description   : update the rootfile database class
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-05-29 11:31:12
   @Last modified : 2023-06-10 17:32:54
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glob.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

#include "Updatedb.h"

Updatedb::Updatedb(Short_t opt_value)
{
   doDebug = false;
   doFigure = false;

   if (opt_value == 1)
      doDebug = true;
   else if (opt_value == 2)
      doFigure = true;

   if (!getenv("SRIMLIB_HOME")) {
      std::cout << "ERROR: load thisSRIMlib.sh" << std::endl;
      exit(1);
   }

   workdir = getenv("SRIMLIB_HOME");
   rootpath = workdir + "/db/SRIMdata.root";
   figbase = workdir + "/figure/SRIM/";
   txtpath = workdir + "/dbinfo.txt";

   filenames = GetSRIMFileNames(workdir + "/SRIMoutput/");

   funcnames[0] = "eloss_e";
   funcnames[1] = "eloss_n";
   funcnames[2] = "eloss_tot";
   funcnames[3] = "range";
   funcnames[4] = "inv_range";
   funcnames[5] = "strg_y";
   funcnames[6] = "strg_x";
}

Updatedb::~Updatedb() {}

std::vector<TString> Updatedb::GetSRIMFileNames(TString input)
{
   glob_t globbuf;
   std::vector<TString> files;
   glob(input + "*.txt", 0, NULL, &globbuf);
   for (Int_t i = 0; i < globbuf.gl_pathc; i++) {
      files.push_back(globbuf.gl_pathv[i]);
   }
   globfree(&globbuf);
   return files;
}

void Updatedb::Initialize()
{
   if (doDebug) {
      std::cout << "[Info] Updatedb::Initialize()" << std::endl;
      std::cout << "       making " << txtpath << std::endl;
      std::cout << "       making " << rootpath << std::endl;
      std::cout << std::endl;
   }

   std::ofstream ftxt(txtpath.Data());
   ftxt << "# funcname, beam, target, min energy, max energy, max thickness" << std::endl;

   TFile *froot = new TFile(rootpath.Data(), "recreate");

   for (const auto &f : filenames) {
      TString ftmp = f(f.Last('/') + 1, f.Length());
      TString filename = ftmp(0, ftmp.Last('.'));
      if (doDebug)
         std::cout << "       processing " << filename << std::endl;

      // 0: eloss_e, 1: eloss_n, 2: eloss_tot, 3: range, 4: inv_range 5: strg_y, 6: strg_x
      TGraph *eloss[Nfunc];
      for (Int_t i = 0; i < Nfunc; i++) {
         eloss[i] = new TGraph();
      }
      MakeGraph(f, eloss);

      ftxt << "_" << filename << "," << filename(0, filename.First('_')) << ","
           << filename(filename.Last('_') + 1, filename.Length()) << "," << eloss[0]->GetPointX(0) << ","
           << eloss[0]->GetPointX(eloss[0]->GetN() - 1) << "," << eloss[3]->GetPointY(eloss[3]->GetN() - 1)
           << std::endl;

      TSpline3 *sp_eloss[Nfunc];
      for (Int_t i = 0; i < Nfunc; i++) {
         TString funcname = "_" + filename + "_" + funcnames[i];
         sp_eloss[i] = new TSpline3(funcnames[i], eloss[i]);
         sp_eloss[i]->Write(funcname.Data());

         if (doFigure) {
            TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
            c1->SetGrid();
            c1->SetLogx();
            c1->SetLogy();
            eloss[i]->SetMarkerStyle(20);
            eloss[i]->SetMarkerSize(1);
            eloss[i]->SetTitle(filename + " " + funcnames[i] + ";;");
            eloss[i]->Draw("ap");
            sp_eloss[i]->SetLineColor(kRed);
            sp_eloss[i]->SetLineWidth(2);
            sp_eloss[i]->Draw("lsame");
            c1->SaveAs(figbase + funcname + ".png");
         }
      }
   }

   if (doDebug)
      std::cout << std::endl;

   froot->Close();
}

void Updatedb::MakeGraph(TString name, TGraph *eloss[])
{
   std::ifstream fin(name.Data());

   std::string buf;
   Bool_t data = false;
   Int_t point = 0;
   while (std::getline(fin, buf)) {
      if (buf[0] == ' ' && buf[1] == ' ' && buf[2] == '-') {
         data = true;
         continue;
      }
      if (!data)
         continue;
      if (buf[0] == '-' && buf[1] == '-')
         break;

      std::vector<TString> line = SplitTString(buf, ' ');
      Double_t val[6] = {0.};

      val[0] = line[0].Atof();
      if (line[1] == "eV")
         val[0] *= 1.0e-6;
      else if (line[1] == "keV")
         val[0] *= 1.0e-3;
      else if (line[1] == "GeV")
         val[0] *= 1.0e+3;

      val[1] = line[2].Atof();
      val[2] = line[3].Atof();

      val[3] = line[4].Atof();
      if (line[5] == "A")
         val[3] *= 1.0e-7;
      else if (line[5] == "um")
         val[3] *= 1.0e-3;
      else if (line[5] == "m")
         val[3] *= 1.0e+3;
      else if (line[5] == "km")
         val[3] *= 1.0e+6;

      val[4] = line[6].Atof();
      if (line[7] == "A")
         val[4] *= 1.0e-7;
      else if (line[7] == "um")
         val[4] *= 1.0e-3;
      else if (line[7] == "m")
         val[4] *= 1.0e+3;
      else if (line[7] == "km")
         val[4] *= 1.0e+6;

      val[5] = line[8].Atof();
      if (line[9] == "A")
         val[5] *= 1.0e-7;
      else if (line[9] == "um")
         val[5] *= 1.0e-3;
      else if (line[9] == "m")
         val[5] *= 1.0e+3;
      else if (line[9] == "km")
         val[5] *= 1.0e+6;

      eloss[0]->SetPoint(point, val[0], val[1]);
      eloss[1]->SetPoint(point, val[0], val[2]);
      eloss[2]->SetPoint(point, val[0], val[1] + val[2]);
      eloss[3]->SetPoint(point, val[0], val[3]);
      eloss[4]->SetPoint(point, val[3], val[0]);
      eloss[5]->SetPoint(point, val[0], val[4]);
      eloss[6]->SetPoint(point, val[0], val[5]);
      point++;
   }
}

std::vector<TString> Updatedb::SplitTString(TString str, TString sep)
{
   std::vector<TString> str_array;

   TString str_copy = str;
   while (str_copy.Contains(sep)) {
      Int_t index = str_copy.First(sep.Data());
      TString str_val, str_remain;
      str_val = str_copy(0, index);
      str_remain = str_copy(index + 1, str_copy.Length());
      str_copy = str_remain;
      if (str_val == "")
         continue;

      str_array.push_back(str_val);
   }
   return str_array;
}
