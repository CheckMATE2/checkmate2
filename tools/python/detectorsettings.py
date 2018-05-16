import os
import pickle
import json
import sys
import shutil
import ConfigParser

from info import Info

""" The functions in this class here determines the post-Delphes steps 
    that have to be performed by the AnalysisHandler and
    fines common settings between analyses to save computational
    time. Results are stored in analysis setup files """


class DetectorSettings:
    
    @classmethod
    def init(cls):
        pass

    # Add invisible PIDs to detector card, if needed
    @classmethod
    def _update_delphes_invpids(cls, line):
        if Info.parameters['invisiblePIDs'] != [] and "  #@@newInvisibles@@" in line:
            line = ""
            for pid in Info.parameters['invisiblePIDs']:
                line += "  add EnergyFraction {"+str(pid)+"} {0.0 0.0}\n"
        return line

    # Add random seed to detector card, if needed
    @classmethod
    def _update_delphes_randomseed(cls, outfile):
        if Info.parameters['randomseed'] == 0:
            return
        outfile.write("set RandomSeed "+str(Info.parameters['randomseed'])+"\n\n")

    # Update the detector card of experiment 'experiment'
    @classmethod
    def _update_delphes_file(cls, experiment):
        if Info.parameters['invisiblePIDs'] == [] and Info.parameters['randomseed'] == 0:
            return
        oldfile_name = Info.files['delphes_global_config'][experiment]
	newfile_name = os.path.join(Info.paths['output_delphes'], "modified_"+experiment+"_card.tcl") 
        Info.files['delphes_global_config'][experiment] = newfile_name
        with open(oldfile_name, "r") as oldfile:
            with open(newfile_name, "w") as newfile:
                for line in oldfile:
                    line = DetectorSettings._update_delphes_invpids(line)
                    newfile.write(line)
                DetectorSettings._update_delphes_randomseed(newfile)

    # Update detector cards if needed
    @classmethod
    def update_delphes_files(cls):
        map(lambda exp: DetectorSettings._update_delphes_file(exp), Info.used_experiments)

    @classmethod
    def set_by_anyone(cls, analyses, parameters, parameter_of_interest, value):
      for analysis in analyses:
        if parameters[analysis][parameter_of_interest] == value:
          return True
      return False
    
    
    # Read input parameters for all given analyses
    @classmethod
    def merge_settings(cls):
      config = Info.config
      map(lambda exp: _config_experiment(config, exp), Info.used_experiments)
      Info.config = config

"""
Add a section to the config.

Arguments:
    config: the Config object the section is added to
    name: the section name
    data: a dict containing the key-value pairs that belong to the section
    handler: the analysishandler this section belongs to
"""
def _add_section(config, name, data, handler):
    config.add_section(name)
    for key in data:
        config.set(name, key, data[key])
        config.set(name, "analysishandler", handler)


"""
Add multiple sections of the same type to the config.

The various sections get numbered in the order that they appear in 'data',
starting from 0.

Arguments:
    config: the Config object to add the sections to
    sectiontype: the type of the sections to add
    data: list of dicts that contain key-value pairs of the individual sections
    prefix: a string that gets appended between section type and the running number of the section
"""
def _add_sections(config, sectiontype, data, handler, prefix=""):
    for i,val in enumerate(data):
        full_name = sectiontype+": "+prefix+str(i)
        _add_section(config, full_name, val, handler)

"""
Takes a python list and turns it into a whitespace separated list (in a single string).

Optionally, every element of the list can have a prefix prepended.
"""
def _make_list(val,prefix=""):
    return " ".join(map(lambda v: prefix+str(v),val))

"""
Add a tautag section with the given label to the config.

The section belongs to the analysishandler handler.
"""
def _add_tautag_section(config, handler, label):
    name = "TAUTAG: "+label
    config.add_section(name)
    config.set(name, "analysishandler", handler)

"""
Takes the name of an analysis and returns the name of the corresponding analysis section.
"""
def _analysis_section_name(analysis):
    return "ANALYSIS: "+analysis

