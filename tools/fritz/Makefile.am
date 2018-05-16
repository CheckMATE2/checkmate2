AUTOMAKE_OPTIONS = subdir-objects

mybindir = $(abs_top_builddir)/tools/fritz/bin/
mybin_PROGRAMS = fritz
fritz_SOURCES = src/main.cc \
                    src/global/Global.cc include/global/Global.h \
                    src/global/FritzConfig.cc include/global/FritzConfig.h \
                    src/global/EventFile.cc include/global/EventFile.h \
                    src/fritz/Fritz.cc include/fritz/Fritz.h \
                    src/fritz/ConfigParser.cc include/fritz/ConfigParser.h \
                    src/delpheshandler/CMExRootTreeWriter.cc include/delpheshandler/CMExRootTreeWriter.h \
                    src/delpheshandler/CMExRootTreeBranch.cc include/delpheshandler/CMExRootTreeBranch.h \
                    src/delpheshandler/DelphesHandler.cc include/delpheshandler/DelphesHandler.h \
                    src/analysishandler/AnalysisHandler.cc include/analysishandler/AnalysisHandler.h \
                    src/analysishandler/AnalysisHandlerATLAS.cc include/analysishandler/AnalysisHandlerATLAS.h \
                    src/analysishandler/AnalysisHandlerATLAS_7TeV.cc include/analysishandler/AnalysisHandlerATLAS_7TeV.h \
                    src/analysishandler/AnalysisHandlerATLAS_8TeV.cc include/analysishandler/AnalysisHandlerATLAS_8TeV.h \
                    src/analysishandler/AnalysisHandlerATLAS_13TeV.cc include/analysishandler/AnalysisHandlerATLAS_13TeV.h \
                    src/analysishandler/AnalysisHandlerATLAS_14TeV_projected.cc include/analysishandler/AnalysisHandlerATLAS_14TeV_projected.h \
                    src/analysishandler/AnalysisHandlerATLAS_14TeV_HL_FlatBtagger.cc include/analysishandler/AnalysisHandlerATLAS_14TeV_HL_FlatBtagger.h \
                    src/analysishandler/AnalysisHandlerCMS.cc include/analysishandler/AnalysisHandlerCMS.h \
                    src/analysishandler/AnalysisHandlerCMS_7TeV.cc include/analysishandler/AnalysisHandlerCMS_7TeV.h \
                    src/analysishandler/AnalysisHandlerCMS_8TeV.cc include/analysishandler/AnalysisHandlerCMS_8TeV.h \
                    src/analysishandler/AnalysisHandlerCMS_13TeV.cc include/analysishandler/AnalysisHandlerCMS_13TeV.h \
                    src/analysishandler/AnalysisHandlerCMS_14TeV_projected.cc include/analysishandler/AnalysisHandlerCMS_14TeV_projected.h

fritz_LDADD = -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ -L@DELPHESLIBDIR@ @DELPHESLIBS@ -L$(abs_top_builddir)/tools/analysis -lanalyses
fritz_CXXFLAGS = \
         @ROOTCFLAGS@ @DELPHESCFLAGS@ \
        -Iinclude/global -Iinclude/fritz -Iinclude/delpheshandler -Iinclude/analysishandler \
        -I$(abs_top_builddir)/tools/analysis/include/base -I$(abs_top_builddir)/tools/analysis/include/kinematics \
        -I$(abs_top_builddir)/tools/analysis/include/kinematics/mctlib -I$(abs_top_builddir)/tools/analysis/include/kinematics/mt2family \
        -I$(abs_top_builddir)/tools/analysis/include/kinematics/topness \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/ATLAS_7TeV \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/ATLAS_8TeV \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/ATLAS_13TeV \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/ATLAS_14TeV_HighLumi \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/CMS_7TeV \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/CMS_8TeV \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/CMS_13TeV \
        -I$(abs_top_builddir)/tools/analysis/include/analyses/CMS_14TeV_HighLumi

fritz_LDFLAGS =  -R@ROOTLIBDIR@ -R@DELPHESLIBDIR@ -R$(abs_top_builddir)/tools/analysis

if HAVE_PYTHIA
   fritz_SOURCES += src/pythiahandler/PythiaHandler.cc include/pythiahandler/PythiaHandler.h
   fritz_LDADD +=  @PYTHIALIBS@
   fritz_CXXFLAGS += @PYTHIAINCLUDE@ -Iinclude/pythiahandler -DHAVE_PYTHIA=1
   fritz_LDFLAGS +=  -R@PYTHIALIBDIR@ 
   LD_RUN_PATH += :@PYTHIALIBDIR@    
if HAVE_GZIPINC
   fritz_CXXFLAGS += @GZIPINC@  
if HAVE_GZIPLIB   
   fritz_CXXFLAGS += @GZIPLIB@
endif   
endif   
if HAVE_HEPMC
   fritz_LDADD += @HEPMCLIBS@    
   fritz_LDFLAGS +=  -R@HEPMCLIBDIR@
   fritz_CXXFLAGS += -DHAVE_HEPMC=1
   LD_RUN_PATH += :@HEPMCLIBDIR@ 
endif
endif
ROOTSYS = @ROOTSYSTEM@:
