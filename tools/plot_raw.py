#!/usr/bin/python3
from scipy import signal
import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':
    arr = np.fromfile('raw.dat', dtype=np.int32)
    win = signal.windows.hann(64)
    win /= win.sum()
    filt = signal.convolve(arr, win, 'valid')
    plt.scatter(np.arange(len(arr)), arr, s=1)
    plt.plot(np.arange(len(filt))+32, filt, c='g')
    plt.show()
