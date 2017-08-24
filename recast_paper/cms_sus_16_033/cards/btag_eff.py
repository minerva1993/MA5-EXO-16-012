# bpython version 0.15 on top of Python 2.7.12 /usr/bin/python
>>> from math import atan
>>> from math import tanh
>>> pt = 30
>>> 0.85*tanh(0.0025*pt)*(25.0/(1+0.063*pt))
0.5504389021139638
>>> 0.25*tanh(0.018*pt)*(1/(1+ 0.0013*pt))
0.11862078120195482
>>> 0.01+0.000038*pt
0.01114
>>> 
