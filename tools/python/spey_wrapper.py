import spey
import json, pyhf
import numpy as np
import os
from info import Info
from advprint import AdvPrint
import multibin_limit as mb
import multibin_limit_full as mbfull

def calc_point( path, analysis, mbsr ):
    
    inv_r = 10. 
    inv_r_exp = 10. 
    cls_obs = 1. 
    cls_exp = [1.,1.,1.,1.,1.]
    
    os.system("mkdir -p " + path + '/multibin_limits')
    
    if analysis == "atlas_2101_01629":
        zeroev = True
        for mbhist in Info.get_analysis_parameters(analysis)["mb_histos"]:
            mbfull.init(path, analysis, mbhist)
            names = mbfull.SR_dict.keys()
            SRs = mbfull.data_from_CMresults(path)
            o, b, db, s, ds = mbfull.select_MBsr(names, SRs)
            if max(s) > 0.:
                zeroev = False
            if mbhist == "2J_bveto":
                patchset = mbfull.create_patchset(path, names, s, ds, systematics = 0)
            else:    
                patchset = mbfull.add_patchset(path, names, s, ds, patchset, systematics = 0)       
            if zeroev:
                AdvPrint.cout("No signal events in the selected SRs! Skipping")
                return 10., 10., 1., 1.
                
    else:        
        mbfull.init(path, analysis, mbsr)
        names = mbfull.SR_dict.keys()
        SRs = mbfull.data_from_CMresults(path)
        o, b, db, s, ds = mbfull.select_MBsr(names, SRs)
        if max(s) == 0.:
            AdvPrint.cout("No signal events in the selected SRs! Skipping")
            return 10., 10., 1., 1.
        patchset = mbfull.create_patchset(path, names, s, ds, systematics = 0)
    
    #with open(path+'/multibin_limits/'+"workspace.json", "w") as write_file:
    #    json.dump(patchset, write_file, indent=4)       
    signal0 = pyhf.PatchSet(patchset)["Signal0"]
    
    stat_wrapper = spey.get_backend ("pyhf")
    
    with open(Info.paths['data']+ "/" + analysis + "/pyhf_conf.json") as serialized:
        conf = json.load(serialized)
    ind = [x['name'] for x in conf["analysis"]].index(mbsr)
    bkgonly = conf["analysis"][ind]['bkgonly']
                
    with open(Info.paths['data']+ "/" + analysis + '/Likelihoods/' + bkgonly) as serialized:
        bckg_input = json.load(serialized)
    
    stat_model = stat_wrapper(analysis="simple_pyhf", background_only_model = bckg_input, signal_patch = signal0)
    
    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    string += f"Limits with full likelihood (spey):\n"
    if Info.flags["mbcls"]:
        AdvPrint.cout("Observed:")
        cls_obs_all = stat_model.exclusion_confidence_level(expected = spey.ExpectationType.observed)
        cls_obs = cls_obs_all[0]
        if 1. - cls_obs < 0.00000001:
            cls_obs = 0.
            AdvPrint.cout("CLs calculation unreliable.")
        AdvPrint.cout("CL95: "+str(1. - cls_obs) )
        string += f"Observed CLs for mu = 1: {1. - cls_obs}"+'\n'
        if Info.flags["expected"]:
            cls_exp =  stat_model.exclusion_confidence_level(expected = spey.ExpectationType.apriori) 
            string = string+f"Expected CLs band for mu = 1: {[1.-exp for exp in cls_exp]}"+'\n'
    if Info.flags["uplim"]:
        AdvPrint.cout("Observed:")
        inv_r = stat_model.poi_upper_limit(expected = spey.ExpectationType.observed)
        string = string+f"Observed upper limit: mu = {inv_r:.4f}"+'\n'
        AdvPrint.cout("Upper limit: "+str(inv_r) )
        if Info.flags["expected"]:
            inv_r_exp =  stat_model.poi_upper_limit(expected = spey.ExpectationType.apriori) 
            #inv_r_exp_1sigma =  stat_model.poi_upper_limit(expected = spey.ExpectationType.apriori, expected_pvalue="1sigma")  # for the band 1 (2) sigma band
            string = string+f"Expected upper limit: mu = {inv_r_exp:.4f}"+'\n'
    string += "\n================================\n"
    
    with open(path+'/multibin_limits/'+"results.dat", "a") as write_file:
        write_file.write(string)
        write_file.close()

    return inv_r, inv_r_exp, 1-cls_obs, cls_exp

