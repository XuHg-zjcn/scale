#!/usr/bin/python3
#########################################################################
# 用温度预测重量偏差
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
from plot_tw import l_mg2, l_tmp2, lx2

l_mg3 = l_mg2[13900:]
lx3 = lx2[13900:]
delt = l_mg3 - lx3

fmg = np.fft.rfft(l_mg3)
fx3 = np.fft.rfft(lx3)
fdlt = np.fft.rfft(delt)

plt.plot(np.abs(fmg))
plt.plot(np.abs(fx3))
#plt.plot(np.fft.irfft((fmg/fx3))[:100])
#plt.plot(np.abs(fdlt))
plt.show()
