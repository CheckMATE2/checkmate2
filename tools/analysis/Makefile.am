AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = @ROOTCFLAGS@ @DELPHESCFLAGS@ -Iinclude/base -Iinclude/kinematics -Iinclude/kinematics/mctlib \
             -Iinclude/kinematics/mt2family -Iinclude/kinematics/topness -Iinclude/global \
             -Iinclude/analyses/ATLAS_7TeV -Iinclude/analyses/ATLAS_8TeV -Iinclude/analyses/ATLAS_13TeV -Iinclude/analyses/ATLAS_14TeV_HighLumi \
             -Iinclude/analyses/CMS_7TeV -Iinclude/analyses/CMS_8TeV -Iinclude/analyses/CMS_13TeV -Iinclude/analyses/CMS_14TeV_HighLumi

if HAVE_ONNX
   AM_CXXFLAGS += @ONNXINCLUDE@ -DHAVE_ONNX=1
endif
             
# the library names to build
mylibdir = $(abs_top_builddir)/tools/analysis/lib
mylib_LTLIBRARIES = libanalyses.la
libanalyses_la_SOURCES =	\
             src/global/Global.cc src/include/Global.h \
             src/base/AnalysisBase.cc include/base/AnalysisBase.h \
             src/base/ETMiss.cc include/base/ETMiss.h \
             src/base/FinalStateObject.cc include/base/FinalStateObject.h \
             src/base/Units.cc include/base/Units.h \
             src/kinematics/mt2family/mt2_bisect.cc include/kinematics/mt2family/mt2_bisect.h \
             src/kinematics/mctlib/mctlib.cc include/kinematics/mctlib/mctlib.h \
             src/kinematics/mt2family/mt2bl_bisect.cc include/kinematics/mt2family/mt2bl_bisect.h \
             src/kinematics/mt2family/mt2w_bisect.cc include/kinematics/mt2family/mt2w_bisect.h \
             src/kinematics/topness/simplex.cc include/kinematics/topness/simplex.h \
             src/kinematics/topness/Wrappertopness.cc include/kinematics/topness/Wrappertopness.h \
             src/kinematics/topness/topness_struct.cc include/kinematics/topness/topness_struct.h

# ATLAS_7TeV
libanalyses_la_SOURCES +=	\
             include/analyses/ATLAS_7TeV/atlas_1210_2979.h src/analyses/ATLAS_7TeV/atlas_1210_2979.cc
