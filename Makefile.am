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
        
        
# only create CheckMATE with make install to ensure that fritz was properly compiled  
install-exec-local:
	@sed 's,\#!.*,\#!@PYTHON@,;/sys.path.append/s,ROOT_LIB_DIR,@ROOTLIBDIR@,' tools/python/CheckMATE.raw | \
	sed "s,config_paths\['mg5_source_path'\]='MADGRAPH_DIR',config_paths\['mg5_source_path'\]='@MADGRAPHDIR@'," | \
	sed "s,config_paths\['pythia8_lib_path'\]='PYTHIA_DIR',config_paths\['pythia8_lib_path'\]='@PYTHIALIBDIR@',"  > bin/CheckMATE;
	@sed "s,mg5_path = 'MADGRAPH_DIR',mg5_path = @MADGRAPHDIR@," data/cards/mg5_default_me5_configuration.txt.raw > data/cards/mg5_default_me5_configuration.txt;
	@if [ "@MADGRAPHDIR@" != "" ]; then echo "- setup link to MG5_aMC@NLO "; fi;
	@if [ "@PYTHIALIBDIR@" != "" ]; then echo "- setup link to Pythia "; fi;
	@chmod +x bin/CheckMATE;
	
AnalysisManager:
	@sed 's,\#!.*,\#!@PYTHON@,' tools/python/AnalysisManager.raw | \
	sed "s,config_paths\['mg5_source_path'\]='MADGRAPH_DIR',config_paths\['mg5_source_path'\]='@MADGRAPHDIR@',"  > bin/AnalysisManager;
	@chmod +x bin/AnalysisManager;
   
clean-local:
	-rm -f bin/CheckMATE
	-rm -f bin/AnalysisManager	
	@$(MAKE) uninstall;
