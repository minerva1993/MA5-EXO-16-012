#!/usr/bin/env python3

import sys
import os
import plot_saf_file as plotssaf
import math
from cms_info import *

def read_cutflow_ma5(txtfile, verbose=False):
    cf = []
    fi = open(txtfile, 'r')
    text = fi.read()
    cuts = text.split('<Counter>')
    for cut in cuts:
        cut = cut.split()
        eventindex = cut.index('nentries') -3
        #print 'index:', eventindex
        nevts = int(cut[eventindex])

        cutname = ''.join(cut[:eventindex])
        if 'Initial' in cutname:
            cutnam = 'analyzed'
            analyzed = nevts
        cutnam = cutname[:cutname.index('#')].strip('"')
        cf.append({'analyzed': analyzed, 'cut': cutnam, 'evts': nevts})
        if verbose:
            print('cut:', cutnam)
            print('# events:', nevts, '\n')
    return cf

def cross_sections(model):
    """
    NOT finished!
    Return cross section in picobarn.
    Use twikis:
        https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu
        https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVsquarkantisquark
        https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVstopsbottom

    >>> cross_sections('T1tttt_1500_100')
    0.0141903
    >>> cross_sections('T1bbbb_1500_100')
    0.0141903
    >>> cross_sections('T1tttt_1200_800')
    0.0856418

    """
    xsecs = {}
    prod = model[:2]
    if prod == 'T1':
        twiki = "https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu"

    xsecs['T1tttt_1500_100'] = 0.0141903 #pb 1500 GeV at https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu
    #xsec_uncs['T1tttt_1500_100'] = 0.227296 * xsecs['T1tttt_1500_100'] # pb, same source
    xsecs['T1bbbb_1500_100'] = xsecs['T1tttt_1500_100']
    #xsecs_unc['T1bbbb_1500_100'] = xsecs_unc['T1tttt_1500_100']
    xsecs['T1tttt_1200_800'] = 0.0856418
    #xsecs_unc['T1tttt_1200_800'] = 0.184814 * xsecs_unc['T1tttt_1200_800'] #pb 1200 GeV at https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu
    return xsecs[model]
