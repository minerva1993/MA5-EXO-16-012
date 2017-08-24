#!/usr/bin/env python

from __future__ import division
import os
import shutil as sh
import subprocess
import sql3 as sql
import glob
import socket
import logging

#############################################
######## Save hostname for logging ##########
#############################################
if '.' in socket.gethostname():
    HOSTNAME = socket.gethostname()[:socket.gethostname().index('.')]
    HOME = '/net/home/lxtsfs1/tpe/sonneveld/'
elif socket.gethostname().startswith('lxte') and socket.gethostname()[-1] in [str(i) for i in range(10)]:
    HOSTNAME = socket.gethostname()
    HOME = '/net/home/lxtsfs1/tpe/sonneveld/'
elif socket.gethostname().startswith('top') or socket.gethostname().startswith('streep'):
    HOSTNAME = socket.gethostname()
    HOME = '/home/jory/rwth/'

def make_run_name(process, model, run_number, masses, energy='8TeV'):
        '''([list of str,int]) -> str
        Append the names of this program to the masses
        to make a run name. Return this string.
        '''

        run_name = process + '_' + model + '_' + energy + '_' + str(run_number)
        for parameter in masses:
                run_name += '_' + str(parameter)
        return run_name

def unique_run_name(madgraph_dir, process, model, run_number, masses, energy='8TeV'):
    """
    Do not overwrite existing directories.
    """
    run_name = make_run_name(process, model, run_number, masses, energy)
    while os.path.exists(madgraph_dir + process + '/Events/' + run_name):
        run_number += 1
        run_name = make_run_name(process, model, run_number, masses, energy)
    return run_name

def do_analysis_ma5(ma5dir, run_name, anname_ma5, rootfiles):
    """
    """

    jobdir = ma5dir + 'Build/'
    inputfile = ma5dir + 'Input/' + run_name
    fi = open(inputfile, 'w')
    for rootfile in rootfiles:
        fi.write(rootfile + '\n')
    fi.close()

    current_dir = os.getcwd()
    os.chdir(jobdir)
    outputdir = ma5dir + 'Output/' + run_name
    if os.path.exists(outputdir):
        sh.rmtree(outputdir)
        print('Removed existing analysis results', outputdir)
    #out1 = subprocess.check_output(['source', 'setup.sh'])
    #out2 = subprocess.check_output(['Make'])
    #if 'error' in out1 or 'error' in out2 or 'ERROR' in out1 or 'ERROR' in out2:
    #    print 'There was an error.', out1, out2
    
    try:
        out = subprocess.check_output(['./MadAnalysis5job', inputfile])
    except:
        print('Did not succeed to analyze with ma5. Will rerun setup.sh:' )
        print(os.getcwd())
        out1 = subprocess.check_output(['source', 'setup.sh'])
        print('Will rerun make:')
        out2 = subprocess.check_output(['make'])
        print('Will retry analysis:')
        out = subprocess.check_output(['./MadAnalysis5job', inputfile])
    print(out[-30:])
    os.chdir(current_dir)

def calculate_most_sensitive_bin_ma5(ma5_dir, run_name, anname_ma5, xsec, verbose='False', cl=False):
    """
    """
    curdir = os.getcwd()
    os.chdir(ma5_dir)
    print(os.getcwd())

    # to obtain UL in pb use negative number:
    if not cl:
        xsec = -1
    # xsec = str(xsec) to obtain the confidence level with which it is excluded
    
    # That is, analysisname (MHT), run_name, run_number, xsec
    info = [anname_ma5[:-2], run_name, str(anname_ma5[-1]), str(xsec)]
    # calculate most sensitive bin
    command = ['./exclusion_CLs.py'] + info
    out = subprocess.check_output(command)
    if not os.path.exists(ma5_dir + 'Output/'+ run_name + '/' + anname_ma5 + '.out'):
        print('Did not find outputfile with most sensitive bin; output:', out)
        print('You gave info:', info, 'and command', command)
    os.chdir(curdir)


