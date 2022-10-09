#!/usr/bin/python3
#########################################################################
# 蠕变修正预测
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
import numpy as np
import matplotlib.pyplot as plt
from plot_tw import l_mg


a = 0.0001
tc = 20


t = np.arange(tc*5)
x = a*np.exp(-t/tc)
h = a/tc*np.exp(-t/tc)
h[0] += 1 - h.sum()
h_ = -a/(1-a)**2/tc*np.exp(-(1+a/(1-a))*t/tc)
h_[0] += 1 - h_.sum()


def iirf(data, chrg=0.04877, coeff=0.0001025):
    creep_stat = data[0]
    res = []
    for i in data:
        creep_stat = creep_stat*(1-chrg) + i*chrg
        d = coeff*(i - creep_stat)
        res.append(i+d)
    return res


conv = np.convolve(l_mg, h_, 'vaild')

plt.plot(l_mg[len(h_)-1:])       # 原始
plt.plot(conv)                   # 卷积
plt.plot(iirf(l_mg)[len(h_)-1:]) # IIR滤波
plt.show()
