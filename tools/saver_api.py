#!/usr/bin/python3
#########################################################################
# 通过MCU上的USB API获取数据并记录
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
from find_usb import find_usb
import struct
import time


if __name__ == '__main__':
    ep_r, ep_w = find_usb()
    f = open('data/save_api.dat', 'wb')
    ep_w.write(b'\x01\x01')
    while True:
        data24 = ep_r.read(13, timeout=0)
        ts = time.time()
        assert data24[0] == 0x00
        lst = [int(ts)]
        for i in range(4):
            x = int.from_bytes(data24[i*3+1:i*3+4], 'little', signed=True)
            lst.append(x)
        data = struct.pack('<iiiii', *lst)
        f.write(data)
        print(lst[1:])