# ATLAS_8TeV
libanalyses_la_SOURCES +=	\
             include/analyses/ATLAS_8TeV/atlas_1308_1841.h src/analyses/ATLAS_8TeV/atlas_1308_1841.cc \
             include/analyses/ATLAS_8TeV/atlas_1308_2631.h src/analyses/ATLAS_8TeV/atlas_1308_2631.cc src/analyses/ATLAS_8TeV/atlas_1308_2631_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_1402_7029.h src/analyses/ATLAS_8TeV/atlas_1402_7029.cc \
             include/analyses/ATLAS_8TeV/atlas_1403_4853.h src/analyses/ATLAS_8TeV/atlas_1403_4853.cc \
             include/analyses/ATLAS_8TeV/atlas_1403_5222.h src/analyses/ATLAS_8TeV/atlas_1403_5222.cc \
             include/analyses/ATLAS_8TeV/atlas_1403_5294.h src/analyses/ATLAS_8TeV/atlas_1403_5294.cc src/analyses/ATLAS_8TeV/atlas_1403_5294_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_1404_2500.h src/analyses/ATLAS_8TeV/atlas_1404_2500.cc \
             include/analyses/ATLAS_8TeV/atlas_1405_7875.h src/analyses/ATLAS_8TeV/atlas_1405_7875.cc \
             include/analyses/ATLAS_8TeV/atlas_1407_0583.h src/analyses/ATLAS_8TeV/atlas_1407_0583.cc \
             include/analyses/ATLAS_8TeV/atlas_1407_0600.h src/analyses/ATLAS_8TeV/atlas_1407_0600.cc \
             include/analyses/ATLAS_8TeV/atlas_1407_0608.h src/analyses/ATLAS_8TeV/atlas_1407_0608.cc \
             include/analyses/ATLAS_8TeV/atlas_1411_1559.h src/analyses/ATLAS_8TeV/atlas_1411_1559.cc \
             include/analyses/ATLAS_8TeV/atlas_1501_07110.h src/analyses/ATLAS_8TeV/atlas_1501_07110.cc \
             include/analyses/ATLAS_8TeV/atlas_1502_01518.h src/analyses/ATLAS_8TeV/atlas_1502_01518.cc \
             include/analyses/ATLAS_8TeV/atlas_1503_03290.h src/analyses/ATLAS_8TeV/atlas_1503_03290.cc \
             include/analyses/ATLAS_8TeV/atlas_1506_08616.h src/analyses/ATLAS_8TeV/atlas_1506_08616.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2012_104.h src/analyses/ATLAS_8TeV/atlas_conf_2012_104.cc src/analyses/ATLAS_8TeV/atlas_conf_2012_104_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2012_147.h src/analyses/ATLAS_8TeV/atlas_conf_2012_147.cc src/analyses/ATLAS_8TeV/atlas_conf_2012_147_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_021.h src/analyses/ATLAS_8TeV/atlas_conf_2013_021.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_024.h src/analyses/ATLAS_8TeV/atlas_conf_2013_024.cc src/analyses/ATLAS_8TeV/atlas_conf_2013_024_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_031.h src/analyses/ATLAS_8TeV/atlas_conf_2013_031.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_035.h src/analyses/ATLAS_8TeV/atlas_conf_2013_035.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_036.h src/analyses/ATLAS_8TeV/atlas_conf_2013_036.cc src/analyses/ATLAS_8TeV/atlas_conf_2013_036_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_037.h src/analyses/ATLAS_8TeV/atlas_conf_2013_037.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_047.h src/analyses/ATLAS_8TeV/atlas_conf_2013_047.cc src/analyses/ATLAS_8TeV/atlas_conf_2013_047_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_048.h src/analyses/ATLAS_8TeV/atlas_conf_2013_048.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_049.h src/analyses/ATLAS_8TeV/atlas_conf_2013_049.cc src/analyses/ATLAS_8TeV/atlas_conf_2013_049_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_061.h src/analyses/ATLAS_8TeV/atlas_conf_2013_061.cc src/analyses/ATLAS_8TeV/atlas_conf_2013_061_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_062.h src/analyses/ATLAS_8TeV/atlas_conf_2013_062.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2013_089.h src/analyses/ATLAS_8TeV/atlas_conf_2013_089.cc src/analyses/ATLAS_8TeV/atlas_conf_2013_089_CR.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2014_014.h src/analyses/ATLAS_8TeV/atlas_conf_2014_014.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2014_033.h src/analyses/ATLAS_8TeV/atlas_conf_2014_033.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2014_056.h src/analyses/ATLAS_8TeV/atlas_conf_2014_056.cc \
             include/analyses/ATLAS_8TeV/atlas_conf_2015_004.h src/analyses/ATLAS_8TeV/atlas_conf_2015_004.cc
# ATLAS_13TeV
libanalyses_la_SOURCES +=	\
             include/analyses/ATLAS_13TeV/atlas_1602_09058.h src/analyses/ATLAS_13TeV/atlas_1602_09058.cc \
             include/analyses/ATLAS_13TeV/atlas_1604_07773.h src/analyses/ATLAS_13TeV/atlas_1604_07773.cc \
             include/analyses/ATLAS_13TeV/atlas_1604_01306.h src/analyses/ATLAS_13TeV/atlas_1604_01306.cc \
             include/analyses/ATLAS_13TeV/atlas_1605_03814.h src/analyses/ATLAS_13TeV/atlas_1605_03814.cc \
             include/analyses/ATLAS_13TeV/atlas_1605_04285.h src/analyses/ATLAS_13TeV/atlas_1605_04285.cc \
             include/analyses/ATLAS_13TeV/atlas_1605_09318.h src/analyses/ATLAS_13TeV/atlas_1605_09318.cc \
             include/analyses/ATLAS_13TeV/atlas_1606_03903.h src/analyses/ATLAS_13TeV/atlas_1606_03903.cc \
             include/analyses/ATLAS_13TeV/atlas_1609_01599.h src/analyses/ATLAS_13TeV/atlas_1609_01599.cc \
             include/analyses/ATLAS_13TeV/atlas_1704_03848.h src/analyses/ATLAS_13TeV/atlas_1704_03848.cc \
             include/analyses/ATLAS_13TeV/atlas_1709_04183.h src/analyses/ATLAS_13TeV/atlas_1709_04183.cc \
             include/analyses/ATLAS_13TeV/atlas_conf_2015_082.h src/analyses/ATLAS_13TeV/atlas_conf_2015_082.cc \
             include/analyses/ATLAS_13TeV/atlas_conf_2016_013.h src/analyses/ATLAS_13TeV/atlas_conf_2016_013.cc \
             include/analyses/ATLAS_13TeV/atlas_conf_2016_050.h src/analyses/ATLAS_13TeV/atlas_conf_2016_050.cc \
             include/analyses/ATLAS_13TeV/atlas_conf_2016_054.h src/analyses/ATLAS_13TeV/atlas_conf_2016_054.cc \
             include/analyses/ATLAS_13TeV/atlas_conf_2016_076.h src/analyses/ATLAS_13TeV/atlas_conf_2016_076.cc \
             include/analyses/ATLAS_13TeV/atlas_conf_2016_078.h src/analyses/ATLAS_13TeV/atlas_conf_2016_078.cc \
             include/analyses/ATLAS_13TeV/atlas_conf_2016_076.h src/analyses/ATLAS_13TeV/atlas_conf_2016_096.cc 
