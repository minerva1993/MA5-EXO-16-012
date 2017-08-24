#!/usr/bin/env python3


def region_name(sr, regionnames=None):
    """
    sr1= Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500
    and so on.

    >>> region_name('sr1')
    Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500
    >>> region_name(1)
    Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500
    >>> region_name('Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500')
    sr1
    >>> region_name('SR1')
    Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500
    """
    if regionnames == None:
        regions = regionnames()
    else:
        regions = region_names()

    if type(sr) == int:
        return [k for k in regions if regions[k] == sr][0]
    elif 'sr' in sr.lower():
        srno = int(sr.strip().strip('sr').strip('SR'))
        return [k for k in regions if regions[k] == srno][0]
    else:
        return regions[sr]


def region_names():
    """
    Define region names according to 1-12 as named by CMS.
    """
    regions = {}

    regions["Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500"]                           = 1
    regions["Nbjets0__Njets_greater_than_or_equal_to_3__HT_greater_than_1500__MHT_greater_than_750"]                          = 2
    regions["Nbjets0__Njets_greater_than_or_equal_to_5__HT_greater_than_1500__MHT_greater_than_750"]                          = 4
    regions["Nbjets0__Njets_greater_than_or_equal_to_5__HT_greater_than_500__MHT_greater_than_500"]                           = 3
    regions["Nbjets0__Njets_greater_than_or_equal_to_9__HT_greater_than_1500__MHT_greater_than_750"]                          = 5
    regions["Nbjets_greater_than_or_equal_to_1__Njets_greater_than_or_equal_to_3__HT_greater_than_750__MHT_greater_than_750"] = 7
    regions["Nbjets_greater_than_or_equal_to_1__Njets_greater_than_or_equal_to_5__HT_greater_than_750__MHT_greater_than_750"] = 12
    regions["Nbjets_greater_than_or_equal_to_1__Njets_greater_than_or_equal_to_7__HT_greater_than_300__MHT_greater_than_300"] = 11
    regions["Nbjets_greater_than_or_equal_to_2__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500"] = 6
    regions["Nbjets_greater_than_or_equal_to_2__Njets_greater_than_or_equal_to_5__HT_greater_than_1500__MHT_greater_than_750"] = 9
    regions["Nbjets_greater_than_or_equal_to_3__Njets_greater_than_or_equal_to_5__HT_greater_than_500__MHT_greater_than_500"] = 8
    regions["Nbjets_greater_than_or_equal_to_3__Njets_greater_than_or_equal_to_9__HT_greater_than_750__MHT_greater_than_750"] = 10

    return regions


