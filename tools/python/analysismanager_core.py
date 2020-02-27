import json
from math import sqrt
from multiprocessing import Pool
import os
import time

from advprint import AdvPrint
from get_s95 import find_s95_obs, find_s95_exp
from info import Info

# Global variable to keep track if indentation
Printlevel = 1

def myprint(text):
    """Indents text according to Printlevel"""
    print "    "*Printlevel + text

def ask(question, allowed_answers="@", allow_empty=False):
    """Asks user a question with right indentation and only accepts a predefined set
         of answers ("@" allows everything, "#" allows numbers only)"""
    global Printlevel
    if question != "":
      myprint(question)
    Printlevel += 1
    while True:
        answer = raw_input("    "*Printlevel)
        if allow_empty and answer == "": # zero entry is only allowed if explicitly told
          break
        elif answer != "" and allowed_answers == "@": # @ allows everything
            break
        elif answer != "" and allowed_answers == "#" and all(c in '+-.0123456789' for c in answer): # # only allows numbers
            answer = str(float(answer)) # integer numbers are always stored as floats to make sure that "1" and "1." are considered as equal
            break
        elif answer != "" and answer in allowed_answers: # allowed_answers can be a string which always includes ""
            break
        print "    "*Printlevel + " (Invalid Input)"
    Printlevel -= 1
    return answer

def ask_list(question, options):
    """
    Ask the user to select an element from a list of options.
    """
    global Printlevel
    myprint(question)
    Printlevel += 1
    max_index = len(options)
    for n, option in enumerate(options):
        myprint("{}) {}".format(n+1, option))
    while True:
        answer = raw_input("    "*Printlevel).strip()
        try:
            index = int(answer)
            if index < 1:
                myprint("Your answer must be larger than 0.")
            elif index > max_index:
                myprint("Your answer must be less than or equal to {}.".format(max_index))
            else:
                break
        except ValueError:
            myprint("Your answer must be an integer.")
    return options[index-1]

def ask_multiline(question, allowed_answers="@"):
    """runs 'ask' and stores answers in list as long as no empty line appears"""
    lines = list()
    allow_empty = False # Allow them after the first item was entered
    while True:
        line = ask(question, allowed_answers, allow_empty)
        allow_empty = True
        question = "" # Only ask the question on the first iteration
        if line == "":
            break
        lines.append(line)
    return lines

def myround(x, y):
    """ Rounds number x to the same number of digits as y """
    y_comma_split = str(y).split(".")
    # if y is an integer, return integer x
    if len(y_comma_split) == 1:
        return int(round(x))
    # otherwise, find number of digits after the comma in y and round x accordingly
    ndigits = len(y_comma_split[1])
    return round(x, ndigits)

def compute_s95(obs, bkg, bkg_err):
    """
    Computes the S95 values and rounds them to the same accuracy as `bkg`.
    s95_exp and s95_obs have to be called in this exact order, because s95_exp
    uses global variables set by s95_obs

    compute_s95 returns a tuple (s95_obs, s95_exp)
    """
    sobs = s95_obs([obs, bkg, bkg_err])
    sexp = s95_exp([obs, bkg, bkg_err])
    return (sobs, sexp)

def s95_obs(x):
    """ Runs s95_obs script and rounds result to accuracy of B"""
    res = find_s95_obs(float(x[0]), 0., float(x[1]), float(x[2]))
    return myround(res, x[1])

def s95_exp(x):
    """ Runs s95_exp script and rounds result to accuracy of B"""
    res = find_s95_exp(float(x[0]), float(x[1]), float(x[2]))
    # round to same number of digits as bkg
    return myround(res, x[1])

def count_finished(queue):
    """ Checks how many queued processes in all queues have finished already"""
    count, total = 0, 0
    for process in queue:
        total += 1
        if process.ready():
            count += 1
    return (count, total)


def write_analysis_parameters(parameters):
    """ Writes the _var.j file of given a set of parameters """
    global Printlevel
    analysis = parameters["analysis"]
    Info.fill_standard_paths_and_files()
    jfile = open(Info.files['analysis_settings'][analysis], "wb")
    jfile.write(json.dumps(parameters, sort_keys=True, indent=2))
    jfile.close()
    myprint(" - Variable values saved in " + Info.files['analysis_settings'][analysis])

def ask_for_general_information(parameters):
    """ Asks for standard information (name, description, author, lumi)
            and stores output in parameters """
    global Printlevel
    BASELEVEL = Printlevel
    Info.fill_standard_paths_and_files()
    while True:
        try:
            Printlevel = BASELEVEL
            Printlevel += 1

            ask_for_analysis_name(parameters, BASELEVEL)

            parameters["author"] = ask("Your Name (to declare the analysis author): ")
            parameters["authoremail"] = ask("Your Email: ")
            parameters["short_info"] = ask("Analysis Description (short, one line): ")
            # Replace two spaces by one, as two spaces are internally interpreted as column separators in CheckMATE
            while parameters["short_info"].find("    ") != -1:
                parameters["short_info"] = parameters["short_info"].replace("    ", " ")

            parameters["long_info"] = ask_multiline("Description (long, multiple lines, finish with empty line: ")
            parameters["lumi"] = ask("Luminosity (in fb^-1): ", "#")
            parameters["ecm"] = ask("Center of Mass Energy (in TeV): ", "#")
            myprint("If wanted, you can add tags which classify the analysis (e.g. stops, etmiss, ...) ")
            myprint("This makes it easier to later to find out if the analysis is useful for whatever")
            myprint("the users think is most sensitive to their input model")
            parameters["tags"] = ask_multiline("Tag (one per line, finish with empty line)")
            parameters["cr"] = ask("Do you plan to implement control regions to that analysis? [(y)es, (n)o]", "yn")
            break
        except KeyboardInterrupt:
            try:
                myprint('')
                myprint('>>> Caught Interrupting Signal')
                myprint('>>> Press any key to restart current question block')
                myprint('        or resend signal to abort')
                raw_input("")
            except KeyboardInterrupt:
                exit(1)
    Printlevel = BASELEVEL
    myprint('')
    return parameters

