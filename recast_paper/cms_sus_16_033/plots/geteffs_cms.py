# Auxiliary material





if __name__ == "__main__":

    replacements = {}
    replacements['$\\text{p}\\text{p} \\to \\PSg\\PSg, \\PSg \\to \\tbbar \\text{W}^{*-} \\PSGczDo$'] = 'T1tbtb'
    replacements['$\\text{p}\\text{p} \\to \\PSg\\PSg, \\PSg \\to \\qqbar \\text{V} \\PSGczDo$'] = 'T1qqqqVV'
    replacements['$\\text{p}\\text{p} \\to \\PSg\\PSg, \\PSg \\to \\qqbar \\text{V} \\PSGczDo$  \\\\'] = 'T1qqqqVV'
    replacements['$\\text{p}\\text{p} \\to \\sBot\\sBot, \\sBot \\to \\text{b} \\PSGczDo$'] = 'T2bb'
    replacements['$\\text{p}\\text{p} \\to \\sTop\\sTop, \\sTop \\to \\text{t} \\PSGczDo$'] = 'T2tt'
    replacements['$\\text{p}\\text{p} \\to \\sQua\\sQua, \\sQua \\to \\text{q} \\PSGczDo$ \\\\'] = 'T2qq'

    replacements['\\multirow{3}{*}{Region}'] = 'sr'
    replacements['$\\text{p}\\text{p} \\to \\PSg\\PSg, \\PSg \\to \\bbbar \\PSGczDo$'] = 'T1bbbb'
    replacements['$\\text{p}\\text{p} \\to \\PSg\\PSg, \\PSg \\to \\ttbar \\PSGczDo$'] = 'T1tttt'
    replacements['$\\text{p}\\text{p} \\to \\PSg\\PSg, \\PSg \\to \\qqbar \\PSGczDo$  \\\\'] = 'T1qqqq'



    note = open("cmseffs.tex")
    line = note.readline()
    # Start over 1

    # Read to table
    # Three times:
    count_tables = 0
    while count_tables < 3 and line != '':
        while 'Expected number of signal events' not in line and line != '':
            line = note.readline()
        count_tables += 1
        # Read to headers
        # Double tables have this twice:
        count_subtables = 0
        while count_subtables < 2 and not ('end' in line and ('tabular' in line or 'table' in line)) and line != '':
            # Search for new table
            while 'multirow' not in line.lower() and line != '':
                 line = note.readline()
            count_subtables += 1
            headers = [h.strip() for h in line.split('&')]
            # Gather models and masses
            models = [replacements[h] for h in headers if 'text{p}' in h and 'to' in h]
            line = note.readline()
            m_mothers = line.split('&')
            mothermasses = [m[m.index('=') + 1:m.index('GeV')-1] for m in m_mothers if '=' in m]
            line = note.readline()
            m_daughters = line.split('&')
            daughtermasses = [m[m.index('=') + 1:m.index('GeV')-1] for m in m_daughters if '=' in m]
            line = note.readline()
            names = {models[i]: models[i] + '_' + mothermasses[i] + '_' + daughtermasses[i] for i in range(len(models))}

            print
            keys = []
            for h in headers:
                # Make easy model name from tex string
                k = h
                if h in replacements:
                   k = replacements[h]
                # Add masses to model name
                if k in names:
                   k = names[k]
                keys.append(k)

            list_of_dicts = []
            while not 'multirow' in line or 'tabular' in line and line != '':
                # read until new table
                line = note.readline()
                if not '&' in line or 'multirow' in line or 'tabular' in line: continue
                list_of_dicts.append({keys[i]: line.split('&')[i] for i in range(len(keys))})

            for name in names.values():
                print '#', name
                for d in list_of_dicts:
                    #print d
                    val = d[name][:d[name].rindex('$') + 1]
                    valstring = '"' + val.strip() + '"'
                    print "regions[region_name(" + str(d['sr']).strip() + ", regionnames)] = " + valstring
                print "sreff['" + name + "'] = regions"
                print ""
            if name == 'T2bb_650_1': break


