#!/usr/bin/python3
#########################################################################
# 记录单片机上的蠕变修正结果
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
from scipy import signal
import math
import numpy as np
import time


if __name__ == '__main__':
    ep_r, ep_w = find_usb()
    f = open('data/save_rb.dat', 'wb')
    while True:
        data = ep_r.read(8, timeout=0)
        ad, rb = struct.unpack('<ii', data)
        time_4B = int(time.time()).to_bytes(4, 'big')
        f.write(time_4B)
        f.write(data)
        f.flush()
        print(f'{ad}, {rb}')