# ATLAS_14TeV_HighLumi
libanalyses_la_SOURCES +=	\
             include/analyses/ATLAS_14TeV_HighLumi/atl_phys_pub_2014_010_sbottom.h src/analyses/ATLAS_14TeV_HighLumi/atl_phys_pub_2014_010_sbottom.cc \
             include/analyses/ATLAS_14TeV_HighLumi/atlas_2014_010_hl_3l.h src/analyses/ATLAS_14TeV_HighLumi/atlas_2014_010_hl_3l.cc \
             include/analyses/ATLAS_14TeV_HighLumi/atlas_phys_2014_010_300.h src/analyses/ATLAS_14TeV_HighLumi/atlas_phys_2014_010_300.cc \
             include/analyses/ATLAS_14TeV_HighLumi/atlas_phys_2014_010_sq_hl.h src/analyses/ATLAS_14TeV_HighLumi/atlas_phys_2014_010_sq_hl.cc \
             include/analyses/ATLAS_14TeV_HighLumi/atlas_phys_pub_2013_011.h src/analyses/ATLAS_14TeV_HighLumi/atlas_phys_pub_2013_011.cc \
             include/analyses/ATLAS_14TeV_HighLumi/dilepton_hl.h src/analyses/ATLAS_14TeV_HighLumi/dilepton_hl.cc
# CMS_7TeV
libanalyses_la_SOURCES +=	\
             include/analyses/CMS_7TeV/cms_1306_1126_WW.h src/analyses/CMS_7TeV/cms_1306_1126_WW.cc
# CMS_8TeV
libanalyses_la_SOURCES +=	\
             include/analyses/CMS_8TeV/cms_1301_4698_WW.h src/analyses/CMS_8TeV/cms_1301_4698_WW.cc \
             include/analyses/CMS_8TeV/cms_1303_2985.h src/analyses/CMS_8TeV/cms_1303_2985.cc src/analyses/CMS_8TeV/cms_1303_2985_CR.cc \
             include/analyses/CMS_8TeV/cms_1405_7570.h src/analyses/CMS_8TeV/cms_1405_7570.cc \
             include/analyses/CMS_8TeV/cms_1408_3583.h src/analyses/CMS_8TeV/cms_1408_3583.cc \
             include/analyses/CMS_8TeV/cms_1502_06031.h src/analyses/CMS_8TeV/cms_1502_06031.cc \
             include/analyses/CMS_8TeV/cms_1504_03198.h src/analyses/CMS_8TeV/cms_1504_03198.cc \
             include/analyses/CMS_8TeV/cms_smp_12_006.h src/analyses/CMS_8TeV/cms_smp_12_006.cc \
             include/analyses/CMS_8TeV/cms_sus_12_019.h src/analyses/CMS_8TeV/cms_sus_12_019.cc \
             include/analyses/CMS_8TeV/cms_sus_13_016.h src/analyses/CMS_8TeV/cms_sus_13_016.cc \
             include/analyses/CMS_8TeV/cms_exo_14_014.h src/analyses/CMS_8TeV/cms_exo_14_014.cc 
# CMS_13TeV
libanalyses_la_SOURCES +=	\
             include/analyses/CMS_13TeV/cms_pas_sus_15_011.h src/analyses/CMS_13TeV/cms_pas_sus_15_011.cc
libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_sus_16_039.h src/analyses/CMS_13TeV/cms_sus_16_039.cc

libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_sus_16_025.h src/analyses/CMS_13TeV/cms_sus_16_025.cc

libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1712_08119.h src/analyses/ATLAS_13TeV/atlas_1712_08119.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1712_02332.h src/analyses/ATLAS_13TeV/atlas_1712_02332.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1802_03158.h src/analyses/ATLAS_13TeV/atlas_1802_03158.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1708_07875.h src/analyses/ATLAS_13TeV/atlas_1708_07875.cc

libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_conf_2016_066.h src/analyses/ATLAS_13TeV/atlas_conf_2016_066.cc

libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_conf_2017_060.h src/analyses/ATLAS_13TeV/atlas_conf_2017_060.cc


libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_sus_16_048.h src/analyses/CMS_13TeV/cms_sus_16_048.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_8TeV/atlas_1507_05493.h src/analyses/ATLAS_8TeV/atlas_1507_05493.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1706_03731.h src/analyses/ATLAS_13TeV/atlas_1706_03731.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_conf_2019_018.h src/analyses/ATLAS_13TeV/atlas_conf_2019_018.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1908_08215.h src/analyses/ATLAS_13TeV/atlas_1908_08215.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1909_08457.h src/analyses/ATLAS_13TeV/atlas_1909_08457.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_conf_2019_040.h src/analyses/ATLAS_13TeV/atlas_conf_2019_040.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_conf_2019_020.h src/analyses/ATLAS_13TeV/atlas_conf_2019_020.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1803_02762.h src/analyses/ATLAS_13TeV/atlas_1803_02762.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_conf_2018_041.h src/analyses/ATLAS_13TeV/atlas_conf_2018_041.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_2101_01629.h src/analyses/ATLAS_13TeV/atlas_2101_01629.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_conf_2020_048.h src/analyses/ATLAS_13TeV/atlas_conf_2020_048.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_2004_14060.h src/analyses/ATLAS_13TeV/atlas_2004_14060.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1908_03122.h src/analyses/ATLAS_13TeV/atlas_1908_03122.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_8TeV/atlas_higg_2013_03.h src/analyses/ATLAS_8TeV/atlas_higg_2013_03.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_2103_11684.h src/analyses/ATLAS_13TeV/atlas_2103_11684.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_2004_10894.h src/analyses/ATLAS_13TeV/atlas_2004_10894.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_8TeV/atlas_1502_05686.h src/analyses/ATLAS_8TeV/atlas_1502_05686.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_2106_09609.h src/analyses/ATLAS_13TeV/atlas_2106_09609.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1911_06660.h src/analyses/ATLAS_13TeV/atlas_1911_06660.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1911_12606.h src/analyses/ATLAS_13TeV/atlas_1911_12606.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_1807_07447.h src/analyses/ATLAS_13TeV/atlas_1807_07447.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_14TeV_HighLumi/atlas_phys_2014_007.h src/analyses/ATLAS_14TeV_HighLumi/atlas_phys_2014_007.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_2010_14293.h src/analyses/ATLAS_13TeV/atlas_2010_14293.cc
libanalyses_la_SOURCES += include/analyses/ATLAS_13TeV/atlas_2211_08028.h src/analyses/ATLAS_13TeV/atlas_2211_08028.cc
libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_sus_19_005.h src/analyses/CMS_13TeV/cms_sus_19_005.cc
libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_sus_19_006.h src/analyses/CMS_13TeV/cms_sus_19_006.cc
libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_1908_04722.h src/analyses/CMS_13TeV/cms_1908_04722.cc
libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_2107_13201.h src/analyses/CMS_13TeV/cms_2107_13201.cc
libanalyses_la_SOURCES += include/analyses/CMS_13TeV/cms_2205_09597.h src/analyses/CMS_13TeV/cms_2205_09597.cc
#@@extraanalysis@@

libanalyses_la_LDFLAGS = -avoid-version -shared
#-module 
libanalyses_la_LIBADD =  -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ -lTMVA -L@DELPHESLIBDIR@ @DELPHESLIBS@

ROOTSYS = @ROOTSYSTEM@:
LD_RUN_PATH = @ROOTLIBDIR@:@DELPHESLIBDIR@