def validate_analysis_name(name):
    """
    Checks if the analysis name contains invalid characters.

    The problem with analysis names is that the name of a C++ class is
    derived from them. Therefore only letters, numbers and '_' are
    allowed.
    Also, only lower case names are allowed, because CheckMATE looks for
    analyses case insensitively. Therefore analyses with names
    that contain uppercase characters can't be found.

    Returns: True if the name is allowed, false otherwise.
    """
    for char in name:
        if not char.isalnum() and char != "_":
            return False
        if char.isupper():
            return False
    return True

def ask_for_analysis_name(parameters, BASELEVEL):
    """
    Asks the user for the name of the new analysis.
    """
    Printlevel = BASELEVEL + 1
    while(True): # Loop until we get a valid name, or the user aborts.
        analysis_name = ask("Analysis Name: ")
        if not validate_analysis_name(analysis_name):
            myprint("This analysis name is not allowed.")
            myprint(
                    "Analysis names must be lowercase and may only contain letters,"
                    " numbers and underscores."
                    )
        else:
            break

    parameters["analysis"] = analysis_name
    if parameters["analysis"] in Info.files["analysis_settings"]:
        # There is already an analysis of this name in CheckMATE.
        overwrite = ask("An analysis of this name already exists! Do you want to overwrite it? [(y)es, (n)o]", "yn")
        if overwrite == "y":
            myprint("Removing files for old analysis.")
            settings_file = Info.files["analysis_settings"][analysis_name]
            group = Info.analysis_group_map[analysis_name]
            old_parameters = read_analysis_parameters(settings_file)
            remove_analysis_files(old_parameters, group, delete_settings=True)
        else:
            exit("")
    else:
        # Check whether parameter file already exists
        for group in Info.analysis_groups:
            settings_file = os.path.join(
                    Info.paths['analysis_info'],
                    group,
                    "{}_var.j".format(analysis_name)
                    )
            if os.path.isfile(settings_file):
                # There was an analysis of this name in CheckMATE, but it has been removed.
                skip = ask("A parameter file for this analysis already exists! [(u)se, (o)verwrite]", "uo")
                if skip == "u":
                    parameters = read_analysis_parameters(settings_file)
                    Info.add_analysis(analysis_name, group)
                    # This is the easiest way to keep everything consistent and
                    # to not add the analysis to some files multiple times.
                    remove_analysis_files(parameters, group, keep_sources=True)
                    add_analysis_files(parameters, group, keep_sources=True)
                    Printlevel -= 1
                    myprint("Analysis " + analysis_name + " has been added successfully!")
                    myprint("Run 'autoreconf', 'configure [+options]', 'make' from the main CheckMATE folder to compile it!")
                    myprint("")
                    exit("")

def normalize_error(reference_data):
    """
    For one signal region, compute the total error on the background from the
    errors given by the user.
    """
    if "bkg_err" in reference_data:
        return float(reference_data["bkg_err"])
    if "bkg_errp" in reference_data and "bkg_errm" in reference_data:
        # Asymmetric error: as a rough approximation, use the mean of the squares
        upper = float(reference_data["bkg_errp"])
        lower = float(reference_data["bkg_errm"])
        return sqrt((upper**2+lower**2)/2)
    if "bkg_err_sys" in reference_data and "bkg_err_stat" in reference_data:
        # Total error = independent quadratic sum of statistical and systematical component
        sys = float(reference_data["bkg_err_sys"])
        stat = float(reference_data["bkg_err_stat"])
        return sqrt(sys**2 + stat**2)
    if "bkg_errp_sys" in reference_data and "bkg_errm_sys" in reference_data and "bkg_err_stat" in reference_data:
        sys_upper = float(reference_data["bkg_errp_sys"])
        sys_lower = float(reference_data["bkg_errm_sys"])
        stat = float(reference_data["bkg_err_stat"])
        return sqrt(stat**2 + (sys_upper**2 + sys_lower**2)/2)
    AdvPrint.cerr_exit("Invalid combination of errors given.")


def recompute_s95_sr(reference_data):
    """
    Computes S95_obs and S95_exp for a single signal region from the reference
    data for the region.

    recompute_s95_sr returns a tuple (s95_obs, s95_exp)
    """
    obs = reference_data["obs"]
    bkg = reference_data["bkg"]
    bkg_err = normalize_error(reference_data)
    sobs, sexp = compute_s95(obs, bkg, bkg_err)
    # Should bkg and obs be identical, the S95 values should be identical too.
    # For numerical reasons, it can happen that they aren't. In that case, choose
    # the mean
    if bkg == obs:
      S95 = myround((s95_exp+s95_obs)/2., bkg)
      sexp = s95 = S95
    return (sobs, sexp)

def recompute_s95(parameters, signal_regions=None):
    """
    Recompute S95_obs and S95_exp for the given signal regions.
    If `signal_regions` is `None`, then all signal regions are used.
    """
    if signal_regions == None:
        signal_regions = parameters["signal_regions"]
    reference_data = parameters["reference_data"]
    for sr in signal_regions:
        reference_data_sr = reference_data[sr]
        sobs, sexp = recompute_s95_sr(reference_data_sr)
        reference_data_sr["S95_obs"] = str(sobs)
        reference_data_sr["S95_exp"] = str(sexp)
        print "SR {}: S95_obs = {}, S95_exp = {}".format(sr, sobs, sexp)


