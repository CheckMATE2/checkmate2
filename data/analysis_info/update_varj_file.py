#!/usr/bin/env python
import os, sys
import shutil
import json

if len(sys.argv) < 2:
   print "This script updates a given _var.j file to the newest CheckMATE format."
   print "Some obsolete parameters are removed and some required ones are added."
   print "The values of the added parameters are guessed or set to standard values."
   print "You might want to edit the file by hand afterwards to edit some of these."
   exit("Usage: ./updater file_var.j ")
   
infile = sys.argv[1]

injfile = open(infile, "rb")
parameters = json.loads(injfile.read())
injfile.close()

try:

    if "version" not in parameters or parameters["version"] < 2.0:
        print "Parameter file has version less than 2.0"
        parameters["version"] = 2.0
        # Add parameters that did not exist before
        parameters["expectation_known"] = "y"
        print "    - 'expectation known' added and set to 'y'"
        
        parameters["author"] = "CheckMATE"
        print "    - 'author' added and set to 'CheckMATE' (note that the analysis.cc file is NOT updated!)"
        
        parameters["authoremail"] = "checkmate@projects.hepforge.org"
        print "    - 'authoremail' added and set to 'checkmate@projects.hepforge.org' (note that the analysis.cc file is NOT updated!)"

        # Remove parameters that should not be there
        if "files" in parameters:
            parameters.pop("files")
        if "CURRLEVEL" in parameters:
            parameters.pop("CURRLEVEL")
        # dict() parameters have to be put as dict()
        for p in parameters:
            if type(parameters[p]) in [type("string"), type(u"string")]:
                if (parameters[p].startswith("[") or parameters[p].startswith("{")):
                    parameters[p] = eval(parameters[p])
        print "    - Some obsolete parameters removed or format changed"

        # Some integers have to be put as real integers
        parameters["electron_niso"] = int(parameters["electron_niso"])
        parameters["muon_niso"] = int(parameters["muon_niso"])
        parameters["photon_niso"] = int(parameters["photon_niso"])
        if parameters["jets_btagging"] == "y":
            parameters["jets_btagging_n"] = int(parameters["jets_btagging_n"])
        print "    - Some obsolete parameters removed or format changed"
    if "version" in parameters and parameters["version"] < 3.0:        
        print "Parameter file has version less than 3.0:"
        parameters["version"] = 3.0
        parameters["tags"] = list()
        if parameters["experiment"] == "A":
            parameters["tags"].append("atlas")
            print "    - Tag 'atlas' added"            
        elif parameters["experiment"] == "C":
            parameters["tags"].append("cms")
            print "    - Tag 'cms' added"
        parameters["ecm"] = 8.0
        if "7 TeV" in parameters["short_info"] or "7TeV" in parameters["short_info"]:
            parameters["ecm"] = 7.0
        parameters["tags"].append(str(int(parameters["ecm"]))+"TeV")
        print "    - Tag 'ecm' added and set to "+str(parameters["ecm"])+" and tag "+parameters["tags"][-1]+" added!"

except Exception, e:
    print str(e)
    print "Problem with "+infile
    exit(1)

outfile = "foo.dat"
jfile = open(outfile, "wb")
jfile.write(json.dumps(parameters, sort_keys=True, indent=2))
jfile.close()
shutil.move(outfile, infile)

print "Translated "+infile+" successfully to verson 3.0!"
