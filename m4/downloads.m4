dnl ##### Downloads #####
AC_DEFUN([DOWNLOADS],
[
  AC_MSG_RESULT([])
  AC_MSG_NOTICE([=== Downloading external resources ===])
  # BDT weights for atlas_2010_14293
  if ! test -f "$srcdir/data/atlas_2010_14293/ZeroLepton2018-SRBDT-weight.tar.gz"
    then
    if ! test -d "$srcdir/data/atlas_2010_14293"
    then 
      mkdir "$srcdir/data/atlas_2010_14293"
    fi  
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2010_14293/ZeroLepton2018-SRBDT-weight.tar.gz" "https://doi.org/10.17182/hepdata.95664.v2/r8"`
    `tar -xvzf "$srcdir/data/atlas_2010_14293/ZeroLepton2018-SRBDT-weight.tar.gz" -C "$srcdir/data/atlas_2010_14293"`
  fi  
  # Likelihoods for atlas_2010_14293
  if ! test -f "$srcdir/data/atlas_2010_14293/Likelihoods.tar.gz"
    then
    if ! test -d "$srcdir/data/atlas_2010_14293"
    then 
      mkdir "$srcdir/data/atlas_2010_14293"
    fi  
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2010_14293/Likelihoods.tar.gz" "https://doi.org/10.17182/hepdata.95664.v2/r9"`
    `tar -xvzf "$srcdir/data/atlas_2010_14293/Likelihoods.tar.gz" -C "$srcdir/data/atlas_2010_14293"`
  fi
  # Likelihoods for atlas_2211_08028
  # `curl -OJLH "Accept: application/x-tar" https://doi.org/10.17182/hepdata.95928.v1/r5`
  # ONNX for atlas_2106_09609
  if ! test -f "$srcdir/data/atlas_2106_09609/SUSY-2019-04-ONNX.tgz"
    then
    if ! test -d "$srcdir/data/atlas_2106_09609/"
    then
      mkdir "$srcdir/data/atlas_2106_09609/"
    fi
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2106_09609/SUSY-2019-04-ONNX.tgz" "https://doi.org/10.17182/hepdata.104860.v1/r3"`
    `tar -zxvf "$srcdir/data/atlas_2106_09609/SUSY-2019-04-ONNX.tgz" --strip-components=1 -C "$srcdir/data/atlas_2106_09609/"`
  fi  
  # Likelihoods for atlas_1908_03122
  if ! test -f "$srcdir/data/atlas_1908_03122/HEPData_workspaces.tar.gz"
    then
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_1908_03122/HEPData_workspaces.tar.gz" "https://doi.org/10.17182/hepdata.89408.v3/r2"`
    if ! test -d "$srcdir/data/atlas_1908_03122/Likelihoods/"
    then
      mkdir "$srcdir/data/atlas_1908_03122/Likelihoods"
    fi  
    `tar -xvzf "$srcdir/data/atlas_1908_03122/HEPData_workspaces.tar.gz" -C "$srcdir/data/atlas_1908_03122/Likelihoods/"`
  fi 
  # Likelihoods for atlas_2101_01629
  if ! test -f "$srcdir/data/atlas_2101_01629/likelihoods_2101_01629.tar.gz"
    then
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2101_01629/likelihoods_2101_01629.tar.gz" "https://doi.org/10.17182/hepdata.97041.v2/r3"`  
    if ! test -d "$srcdir/data/atlas_2101_01629/Likelihoods/"
    then
      mkdir "$srcdir/data/atlas_2101_01629/Likelihoods"
    fi   
    `tar -xvzf "$srcdir/data/atlas_2101_01629/likelihoods_2101_01629.tar.gz" --strip-components=1 -C "$srcdir/data/atlas_2101_01629/Likelihoods"`
  fi 
  # Likelihoods for atlas_1908_08215
  if ! test -f "$srcdir/data/atlas_1908_08215/likelihoods_1908_08215.tar.gz"
    then
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_1908_08215/likelihoods_1908_08215.tar.gz" "https://doi.org/10.17182/hepdata.89413.v4/r5"`
    if ! test -d "$srcdir/data/atlas_1908_08215/Likelihoods/"
      then
      mkdir "$srcdir/data/atlas_1908_08215/Likelihoods"
    fi    
    `tar -xvzf "$srcdir/data/atlas_1908_08215/likelihoods_1908_08215.tar.gz" -C "$srcdir/data/atlas_1908_08215/Likelihoods"`
  fi 
  # Likelihoods for atlas_1911_12606
  if ! test -f "$srcdir/data/atlas_1911_12606/likelihoods_1911_12606.tar.gz"
    then
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_1911_12606/likelihoods_1911_12606.tar.gz" "https://doi.org/10.17182/hepdata.91374.v5/r6"`
    if ! test -d "$srcdir/data/atlas_1911_12606/Likelihoods/"
      then
      mkdir "$srcdir/data/atlas_1911_12606/Likelihoods"
    fi    
    `tar -xvzf "$srcdir/data/atlas_1911_12606/likelihoods_1911_12606.tar.gz" --strip-components=1 -C "$srcdir/data/atlas_1911_12606/Likelihoods"`
  fi
  # Likelihoods for atlas_2106_01676
  if ! test -f "$srcdir/data/atlas_2106_01676/likelihoods_2106_01676.tar.gz"
    then
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2106_01676/likelihoods_2106_01676.tar.gz" "https://doi.org/10.17182/hepdata.95751.v2/r3"`
    if ! test -d "$srcdir/data/atlas_2106_01676/Likelihoods/"
      then
      mkdir "$srcdir/data/atlas_2106_01676/Likelihoods"
    fi    
    `tar -xvzf "$srcdir/data/atlas_2106_01676/likelihoods_2106_01676.tar.gz" -C "$srcdir/data/atlas_2106_01676/Likelihoods"`
  fi  
  # Likelihoods for atlas_1911_06660
  if ! test -f "$srcdir/data/atlas_1911_06660/likelihoods_1911_06660.tar.gz"
    then
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_1911_06660/likelihoods_1911_06660.tar.gz" "https://doi.org/10.17182/hepdata.92006.v2/r2"`
    if ! test -d "$srcdir/data/atlas_1911_06660/Likelihoods/"
      then
      mkdir "$srcdir/data/atlas_1911_06660/Likelihoods"
    fi    
    `tar -xvzf "$srcdir/data/atlas_1911_06660/likelihoods_1911_06660.tar.gz" -C "$srcdir/data/atlas_1911_06660/Likelihoods"`
    #`mv "$srcdir/data/atlas_1911_06660/Region-combined/" "$srcdir/data/atlas_1911_06660/Likelihoods/"`
  fi  
  # BDT weights for atlas_2209_13935
  if ! test -f "$srcdir/data/atlas_2209_13935/SimpleAnalysisFiles.tar.gz"
    then
    if ! test -d "$srcdir/data/atlas_2209_13935"
    then 
      mkdir "$srcdir/data/atlas_2209_13935"
    fi  
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2209_13935/SimpleAnalysisFiles.tar.gz" "https://doi.org/10.17182/hepdata.134068.v1/r3"`
    `tar -xvzf "$srcdir/data/atlas_2209_13935/SimpleAnalysisFiles.tar.gz" -C "$srcdir/data/atlas_2209_13935"`
  fi
  # Likelihoods for atlas_2411_02040
  if ! test -f "$srcdir/data/atlas_2411_02040/ANA-HIGP-2024-32_HHH_SplusBfit_Likelihoods.tar.gz"
    then
    if ! test -d "$srcdir/data/atlas_2411_02040/Likelihoods"
    then
      mkdir "$srcdir/data/atlas_2411_02040/Likelihoods"
    fi
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2411_02040/ANA-HIGP-2024-32_HHH_SplusBfit_Likelihoods.tar.gz" "https://doi.org/10.17182/hepdata.157024.v1/r1"`
    `tar -xvzf "$srcdir/data/atlas_2411_02040/ANA-HIGP-2024-32_HHH_SplusBfit_Likelihoods.tar.gz" --strip-components=1 -C "$srcdir/data/atlas_2411_02040/Likelihoods/"`
  fi
  # Likelihoods for atlas_2211_08028
  if ! test -f "$srcdir/data/atlas_2211_08028/LH_jsons.tar.gz"
    then
    `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2211_08028/LH_jsons.tar.gz" "https://doi.org/10.17182/hepdata.95928.v2/r5"`
    if ! test -d "$srcdir/data/atlas_2211_08028/Likelihoods/"
      then
      mkdir "$srcdir/data/atlas_2211_08028/Likelihoods"
    fi    
    `tar -xvzf "$srcdir/data/atlas_2211_08028/LH_jsons.tar.gz" -C "$srcdir/data/atlas_2211_08028/Likelihoods"`
  fi
  # Likelihoods for atlas_2102_10874
  if ! test -f "$srcdir/data/atlas_2102_10874/Likelihoods/bOnlyFit_postFit.root"
    then
    if ! test -d "$srcdir/data/atlas_2102_10874/Likelihoods/"
      then
      mkdir "$srcdir/data/atlas_2102_10874/Likelihoods"
    fi
    `curl -JLH "Accept: text/plain" -o "$srcdir/data/atlas_2102_10874/Likelihoods/bOnlyFit_postFit.root" "https://doi.org/10.17182/hepdata.102093.v4/r3"`
  fi  
  AC_MSG_RESULT([])
])
