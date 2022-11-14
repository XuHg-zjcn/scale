#!/usr/bin/python3
########################################################################
# 绘制用USB API记录的数据
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

if __name__ == '__main__':
    f = open('data/save_api.dat', 'rb')
    lst = []
    while True:
        data = f.read(20)
        if len(data) != 20:
            break
        tup = struct.unpack('<iiiii', data)
        lst.extend(tup[1:])
    f.close()
    lst = np.array(lst)
    plt.plot(lst)
    plt.show()
    print(lst.std())
    print(lst[:len(lst)//2].std())
    print(lst[len(lst)//2:].std())
