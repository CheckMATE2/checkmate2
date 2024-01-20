# installation

This is a brief note on how to install CheckMATE from the master branch on https://github.com/CheckMATE2/checkmate2

0) The current version is compatible with both Python2 (2.7.4 or later) and Python3 (possibly >= 3.4). 'future' library is required ('pip install future'). You can control which Python version is used at configuring with '--with-python=/path/to/python'. Other than that your default 'python' will be used. Note that the features like multi-bin signal region and likelihoods require Python3. 


1) Requirements

  a) root; anything later than version 6.20 should work just fine; the precompiled root can be obtained from:
     https://root.cern/install/
  
  b) Delphes 3.5 or later
     https://cp3.irmp.ucl.ac.be/projects/delphes/wiki/Releases
     (Should you need version 3.4 you need several changes in the code which you will figure out from compiler errors)
  
  c) HEPMC2
     https://gitlab.cern.ch:8443/hepmc/HepMC/-/tags
  
  optional:
  
  d) Pythia; we currently only support 8.3 series
  
  e) MadGraph5_aMC@NLO; versions >=2.7 should work just fine; 
     with 3 series there are some hiccups but you should get the result anyway; follow the messages on screen 
     
     
2) Installation
  
  a) autoreconf
  
  b) sample configuration call:
  
     ./configure --with-delphes=/path/to/delphes --with-hepmc=/path/to/hepmc --with-madgraph=/path/to/madgraph --with-pythia=/path/to/pythia
  
  c) make
     (optional e.g. -j4 if you would like to compile on 4 cores)
  
  d) to test the installation (if you linked Pythia):

     cd bin

     ./CheckMATE 13tev_test.in
     
3) New features:

  a) Multibin fits available in atlas_2010_14293, atlas_1908_03122, atlas_1911_12606, atlas_2101_01629, atlas_1908_08215, atlas_2004_14060:
     For command line use "-mb simple/cls/full"; in input file "Multibin: simple/cls/full"
     simple: simple fitting of shape in several bins as defined in a search, reasonably fast;
     cls: calculates CLs using full likelihood provided by the collaboration, slower but still manageable
     full: calculates CLs and upper limits using full likelihood but quite slow (extremely slow for atlas_2010_14293 - running time in days). 
     The results are stored in the multibin_limits folder.
     
  b) Neural nets implemented with ONNX Runtime, available in atlas_2106_09609 and atlas_2211_08028:
     During configure use "--with-onnx=/path/to/onnxruntime"; see: https://onnxruntime.ai/; tested with versions 1.12.1 and 1.13.1 (linux-x64)
     
  c) Boosted decision trees in atlas_2010_14293 brought by ROOT TMVA.
     

# current

2023-03-31   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ more ATLAS analyses updated to multibin

2023-01-11   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ multibin profile likelihood

2022-11-28   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ Python3 compatibility

2022-11-17   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ added atlas_2010_14293
    ~ statistical combinations
    ~ TMVA support

2022-11-04   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ providing master path for external analysis resources (like in LLP)
    ~ Delphes 3.5 required

2022-09-30   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ implementation of NN method in atlas_2106_09609

2021-10-06   Jong Soo Kim <jsk@th.physik.uni-bonn.de>
        
    ~ added added atlas_1502_05686, atlas_2103_11684, atlas_2004_10894, atlas_2106_09609
	atlas_1911_06660

2021-08-23   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_phys_2014_007

2021-08-20   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_1807_07447

2021-07-16   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_1911_12606

2021-03-14   Arran Freegard <acf1g14@soton.ac.uk>
        
    ~ added atlas_1403_5294 and atlas_higg_2013_03

2021-02-12   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_1908_03122

2020-11-03   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_conf_2018_041

2021-02-10   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_2101_01629
    
020-04-08   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ added atlas_conf_2020_048

2020-09-27   Manimala Chakraborti, Ipsita Saha

    ~ added atlas_1803_02762

2020-09-27   Manimala Chakraborti, Ipsita Saha

    ~ added atlas_conf_2019_020

2020-09-29   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_2004_14060

2020-04-08   Krzysztof Rolbiecki <krolb@fuw.edu.pl>

    ~ removed python Root check due to a change in naming conventions (python->pyroot) around v6.20 

2019-11-28   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_conf_2019_040

2019-11-22   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_1909_08457

2019-10-03   Krzysztof Rolbiecki <krolb@fuw.edu.pl>
        
    ~ added atlas_1706_03731
