#!/usr/bin/python3
#########################################################################
# 记录原始ADC数据
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
from find_usb import find_usb

if __name__ == '__main__':
    f = open('raw.dat', 'wb')
    ep_r, ep_w = find_usb()
    ep_w.write(b'\x01\x01')
    while True:
        data = ep_r.read(13, timeout=0)
        lst = []
        for i in range(len(data)//3):
            lst.append(int.from_bytes(data[1+i*3:1+i*3+3], 'little', signed=True))
        f.write(np.array(lst, dtype=np.int32).tobytes())
        f.flush()
