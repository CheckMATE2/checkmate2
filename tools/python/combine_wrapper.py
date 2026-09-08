import ROOT

# Configurations
FILE_PATH = "histograms.root"
YEARS = ["2016", "2017", "2018", "2022"]
CHANNELS = ["Electron", "Muon"]
SUB_PATH_TEMPLATE = "{year}/SR/{channel}/idm"

def new_signal(nominal):

    n_bins = nominal.GetNbinsX()
    for bin_idx in range(0, n_bins + 2):
    # Example: custom manipulation per bin
        scaled_content = h_nominal_copy.GetBinContent(bin_idx) * 2.0
        scaled_error = h_nominal_copy.GetBinError(bin_idx) * 2.0

        nominal.SetBinContent(bin_idx, scaled_content)
        nominal.SetBinError(bin_idx, scaled_error)



# 1. Open the file in UPDATE mode
f = ROOT.TFile.Open(FILE_PATH, "UPDATE")
if not f or f.IsZombie():
    print(f"Error: Could not open file {FILE_PATH}")
    exit(1)

for year in YEARS:
    for channel in CHANNELS:
        dir_path = SUB_PATH_TEMPLATE.format(year=year, channel=channel)
        print(f"\nProcessing directory: {dir_path}...")
        
        # 2. Safely navigate to the directory
        if not f.cd(dir_path):
            print(f"  --> Warning: Directory path not found, skipping {year} {channel}.")
            continue
            
        current_dir = ROOT.gDirectory
        
        # 3. Retrieve the nominal template
        h_nominal = current_dir.Get("nominal")
        if not h_nominal:
            print("  --> Error: 'nominal' histogram not found in this folder! Skipping.")
            continue
            
        # Clone nominal to safely keep a pristine copy in local memory
        h_nominal_copy = h_nominal.Clone("h_nominal_copy")

        new_signal(h_nominal_copy)

        h_nominal_copy.Write("nominal", ROOT.TObject.kOverwrite)

        n_bins = h_nominal_copy.GetNbinsX()
        
        # 4. Loop through all objects in the directory to find systematic histograms
        keys = current_dir.GetListOfKeys()
        systematic_histograms = []
        
        for key in keys:
            obj_name = key.GetName()
            # Skip the nominal histogram itself
            if obj_name == "nominal":
                continue
                
            obj = key.ReadObj()
            if isinstance(obj, ROOT.TH1):
                systematic_histograms.append(obj_name)
                
        if not systematic_histograms:
            print("  --> No systematic histograms found in this folder.")
            continue
            
        print(f"  --> Found {len(systematic_histograms)} systematic shapes to nullify.")
        
        # 5. Overwrite each systematic histogram bin-by-bin
        for hist_name in systematic_histograms:
            h_sys = current_dir.Get(hist_name)
            
            # Overwrite all bins, including underflow (0) and overflow (n_bins + 1)
            for bin_idx in range(0, n_bins + 2):
                nom_content = h_nominal_copy.GetBinContent(bin_idx)
                nom_error = h_nominal_copy.GetBinError(bin_idx)
                
                h_sys.SetBinContent(bin_idx, nom_content)
                h_sys.SetBinError(bin_idx, nom_error)
                
            # Write the modified histogram back using kOverwrite to replace existing cycles
            h_sys.Write(hist_name, ROOT.TObject.kOverwrite)
            #current_dir.WriteObject(h_sys, hist_name, ROOT.TObject.kOverwrite)
            print(f"    * Overwrote {hist_name}")

# 6. Save changes and clean up
print("\nSaving and closing file...")
f.Close()
print("All done! Both Electron and Muon systematic channels are completely nullified.")