"""
Links other sections to an analysis section.

NOTE: The analysis section has to be in config already.

Arguments:
    config: The config where to find the analysis section.
    analysis: The name of the analysis.
    key: The key to introduce into the analysis section.
    value: A list of labels of other sections.
"""
def _add_link(config, analysis, key, value, prefix = ""):
    name = _analysis_section_name(analysis)
    value_as_string = _make_list(value, prefix)
    if value_as_string == "":
        return
    config.set(name, key, value_as_string)


"""
Extracts jet-tagging information from analyses and adds it to config.

The function both writes b- and tau-tag sections to config, and adds
links to those sections to the analyses that need them.

NOTE: All analysis sections must already be present in config.

Arguments:
    config: The config to write to.
    parameters: A dict from analysis to analysis parameters, where each entry is
        in the format as returned by Info.get_analysis_parameters
    prefix: A prefix to add to the label of the sections.
    handler: The analysis handler that the analyses belong to
"""
def _filter_jettags(config, parameters, handler, prefix=""):
    tags = list()
    tautag = False
    for key in parameters:
        analysis_params = parameters[key]
        if analysis_params["jets_tautagging"] == "y":
            tautag = True
        if analysis_params["jets_btagging"] != "y":
            continue
        analysis_tags = list()
        for eff in analysis_params["jets_btagging_eff"]:
            deff = {"eff": eff}
            if not deff in tags:
                tags.append(deff)
            analysis_tags.append(str(tags.index(deff)))
        _add_link(config, key, "jet_btags", analysis_tags, prefix)
    _add_sections(config, "BTAG", tags, handler, prefix)
    if tautag:
        _add_tautag_section(config, handler, prefix+"0")

"""
Extracts isolation information for particle from analyses and adds it to config.

The function both writes isolation sections to config, and adds
links to those sections to the analyses that need them.

NOTE: All analysis sections must already be present in config.

Arguments:
    config: The config to write to.
    parameters: A dict from analysis to analysis parameters, where each entry is
        in the format as returned by Info.get_analysis_parameters
    particle: The particle to do isolation for.
        Either 'electron', 'muon' or 'photon'
    section: The section type.
        Either 'ELECTRONISO', 'MUONISO' or 'PHOTONISO'
    handler: The analysishandler that the analyses belong to
    prefix: A prefix to add to the label of the sections.
"""
def _filter_isolation(config, parameters, particle, section, handler, prefix=""):
    criteria = list()
    for key in parameters:
        analysis_params = parameters[key]
        analysis_crits = list()
        for n in range(int(analysis_params[particle+"_niso"])):
            iso = dict()
            iso["source"] = analysis_params[particle+"_iso_source"][n]
            iso["dr"] = analysis_params[particle+"_iso_dR"][n]
            iso["ptmin"] = analysis_params[particle+"_iso_ptmin"][n]
            iso["maxval"] = analysis_params[particle+"_iso_ptratiomax"][n]
            iso["absorrel"] = analysis_params[particle+"_iso_absorrel"][n]
            if not iso in criteria:
                criteria.append(iso)
            analysis_crits.append(str(criteria.index(iso)))
        _add_link(config, key, particle+"_isolation", analysis_crits, prefix)
    _add_sections(config, section, criteria, handler, prefix)

"""
Writes the configuration for all analyses that belong to experiment into config.

Arguments:
    config: The config object to write to
    experiment: The experiment to do this for
        Either 'A' for ATLAS or 'C' for CMS
"""
def _config_experiment(config, experiment):
    exists = False # are there any analyses for this experiment?
    parameters = dict()
    for analysis in Info.analyses:
        params = Info.get_analysis_parameters(analysis)
        if params["experiment"] != experiment:
            continue
        exists = True
        parameters[analysis] = params
        section_name = _analysis_section_name(analysis)
        config.add_section(section_name)
        config.set(section_name, "analysishandler", experiment)
    if not exists:
        # no analyses found.
        return
    _filter_jettags(config, parameters, experiment, experiment)
    _filter_isolation(config, parameters, "electron", "ELECTRONISO", experiment, experiment)
    _filter_isolation(config, parameters, "muon", "MUONISO", experiment, experiment)
    _filter_isolation(config, parameters, "photon", "PHOTONISO", experiment, experiment)

