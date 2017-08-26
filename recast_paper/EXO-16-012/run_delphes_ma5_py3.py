#!/usr/bin/env python

import shutil as sh
import subprocess as sub
import glob
import os
import argparse
import sys

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', dest='file', type=str, required=True, help='Specify the filename on which delphesMA5 should be run.')
parser.add_argument('--ma5dir', '-m', dest='ma5dir', type=str, required=False, help='Specify the filename on which delphesMA5 should be run.')
parser.add_argument('--card', '-c', dest='delphescard', type=str, required=True, help='Specify the delphescard to be used.')
parser.add_argument('--atlas', '-a', action='store_true', default=False, dest='atlas', help='Use this option if ATLAS instead of CMS (default) detector should be used.')
parser.add_argument('--output', '-o', dest='output', help='Specify the output name to which the rootfile should be written.')
arguments = vars(parser.parse_args())

def run_delphes_ma5(hepfile, delphescard, ma5dir):
    """
    Run delphes interactively and return the created rootfile.

    >>> p1 = sub.Popen(['cat', '-'], stdin = sub.PIPE, stdout = sub.PIPE)
    >>> p1.communicate(input='echo dit')
    ('echo dit', None)
    """

    # In order to generate directories in program directory, not homedir:
    curdir = os.getcwd()
    os.chdir(ma5dir)

    if hepfile.endswith('gz'):
        os.system("gunzip " + hepfile)
        hepfile = hepfile[:-3]
    # Put the delphes file in place:
    delphescard_orig = os.path.join(ma5dir, 'tools/SampleAnalyzer/Process/Detector/delphesMA5tune_card_CMS.tcl')
    sh.copy(delphescard_orig, delphescard_orig + '.orig')
    sh.copy(delphescard, delphescard_orig)

    # Tell MadAnalysis where to store resulting ROOTfile:
    if 'Events/' in hepfile:
        dirname = hepfile[hepfile.index('Events/') + 7:hepfile.rindex('/')]
    else:
        dirname = hepfile[hepfile.rindex('/') + 1:-4]
    # Delete path if it exists already:
    if os.path.exists(dirname):
        sh.rmtree(dirname)

    # Write the inputfile for MA5:
    inputname = 'thisgoesintodelphesma5'
    inputfile = open(inputname, 'w')
    inputfile.write('set main.fastsim.package = delphesMA5tune\n')
    inputfile.write('set main.fastsim.detector = cms\n')
    inputfile.write('import ' + hepfile + '\n')
    inputfile.write('submit ' + dirname + '\n')
    #inputfile.write('N\n')
    inputfile.close()

    # Run MA5:
    #p1 = sub.Popen(['cat', inputname], stdout=sub.PIPE)
    #p2 = sub.Popen(['python', ma5dir + 'bin/ma5', '-R'], stdin=p1.stdout)
    #p1.stdout.close()
    #out = p2.communicate()[0]
    #p = sub.Popen(['python', ma5dir + 'bin/ma5', '-R'], stdin=sub.PIPE, stdout=sub.PIPE)
    #out = p.communicate(input=open(inputname, 'r').read())
    #print out

    # Run MA5 in script mode:
    out = sub.check_output(['python2', os.path.join(ma5dir, 'bin/ma5'), '-s', '-R', inputname])

    # Return the rootfile
    analysisdir = os.path.join(os.getcwd(), dirname)
    #analysisdir = os.path.join(os.getcwd(), glob.glob('ANALYSIS*')[-1])
    rootfile = os.path.join(analysisdir, 'Output/_defaultset/TheMouth.root')
    if not os.path.exists(rootfile):
        print("No rootfile generated? Not present at expected place:")
        print(rootfile)
        rootfile = None

    # Return to homedir:
    os.chdir(curdir)

    return rootfile


def remove_delphesma5_files(rootfile, anname_ma5='cms_sus_13_012', anname='MHT'):
    """
    """
    sh.rmtree(rootfile[:rootfile.rindex('Output/_defaultset/TheMouth.root')])


def delphes_ma5_cms(hepfile, delphescard, ma5dir, outputfile):
    """
    Run delphesMA5tune on given hepfile with given delphescard
    in given ma5dir for cms detector.
    Save rootfile to given outputfile and remove all junk.
    """
    rootfile = run_delphes_ma5(hepfile, delphescard, ma5dir)
    sh.copy(rootfile, outputfile)
    remove_delphesma5_files(rootfile)
    return outputfile



if __name__ == '__main__':
    hepfile = os.path.abspath(arguments['file'])
    output = os.path.abspath(arguments['output'])
    delphescard = os.path.abspath(arguments['delphescard'])
    atlas = arguments['atlas']
    if arguments['ma5dir'] == None:
        home = '/home/jory/rwth/'
        ma5dir = os.path.join(home, 'bin/madanalysis5')
    else:
        ma5dir = arguments['ma5dir']
    if atlas:
        print('Not possible to use ATLAS detector. Change the program.')
        sys.exit()
    delphes_ma5_cms(hepfile, delphescard, ma5dir, output)
    print('Ran delphesMA5 in', ma5dir, 'on', hepfile, 'with ', delphescard, '; output written to', output)

    #import doctest
    #doctest.testmod()

    #hepfile = "/net/work/lxte173/sonneveld/lhefiles/cms_8tev_t2qq_ownfiles/hepfiles/T2qq_700_100_lhe_cms_pythia_dipan.hep"
    #ma5dir = "/net/work/lxte173/sonneveld/madanalysis5/"
    #delphescard = "/net/home/lxtsfs1/tpe/sonneveld/madanalysis/mht/delphesMA5tune_card_CMS_SUSY.tcl"

    #hepfile = "/home/jory/rwth/bin/run_pythia_on_lhe/lhe_pythia_card_dipan/T2qq_700_100_dipan.hep"
    #ma5dir = "/home/jory/rwth/bin/madanalysis5/"
    #delphescard = "/home/jory/rwth/madanalysis/mht/delphesMA5tune_card_CMS_SUSY.tcl"

    #run_name = hepfile[hepfile.rindex('/') + 1: hepfile.index('.hep')]
    #rootfile = run_delphes_ma5(hepfile, delphescard, ma5dir)
    #rootfiles = [rootfile]
    #mafcn.do_analysis_ma5(ma5dir + anname + '/', run_name, anname_ma5, rootfiles)
    #print(run_name)
    #for rootf in rootfiles:
    #    remove_delphesma5_files(rootf)