def ask_for_signal_region_information(parameters):
    """ Fills the signal region table with information like B, dB, O and starts S95 calculations.
            Returns parameters and queueus to access the S95 results later."""
    global Printlevel
    BASELEVEL = Printlevel

    myprint("Information: We are now going to ask you which numbers you want to provide for each signal region. ")
    myprint("The following items are possible:")
    Printlevel += 1
    allowed = ["obs", "bkg", "bkg_err_stat", "bkg_err_sys", "bkg_errm_sys", "bkg_errp_sys", "bkg_err", "bkg_errp", "bkg_errm"]
    myprint("obs:                    Observed number of events")
    myprint("bkg:                    Expected number of background events")
    myprint("bkg_err:            Expected total error on bkg ")
    myprint("bkg_errp:         Expected total upper error (in case of asymmetric errors)")
    myprint("bkg_errm:         Expected total lower error (in case of asymmetric errors)")
    myprint("bkg_err_stat: Expected statistical error on bkg")
    myprint("bkg_err_sys:    Expected systematical error on bkg (in case of symmetric errors)")
    myprint("bkg_errp_sys: Expected systematical upper error (in case of asymmetric errors)")
    myprint("bkg_errm_sys: Expected systematical lower error (in case of asymmetric errors)")
    Printlevel -= 1
    myprint("Note that not all of these numbers have to be given (e.g. you don't have to give the total error if you give the individual stat and sys contributions)")
    myprint("However, there are some requirements, about which you will be warned if you don't meet them (e.g. giving xyz_errp without xyz_errm)")
    myprint("The standard, minimum set of information consists of obs, bkg and bkg_err")
    while True:  # Pseudoloop that breaks if loop code ended successfully
        Printlevel = BASELEVEL+1
        parameters["info_set"] = ask_multiline("List all categories you want to supply (one per line)", allowed)
        if "bkg" not in parameters["info_set"]:
            myprint("You have to provide the number of background events! Starting Over.")
        elif "obs" not in parameters["info_set"]:
            myprint("You have to provide the number of observed events! Starting Over.")
        elif "bkg_err" not in parameters["info_set"] and "bkg_errp" not in parameters["info_set"] and "bkg_err_sys" not in parameters["info_set"] and "bkg_errp_sys" not in parameters["info_set"]:
            myprint("You have to provide an error on the number of background events! Starting Over")
        elif ("bkg_err_sys" in parameters["info_set"] or "bkg_errp_sys" in parameters["info_set"]) and "bkg_err_stat" not in parameters["info_set"]:
            myprint("You cannot give the systematic error without the statistical error! Starting Over")
        elif "bkg_errp_sys" in parameters["info_set"] and "bkg_errm_sys" not in parameters["info_set"]:
            myprint("You have to give both sides in case of asymmetric errors! Starting Over")
        elif "bkg_errp" in parameters["info_set"] and "bkg_errm" not in parameters["info_set"]:
            myprint("You have to give both sides in case of asymmetric errors! Starting Over")
        else:
            break

    PRINTLEVEL = BASELEVEL
    myprint("The set of information you entered is valid.")
    myprint("You now have to add the numbers for each of the given signal regions.")
    myprint("Note that while you enter more numbers, the corresponding model independent")
    myprint(" 95\% confidence limits for the items you have already entered are calculated")
    myprint(" in the background. ")

    try:
        while True:  # Pseudoloop that breaks if loop code ended successfully
            Printlevel = BASELEVEL+1
            parameters["reference_data"] = dict()
            # Needed for internal evaluation of S95 values
            pool = Pool(processes=1)
            S95_queue, S95_inputs = list(), list()
            for sr in parameters["signal_regions"]:
                myprint(sr)
                Printlevel += 1
                parameters["reference_data"][sr] = dict()
                for info in parameters["info_set"]:
                    parameters["reference_data"][sr][info] = ask(info + ": ")

                # Find the information necessary for S95 calculation
                obs = parameters["reference_data"][sr]["obs"]
                bkg = parameters["reference_data"][sr]["bkg"]

                # The S95 calculation needs a total background error
                bkg_err = normalize_error(parameters["reference_data"][sr])

                # Push S95 calculations into queue
                S95_queue.append(pool.apply_async(compute_s95, (obs, bkg, bkg_err)))
                S95_inputs.append((sr, obs, bkg, bkg_err))
                progress = count_finished(S95_queue)
                myprint("S95obs and S95exp values are calculated internally (progress: " + str(progress[0]) + " / " + str(progress[1]) + ")")
                Printlevel -= 1
            parameters["info_set"].append("S95_obs")  # Added internally since internally calculated
            parameters["info_set"].append("S95_exp")  # Added internally since internally calculated
            break
    except KeyboardInterrupt:
        try:
            myprint('')
            myprint('>>> Caught Interrupting Signal')
            myprint('>>> Press any key to restart current question block')
            myprint('        or resend signal to abort')
            raw_input("")
        except KeyboardInterrupt:
            exit(1)
    Printlevel = BASELEVEL
    return parameters, S95_queue, S95_inputs

def ask_for_experiment(parameters):
    detector = ask("To which experiment does the analysis correspond? [(A)TLAS, (C)MS]", "AC");
    if detector == "A":
        detector = "ATLAS"
    else:
        detector = "CMS"

    ecm = float(parameters["ecm"])
    setups = Info.detector_setups[detector].get(ecm)
    if setups == None:
        AdvPrint.cerr_exit("Error: There is no CheckMATE setup for {} at a center of mass energy of {}TeV.".format(detector, ecm))
        return Info.detector_setups[detector]["default"]
    setups = setups.keys()
    if len(setups) == 1:
        return Info.detector_setups[detector][ecm][setups[0]]
    else:
        setup = ask_list("Select one of the available setups for {} at {}TeV.".format(detector, ecm), setups)
        return Info.detector_setups[detector][ecm][setup]


