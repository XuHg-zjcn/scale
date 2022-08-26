#############################################################################
# MCU操作
# Copyright (C) 2022  Xu Ruijun
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

ocd_erase:
	@$(OPENOCD) $(OCDFLAGS) -c init -c halt -c "flash erase_sector wch_riscv 0 last" -c exit

ocd_prog: $(TARGET).hex
	@$(OPENOCD) $(OCDFLAGS) -c init -c halt -c "program \"$<\" 0x08000000" -c exit

ocd_verify: $(TARGET).hex
	@$(OPENOCD) $(OCDFLAGS) -c init -c halt -c "verify_image \"$<\"" -c exit

ocd_reset:
	@$(OPENOCD) $(OCDFLAGS) -c init -c reset -c exit

ocd_flash: ocd_erase ocd_prog ocd_verify ocd_reset


isp_erase:
	@$(WCHISP) erase

isp_verify: $(TARGET).hex
	@$(WCHISP) verify "$<"

isp_reset:
	@$(WCHISP) reset

isp_flash: $(TARGET).hex
	@$(WCHISP) flash "$<"


erase: $(MCU_OP)_erase
verify: $(MCU_OP)_verify
reset: $(MCU_OP)_reset
flash: $(MCU_OP)_flash
