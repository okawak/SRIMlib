# NOTE

THIS IS LAGACY CODE. We will plan to use [TSrim](https://www.cns.s.u-tokyo.ac.jp/gitlab/hayakawa/tsrim) in the future.

# SRIMlib

Energy loss calculator using SRIM output table

# preparation

This source use ROOT library. So firstly please install CERN ROOT.

Also this use SRIM output textfile.
(Example files are in ./SRIMoutput/\*.txt)
Basically SRIM can be execute by Windows, so if you use MacOS or Linux, you need wine or some virtual environment to get the textfiles.

## SRIM file name format

The scripts depend on the file name of SRIM output. The name should be

```
BeamParticle_in_Target.txt
```

The unit need **keV/micron** or **MeV/mm** (same value).
From the file name, the SRIMlib object instance generated, so the file name is very important.

# install

install

```shell
> git clone http://www.cns.s.u-tokyo.ac.jp/gitlab/Okawa/srimlib.git
> cd SRIMlib
> mkdir build
> cd build
> cmake ..
> make
> make install
```

The default CMAKE_INSTALL_PREFIX is ./install/

If you want to uninstall

```shell
> source clean.sh
```

This script delete all files related to current database, including figure, rootfiles, build, install files and so on.

# before using this library

After conpiling it, we need to update the rootfile (database)

```shell
> source thisSRIMlib.sh
> update -f
```

The **update** command have "f" (make figure) and "d" (debug mode) option.
Then we can use the library now

# example binary

As for now, this code has these commands

* Allinfo (for check the code)
* eloss (energy loss at this energy)
* newe (same as **enewz**)
* olde (same as **eoldz**)
* range (range value at this energy)
* straggling (straggling value at this energy)
* update (update the database)
* example (example code)

We can check the usage for this library in **./example.cc**, and the executable is **example**.

# Use this library from other cmake project

This library can be found by **find_package** in CMakeList.txt.
So this script may work well

in your cmake project

```cmake

...

find_package(SRIMlib HINTS ${SRIMLIB_DIR} REQUIRED)

...

target_link_libraries(hoge SRIMlib)
```