def ask_for_detector_information(parameters):
    """ Asks for final state properties that have to be determined within Delphes like
            jet properties and lepton isolation critera."""
    global Printlevel
    BASELEVEL = Printlevel
    while True:
        try:
            Printlevel = BASELEVEL
            Printlevel += 1
            myprint("3.1: Miscellaneous")
            Printlevel += 1
            experiment, group = ask_for_experiment(parameters);
            parameters["experiment"] = experiment
            Printlevel -= 1

            myprint("3.2: Electron Isolation")
            parameters["electron_iso_source"] = ["c"]
            parameters["electron_iso_dR"] = ["0.2"]
            parameters["electron_iso_ptmin"] = ["0.1"]
            parameters["electron_iso_absorrel"] = ["r"]
            parameters["electron_iso_ptratiomax"] = ["0.2"]
            parameters["electron_niso"] = 1
            Printlevel += 1
            c = ask("Do you need any particular isolation criterion? [(y)es, (n)o]", "yn")
            while c != "n":
                myprint("Isolation " + str(parameters["electron_niso"]) + ":")
                parameters["electron_niso"] += 1
                Printlevel += 1
                parameters["electron_iso_source"].append(ask("Which objects should be considered for isolation? [(t)racks, (c)alo objects?]", "tc"))
                parameters["electron_iso_ptmin"].append(ask("What is the minimum pt of a surrounding object to be used for isolation (default ATLAS values: tracks=0.4, calo=0.1)? [in GeV]", "#"))
                parameters["electron_iso_dR"].append(ask("What is the dR used for isolation?", "#"))
                parameters["electron_iso_absorrel"].append(ask("Is there an absolute or a relative upper limit for the surrounding pt? [(a)bsolute, (r)elative]", "ar"))
                if parameters["electron_iso_absorrel"][-1] == "a":
                    parameters["electron_iso_ptratiomax"].append(ask("What is the maximum surrounding pt used for isolation [in GeV]?", "#"))
                else:
                    parameters["electron_iso_ptratiomax"].append(ask("What is the maximum pt ratio used for isolation?", "#"))
                Printlevel -= 1
                myprint("")
                c = ask("Do you need more isolation criteria? [(y)es, (n)o]", "yn")
            Printlevel -= 1

            myprint("3.3: Muon Isolation")
            parameters["muon_iso_source"] = ["t"]
            parameters["muon_iso_dR"] = ["0.05"]
            parameters["muon_iso_ptmin"] = ["0.5"]
            parameters["muon_iso_absorrel"] = ["r"]
            parameters["muon_iso_ptratiomax"] = ["0.2"]
            parameters["muon_niso"] = 1
            Printlevel += 1
            c = ask("Do you need any particular isolation criterion? [(y)es, (n)o]", "yn")
            while c != "n":
                myprint("Isolation " + str(parameters["muon_niso"]) + ":")
                parameters["muon_niso"] += 1
                Printlevel += 1
                parameters["muon_iso_source"].append(ask("Which objects should be considered for isolation? [(t)racks, (c)alo objects?]", "tc"))
                parameters["muon_iso_ptmin"].append(ask("What is the minimum pt of a surrounding object to be used for isolation (default ATLAS values: tracks=0.4, calo=0.1)? [in GeV]", "#"))
                parameters["muon_iso_dR"].append(ask("What is the dR used for isolation?", "#"))
                parameters["muon_iso_absorrel"].append(ask("Is there an absolute or a relative upper limit for the surrounding pt? [(a)bsolute, (r)elative]", "ar"))
                if parameters["muon_iso_absorrel"][-1] == "a":
                    parameters["muon_iso_ptratiomax"].append(ask("What is the maximum surrounding pt used for isolation [in GeV]?", "#"))
                else:
                    parameters["muon_iso_ptratiomax"].append(ask("What is the maximum pt ratio used for isolation?", "#"))
                Printlevel -= 1
                myprint("")
                c = ask("Do you need more isolation criteria? [(y)es, (n)o]", "yn")
            Printlevel -= 1

            myprint("3.4: Photon Isolation")
            parameters["photon_iso_source"] = ["c"]
            parameters["photon_iso_dR"] = ["0.2"]
            parameters["photon_iso_ptmin"] = ["0.1"]
            parameters["photon_iso_absorrel"] = ["r"]
            parameters["photon_iso_ptratiomax"] = ["0.2"]
            parameters["photon_niso"] = 1
            Printlevel += 1
            c = ask("Do you need any particular isolation criterion? [(y)es, (n)o]", "yn")
            while c == "y":
                myprint("Isolation " + str(parameters["photon_niso"]) + ":")
                parameters["photon_niso"] += 1
                Printlevel += 1
                parameters["photon_iso_source"].append(ask("Which objects should be considered for isolation? [(t)racks, (c)alo objects?", "tc"))
                parameters["photon_iso_ptmin"].append(ask("What is the minimum pt of a surrounding object to be used for isolation (default ATLAS values: tracks=0.4, calo=0.1)? [in GeV]", "#"))
                parameters["photon_iso_dR"].append(ask("What is the dR used for isolation?", "#"))
                parameters["photon_iso_absorrel"].append(ask("Is there an absolute or a relative upper limit for the surrounding pt? [(a)bsolute, (r)elative]", "ar"))
                if parameters["photon_iso_absorrel"][-1] == "a":
                    parameters["photon_iso_ptratiomax"].append(ask("What is the maximum surrounding pt used for isolation [in GeV]?", "#"))
                else:
                    parameters["photon_iso_ptratiomax"].append(ask("What is the maximum pt ratio used for isolation?", "#"))
                Printlevel -= 1
                myprint("")
                c = ask("Do you need more isolation criteria? [(y)es, (n)o]", "yn")
            Printlevel -= 1

            myprint("3.5: Jets")
            Printlevel += 1
            if parameters["experiment"] == "A":
                parameters["jets_conedR"] = 0.4
                parameters["jets_ptmin"] = 10
            elif parameters["experiment"] == "C":
                parameters["jets_conedR"] = 0.5
                parameters["jets_ptmin"] = 10

            #parameters["jets_conedR"] = ask("Which dR cone radius do you want to use for the FastJet algorithm?", "#")
            #parameters["jets_ptmin"] = ask("What is the minimum pt of a jet? [in GeV]", "#")
            #parameters["jets_second"] = ask("Do you need a separate, extra type of jet? [(y)es, (n)o]", "yn")
            #if parameters["jets_second"] == "y":
            #    parameters["jets_conedR_second"] = ask("Second Jet: Which dR cone radius do you want to use for the FastJet algorithm?", "#")
            #    parameters["jets_ptmin_second"] = ask("Second Jet: What is the minimum pt of a jet? [in GeV]", "#")

            parameters["jets_btagging"] = ask("Do you want to use jet b-tagging? [(y)es, (n)o]", "yn")
            if parameters["jets_btagging"] == "y":
                c = "y"
                parameters["jets_btagging_n"] = 0
                parameters["jets_btagging_eff"] = []

            while c == "y":
                parameters["jets_btagging_n"] += 1
                myprint("b-Tagging " + str(parameters["jets_btagging_n"]) + ":")
                Printlevel += 1
                parameters["jets_btagging_eff"].append(ask("What is the signal efficiency to tag a b-jet? [in %]", "#"))
                Printlevel -= 1
                c = ask("Do you need more b tags? [(y)es, (n)o]", "yn")

            parameters["jets_tautagging"] = ask("Do you want to use jet tau-tagging? [(y)es, (n)o]", "yn")
            break
        except KeyboardInterrupt:
            try:
                myprint('')
                myprint('>>> Caught Interrupting Signal')
                myprint('>>> Press any key to restart current question block')
                myprint('        or resend signal to abort')
                raw_input("")
            except KeyboardInterrupt:
                exit(1)
    Printlevel = BASELEVEL
    return parameters, group