def read_sensitive_bin_from_file(ma5_dir, run_name, anname_ma5, db, binnotb, annamedb, verbose=False):
    """
    """
    loc = ma5_dir + 'Output/'+ run_name + '/' + anname_ma5 + '.out'
    fi = open(loc, 'r')
    line = fi.readline()
    binspec = line.strip('\n').replace('>', '_greater_than_').replace(', ', '__').replace('-', '_')
    binnos = sql.dicts_from_db_table(db, binnotb, constraints={'an': '"' + annamedb + '"'})
    binspecs = sql.dict_from_dicts(binnos, ['binspec'])
    line = fi.readline()
    ul = float(line.strip('\n'))
    return binspecs[binspec]['binno'], ul


def read_eff_from_file(ma5_dir, run_name, anname_ma5, db, binnotb, annamedb, verbose=False):
    """
    Read efficiencies from files produced by madanalysis.

    """

    binnos = sql.dicts_from_db_table(db, binnotb, constraints={'an': '"' + annamedb + '"'})
    loc = ma5_dir + 'Output/'+ run_name + '/' + anname_ma5 + '/Cutflows/*'
    if verbose:
        print('Looking for files in', loc)
    results = glob.glob(loc)
    if verbose:
        print('Will read from files:', results)
    effs = {}
    for fi in results:

        binnos_sorted = sql.dict_from_dicts(binnos, ['an', 'binspec'])
        binno = binnos_sorted[(annamedb, fi[fi.rindex('/') + 1:fi.index('.saf')])]['binno']
    
        # Total number of events:
        fopen = open(fi, 'r')
        line = fopen.readline()
        #line = fopen.readline()
        #line = fopen.readline()
        #line = fopen.readline()
        #line = fopen.readline()
        while not 'Initial number of events' in line:
            line = fopen.readline()
        line = fopen.readline()
        number_of_events_analyzed = int(line.split()[0])
        fopen.close()


        # Number of events that passed:
        txt = open(fi, 'r').read()
        number_of_events_passed = int(float(txt.split('<Counter>')[-1].split()[txt.split('<Counter>')[-1].split().index('nentries')-3]))
        #while not '8st' in line:
        #    line = fopen.readline()
        #line = fopen.readline()
        #number_of_events_passed = int(line.split()[0])
    
        eff = number_of_events_passed/number_of_events_analyzed
        effs[binno] = {'analyzed': number_of_events_analyzed, 'passed': number_of_events_passed}
        effs[binno]['eff'] = eff
    return effs



def run_delphes_ma5tune(event_file, delphesma5_dir, outputfile, verbose, delphes_card= 'examples/delphes_card_CMS.tcl'):
        """ (str, str, str) -> in
        Generate Delphes LHCO file using a Delphes program outside madgraph.
        Delphes is provided with the pythia HEP file from the madgraph_dir,
        here named run_name/tag_1_pythia_events.hep.gz.
        The output is then run_name.root. The run_delphesma5_file program
        automatically converts this to an LHCO file.
        Return outputfile.
        """
        current_dir = os.getcwd()
        os.chdir(delphesma5_dir)
        
        # Rename the appropriate variables accordingly:
        if event_file.endswith('gz'):
            os.system("gunzip " + event_file)
            event_file = event_file[:-3]

         
        # Filenames
        rootfile = delphesma5_dir + outputfile + '.root'
        
        if os.path.exists(rootfile):
            remove_delphesma5_files(rootfile)
            if verbose:
                print("Removed existing delphesma5 files:", rootfile)
                
        if not os.path.exists(rootfile):
            out = subprocess.check_output(['./DelphesSTDHEP', delphes_card, rootfile, event_file])
        

        ######## Verbosity #######
        if verbose:
                print("Running Delphes ma5 on : ", event_file)
                print(out[-190:90])
        #########################

        os.chdir(current_dir)
        if 'ERROR' in out or 'Warning' in out or 'Error' in out:
                print('There was an error or warning in Delphes ma5 on files in/out:', inputfile, outputfile)
                print('output:', out)
        return rootfile

