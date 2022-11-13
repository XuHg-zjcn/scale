#!/usr/bin/python3
########################################################################
# 绘制记录的蠕变修正曲线
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
from scipy import signal


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


f = open('save_rb.dat', 'rb')
l_ts = []
l_ad = []
l_rb = []
while True:
    data = f.read(12)
    if len(data) != 12:
        break
    ts = struct.unpack('>I', data[:4])[0]
    ad, rb = struct.unpack('<ii', data[4:])
    l_ts.append(ts)
    l_ad.append(ad)
    l_rb.append(rb)

l_ts = np.array(l_ts)
l_ad = np.array(l_ad)
l_rb = np.array(l_rb)

filt = signal.windows.hann(10)[1:-1]
filt /= filt.sum()
l_adf = np.convolve(l_ad, filt, 'vaild')
l_rbf = np.convolve(l_rb, filt, 'vaild')

x = np.arange(len(l_adf))


if __name__ == "__main__":
    plt.scatter(x, l_ad[4:-3], c='r', s=1)
    plt.plot(l_adf, c='r')
    plt.scatter(x, l_rb[4:-3], c='g', s=1)
    plt.plot(l_rbf, c='g')
    #plt.ylim(210100, 210100+500)
    plt.ylim(216300, 216400+500)
    #plt.ylim(554200, 554200+500)
    plt.show()