def check_queue_progress(parameters, S95_queue, S95_inputs):
    """Checks the progress of the internal S95 calculation queues and waits until
         all finished. """
    global Printlevel
    myprint("")
    myprint("All necessary information has been entered. Before the AnalysisManager")
    myprint(" can create all required files, the internal S95obs and S95exp")
    myprint(" calculations have to finish. The calculation should take 10s up to a minute")
    myprint(" per point.")
    Printlevel += 2
    while True:
        progress = count_finished(S95_queue)
        if progress[0] == progress[1]:
            myprint("... done!")
            break
        myprint("... Progress: " + str(progress[0]) + "/" + str(progress[1]))
        time.sleep(10)
    Printlevel -= 2

    myprint("Please check the below results for sanity. If anything looks")
    myprint(" suspicious, please contact the CheckMATE authors.")
    Printlevel += 1
    myprint("obs    bkg    bkgerr    S95obs    S95exp")
    for inputs, s95 in zip(S95_inputs, S95_queue):
        analysis, obs, bkg, bkg_err = inputs
        S95obs, S95exp = s95.get()
        # Should bkg and obs be identical, the S95 values should be identical too.
        # For numerical reasons, it can happen that they aren't. In that case, choose
        # the mean
        if bkg == obs:
          S95 = myround((S95exp+S95obs)/2., bkg)
          S95exp = S95obs = S95
        parameters["reference_data"][analysis]["S95_obs"] = str(S95obs)
        parameters["reference_data"][analysis]["S95_exp"] = str(S95exp)
        myprint(str(obs) + "    " + str(bkg) + "    " + str(bkg_err) + "    " + str(S95obs) + "    " + str(S95exp))
    ask("(Press any key to continue)", "@", True)
    Printlevel -= 1
    return parameters

def add_analysis_reference(parameters):
    """Creates a txt file in tabular form which shows all the reference data"""
    global printlevel
    analysis = parameters["analysis"]
    Info.fill_standard_paths_and_files()
    reference_file = open(Info.files['evaluation_reference'][analysis], "w")
    first_row = "SR    "
    for info in parameters["info_set"]:
        first_row += info + "    "
    reference_file.write(first_row + "\n")
    for sr in parameters["signal_regions"]:
        whole_row = sr + "    "
        for info in parameters["info_set"]:
            whole_row += parameters["reference_data"][sr][info] + "    "
        reference_file.write(whole_row + "\n")
    reference_file.close()
    AdvPrint.format_columnated_file(Info.files["evaluation_reference"][analysis])
    myprint(" - Reference file created")



def list_analyses():
    global Printlevel
    """Lists all analyses in "list of analyses"""
    Info.fill_standard_paths_and_files()
    for group in Info.analysis_groups:
        f = open(Info.files['list_of_analyses'][group], "r")
        x = f.readline()
        myprint("#"*(len(x) + 4))
        myprint("# " + x[:-1] + " #")
        for x in f:
            myprint("# " + x[:-1] + " #")
        myprint("#"*(len(x) + 4))    
    
    

def create_analysis():
    """ Asks user for all information required to add an analysis to the framework. """
    global Printlevel
    myprint('This will collect all necessary information to create a full analysis and')
    myprint('Takes care for the creation and implementagtion of the source files into the code.')
    myprint('Please answer the following questions.')
    myprint('Attention: Your input is NOT saved before you answered all questions!')
    myprint('')
    #==========================================================================
    parameters = dict()

    myprint('1. General Information to build analysis')
    parameters = ask_for_general_information(parameters)

    myprint('2. Information on Signal Regions')
    parameters["signal_regions"] = ask_multiline("List all signal regions (one per line, finish with an empty line):")
    parameters["expectation_known"] = ask("Is the SM expectation B known? [(y)es, (n)o]?", "yn")
    if parameters["expectation_known"] == "y":
        parameters, S95_queue, S95_inputs = ask_for_signal_region_information(parameters)
    else:
        myprint("Signal regions are registered but without any numbers associated to them.")
        myprint("IMPORTANT: The analysis will be created and can then be used like any other")
        myprint("           analysis. CheckMATE will skip the model exclusion tests as long as")
        myprint("           the expectation is not known. You can e.g. use CheckMATE on background")
        myprint("           samples to estimate B and dB. As soon as you know these numbers, ")
        myprint("           run the AnalysisManager again and use the (e)dit feature to add them.")
        ask("Press key to continue!", "@")

    myprint('')
    myprint("3. Settings for Detector Simulation")
    parameters, group = ask_for_detector_information(parameters)

     # Loop to wait for S95 calculations to finish
    if parameters["expectation_known"] == "y":
      check_queue_progress(parameters, S95_queue, S95_inputs)
    # Writes necessary files
    add_analysis_files(parameters, group)
    Printlevel -= 1
    myprint("Analysis " + parameters["analysis"] + " has been added successfully!")
    myprint("Run 'autoreconf', 'configure [+options]', 'make' from the main CheckMATE folder to compile it!")
    myprint("")

