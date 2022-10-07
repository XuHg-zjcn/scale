#!/usr/bin/python3
#########################################################################
# 记录ADC值数据
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
import usb
import struct
from scipy import signal
import math
import numpy as np
import time

def find_usb(vid=0xffff, pid=0xa05c):
    dev = usb.core.find(idVendor=vid, idProduct=pid)
    if dev is None:
        raise ValueError('Device not found')
    if dev.is_kernel_driver_active(0):
        dev.detach_kernel_driver(0)
    dev.reset()
    cfg = dev.get_active_configuration()
    intf = cfg[(0, 0)]
    endps = intf.endpoints()
    ep_read = usb.util.find_descriptor(intf, custom_match =
        lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN)
    ep_write = usb.util.find_descriptor(intf, custom_match =
        lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT)
    return ep_read, ep_write

N = 256

if __name__ == '__main__':
    ep_r, ep_w = find_usb()
    lst = []
    win = signal.windows.hann(N)
    win /= win.mean()
    f = open('save.dat', 'wb')
    i = 0
    while True:
        data = ep_r.read(4*8, timeout=0)
        values = struct.unpack('i'*8, data)
        time_4B = int(time.time()).to_bytes(4, 'big')
        f.write(time_4B)
        f.write(data)
        lst.extend(values)
        i += 8
        if len(lst) < N:
            continue
        if i % 128 != 0:
            continue
        lst = lst[-N:]
        nlst = np.array(lst)
        wlst = win*nlst
        wmlst = win*(nlst-nlst.mean())
        print(f'{np.mean(wlst):10.1f}, {np.std(nlst):6.2f}')

