/*
 * This program is decoding and printing SPD contents
 * in human readable format
 * As an argument program, you must specify the file name.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Copyright (C) 2015 Alexander Smirnov <alllecs@yandex.ru>
 *
 * Originally from https://github.com/groeck/i2c-tools/blob/master/eeprom/decode-dimms
 */

#include <common.h>
#include <command.h>
#include <libfile.h>
#include <malloc.h>
#include <ddr_spd.h>

static int do_decode(int argc, char *argv[])
{
	int ret;
	size_t size;
	void *data;

	if (argc != 2)
		return COMMAND_ERROR_USAGE;

	ret = read_file_2(argv[1], &size, &data, 256);
	if (ret && ret != -EFBIG) {
		printf("unable to read %s: %s\n", argv[1], strerror(-ret));
		return COMMAND_ERROR;
	}

	printf("Decoding EEPROM: %s\n\n", argv[1]);
	ddr_spd_print(data);
	printf("\n\n");

	free(data);

	return 0;
}

BAREBOX_CMD_HELP_START(decode)
BAREBOX_CMD_HELP_TEXT("Not enough or more than one argument to continue.")
BAREBOX_CMD_HELP_END

BAREBOX_CMD_START(decode)
	.cmd	= do_decode,
	BAREBOX_CMD_HELP(cmd_decode_help)
BAREBOX_CMD_END