def edit_analysis():
    global Printlevel
    myprint('You can now edit or update some of the information for a given analysis.')

    # which analysis?
    files = dict()
    Info.fill_standard_paths_and_files()
    while True:
        analysis = ask("Enter the identifier of the analysis you want to edit: ")
        if os.path.isfile(Info.files['analysis_settings'][analysis]):
            break
        myprint("ERROR: Analysis " + analysis + " not known!")

    # Read out analysis information
    parameters = Info.get_analysis_parameters(analysis)
    myprint("Analysis " + analysis + " successfully read in.")
    while True:
        myprint("Do you want to...")
        Printlevel += 1
        myprint("...list all defined entries? (l)")
        myprint("...restart the detector settings questions? (d)")
        myprint("...enter signal region numbers for observation and background? (n)")
        Printlevel -= 1
        #myprint("(Note that we do not allow individual entries to be changed as most of them are not")
        #myprint(" independent and/or values are imprinted in other files.)")
        what_to_edit = ask("", "dln")
        myprint("")
        if what_to_edit == "l":
            myprint("Here are all defined objects in alphabetic order.")
            Printlevel += 1
            sorted_keys = parameters.keys()
            sorted_keys.sort()
            for p in sorted_keys:
                myprint(str(p)+":   "+str(parameters[p]).replace("u'", "'")) # replace avoids that the unicde identifier is printed
            Printlevel -= 1
            myprint("")
        if what_to_edit == "d":
            parameters = ask_for_detector_information(parameters)
            ask("Press any key to overwrite old detector settings (note that the old results are deleted!)", "@", True)
            write_analysis_parameters(parameters)
            myprint("Detector Information successfully changed")
            break
        if what_to_edit == "n":
            parameters, S95_queue, S95_inputs = ask_for_signal_region_information(parameters)
            parameters["expectation_known"] = "y"
            check_queue_progress(parameters, S95_queue, S95_inputs)
            ask("Press any key to overwrite reference tables (note that existing old numbers are deleted!)", "@", True)
            write_analysis_parameters(parameters)
            add_analysis_reference(parameters)
            myprint("Detector Information successfully changed")
            break


def remove_analysis():
    global Printlevel
    myprint('This will remove all source files and other appearances in the analysis framework.')
    myprint('(You can re-add the analysis as long as you keep the settings file in the /data directory).')
    Printlevel += 1
    analysis = ask("Analysis Name: ")
    Info.fill_standard_paths_and_files()

    if analysis not in Info.analysis_list:
        myprint('There is no analysis with that name.')
        return

    with open(Info.files['analysis_settings'][analysis], "rb") as jfile:
        parameters = json.loads(jfile.read())

    group = Info.analysis_group_map[analysis]
    remove_analysis_files(parameters, group)

    myprint('All files, except for /data/' + analysis + '_var.j have been removed successfully.')
    myprint('Keeping the _var.j file will allow you to conveniently re-add the analysis later.')

def update_s95():
    global Printlevel
    analysis = ask("Analysis Name: ")
    if not analysis in Info.analysis_list:
        AdvPrint.cerr_exit("There is no analysis with name '{}'.".format(analysis))
    Info.fill_standard_paths_and_files()

    with open(Info.files['analysis_settings'][analysis], "rb") as jfile:
        parameters = json.loads(jfile.read())

    action = ask("Do you want to update S95 values for:\n        (a)ll signal regions\n        (s)elected signal regions", "as")
    signal_regions = None
    if action == "s":
        all_signal_regions = parameters["signal_regions"]
        signal_regions = ask_multiline("Enter all the signal regions names that you want to select. (finish with an empty line)")
        unknown = set(signal_regions)-set(all_signal_regions)
        if len(unknown) != 0:
            AdvPrint.cerr_exit("{} doesn't have signal regions with the following names: {}".format(analysis, ", ".join(list(unknown))))
    recompute_s95(parameters, signal_regions)

    add_analysis_reference(parameters)
    write_analysis_parameters(parameters)




######################################################
#####     Adding and removing analysis files     #####
######################################################

def add_analysis_files(parameters, group, keep_sources=False):
    """
    Write all files for a new analysis
    """
    # This has to be done first, because that's where info
    # gets the list of analyses from
    add_analysis_to_list_of_analyses(parameters, group)
    # Now that Info knows about the analysis, we can update it
    Info.fill_standard_paths_and_files()
    create_analysis_header(parameters, keep_sources)
    create_analysis_source(parameters, keep_sources)
    create_analysis_control_regions_source(parameters, keep_sources)
    add_analysis_to_handler(parameters)
    add_analysis_to_makefile(parameters)

    if parameters["expectation_known"] == "y":
        add_analysis_reference(parameters)
    write_analysis_parameters(parameters)

def remove_analysis_files(parameters, group, keep_sources=False, delete_settings=False):
    """
    Remove all files for a new analysis

    Only the _var.j file is allowed to survive.
    If keep sources=True, the .h and .cc files are not removed either.
    """
    if parameters["expectation_known"] == "y" and not keep_sources:
        remove_analysis_reference_file(parameters)
    remove_analysis_from_makefile(parameters)
    remove_analysis_from_handler(parameters)
    if not keep_sources:
        remove_analysis_sources(parameters)
    remove_analysis_from_list_of_analyses(parameters, group)
    if delete_settings:
        try:
            os.remove(Info.files["analysis_settings"][parameters["analysis"]])
        except OSError as err:
            print "\nWARNING: Failed to delete settings file with error\n\t{}\n".format(err)

def read_analysis_parameters(path):
    """
    Read analysis parameters from the settings file at `path`.
    """
    with open(path, "r") as jfile:
        return json.load(jfile)

# Update the Makefile

def remove_analysis_reference_file(parameters):
    analysis_name = parameters["analysis"]
    if parameters["expectation_known"] == "y":
        file_name = Info.files["evaluation_reference"].get(analysis_name)
        if file_name is None:
            return
        try:
            os.remove(Info.files["evaluation_reference"][analysis_name])
        except OSError as err:
            print "\nWARNING: Failed to delete the analysis reference file with error\n\t{}\n".format(err)

def _strip_path_prefix(path, prefix_path):
    """
    Turns an absolute path into a relative one by removing a prefix

    Example:
    >>> _strip_path_prefix("/a/b/c/d/e.f", "/a/b/c")
    "d/e.f"
    """
    assert(path.startswith(prefix_path))
    relative = path[len(prefix_path):]
    if relative[0] == "/":
        relative = relative[1:]
    return relative

def _makefile_analysis_string(parameters, marker=""):
    """
    The string that gets inserted into the makefile for a new analysis
    """
    analysis_name = parameters["analysis"]
    header_file = Info.files["analysis_header"][analysis_name]
    source_file = Info.files["analysis_source"][analysis_name]
    # The makefile uses paths relative to 'Info.paths['analyses']
    header_file = _strip_path_prefix(header_file, Info.paths["analysis"])
    source_file = _strip_path_prefix(source_file, Info.paths["analysis"])
    if parameters["cr"] == "y":
        cr_source_file = Info.files["analysis_CR_source"][analysis_name]
        cr_source_file = _strip_path_prefix(cr_source_file, Info.paths["analysis"])
        analysis_string = "libanalyses_la_SOURCES += {} {} {}\n{}".format(
                header_file,
                source_file,
                cr_source_file,
                marker
                )
    else:
        analysis_string = "libanalyses_la_SOURCES += {} {}\n{}".format(
                header_file,
                source_file,
                marker
                )
    return analysis_string