def calc_cov(path, analysis, mbsr):
    
    inv_r = 10. 
    inv_r_exp = 10. 
    cls_obs = 1.
    cls_exp = [1.,1.,1.,1.,1.]
    
    os.system("mkdir -p " + path + '/multibin_limits')
    
    mb_signal_regions = Info.get_analysis_parameters(analysis)["mb_signal_regions"]
    sr_list = mb_signal_regions[mbsr]
    o, b, db, s, ds = mb.select_MBsr(sr_list, mb.data_from_CMresults(Info.paths['output'])) #prepare data
    stat_wrapper = spey.get_backend ("default_pdf.correlated_background")
    
    cov_mat = mb.get_cov(analysis, db, Info.flags["corr"], mbsr)
    if  Info.flags["corr"] and analysis == "cms_1908_04722":
        covariance = "correlation"
    else:    
        covariance = "covariance"
    #dim = 2
    #AdvPrint.cout(np.array(cov_mat)[0,0])
    #det = np.linalg.det(np.array(cov_mat)[0:dim,0:dim])
    #det = np.linalg.det(np.array(cov_mat))
    #sign, logdet = np.linalg.slogdet(np.array(cov_mat)[0:dim,0:dim])
    #sign, logdet = np.linalg.slogdet(np.array(cov_mat))
    #AdvPrint.cout(mbsr)
    #AdvPrint.cout("Det: " + str(det))
    #AdvPrint.cout("Log det: " + str(sign) + " " + str(logdet))
    #AdvPrint.cout(str(s))
    #stat_model = stat_wrapper(signal_yields = np.array(s)[0:dim], background_yields = np.array(b)[0:dim], data = np.array(o)[0:dim], covariance_matrix = np.array(cov_mat)[0:dim,0:dim])
    stat_model = stat_wrapper(analysis = analysis+mbsr , signal_yields = np.array(s), background_yields = np.array(b), data = np.array(o), covariance_matrix = np.array(cov_mat))
    
    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    string = string + "Limits with "+ covariance + " matrix likelihood (spey):\n"
    if Info.flags["mbcls"]:
        AdvPrint.cout("Observed:")
        cls_obs = stat_model.exclusion_confidence_level(expected = spey.ExpectationType.observed)[0]
        AdvPrint.cout("CL95: "+str(1. - cls_obs) )
        string += f"Observed CLs for mu = 1: {1. - cls_obs}"+'\n'
        if Info.flags["expected"]:
            cls_exp =  stat_model.exclusion_confidence_level(expected = spey.ExpectationType.apriori) 
            string = string+f"Expected CLs band for mu = 1: {[1.-exp for exp in cls_exp]}"+'\n'
    if Info.flags["uplim"]:
        AdvPrint.cout("Observed:")
        inv_r = stat_model.poi_upper_limit(expected = spey.ExpectationType.observed)
        string = string+f"Observed upper limit: mu = {inv_r:.4f}"+'\n'
        AdvPrint.cout("Upper limit: "+str(inv_r) )
        if Info.flags["expected"]:
            inv_r_exp =  stat_model.poi_upper_limit(expected = spey.ExpectationType.apriori) 
            #inv_r_exp_1sigma =  stat_model.poi_upper_limit(expected = spey.ExpectationType.apriori, expected_pvalue="1sigma")  # for the band 1 (2) sigma band
            string = string+f"Expected upper limit: mu = {inv_r_exp:.4f}"
    string += "\n================================\n"
    
    with open(path+'/multibin_limits/'+"results.dat", "a") as write_file:
        write_file.write(string)

    return inv_r, inv_r_exp, 1-cls_obs, cls_exp, stat_model    


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
            mb_signal_regions = ["SR-S-high-ee"]
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
                
                AdvPrint.cout("Expected:")
                #AdvPrint.cout( stat_model.exclusion_confidence_level(expected = spey.ExpectationType.apriori) )
                inv_r_exp =  stat_model.poi_upper_limit(expected = spey.ExpectationType.apriori) 
                AdvPrint.cout("Upper limit: "+str(inv_r_exp) )                
                
                string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
                string += f"Limits with full likelihood (method = SPEY):\n\n"
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
    mbsr = "combination"
    analysis = "cms_sus_19_005"
    mbsr = "AllBins"    
    mb_signal_regions = Info.get_analysis_parameters(analysis)["mb_signal_regions"]
    AdvPrint.cout("Calculating approximate likelihood with covariance matrix: "+analysis+", SR: "+mbsr+"... ")
    sr_list = mb_signal_regions[mbsr]
    o, b, db, s, ds = mb.select_MBsr(sr_list, mb.data_from_CMresults(Info.paths['output'])) #prepare data
    stat_wrapper = spey.get_backend ("default_pdf.correlated_background")
    corrmat = False
    cov_mat = mb.get_cov(analysis, corrmat)
    stat_model1 = stat_wrapper(analysis = analysis, signal_yields = np.array(s), background_yields = np.array(b), data = np.array(o), covariance_matrix = np.array(cov_mat))
    AdvPrint.cout("Upper limit: "+str(stat_model1.poi_upper_limit(expected = spey.ExpectationType.observed)) )
    
    analysis = "atlas_1908_08215"
    mbsr = "SR-DF-0J"
    analysis = "atlas_2010_14293"
    mbsr = "MB-SSd"    
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
    stat_model2 = stat_wrapper(analysis = analysis, background_only_model = bckg_input, signal_patch = signal0)
    
    #AdvPrint.cout("Upper limit: "+ str(stat_model2.poi_upper_limit(expected = spey.ExpectationType.observed) ))
    
    AdvPrint.cout("Combination")
    combined = spey.UnCorrStatisticsCombiner(*[stat_model1, stat_model2])
    AdvPrint.cout("Combined upper limit: " + str(combined.poi_upper_limit(expected = spey.ExpectationType.observed)) )
    
def combination_stat(model_list):
    #AdvPrint.cout(model_list)
    combined = spey.UnCorrStatisticsCombiner(*model_list)
    AdvPrint.cout("Combined upper limit: " + str(combined.poi_upper_limit(expected = spey.ExpectationType.observed)) )
