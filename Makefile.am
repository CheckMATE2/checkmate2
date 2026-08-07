ACLOCAL_AMFLAGS = -I m4
SUBDIRS = tools/analysis tools/fritz 
# TODO: Check if these are still needed, they were originally introduced
#       to easily compile Delphes
LD_RUN_PATH = @LD_RUN_PATH@:@ROOTLIBDIR@
LD_LIBRARY_PATH = @LD_LIBRARY_PATH@:@ROOTLIBDIR@
DYLD_LIBRARY_PATH = @DYLD_LIBRARY_PATH@:@ROOTLIBDIR@
ROOTSYS = @ROOTSYSTEM@
export ROOTSYS
export LD_RUN_PATH
export LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH

all-local: bin/CheckMATE data/cards/mg5_default_me5_configuration.txt bin/13tev_test.in bin/13tev_testfull2.in

CLEANFILES = bin/CheckMATE data/cards/mg5_default_me5_configuration.txt bin/13tev_test.in bin/13tev_testfull2.in

if USER_PREFIX
# Define the destination directory
mydatadir = $(datadir)
cardsdir = $(datadir)

# 'nobase_' preserves the directory structure of whatever follows it
nobase_mydata_DATA = $(shell find data -type f)
nobase_cards_DATA = $(shell find bin -type f)

bin_SCRIPTS = bin/CheckMATE tools/python/advprint.py tools/python/checkmate_core.py tools/python/cls.py tools/python/detectorsettings.py tools/python/evaluator.py tools/python/events.py tools/python/example.py tools/python/get_s95.py tools/python/hs3_wrapper.py tools/python/info.py tools/python/initialize.py tools/python/multibin_limit_full.py tools/python/multibin_limit.py tools/python/process.py tools/python/resultcollector.py tools/python/spey_wrapper.py tools/python/workspace_calc.py

remote = True
else
cardsdir = $(prefix)/
remote = False
endif

CHECKMATE_DATA = $(datadir)
export CHECKMATE_DATA

bin/CheckMATE: tools/python/CheckMATE.raw
	$(AM_V_GEN)$(MKDIR_P) bin
	sed -e 's,\#!.*,\#!$(PYTHON),' \
	    -e 's,RUN_PATH,$(runpath),g' \
	    -e 's,ROOT_LIB_DIR,$(ROOTLIBDIR),g' \
	    -e "s,'MADGRAPH_DIR','$(MADGRAPHDIR)',g" \
	    -e "s,'PYTHIA_DIR','$(PYTHIALIBDIR)',g" \
	    -e "s,'PREFIX','$(prefix)',g" \
	    -e "s,'SOURCE_DIR','$(sourcedir)',g" \
	    -e "s,'DATA_DIR','$(datadir)',g" \
	    -e "s,'RESULTS_DIR','$(RESULTSDIR)',g" \
	    -e "s,'REMOTE',$(remote),g" $< > $@
	chmod +x $@
data/cards/mg5_default_me5_configuration.txt: data/cards/mg5_default_me5_configuration.txt.raw
	sed "s,'MADGRAPH_DIR','$(MADGRAPHDIR)',g" $< > $@
bin/13tev_test.in: bin/13tev_test.in.raw
	sed "s,'CARDSDIR',$(cardsdir),g" $< > $@
bin/13tev_testfull2.in: bin/13tev_testfull2.in.raw
	sed "s,'CARDSDIR',$(cardsdir),g" $< > $@
	
all:
	@rm -f bin/CheckMATE;
	@sed 's,sys\.path\.append.*,sys\.path\.append("@ROOTLIBDIR@"),g' tools/python/calculate_zsig.py.raw | \
        sed 's,cdll\.LoadLibrary(.*/libCint.so\"),cdll\.LoadLibrary("@ROOTLIBDIR@/libCint.so\"),' | \
        sed 's,os\.path\.isfile(.*/libCint.so\"),os\.path\.isfile("@ROOTLIBDIR@/libCint.so\"),' | \
        sed 's,cdll\.LoadLibrary(.*/libCore.so\"),cdll\.LoadLibrary("@ROOTLIBDIR@/libCore.so\"),' | \
        sed 's,os\.path\.isfile(.*/libCore.so\"),os\.path\.isfile("@ROOTLIBDIR@/libCore.so\"),' | \
        sed 's,cdll\.LoadLibrary(.*/libThread.so\"),cdll\.LoadLibrary("@ROOTLIBDIR@/libThread.so\"),' | \
        sed 's,os\.path\.isfile(.*/libThread.so\"),os\.path\.isfile("@ROOTLIBDIR@/libThread.so\"),' | \
        sed 's,cdll\.LoadLibrary(.*/libRIO.so\"),cdll\.LoadLibrary("@ROOTLIBDIR@/libRIO.so\"),' | \
        sed 's,os\.path\.isfile(.*/libRIO.so\"),os\.path\.isfile("@ROOTLIBDIR@/libRIO.so\"),' | \
        sed 's,cdll\.LoadLibrary(.*/libMathCore.so\"),cdll\.LoadLibrary("@ROOTLIBDIR@/libMathCore.so\"),'  | \
        sed 's,os\.path\.isfile(.*/libMathCore.so\"),os\.path\.isfile("@ROOTLIBDIR@/libMathCore.so\"),'  | \
        sed 's,cdll\.LoadLibrary(.*/libNet.so\"),cdll\.LoadLibrary("@ROOTLIBDIR@/libNet.so\"),'  | \
        sed 's,os\.path\.isfile(.*/libNet.so\"),os\.path\.isfile("@ROOTLIBDIR@/libNet.so\"),'  | \
        sed 's,cdll\.LoadLibrary(.*/libTree.so\"),cdll\.LoadLibrary("@ROOTLIBDIR@/libTree.so\"),'  | \
        sed 's,os\.path\.isfile(.*/libTree.so\"),os\.path\.isfile("@ROOTLIBDIR@/libTree.so\"),' > tools/python/calculate_zsig.py;
	@$(MAKE) install;
        

	
AnalysisManager:
	@sed 's,\#!.*,\#!@PYTHON@,' tools/python/AnalysisManager.raw | \
	sed "s,config_paths\['mg5_source_path'\]='MADGRAPH_DIR',config_paths\['mg5_source_path'\]='@MADGRAPHDIR@',"  > bin/AnalysisManager;
	@chmod +x bin/AnalysisManager;
   
clean-local:
	-rm -f bin/CheckMATE
	-rm -f bin/AnalysisManager	
	@$(MAKE) uninstall;
