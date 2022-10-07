#!/bin/python
########################################################################
# 将"num24.png"转换为"font_12x24.h"
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
import numpy as np
from imageio import imread

from np2c import numpy2ccode

x0 = 0
dx = 14
nX = 14

# 图片使用GIMP制作，字体为Ubuntu自带的"Monospace"字体
img = imread('num24.png')
img = img[:, x0:x0+dx*nX, 2]    # cut image
img = img.reshape(-1, nX, dx)   # y, X, x
img = img.transpose(1, 0, 2)    # X, y, x

img = img[:, :, 1:-1]                   # cut edge, flip y
img = img >= 128                        # bin
img = img.astype(np.uint8)

# show example
#plt.imshow(img.reshape(-1, 3)[(dy-1)*0x30:(dx-1)*0x40])
#plt.show()

tmp = np.zeros((img.shape[0], img.shape[2]), dtype=np.uint32)
for i in range(24):
    tmp += img[:, i, :] * (1 << i)
out = np.zeros((tmp.shape[0], tmp.shape[1], 3), dtype=np.uint8)
out[:, :, 0] = tmp&0xff
out[:, :, 1] = (tmp>>8)&0xff
out[:, :, 2] = (tmp>>16)&0xff
out = out.reshape(-1)


commit = []
commit.append('')
commit.extend('0123456789.-g ')

# write to .c file, arr shape must 2d
with open('../User/font_12x24.h', 'w') as f:
    np2c = numpy2ccode(f, '{:#0Mx}')
    np2c.write_1d('font12x24', out, Ncol=12*3, commits=commit)
