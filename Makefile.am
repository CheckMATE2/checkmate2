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
	@chmod +x bin/CheckMATE
	@chmod +x bin/AnalysisManager
	@echo "========================================="
	@echo " Build complete! "
	@echo " You might need to download statistical models "
	@echo " If this step was skipped during configure "
	@echo " Run: "
	@echo " ./download.sh "

CLEANFILES = bin/CheckMATE data/cards/mg5_default_me5_configuration.txt bin/13tev_test.in bin/13tev_testfull2.in download.sh

if USER_PREFIX
# Define the destination directory
mydatadir = $(datadir)

# 'nobase_' preserves the directory structure of whatever follows it
nobase_mydata_DATA = $(shell find data -type f)
nobase_cards_DATA = $(shell find bin -type f)

bin_SCRIPTS = bin/CheckMATE tools/python/advprint.py tools/python/checkmate_core.py tools/python/cls.py tools/python/detectorsettings.py tools/python/evaluator.py tools/python/events.py tools/python/example.py tools/python/get_s95.py tools/python/hs3_wrapper.py tools/python/info.py tools/python/initialize.py tools/python/multibin_limit_full.py tools/python/multibin_limit.py tools/python/process.py tools/python/resultcollector.py tools/python/spey_wrapper.py tools/python/workspace_calc.py

remote = True
else
remote = False
endif
	
all:
	@rm -f bin/CheckMATE;
	@$(MAKE) install;
        

	
AnalysisManager:
#	@sed 's,\#!.*,\#!@PYTHON@,' tools/python/AnalysisManager.raw | \
#	sed "s,config_paths\['mg5_source_path'\]='MADGRAPH_DIR',config_paths\['mg5_source_path'\]='@MADGRAPHDIR@',"  > bin/AnalysisManager;
	@chmod +x bin/AnalysisManager;
   
clean-local:
	-rm -f bin/CheckMATE
	-rm -f bin/AnalysisManager	
	@$(MAKE) uninstall;
