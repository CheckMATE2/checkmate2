import os, sys
import json

all_files = os.listdir(".")
all_analyses = [x.replace("_ref.dat", "") for x in all_files if "_ref.dat" in x]

for a in all_analyses:
  jfilename = a+"_var.j"
  reffilename = a+"_ref.dat"
  refresults = list()
  try:
      with open (reffilename, "r") as ref_file:
          for line in ref_file:
              if line.strip() == "" or line.startswith("#"):
                  continue
              if line.startswith("SR "):
                  columntokens = line.split()
                  continue
              ref_sr_results = dict()
              tokens = line.split() 
              for i in range(len(columntokens)):
                  ref_sr_results[columntokens[i]] = tokens[i]
              refresults.append(ref_sr_results)
  except:
      print columntokens
      print i
      print tokens
      exit()
  jfile = open(jfilename, "rb")
  jvarresults = json.loads(jfile.read())
  print "Testing "+a+" from ref to var"
  # test if everything in ref is present in var.j
  for col in refresults[0]:
      if col == "SR":
          continue
      if col not in jvarresults["info_set"]:
          print " >>>column "+col+" not set in infoset of "+jfilename+" but in "+reffilename

      for ref_sr_results in refresults:
          sr = ref_sr_results["SR"]
          if sr not in jvarresults["reference_data"]:
              print " >>>SR "+sr+" present in "+reffilename+" but not in "+jfilename
              continue # FIXME
          if col not in jvarresults["reference_data"][sr]:
              print " >>>column "+col+" has no value for "+sr+" in "+jfilename+" but in "+reffilename
              continue #FIXME
          try:
              if float(ref_sr_results[col]) != float(jvarresults["reference_data"][sr][col]):
                  print " >>>column "+col+" has different values for "+sr+" in "+jfilename+" ("+jvarresults["reference_data"][sr][col]+")  and in "+reffilename+" ("+ref_sr_results[col]+")"
                  continue #FIXME
          except:
              print ref_sr_results[col]
              print jvarresults["reference_data"][sr][col]
              exit()


  print "Testing "+a+" from var to ref"
  # test if everything in var.j is present in ref.dat
  srindices = dict()
  for i in range(len(refresults)):
      srindices[refresults[i]["SR"]] = i 
  for sr in jvarresults["reference_data"]:
      if sr not in srindices:
          print " >>>SR "+sr+" present in "+jfilename+" but not in "+reffilename
          continue # FIXME
      for col in jvarresults["reference_data"][sr]:
          if col not in refresults[srindices[sr]]:
              print " >>>column "+col+" has no value for "+sr+" in "+reffilename+" but in "+jfilename
              continue #FIXME
#          if float(refresults[srindices[sr]][col]) != float(jvarresults["reference_data"][sr][col]):
#              print " >>>column "+col+" has different values for "+sr+" in "+jfilename+" ("+jvarresults["reference_data"][sr][col]+") and in "+reffilename+"(+"+refresults[srindices[sr]][col]+")"
#              continue #FIXME
  print ""
  i = raw_input("Click for Next (press X to write new var.j from ref.dat) ")
  if i == "X":
      newrefdata = dict()
      jvarresults["info_set"] = []
      for col in refresults[0]:
          if col == "SR":
              continue       
          jvarresults["info_set"].append(col)
      jvarresults["info_set"].sort()
      jvarresults["info_set"].reverse()
      for ref_sr_results in refresults:
          sr = ref_sr_results["SR"]
          newrefdata[sr] = dict()
          for col in refresults[0]:
              if col == "SR":
                  continue            
              newrefdata[sr][col] = ref_sr_results[col]
      jvarresults["reference_data"] = newrefdata
      jfile = open(jfilename+".2", "wb")
      jfile.write(json.dumps(jvarresults, sort_keys=True, indent=2))

      
      
