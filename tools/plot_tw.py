#!/usr/bin/python3
# 绘制温度与显示重量变化曲线
import struct
import numpy as np
import matplotlib.pyplot as plt

def fill(x):
    x2 = list(x)
    for i in range(len(x)):
        if x2[i] is None:
            j = 1
            lst2 = []
            while True:
                if i-j >= 0 and x[i-j] is not None:
                    lst2.append(x[i-j])
                if i+j < len(x) and x[i+j] is not None:
                    lst2.append(x[i+j])
                if len(lst2) >= 2:
                    break
                else:
                    j += 1
            x2[i] = np.mean(lst2)
    return x2

f = open('save_tw3.dat', 'rb')
l_ts = []
l_mg = []
l_tmp = []
while True:
    data = f.read(10)
    if len(data) != 10:
        break
    ts = struct.unpack('>I', data[:4])[0]
    mg, tmp = struct.unpack('ih', data[4:])
    if mg < -2000 or mg > 2500:
        mg = None
    if tmp < 380 or tmp > 450:
        tmp = None
    l_ts.append(ts)
    l_mg.append(mg)
    l_tmp.append(tmp)

nlst = np.array((l_ts, l_mg, l_tmp)).transpose()
for i in range(2, len(l_tmp)-2):
    if l_tmp[i] and l_tmp[i-3:i+4].count(None) <= 3:
        filt = list(filter(lambda x:x is not None, l_tmp[i-3:i]+l_tmp[i+1:i+4]))
        mead = np.median(filt)
        if abs(l_tmp[i] - mead) > 2:
            nlst[i,2] = None

l_mg2 = np.array(fill(nlst[:,1]))
l_tmp2 = np.array(fill(nlst[:,2]))

lx2 = (l_tmp2-445)*80
t = np.arange(2000)
w = np.exp(-t/50) + np.exp(-t/150)
w /= w.sum()
lx3 = np.convolve(lx2, w)
lx3 = lx3[:len(l_mg2)]

plt.plot(l_mg2)
#plt.twinx()
plt.plot(lx2, c='r')
plt.plot(lx3, c='g')
#plt.show()
plt.plot(l_mg2-lx3)
plt.show()
