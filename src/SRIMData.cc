/**
 * @file    SRIMData.cc
 * @brief   include db indoemrion deom SRIM outputs
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-23 18:33:57
 * @note
 */

#include <SRIMData.h>
#include <SRIMtable.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

SRIMData::SRIMData() {
    if (!getenv("SRIMLIB_HOME")) {
        std::cout << "ERROR: load thisSRIMlib.sh" << std::endl;
        exit(1);
    }

    TString workdir = getenv("SRIMLIB_HOME");
    TString txtpath = workdir + "/dbinfo.txt";

    std::ifstream fin(txtpath.Data());
    std::string str;
    while (std::getline(fin, str)) {
        if (str[0] == '#')
            continue;

        std::stringstream ss{str};
        std::string val;
        Int_t col = 0;
        while (std::getline(ss, val, ',')) {
            if (val == "")
                continue;
            else if (col == 0)
                funcnames.push_back(val);
            else if (col == 1)
                beams.push_back(val);
            else if (col == 2)
                targets.push_back(val);
            else if (col == 3)
                vec_min_e.push_back(std::stod(val));
            else if (col == 4)
                vec_max_e.push_back(std::stod(val));
            else if (col == 5)
                vec_max_thick.push_back(std::stod(val));
            col++;
        }
    }
    if (!(funcnames.size() == beams.size() && funcnames.size() == targets.size() &&
          funcnames.size() == vec_min_e.size() && funcnames.size() == vec_max_e.size() &&
          funcnames.size() == vec_max_thick.size())) {
        std::cout << "ERROR source code error!" << std::endl;
        std::exit(1);
    }
}

SRIMData::~SRIMData() {}

SRIMtable *SRIMData::GetTable(TString beam, TString target) {
    Int_t itr = -1;
    for (Int_t i = 0; i < funcnames.size(); i++) {
        if (beam == beams[i] && target == targets[i]) {
            itr = i;
            break;
        }
    }

    if (itr < 0) {
        std::cout << "[SRIMlib] incorrect beam (" << beam << ") or target (" << target << ") input, need update SRIM data" << std::endl;
        return NULL;
    }
    SRIMtable *tb = new SRIMtable(funcnames[itr], vec_min_e[itr], vec_max_e[itr], vec_max_thick[itr]);
    return tb;
}

void SRIMData::PrintBeams() {
    std::cout << "List of currently available beam particles" << std::endl;
    std::cout << "==========================================" << std::endl;
    Int_t num = 0;
    TString tmpstr = "dummy";
    for (Int_t i = 0; i < beams.size(); i++) {
        if (tmpstr != beams[i]) {
            std::cout << beams[i] << " ";
            tmpstr = beams[i];
            num++;
        }
    }
    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;
}

void SRIMData::PrintTargets(TString beam) {
    std::cout << "List of currently available target materials" << std::endl;
    std::cout << "==========================================" << std::endl;
    for (Int_t i = 0; i < targets.size(); i++) {
        if (beam != beams[i])
            continue;
        std::cout << targets[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;
}

void SRIMData::PrintAll() {
    std::cout << "db information" << std::endl;
    for (Int_t i = 0; i < funcnames.size(); i++) {
        std::cout << "beam              : " << beams[i] << std::endl;
        std::cout << "target            : " << targets[i] << std::endl;
        std::cout << "minimum_e         : " << vec_min_e[i] << std::endl;
        std::cout << "maximum_e         : " << vec_max_e[i] << std::endl;
        std::cout << "maximum_thickness : " << vec_max_thick[i] << std::endl;
        std::cout << std::endl;
    }
}
