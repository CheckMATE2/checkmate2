import spey
import json, pyhf
import numpy as np
import os
from info import Info
from advprint import AdvPrint
import multibin_limit as mb
import multibin_limit_full as mbfull

def get_limits():

    best_invr=10.
    best_cls=1.
    best_analysis_r=""
    best_sr_r=""
    best_analysis_cls=""
    best_sr_cls=""
    full = "?"
    AdvPrint.cout("")
    path = Info.paths['output']
    os.system("mkdir -p " + Info.paths['output'] + '/multibin_limits')
    global hepfiles_folder 
    hepfiles_folder = Info.paths['data']+"/" 
    for analysis in Info.analyses:
        if "mb_signal_regions" in Info.get_analysis_parameters(analysis) and Info.get_analysis_parameters(analysis)["likelihoods"] == "y": #ATLAS
            mb_signal_regions = Info.get_analysis_parameters(analysis)["mb_signal_regions"]
            for mbsr in mb_signal_regions:
                
                AdvPrint.cout("Calculating full likelihood upper limits and CLs for analysis: "+analysis+", SR: "+mbsr+"... ")
                mbfull.init(path, analysis, mbsr)
                names = mbfull.SR_dict.keys()
                SRs = mbfull.data_from_CMresults(path)
                o, b, db, s, ds = mbfull.select_MBsr(names, SRs)
                patchset = mbfull.create_patchset(path, names, s, ds, systematics = 0)

                signal0 = pyhf.PatchSet(patchset)["Signal0"]
                
                stat_wrapper = spey.get_backend ("pyhf")
                
                with open(hepfiles_folder + analysis + "/pyhf_conf.json") as serialized:
                    conf = json.load(serialized)
                ind = [x['name'] for x in conf["analysis"]].index(mbsr)
                bkgonly = conf["analysis"][ind]['bkgonly']
                
                with open(Info.paths['data']+ "/" + analysis + '/Likelihoods/' + bkgonly) as serialized:
                    bckg_input = json.load(serialized)
                #stat_model = stat_wrapper(bckg_input, signal0)
                stat_model = stat_wrapper(analysis="simple_pyhf", background_only_model = bckg_input, signal_patch = signal0)
                
                AdvPrint.cout("Observed:")
                #cls_o = 1. - stat_model.exclusion_confidence_level(expected = spey.ExpectationType.observed)
                inv_r = stat_model.poi_upper_limit(expected = spey.ExpectationType.observed)
                AdvPrint.cout("Upper limit: "+str(inv_r) )
        
        
        if "mb_signal_regions" in Info.get_analysis_parameters(analysis) and Info.get_analysis_parameters(analysis)["likelihoods"] == "cov":
            mb_signal_regions = Info.get_analysis_parameters(analysis)["mb_signal_regions"]
            for mbsr in mb_signal_regions:
                
                AdvPrint.cout("Calculating approximate likelihood with covariance matrix: "+analysis+", SR: "+mbsr+"... ")
                sr_list = mb_signal_regions[mbsr]
                o, b, db, s, ds = mb.select_MBsr(sr_list, mb.data_from_CMresults(Info.paths['output'])) #prepare data
                stat_wrapper = spey.get_backend ("default_pdf.correlated_background")
                corrmat = False
                cov_mat = mb.get_cov(analysis, corrmat)

                stat_model = stat_wrapper(signal_yields = np.array(s), background_yields = np.array(b), data = np.array(o), covariance_matrix = np.array(cov_mat))
                
                AdvPrint.cout("Observed:")
                #cls_o = 1. - stat_model.exclusion_confidence_level(expected = spey.ExpectationType.observed)
                inv_r = stat_model.poi_upper_limit(expected = spey.ExpectationType.observed)
                AdvPrint.cout("Upper limit: "+str(inv_r) )

                AdvPrint.cout("Expected:")
                #AdvPrint.cout( stat_model.exclusion_confidence_level(expected = spey.ExpectationType.apriori) )
                inv_r_exp =  stat_model.poi_upper_limit(expected = spey.ExpectationType.apriori) 
                AdvPrint.cout("Upper limit: "+str(inv_r_exp) )
                
                string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
                string += f"Limits with covariance matrix (method = SPEY, correlation matrix = "+ str(corrmat) + "):\n\n"
                if inv_r < 10.:
                    string += f"Upper limit (observed): mu = {inv_r:.10f}"+'\n'
                else:
                    string += f"Calculation of the upper limit failed or the upper limit out of range\n"
                if inv_r_exp < 10.:
                    string += f"Upper limit (expected): mu = {inv_r_exp:.10f}"+'\n'
                else:
                    string += f"Calculation of the upper limit failed or the upper limit out of range\n"
                string += "\n================================\n"
                with open(Info.paths['output'] + '/multibin_limits/'+"results.dat", "a") as write_file:
                    write_file.write(string)
                
                AdvPrint.cout("Done!")
                if inv_r < best_invr:
                    best_invr = inv_r
                    best_analysis_r = analysis
                    best_sr_r = mbsr
                    full = "cov"
                    
    if best_invr < 10.:
        AdvPrint.set_cout_file(Info.files["output_result"], False)
        if full == "cov":
            AdvPrint.cout("\nTest: Calculation of upper limit using covariance matrix for multibin signal regions")  
        else:
            AdvPrint.cout("\nTest: Calculation of upper limit using full likelihood for multibin signal regions")            
        if best_invr < 1.:
            result = "\033[31mExcluded\033[0m"
        else:
            result = "\033[32mAllowed\033[0m"
        AdvPrint.cout("Result: "+result)
        AdvPrint.cout("Result for 1/mu (r): "+str(1./best_invr))
        AdvPrint.cout("Analysis: "+best_analysis_r)
        AdvPrint.cout("MBSR: "+best_sr_r)
        AdvPrint.set_cout_file("#None")                    


def combination(analyses):
    
    path = Info.paths['output']
    os.system("mkdir -p " + Info.paths['output'] + '/multibin_limits')
    global hepfiles_folder 
    hepfiles_folder = Info.paths['data']+"/" 
    
    analysis = "cms_2205_09597"
    mbsr = "AllBins"
    sr_list = mb_signal_regions[mbsr]
    o, b, db, s, ds = mb.select_MBsr(sr_list, mb.data_from_CMresults(Info.paths['output'])) #prepare data
    stat_wrapper = spey.get_backend ("default_pdf.correlated_background")
    corrmat = False
    cov_mat = mb.get_cov(analysis, corrmat)
    stat_model1 = stat_wrapper(signal_yields = np.array(s), background_yields = np.array(b), data = np.array(o), covariance_matrix = np.array(cov_mat))
    AdvPrint.cout("Upper limit: "+str(stat_model1.poi_upper_limit(expected = spey.ExpectationType.observed)) )
    
    analysis = "atlas_2106_01676"
    mbsr = "AllBins"
