#############################################################################
# 工具链配置文件
# Copyright (C) 2021  Xu Ruijun
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#############################################################################

#MRS工具链路径，安装时需要修改成您自己的路径
MRS_BASE = "/home/xrj/MRS/MounRiver Studio/toolchain"

TOOL_CHAIN = $(MRS_BASE)"/RISC-V Embedded GCC/bin/riscv-none-embed-"
OPENOCD = $(MRS_BASE)/OpenOCD/bin/openocd
OCD_CFG = $(MRS_BASE)/OpenOCD/bin/wch-riscv.cfg

WCHISP = wchisp

#MCU操作(擦除,烧录,验证,复位等)的方式
#ocd: 使用OpenOCD操作
#isp: 使用WCHISP操作
MCU_OP = isp

VERBOSE ?= NO

#从xPack下载的编译器，版本较新。如需使用，请修改路径。
#TOOL_CHAIN = "/home/xrj/elec/xpack-riscv-none-embed-gcc-10.2.0-1.2/bin/riscv-none-embed-"

#输出文件目录
OBJODIR = obj
TARGET = obj/target
