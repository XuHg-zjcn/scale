#!/usr/bin/python3
import usb
import struct
from scipy import signal
import math
import numpy as np
import matplotlib.pyplot as plt
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

N = 64
#flt = signal.windows.hann(64)

if __name__ == '__main__':
    ep_r, ep_w = find_usb()
    ep_w.write(b'\x01\x01')
    lst = []
    win = signal.windows.hann(N)
    win /= win.mean()
    while True:
        data = ep_r.read(13, timeout=0)
        for i in range(len(data)//3):
            lst.append(int.from_bytes(data[1+i*3:1+i*3+3], 'little', signed=True))
        if len(lst) < N:
            continue
        lst = lst[-N:]
        nlst = np.array(lst)
        wlst = win*nlst
        wmlst = win*(nlst-nlst.mean())
        print(f'{np.mean(wlst):10.1f}, {np.std(nlst,ddof=1):6.2f}')
        plt.cla()
        #fftr = np.abs(np.fft.rfft(wmlst))[1:]/math.sqrt(N)
        #plt.plot(np.convolve(fftr, flt, 'same'))
        plt.plot(nlst)
        plt.pause(0.01)