def remove_delphesma5_files(rootfile, verbose=False):
        '''(str, str) -> Nonetype
        Remove files created in Delphes directory by Delphes program.
        These take up a lot of space and unfortunately cannot be kept.
        There are the root, lhco, and run.log files.
        '''
        name = rootfile[:-(len('.root'))]
        try: 
                os.remove(name + '.root')
                if verbose:
                    print('removed rootfile', rootfile)
        except OSError:
                print("One of the files in the delphes dir does not exist. Perhaps Delphes did not run.")


def param_points(masses):
    """
    """

    params = []
    for mgo in range(int(masses['mgo_lower']), int(masses['mgo_upper']) + int(masses['mgo_steps']), int(masses['mgo_steps'])):
        for msq in range(int(masses['msq_lower']), int(masses['msq_upper']) + int(masses['msq_steps']), int(masses['msq_steps'])):
            for mlsp in range(int(masses['mlsp_lower']), min(int(masses['mlsp_upper']) + int(masses['mlsp_steps']), msq), int(masses['mlsp_steps'])):
                if masses['factor_mgo'] != 'inf':
                    mgowas = mgo
                    mgo = int(float(masses['factor_mgo']) * msq)
                if mgo - mlsp >= 100: params.append((mgo, msq, mlsp))
                if masses['factor_mgo'] != 'inf': mgo = mgowas
    return params

def cross_sections(mg5dir, proc, run_name, mg5output = HOME + 'logging/' + HOSTNAME+'mg5out.log'):
    '''(str, str, str) -> float
    Read matrix element cross section and return this.
    '''
    bannerfile = mg5dir + proc + '/Events/' + run_name + '/' + run_name + '_tag_1_banner.txt'
    fi = open(bannerfile, 'r')
    line = fi.readline()
    while 'Integrated weight' not in line:
        line = fi.readline()

    # Cross section at matrix level:
    xsme = float(line.split()[-1])
    line = fi.readline()
    
    # Matched cross section
    xspy = float(line.split()[-1])
    xspyerr = 0

    # Try to obtain uncertainties:
    
    f = mg5dir + proc + '/' + 'crossx.html'
    fopen = open(f, 'r')
    line = fopen.readline()
    while line != '' and not run_name in line:
        line = fopen.readline()
    line = fopen.readline()
    line = fopen.readline()
    line = fopen.readline()
    line = fopen.readline()
    line = fopen.readline()
    xsme_unsafe = float(line.split()[3].strip('</a>'))
    xsmerr = float(line.split()[6].strip('</a>'))
    line = fopen.readline()
    line = fopen.readline()
    line = fopen.readline()
    line = fopen.readline()
    line = fopen.readline()
    try:
        xspy_unsafe = float(line.split()[3].strip('</a>'))
        xspyerr = float(line.split()[6].strip('</a>'))
    except:
        logging.warning('Could not read mg5 xsec. Will read from output')
        f = open(mg5output, 'r')
        out = f.readlines()
        ##### REAL SHITTY! ####
        try:
            pythia_xsec = [float(i) for i in out[out.index('Matched Cross-section'):out.index('pb\n')].split(':')[1].split('+-')]
            me_xsec = [float(i) for i in out[out.index('  Cross-section'):out.index('pb\n')].split(':')[1].split('+-')]
            xspy_unsafe = pythia_xsec[0]
            xspyerr = pythia_xsec[1]
            xsme_unsafe = me_xsec[0]
            xsmerr = me_xsec[1]
        except:
            print('Could not read uncertainties on cross sections from html files.')
    
    if xsme_unsafe != xsme:
        print('Cross sections from banner and html file not the same. Banner=', xsme, 'HTML=', xsme_unsafe)
    if xspy_unsafe != xspy:
        print('Matched cross sections from banner and html file not the same. Banner=', xspy, 'HTML=', xspy_unsafe)
    return (xsme, xsmerr, xspy, xspyerr)


