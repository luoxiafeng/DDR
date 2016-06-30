/*
 * (C) Copyright 2009
 * Ilya Yanok, Emcraft Systems Ltd, <yanok@emcraft.com>
 *
 * Copyright (C) 2005-2007 Samsung Electronics
 * Kyungin Park <kyugnmin.park@samsung.com>
 *
 * Copyright (c) 2004 Texas Instruments
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
MEMORY
{
	/* physical memory base */
	IROM_BASE  : ORIGIN = 0x00000000, LENGTH = 120K
	IRAM_DATA  : ORIGIN = 0x08000000, LENGTH = 64K
	IRAM_STAC  : ORIGIN = 0x08010000, LENGTH = 32K

	LDS_DAT    : ORIGIN = 0x60000000, LENGTH = 16M
	LDS_OUT    : ORIGIN = 0x42000000, LENGTH = 32M
	LDS_TXT    : ORIGIN = 0x44000000, LENGTH = 16M
}

SECTIONS
{
	__lds_rtl_case     = ORIGIN(LDS_DAT) + 0x1000;
	__lds_soc_base     = ORIGIN(LDS_TXT);
	__lds_indicate     = ORIGIN(LDS_OUT);

	__lds_print        = 1;
	__lds_print_size   = 0;

	.text : {
		rom/start.o	(.text)
		*(.text)
	} > IROM_BASE
	. = ALIGN(4);

	.rodata : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	} > IROM_BASE
	. = ALIGN(4);

	_data_load = .;
	.data : AT(_data_load) {
		_data_start = .;
		*(.data)
		. = ALIGN(4);
	} > IRAM_DATA
	. = ALIGN(4);

	.bss (NOLOAD) : {
		__bss_start = .;
		*(.bss)
	} > IRAM_DATA
	. = ALIGN(4);
	_end = .;

	/* set data buandry */
	_data_len  = SIZEOF(.data);

	/* also init stack offset
	 * leave 4 words for abort-stack
	 */
	__lds_stack_svc = ORIGIN(IRAM_STAC) + LENGTH(IRAM_STAC) - 0x20;
}