def _add_analysis_to_makefile(contents, parameters):
    search_for = "#@@extraanalysis@@"
    replace_with = _makefile_analysis_string(parameters, search_for)
    return replace_in_contents(contents, (search_for, replace_with))

def add_analysis_to_makefile(parameters):
    """
    Update the makefile to include the new analysis in the compilation
    """
    makefile_path = Info.files["analysis_makefile"]
    replace_in_file(
            _add_analysis_to_makefile,
            parameters,
            makefile_path
            )

def _remove_analysis_from_makefile(contents, parameters):
    search_for = _makefile_analysis_string(parameters)
    replace_with = ""
    return replace_in_contents(contents, (search_for, replace_with))

def remove_analysis_from_makefile(parameters):
    """
    Update the makefile to not include the analysis in the compilation
    """
    makefile_path = Info.files["analysis_makefile"]
    replace_in_file(
            _remove_analysis_from_makefile,
            parameters,
            makefile_path
            )

# Update the AnalysisHandler

def class_name_from_analysis(analysis_name):
    """
    Find the name of the C++ class of an analysis
    """
    return analysis_name.capitalize()

def _handler_header_replacement(analysis_name, marker=""):
    """
    String that gets inserted or removed in analysis handler header
    """
    return '#include "{}.h"\n{}'.format(analysis_name, marker)

def _handler_source_replacement(
        analysis_name,
        analysis_class,
        marker=""
        ):
    """
    String that gets inserted or removed in analysis handler source file
    """
    replacement = (
            # The first line is not indented, because the 'search_for'
            # string already has the correct indentation level
            'if(analysisName == "{}")\n'
            '        a = new {}();\n'
            '    else {}'
            ).format(
                    analysis_name,
                    analysis_class,
                    marker
                    )
    return replacement


def _add_analysis_to_handler_header(contents, parameters):
    analysis_name = parameters["analysis"]

    search_for = "//@extraheader@"
    replace_with = _handler_header_replacement(analysis_name, search_for)
    return replace_in_contents(contents, (search_for, replace_with))

def _add_analysis_to_handler_source(contents, parameters):
    analysis_name = parameters["analysis"]
    analysis_class = class_name_from_analysis(analysis_name)

    search_for = "//@@extracode@@"
    replace_with = _handler_source_replacement(
                    analysis_name,
                    analysis_class,
                    search_for
                    )
    return replace_in_contents(contents, (search_for, replace_with))

def add_analysis_to_handler(parameters):
    """
    Update the AnalysisHandler to include the new analysis
    """
    analysis_name = parameters["analysis"]
    experiment = parameters["experiment"]
    analysis_handler = Info.analysis_handlers[experiment]

    # Header
    header_path = Info.files[analysis_handler]["include"]
    replace_in_file(
            _add_analysis_to_handler_header,
            parameters,
            header_path
            )

    # Source
    source_path = Info.files[analysis_handler]["src"]
    replace_in_file(
            _add_analysis_to_handler_source,
            parameters,
            source_path
            )

def _remove_analysis_from_handler_header(contents, parameters):
    analysis_name = parameters["analysis"]

    search_for = _handler_header_replacement(analysis_name)
    return replace_in_contents(contents, (search_for, ""))

def _remove_analysis_from_handler_source(contents, parameters):
    analysis_name = parameters["analysis"]
    analysis_class = class_name_from_analysis(analysis_name)
    search_for = _handler_source_replacement(
                    analysis_name,
                    analysis_class
                    )
    return replace_in_contents(contents, (search_for, ""))

def remove_analysis_from_handler(parameters):
    """
    Update the AnalysisHandler to not include the analysis
    """
    analysis_name = parameters["analysis"]
    experiment = parameters["experiment"]
    analysis_handler = Info.analysis_handlers[experiment]

    # Header
    header_path = Info.files[analysis_handler]["include"]
    replace_in_file(
            _remove_analysis_from_handler_header,
            parameters,
            header_path
            )

    # Source
    source_path = Info.files[analysis_handler]["src"]
    replace_in_file(
            _remove_analysis_from_handler_source,
            parameters,
            source_path
            )

# Create the sources

def _create_analysis_header(contents, parameters):
    return _create_analysis_source_common(contents, "", parameters)

def create_analysis_header(parameters, keep_sources):
    """
    Write the header file for a new analysis
    """
    analysis_name = parameters["analysis"]
    header_path = Info.files["analysis_header"][analysis_name]
    if keep_sources and os.path.isfile(header_path):
        return
    if parameters["cr"] == "y":
        template_path = Info.files["analysis_template_header_cr"]
    else:
        template_path = Info.files["analysis_template_header"]

    replace_in_file(
            _create_analysis_header,
            parameters,
            template_path,
            header_path
            )

def _create_analysis_source_common(contents, long_info, parameters):
    """
    Write the source file for a new analysis, either cr or not
    """
    analysis_name = parameters["analysis"]
    analysis_class = class_name_from_analysis(analysis_name)

    signal_regions = ";".join(parameters["signal_regions"])
    replacements = [
            ("@@author@@", parameters["author"]),
            ("@@authoremail@@", parameters["authoremail"]),
            ("@@analysis@@", analysis_name),
            ("@@Analysis@@", analysis_class),
            ("@@ANALYSIS@@", analysis_name.upper()),
            ("@@lumi@@", parameters["lumi"]),
            ("@@longInfo@@", long_info),
            ("@@signalregions@@", signal_regions)
            ]
    return replace_in_contents(contents, replacements)

def _create_analysis_source(contents, parameters):
    analysis_name = parameters["analysis"]
    long_info = parameters["long_info"]
    long_info = "\n".join(map( lambda l: '    "# {}\\n"'.format(l), long_info))
    return _create_analysis_source_common(contents, long_info, parameters)

