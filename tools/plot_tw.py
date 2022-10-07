#!/usr/bin/python3
########################################################################
# 绘制温度与显示重量变化曲线
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
########################################################################
import struct
import numpy as np
import matplotlib.pyplot as plt

def fill(x):
    x2 = list(x)
    for i in range(len(x)):
        if x2[i] is None:
            j = 1
            lst2 = []
            while True:
                if i-j >= 0 and x[i-j] is not None:
                    lst2.append(x[i-j])
                if i+j < len(x) and x[i+j] is not None:
                    lst2.append(x[i+j])
                if len(lst2) >= 2:
                    break
                else:
                    j += 1
            x2[i] = np.mean(lst2)
    return x2

f = open('save_tw.dat', 'rb')
l_ts = []
l_mg = []
l_tmp1 = []
l_tmp2 = []
while True:
    data = f.read(12)
    if len(data) != 12:
        break
    ts = struct.unpack('>I', data[:4])[0]
    mg, tmp1, tmp2 = struct.unpack('<ihh', data[4:])
    #if mg < 200000 or mg > 213000:
    #    mg = None
    tmp1 = tmp1/256+40
    tmp2 = tmp2/256+40
    l_ts.append(ts)
    l_mg.append(mg)
    l_tmp1.append(tmp1)
    l_tmp2.append(tmp2)


l_ts = np.array(l_ts)
l_mg = np.array(l_mg)
l_tmp1 = np.array(l_tmp1)
l_tmp2 = np.array(l_tmp2)
l_tmp = (l_tmp1+l_tmp2)/2
l_pred1 = 211300-(l_tmp-26)*800
l_pred2 = 212100-(l_tmp-26)*650
l_pred3 = 211000-(l_tmp-26)*500
l_pred4 = 211080-(l_tmp-26)*300

h = np.exp(-np.arange(300)/60)
h /= h.sum()
l_pred5 = np.convolve(l_pred1, h, 'vaild')

plt.plot(l_mg[299:], c='r')
plt.plot(l_pred1[299:])
plt.plot(l_pred5)
#plt.twinx()
plt.plot(l_mg[299:] - l_pred5+210000)
#plt.plot(l_mg-l_pred2, c='r')
#plt.plot(l_tmp1)
#plt.plot(l_tmp2)
#plt.plot(l_tmp1-l_tmp2)
plt.show()