if __name__ == "__main__":
    textfile = '/home/jory/bin/madanalysis5/ra2b/Output/SMS-GlGl_onejet_T1bbbb_1500_100/cms_sus_16_033_4/Cutflows/Nbjets_greater_than_or_equal_to_3__Njets_greater_than_or_equal_to_9__HT_greater_than_750__MHT_greater_than_750.saf'
    if len(sys.argv) > 1:
        if 'Nbjets' in sys.argv:
            textfile = sys.argv[1]
        else:
            textfile = os.path.join(sys.argv[1], "Cutflows/Nbjets_greater_than_or_equal_to_3__Njets_greater_than_or_equal_to_9__HT_greater_than_750__MHT_greater_than_750.saf")
    runname = plotssaf.extract_runname(textfile)
    print runname
    model = ''.join([it + '_' for it in runname.split('_')[-3:]])[:-1]
    print model
    cutflowfile = runname + '_cutflow.tex'
    cms_abseffs, cms_abseff_unc = cms_effs(model)
    lumi = 35.9 * 1000.0 # pb-1
    xsec =  1.2756920796493025E-003 #pb
    xsec = 0.0141903 #pb 1500 GeV at https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu
    xsec_uncert = 0.227296 * xsec # pb, same source
    normevts = round(lumi*xsec, 2)
    # print(normevts)
    cutflow = read_cutflow_ma5(textfile)
    # print(cutflow)
    print('\n\n')
    analyzed = cutflow[-1]['analyzed']
    cuf = open(cutflowfile, 'w')
    split = '\t&\t'
    end = '\t\\\\'
    line =  'Cut name'.ljust(25) + split + 'CMS'.ljust(20) + split + 'MA5'.rjust(20) + end + '\\hline'
    print(line)
    cuf.write(line + '\n')
    #line = 'Before cuts'.ljust(25) + split + str(normevts).ljust(20) + split + str(normevts).rjust(20) + end
    #cuf.write(line + '\n')
    #print(line)
    events_left = analyzed

    for cut in cutflow[1:-1]:
        cutname = cut['cut']
        if 'NbJets' in cutname: break
        if 'Initial' in cut['cut']:
            cutn = 'Initial # of evts'
        # print(cutn.ljust(25) + '\t' + str(cut['evts']))
        abseff = cut['evts'] / (1.0*analyzed)
        #eff = cut['evts'] / events_left
        events_left = cut['evts']
        #ma5 = str(round(abseff * normevts, 2)) + ' ('
        ma5 = str(round(abseff*100.0,2)) + ' $\pm$ '
        #staterr = math.sqrt(events_left)/events_left
        staterr = math.sqrt(analyzed)/analyzed
        ma5 += str(round(staterr*100.0, 2)) + end
        #cutname = cut['cut'].replace('Lep', ' Lep').replace('Phot', ' Phot').replace('Jet', ' Jet')
        if 'first' in cutname:
            cms = '--'
        else:
            cmsabseff = cms_abseffs[cutname]
            cmsabsefferr = cms_abseff_unc[cutname]
            # cms = str(round(cmsabseff * 0.01 * normevts, 2)) + ' ('
            cms = str(round(cmsabseff,2)) + ' $\pm$ '
            cms += str(round(cmsabsefferr, 2))
        #cutname = cutname.replace('>', '$>$').replace('<', '$<$').replace('ratio', ' ratio')
        cutn = cut['cut'].replace('muon', '$\mu$').replace('b', '$b$')
        cutn = cutn.replace('N', 'Number of').replace('_',  ' ').replace('pT', '$p_{\mathrm{T}}$')
        cutn = cutname.replace('muon', '$\mu$').replace('b', '$b$').replace('N', 'Number of').replace('_',  ' ')
        cutn = cutn.replace('pT', '$p_{\mathrm{T}}$').replace('>', '$>$').replace('<', '$<$')
        line = cutn.ljust(25) + split + cms.ljust(20) + split + ma5.rjust(20)
        print(line)
        cuf.write(line + '\n')
        #print cut['cut'].ljust(20) + split + '--' + split + str(round(cut['evts'] / effs[sm][0]['analyzed'] * evts, 2)) + end
    cuf.close()
    print('\n')
    #print 'cut:', cutnam
    #print '# events:', nevts, '\n'

    print("Signal regions")
    srfiles = {1: "Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500.saf",
            2: "Nbjets0__Njets_greater_than_or_equal_to_3__HT_greater_than_1500__MHT_greater_than_750.saf",
            3: "Nbjets0__Njets_greater_than_or_equal_to_5__HT_greater_than_500__MHT_greater_than_500.saf",
            4: "Nbjets0__Njets_greater_than_or_equal_to_5__HT_greater_than_1500__MHT_greater_than_750.saf",
            5: "Nbjets0__Njets_greater_than_or_equal_to_9__HT_greater_than_1500__MHT_greater_than_750.saf",
            6: "Nbjets_greater_than_or_equal_to_2__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500.saf",
            7: "Nbjets_greater_than_or_equal_to_1__Njets_greater_than_or_equal_to_3__HT_greater_than_750__MHT_greater_than_750.saf",
            8: "Nbjets_greater_than_or_equal_to_3__Njets_greater_than_or_equal_to_5__HT_greater_than_500__MHT_greater_than_500.saf",
            9: "Nbjets_greater_than_or_equal_to_2__Njets_greater_than_or_equal_to_5__HT_greater_than_1500__MHT_greater_than_750.saf",
            10: "Nbjets_greater_than_or_equal_to_3__Njets_greater_than_or_equal_to_9__HT_greater_than_750__MHT_greater_than_750.saf",
            11: "Nbjets_greater_than_or_equal_to_1__Njets_greater_than_or_equal_to_7__HT_greater_than_300__MHT_greater_than_300.saf",
            12: "Nbjets_greater_than_or_equal_to_1__Njets_greater_than_or_equal_to_5__HT_greater_than_750__MHT_greater_than_750.saf",}
    directory = textfile[:textfile.index('Nbjets')]
    signaleffs = signal_effs(model)
    head = 'SR #'.ljust(3) + split + 'SR definition'.ljust(90) + split + 'MA5 left'.rjust(8) + split
    head += 'CMS signal yield'.rjust(18) + split
    head += 'MA5 efficiency'.rjust(8) + end
    print head
    cutflowfile = runname + '_sr.tex'
    cuf = open(cutflowfile, 'w')
    cuf.write(head + '\n')
    for region in range(1, 13):
        srfile = srfiles[region]
        srpath = directory + srfile
        cf = read_cutflow_ma5(srpath)
        lastcut = cf[-1]
        srname = srfile[:-4].replace('__', ', ').replace('_greater_than_or_equal_to_', '$\geq$')
        srname = srname.replace('_less_than_or_equal_to_', '$\leq$')
        srname = srname.replace('_less_than_', '$<$')
        srname = srname.replace('_greather_than_)', '$\leq$')
        srname = srname.replace('Nbjets0', '$N_{\mathrm{b}} = 0$')
        srname = srname.replace('Nbjets', '$N_{\mathrm{b}}$')
        srname = srname.replace('Njets', '$N_{\mathrm{j}}$')
        xsec = cross_sections(model)
        eff = lastcut['evts']/(1.0*lastcut['analyzed'])
        evts = xsec * lumi * eff
        out = str(region).ljust(3) + split + srname.ljust(90) + split
        out += str(round(evts, 2)).rjust(8) + split 
        out += signaleffs[srfile[:-4]].rjust(18) + split
        out += str(round(100.0*eff, 2)).rjust(8) + ' %   '.rjust(2) + end
        cuf.write(out + '\n')
        print out



    cuf.close()