Download Crow and Boost and put the unzipped library folders here:
- Download Crow to get a directory named Crow with: git clone https://github.com/CrowCpp/Crow.git
- Download Boost at https://www.boost.org/users/download/, unzip and put the unzipped boost_1_86_0 directory here
- Download WordNet 3.0, unzip and place in this directory.  See below for detailed install instructions.

The resulting directory structure should look like:
external_libraires/
├── README.txt
├── Crow
├── boost_1_86_0
├── stubs.c
└── WordNet-3.0

The matching algorithm uses WordNet 3.0 to build a list of synonyms for word comparison between user profile and job listing. To install this, download WordNet 3.0 from:
  
  https://wordnet.princeton.edu/download/current-version

Place the uncompressed folder in the external_libraries directory.

WordNet needs the tcl-tk package to be installed. Install tcl-tk on Mac using:
  
  brew install tcl-tk

If using Windows, this can be installed using ActiveTcl.

Find the path where you installed tcl-tk, in particular the lib folder. For me, this was located in:
  /opt/homebrew/Cellar/tcl-tk/8.6.15/bin/tcl-tk/lib

This directory should contain the files tclConfig.sh and tkConfig.sh among others.

Copy this path and go back to the WordNet-3.0 folder in this external_libraries folder.
Run the following command:

./configure --with-tcl=/path__to_tcl-tk_library --with-tk=/path_to_tcl-tk_library

replacing the path with the actual path to your tcl-tk lib folder.

There will be a stubs.c file inside this external_libraries folder. Copy it into WordNet-3.0/src

  sudo cp stubs.c WordNet3.0/src/stubs.c

Finally, run:
  sudo make
  sudo make install

This should complete the WordNet library installation.
