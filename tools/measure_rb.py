#!/usr/bin/python3
#########################################################################
# 蠕变修正
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
from scipy.optimize import leastsq
from plot_tw import l_mg

def func(p, x):
    A, tc, k, b = p
    return A*np.exp(-x/tc) + k*x + b

def error(p, x, y):
    return func(p, x) - y

data = l_mg[2344:2344+30*3]
#data = l_mg[6801:6900]

#data = l_mg[97+2:97+30+15]
#data = l_mg[172+2:172+30*4]

#data = l_mg[412+2:412+30*3]
#data = l_mg[568:568+30*2]
#data = l_mg[1465+2:1465+30*7]

p0 = [100, 30, -0.4, data[-1]]
Xi = np.arange(len(data))
Yi = data
Para = leastsq(error, p0, args=(Xi, Yi))
print(Para)

pred = func(Para[0], Xi)

plt.plot(data)
plt.plot(pred)
plt.show()
