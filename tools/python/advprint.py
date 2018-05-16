''' Module contains advanced printing and formatting routines '''
import os, re, sys

class AdvPrint:
    """Prints unless a flag is set"""
    quiet = False
    cout_file = "#None"
    color_remover = re.compile(r'\x1b[^m]*m')
    
    @classmethod
    def mute(cls):
        cls.quiet = True
        
    @classmethod
    def unmute(cls):
        cls.quiet = False
    
    @classmethod
    def set_cout_file(cls, cout_file, overwrite=False):
        cls.cout_file = cout_file
        if overwrite and os.path.exists(cout_file):
            os.remove(cout_file)  
    
    @classmethod
    def cout(cls, message, option=""):
        if cls.quiet == False:
            if option in ["nlb", "nlb*"]:
                print message, 
            else:
                print message
        if cls.cout_file != "#None":            
            message = cls.color_remover.sub('', message)
            f = open(cls.cout_file, "a+")
            if option == "nlb":
                f.write(message)
            else:
                f.write(message+"\n")            
            f.close()        
            
    @classmethod
    def cerr(cls, message):
        sys.stderr.write(message)
        
    @classmethod
    def cerr_exit(cls, message):
        cls.cerr("!!! ERROR !!!\n")
        cls.cerr(message + "\n")
        cls.cerr("Exiting.\n")
        exit(1) 
        
    @classmethod
    def get_terminal_width(cls):
        # careful: this command might not work on all machines
        stty_output = os.popen('stty size', 'r').read().split()
        if len(stty_output) >= 2:
            return int(stty_output[1])
        return 80 # standard value
    
    @classmethod        
    def remove_extra_spaces(cls, text):  
        # Replaces more than two spaces by exactly two spaces, which is the Checkmate standard for separating columns
        while text.find("   ") != -1:
            text = text.replace("   ", "  ")
        return text
    
    @classmethod
    def format_columnated_file(cls, unformated_file):
        uncolumnated_lines = list()
        columnated_lines = list()
        f = open(unformated_file, "r")
        # Read file and divice into columnated and uncolumnated lines
        # Keep order of lines in separate list  
        order = list()
        for line in f:
            line = line.rstrip()
            if line == "" or line[0:2] == "# ":
                uncolumnated_lines.append(line)
                order.append("u")
            elif line[0] == "@": # "@ prevents lines from being columnated once"
                uncolumnated_lines.append(line[1:])
                order.append("u")
            else:
                columnated_lines.append(line)
                order.append("c")
        
        # Divide items in columnated lines
        line_items = list()
        for line in columnated_lines:
            # Split whenever there are two or more spaces
            line = cls.remove_extra_spaces(line)
            line_items.append(line.split("  "))
        trans_lines = zip(*line_items)
        col_widths = [max(len(c) for c in b)+2 for b in trans_lines]
        #col_width = max(len(word) for line in line_items for word in line) + 2  # padding
        
        f.close()
        f = open(unformated_file, "w")
        u = c = 0
        for i in range(len(order)):
            if order[i] == "u":
                f.write(uncolumnated_lines[u]+"\n")      
                u += 1
            else:
                line = line_items[c]      
                f.write("".join(word.ljust(col_width) for word, col_width in zip(line, col_widths))+"\n")
                c += 1


def get_eventcounter(rdir, analysis):
    """If there already exist result files in rdir up to number N, return N+1. Otherwise 0"""
    rfiles = get_result_files(rdir, analysis)
    for eventcounter in range(1000):
        counter_used = False
        for rfile in rfiles["results_signal"]:
            if ("%03i" % eventcounter) in rfile:
                counter_used = True
        if not counter_used:
            return eventcounter        
    pr = AdvPrint()
    pr.cerr_exit("Eventcounter is out of range (>999)")
    
def check_agreement_of_identifiers_and_crosssections(events):
    """Events which belong to the same process must have the same cross section"""
    pr = AdvPrint()
    N = len(events['processes'])
    for i in range(N):
        for j in range(N):
            if events['processes'][i] == events['processes'][j] and events['xsects'][i] != events['xsects'][j]:
                pr.cerr_exit("Events from the same process must have identical cross sections!")
    return

def parse_progress(pfile):
    """Reads the progress file 'pfile' and puts its information into structured dictionaries"""
    results = dict()
    f = open(pfile)
    # First, read in all items of progress file.
    for line in f:
        line = line.rstrip()
        if line == "" or line[0] == "#":
            continue
          
        # Reduce spacing properly to split the string.        
        while line.find("   ") != -1:
          line = line.replace("   ", "  ")
        tokens = line.split("  ")
        #Prefix  Eventfile  Checksum  Xsect  Process  Date
        prefix = tokens[0]
        checksum = tokens[2]
        xsect = tokens[3]
        process = tokens[4]
        
        results[prefix] = dict()
        results[prefix]['prefix'] = prefix
        results[prefix]['checksum'] = checksum
        results[prefix]['xsect'] = xsect
        results[prefix]['process'] = process
    f.close()
    
    # Next, group items according to their processes
    processes = dict()
    prefixes = results.keys()
    prefixes.sort()
    for prefix in prefixes:
        result = results[prefix]
        process = result['process']
        # If the process is new, add to dict
        if process not in processes.keys():
            processes[process] = []
            processes[process].append(result)
        else:
            processes[process].append(result)            
    return processes

    