def create_analysis_source(parameters, keep_sources):
    """
    Write the source file for a new analysis, either cr or not
    """
    analysis_name = parameters["analysis"]
    template_path = Info.files["analysis_template_source"]
    source_path = Info.files["analysis_source"][analysis_name]
    if keep_sources and os.path.isfile(source_path):
        return
    replace_in_file(
        _create_analysis_source,
        parameters,
        template_path,
        source_path
        )

def _create_analysis_control_regions_source(contents, parameters):
    analysis_name = parameters["analysis"]
    long_info = parameters["long_info"]
    long_info = "\n".join(map(lambda l: '    "@#{}\\n"'.format(l), long_info))
    return _create_analysis_source_common(contents, long_info, parameters)

def create_analysis_control_regions_source(parameters, keep_sources):
    """
    Write the source file for the control regions of a new analysis

    If there are no control regions for this analysis, do nothing
    """
    if parameters["cr"] != "y":
        return

    analysis_name = parameters["analysis"]
    template_path = Info.files["analysis_template_CR_source"]
    cr_source_path = Info.files["analysis_CR_source"][analysis_name]
    if keep_sources and os.path.isfile(cr_source_path):
        return
    replace_in_file(
        _create_analysis_control_regions_source,
        parameters,
        template_path,
        cr_source_path
        )


def remove_analysis_sources(parameters):
    """
    Delete all C++ source files belonging to an analysis.

    This includes the header file, the source file and control regions,
    if any.
    """
    analysis_name = parameters["analysis"]

    header_path = Info.files["analysis_header"][analysis_name]
    try:
        os.remove(header_path)
    except OSError as err:
        print "\nWARNING: Failed to remove header file with error\n\t{}\n".format(err)

    source_path = Info.files["analysis_source"][analysis_name]
    try:
        os.remove(source_path)
    except OSError as err:
        print "\nWARNING: Failed to remove source file with error\n\t{}\n".format(err)

    if parameters["cr"] == "y":
        cr_source_path = Info.files["analysis_CR_source"][analysis_name]
        try:
            os.remove(cr_source_path)
        except OSError as err:
            print "\nWARNING: Failed to remove control region source file with error\n\t{}\n".format(err)

# Update list of analyses

def _add_analysis_to_list_of_analyses(contents, parameters):
    """
    Add a new analysis to the contents of list_of_analyses.txt

    :param str contents:
        The contents of list_of_analyses.txt"
    :param dict parameters:
        The parameters of the analysis to add.

    :returns:
        `contents` with the analysis added.
    """
    analysis_name = parameters["analysis"]
    if parameters["cr"] == "y":
        has_cr = "yes"
    else:
        has_cr = "no"
    new_analysis = "{}    {}    {}    {}    {}    {}    {}".format(
            analysis_name,
            len(parameters["signal_regions"]),
            parameters["short_info"],
            parameters["lumi"],
            has_cr,
            parameters["author"],
            parameters["authoremail"]
            )
    lines = contents.splitlines()
    lines.append(new_analysis)
    return "\n".join(lines)

def add_analysis_to_list_of_analyses(parameters, group):
    """
    Update the list of analysis to include the new analysis
    """
    list_path = Info.files["list_of_analyses"][group]
    replace_in_file(
            _add_analysis_to_list_of_analyses,
            parameters,
            list_path
            )
    AdvPrint.format_columnated_file(list_path)

def _remove_analysis_from_list_of_analyses(contents, parameters):
    """
    Remove an analysis from the contents of list_of_analyses.txt

    :param str contents:
        The contents of list_of_analyses.txt"
    :param dict parameters:
        The parameters of the analysis to remove.

    :returns:
        `contents` with the analysis removed.
    """
    analysis_name = parameters["analysis"]
    lines = list()
    for line in contents.splitlines():
        spl = line.split()
        if len(spl) > 0 and spl[0] == analysis_name:
            continue
        lines.append(line)
    return "\n".join(lines)

def remove_analysis_from_list_of_analyses(parameters, group):
    """
    Remove an analysis from list_of_analyses.txt

    :param dict parameters:
        The parameters of the analysis to remove.
    """
    list_path = Info.files["list_of_analyses"][group]
    replace_in_file(
            _remove_analysis_from_list_of_analyses,
            parameters,
            list_path
            )


# Helper

def replace_in_contents(contents, replacements):
    if type(replacements) == tuple:
        contents = contents.replace(replacements[0], replacements[1])
    elif type(replacements) == list:
        for r in replacements:
            contents = contents.replace(r[0], r[1])
    else:
        raise TypeError("Wrong type for 'replacements': {}".type(replacements))
    return contents

def replace_in_file(fun, parameters, in_file_name, out_file_name=None):
    with open(in_file_name, "r") as f:
        contents = f.read()
    contents = fun(contents, parameters)
    if out_file_name is None:
        out_file_name = in_file_name
    with open(out_file_name, "w") as f:
        f.write(contents)

########################
#####     Main     #####
########################

def main():
    print """
   ____ _               _    __  __    _  _____ _____ ____
  / ___| |__   ___  ___| | _|  \/  |  / \|_   _| ____|___ \ 
 | |   | '_ \ / _ \/ __| |/ / |\/| | / _ \ | | |  _|   __) |
 | |___| | | |  __/ (__|   <| |  | |/ ___ \| | | |___ / __/ 
  \____|_| |_|\___|\___|_|\_\_|  |_/_/   \_\_| |_____|_____|

                        /\  _  _ |   _. _  |\/| _  _  _  _  _ _
                       /--\| )(_||\/_)|_)  |  |(_|| )(_|(_)(-|
                                  /                     _/
                                             """
    action = ask("""What would you like to do?
        -(l)ist all analyses,
        -(a)dd a new analysis to CheckMATE,
        -(e)dit analysis information,
        -(r)emove an analysis from CheckMATE
        -(u)pdate the S95 values of an analysis""", "lareu")
#    Info.init()
    Info.fill_standard_paths_and_files()
    if action == "a":
        create_analysis()
    elif action == "l":
        list_analyses()
    elif action == "e":
        edit_analysis()
    elif action == "r":
        remove_analysis()
    elif action == "u":
        update_s95()