def signal_effs(model):
    """
    >>> signal_effs('T1tttt_1200_800')["Nbjets0__Njets_greater_than_or_equal_to_2__HT_greater_than_500__MHT_greater_than_500"].strip()
    "1.09 \pm 0.07"
    """
    signal_eff = {}
    signal_eff_unc = {}
    sreff = {}
    regionnames = region_names()

    # T1bbbb_1500_100
    regions = {}
    regions[region_name(1, regionnames)] = "$14.68 \pm 0.29$"
    regions[region_name(2, regionnames)] = "$5.15 \pm 0.16$"
    regions[region_name(3, regionnames)] = "$9.75 \pm 0.21$"
    regions[region_name(4, regionnames)] = "$3.74 \pm 0.13$"
    regions[region_name(5, regionnames)] = "$0.28 \pm 0.03$"
    regions[region_name(6, regionnames)] = "$137.5 \pm 1.6$"
    regions[region_name(7, regionnames)] = "$93.8 \pm 1.3$"
    regions[region_name(8, regionnames)] = "$52.78 \pm 0.99$"
    regions[region_name(9, regionnames)] = "$40.20 \pm 0.85$"
    regions[region_name(10, regionnames)] = "$2.69 \pm 0.22$"
    regions[region_name(11, regionnames)] = "$82.6 \pm 1.1$"
    regions[region_name(12, regionnames)] = "$71.4 \pm 1.1$"
    sreff['T1bbbb_1500_100'] = regions
    
    # T1tttt_1500_100
    regions = {}
    regions[region_name(1, regionnames)] = "$3.58 \pm 0.09$"
    regions[region_name(2, regionnames)] = "$1.07 \pm 0.04$"
    regions[region_name(3, regionnames)] = "$3.55 \pm 0.09$"
    regions[region_name(4, regionnames)] = "$1.06 \pm 0.04$"
    regions[region_name(5, regionnames)] = "$0.59 \pm 0.03$"
    regions[region_name(6, regionnames)] = "$55.81 \pm 0.75$"
    regions[region_name(7, regionnames)] = "$30.14 \pm 0.53$"
    regions[region_name(8, regionnames)] = "$30.35 \pm 0.59$"
    regions[region_name(9, regionnames)] = "$17.99 \pm 0.43$"
    regions[region_name(10, regionnames)] = "$8.97 \pm 0.33$"
    regions[region_name(11, regionnames)] = "$96.32 \pm 0.93$"
    regions[region_name(12, regionnames)] = "$30.09 \pm 0.53$"
    sreff['T1tttt_1500_100'] = regions
    
    # T1qqqq_1400_100
    regions = {}
    regions[region_name(1, regionnames)] = "$284.4 \pm 3.1$"
    regions[region_name(2, regionnames)] = "$88.9 \pm 1.8$"
    regions[region_name(3, regionnames)] = "$213.3 \pm 2.7$"
    regions[region_name(4, regionnames)] = "$69.9 \pm 1.5$"
    regions[region_name(5, regionnames)] = "$5.83 \pm 0.38$"
    regions[region_name(6, regionnames)] = "$11.20 \pm 0.23$"
    regions[region_name(7, regionnames)] = "$37.43 \pm 0.62$"
    regions[region_name(8, regionnames)] = "$1.05 \pm 0.06$"
    regions[region_name(9, regionnames)] = "$3.49 \pm 0.13$"
    regions[region_name(10, regionnames)] = "$0.12 \pm 0.02$"
    regions[region_name(11, regionnames)] = "$42.97 \pm 0.66$"
    regions[region_name(12, regionnames)] = "$30.67 \pm 0.56$"
    sreff['T1qqqq_1400_100'] = regions
    
    
    # T1bbbb_1000_900
    regions = {}
    regions[region_name(1, regionnames)] = "$46.29 \pm 0.94$"
    regions[region_name(2, regionnames)] = "$2.19 \pm 0.18$"
    regions[region_name(3, regionnames)] = "$18.40 \pm 0.46$"
    regions[region_name(4, regionnames)] = "$1.49 \pm 0.12$"
    regions[region_name(5, regionnames)] = "$0.15 \pm 0.03$"
    regions[region_name(6, regionnames)] = "$138.7 \pm 1.9$"
    regions[region_name(7, regionnames)] = "$63.1 \pm 1.3$"
    regions[region_name(8, regionnames)] = "$34.77 \pm 0.91$"
    regions[region_name(9, regionnames)] = "$9.68 \pm 0.47$"
    regions[region_name(10, regionnames)] = "$1.10 \pm 0.16$"
    regions[region_name(11, regionnames)] = "$133.2 \pm 1.7$"
    regions[region_name(12, regionnames)] = "$43.9 \pm 1.0$"
    sreff['T1bbbb_1000_900'] = regions
    
    # T1tttt_1200_800
    regions = {}
    regions[region_name(1, regionnames)] = "$1.09 \pm 0.07$"
    regions[region_name(2, regionnames)] = "$0.11 \pm 0.02$"
    regions[region_name(3, regionnames)] = "$1.09 \pm 0.07$"
    regions[region_name(4, regionnames)] = "$0.11 \pm 0.02$"
    regions[region_name(5, regionnames)] = "$0.10 \pm 0.02$"
    regions[region_name(6, regionnames)] = "$15.47 \pm 0.50$"
    regions[region_name(7, regionnames)] = "$3.62 \pm 0.23$"
    regions[region_name(8, regionnames)] = "$8.20 \pm 0.39$"
    regions[region_name(9, regionnames)] = "$2.50 \pm 0.20$"
    regions[region_name(10, regionnames)] = "$1.47 \pm 0.16$"
    regions[region_name(11, regionnames)] = "$83.8 \pm 1.1$"
    regions[region_name(12, regionnames)] = "$3.62 \pm 0.23$"
    sreff['T1tttt_1200_800'] = regions
    
    # T1qqqq_1000_800
    regions = {}
    regions[region_name(1, regionnames)] = "$272.5 \pm 3.9$"
    regions[region_name(2, regionnames)] = "$18.79 \pm 0.98$"
    regions[region_name(3, regionnames)] = "$219.1 \pm 3.5$"
    regions[region_name(4, regionnames)] = "$17.51 \pm 0.94$"
    regions[region_name(5, regionnames)] = "$3.06 \pm 0.34$"
    regions[region_name(6, regionnames)] = "$9.42 \pm 0.26$"
    regions[region_name(7, regionnames)] = "$15.89 \pm 0.52$"
    regions[region_name(8, regionnames)] = "$0.81 \pm 0.05$"
    regions[region_name(9, regionnames)] = "$0.94 \pm 0.08$"
    regions[region_name(10, regionnames)] = "$0.07 \pm 0.01$"
    regions[region_name(11, regionnames)] = "$87.1 \pm 1.1$"
    regions[region_name(12, regionnames)] = "$14.07 \pm 0.49$"
    sreff['T1qqqq_1000_800'] = regions
    
    
    # T2tt_700_50
    regions = {}
    regions[region_name(1, regionnames)] = "$59.9 \pm 1.3$"
    regions[region_name(2, regionnames)] = "$1.38 \pm 0.18$"
    regions[region_name(3, regionnames)] = "$38.63 \pm 0.92$"
    regions[region_name(4, regionnames)] = "$1.08 \pm 0.14$"
    regions[region_name(5, regionnames)] = "$0.26 \pm 0.05$"
    regions[region_name(6, regionnames)] = "$95.3 \pm 1.4$"
    regions[region_name(7, regionnames)] = "$21.46 \pm 0.88$"
    regions[region_name(8, regionnames)] = "$11.34 \pm 0.35$"
    regions[region_name(9, regionnames)] = "$2.29 \pm 0.23$"
    regions[region_name(10, regionnames)] = "$0.30 \pm 0.06$"
    regions[region_name(11, regionnames)] = "$152.2 \pm 2.1$"
    regions[region_name(12, regionnames)] = "$15.66 \pm 0.74$"
    sreff['T2tt_700_50'] = regions
    
    # T2qq_1000_100
    regions = {}
    regions[region_name(1, regionnames)] = "$729.2 \pm 7.6$"
    regions[region_name(2, regionnames)] = "$58.4 \pm 2.1$"
    regions[region_name(3, regionnames)] = "$179.2 \pm 3.5$"
    regions[region_name(4, regionnames)] = "$28.0 \pm 1.6$"
    regions[region_name(5, regionnames)] = "$1.16 \pm 0.23$"
    regions[region_name(6, regionnames)] = "$11.61 \pm 0.37$"
    regions[region_name(7, regionnames)] = "$50.2 \pm 1.0$"
    regions[region_name(8, regionnames)] = "$0.56 \pm 0.06$"
    regions[region_name(9, regionnames)] = "$1.13 \pm 0.12$"
    regions[region_name(10, regionnames)] = "$0.04 \pm 0.02$"
    regions[region_name(11, regionnames)] = "$16.50 \pm 0.58$"
    regions[region_name(12, regionnames)] = "$21.82 \pm 0.68$"
    sreff['T2qq_1000_100'] = regions
    
    # T2bb_650_1
    regions = {}
    regions[region_name(1, regionnames)] = "$168.1 \pm 1.4$"
    regions[region_name(2, regionnames)] = "$3.57 \pm 0.21$"
    regions[region_name(3, regionnames)] = "$32.40 \pm 0.54$"
    regions[region_name(4, regionnames)] = "$1.65 \pm 0.13$"
    regions[region_name(5, regionnames)] = "$0.08 \pm 0.02$"
    regions[region_name(6, regionnames)] = "$202.3 \pm 1.6$"
    regions[region_name(7, regionnames)] = "$47.23 \pm 0.92$"
    regions[region_name(8, regionnames)] = "$6.92 \pm 0.21$"
    regions[region_name(9, regionnames)] = "$2.31 \pm 0.15$"
    regions[region_name(10, regionnames)] = "$0.08 \pm 0.02$"
    regions[region_name(11, regionnames)] = "$57.00 \pm 0.86$"
    regions[region_name(12, regionnames)] = "$19.57 \pm 0.55$"
    sreff['T2bb_650_1'] = regions



    return sreff[model]

