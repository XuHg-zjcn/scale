#!/usr/bin/python3
#########################################################################
# 蠕变拟合
# Copyright (C) 2022  Xu Ruijun
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#########################################################################
import math
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import leastsq
from scipy import stats

def func(p, x):
    y0,A,tc = p
    return y0+k*x+A*np.exp(-x/tc)

def err(p, x, y):
    return y-func(p, x)

def fit(x, y):
    p0 = [data.mean(), -20, 15]
    Para = leastsq(err, p0, (x, y))
    #print(Para[0])
    yfit = func(Para[0], x)
    return Para[0], yfit

def func2(p, x):
    y0,A1,tc1,A2,tc2 = p
    return y0+k*x+A1*np.exp(-x/tc1)+A2*np.exp(-x/tc2)

def err2(p, x, y):
    return y-func2(p, x)

def fit2(x, y):
    p0 = [data.mean(), -25, 11, -40, 170]
    Para = leastsq(err2, p0, (x, y))
    #print(Para[0])
    yfit = func2(Para[0], x)
    return Para[0], yfit

data = np.fromfile('raw.dat', dtype=np.int32)
#200g
#load
#start = 3490
#start = 13738
start = 21646
#unload
#start = 6431
#start = 18162
#start = 24626

#500g
#start = 25163
#start = 29723
#start = 32487

x = np.arange(1000)
y = data[start+1000:start+2000]
k = np.polyfit(x, y, 1)[0]
print(k)

cut = data[start:start+500]
x = np.arange(len(cut))
para, yfit = fit(x, cut)
print(para)
plt.subplot(211)
plt.plot(x, yfit)
errs = cut-yfit
print(stats.normaltest(errs))
plt.scatter(x, cut, c=np.abs(errs))
plt.subplot(212)
plt.scatter(x, errs, s=1)
plt.show()

l = []
for n in range(30, 1000):
    y = data[start:start+n]
    x = np.arange(len(y))
    p, yfit = fit2(x, y)
    errs = y - yfit
    l.append((*p[1:], errs.std(), np.abs(errs[:2].mean()*math.sqrt(2))))
#label=['A1','tc1','err_std','err_2mean']
label=['A1','tc1','A2','tc2','err_std','err_2mean']
plt.plot(l, label=label)
plt.ylim(-500,1000)
plt.legend()
plt.show()