def cms_effs(model):
    """
    """
    cms_eff = {}
    cms_eff_unc = {}
    if model == "T1bbbb_1500_100":
        cms_eff['Njets>=2'] = 100.0
        cms_eff_unc['Njets>=2'] = 0.0
        cms_eff['HT>300GeV'] = 100.0
        cms_eff_unc['HT>300GeV'] = 0.0
        cms_eff['MHT>300GeV'] = 80.3
        cms_eff_unc['MHT>300GeV'] = 0.4
        cms_eff['muon_veto'] = 79.8
        cms_eff_unc['muon_veto'] = 0.4
        cms_eff['muon_isoTrack_veto'] = 79.6
        cms_eff_unc['muon_isoTrack_veto'] = 0.4
        cms_eff['electron_veto'] = 79.2
        cms_eff_unc['electron_veto'] = 0.4
        cms_eff['electron_isoTrack_veto'] = 78.7
        cms_eff_unc['electron_isoTrack_veto'] = 0.4
        cms_eff['hadron_isoTrack_veto'] = 78.0
        cms_eff_unc['hadron_isoTrack_veto'] = 0.4
        cms_eff['dphi_j1_mht>0.5'] = 76.7
        cms_eff_unc['dphi_j1_mht>0.5'] = 0.4
        cms_eff['dphi_j2_mht>0.5'] = 69.2
        cms_eff_unc['dphi_j2_mht>0.5'] = 0.5
        cms_eff['dphi_j3_mht>0.3'] = 63.9
        cms_eff_unc['dphi_j3_mht>0.3'] = 0.5
        cms_eff['dphi_j4_mht>0.3'] = 58.6
        cms_eff_unc['dphi_j4_mht>0.3'] = 0.5
    elif model == "T1tttt_1200_800":
        cms_eff['Njets>=2'] = 100.0
        cms_eff_unc['Njets>=2'] = 0.0
        cms_eff['HT>300GeV'] = 99.0
        cms_eff_unc['HT>300GeV'] = 0.0
        cms_eff['MHT>300GeV'] = 14.9
        cms_eff_unc['MHT>300GeV'] = 0.1
        cms_eff['muon_veto'] = 9.6
        cms_eff_unc['muon_veto'] = 0.1
        cms_eff['muon_isoTrack_veto'] = 9.2
        cms_eff_unc['muon_isoTrack_veto'] = 0.1
        cms_eff['electron_veto'] = 6.2
        cms_eff_unc['electron_veto'] = 0.1
        cms_eff['electron_isoTrack_veto'] = 5.8
        cms_eff_unc['electron_isoTrack_veto'] = 0.1
        cms_eff['hadron_isoTrack_veto'] = 5.3
        cms_eff_unc['hadron_isoTrack_veto'] = 0.1
        cms_eff['dphi_j1_mht>0.5'] = 5.3
        cms_eff_unc['dphi_j1_mht>0.5'] = 0.1
        cms_eff['dphi_j2_mht>0.5'] = 4.5
        cms_eff_unc['dphi_j2_mht>0.5'] = 0.1
        cms_eff['dphi_j3_mht>0.3'] = 4.0
        cms_eff_unc['dphi_j3_mht>0.3'] = 0.1
        cms_eff['dphi_j4_mht>0.3'] = 3.6
        cms_eff_unc['dphi_j4_mht>0.3'] = 0.1

    return cms_eff, cms_eff_unc

if __name__ == "__main__":
    print 'hello'
