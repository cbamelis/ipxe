/*
 * Copyright (C) 2026 Christophe Bamelis.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * You can also choose to distribute this program under the terms of
 * the Unmodified Binary Distribution Licence (as given in the file
 * COPYING.UBDL), provided that you have satisfied its requirements.
 */

FILE_LICENCE ( GPL2_OR_LATER_OR_UBDL );
FILE_SECBOOT ( PERMITTED );

#include <errno.h>
#include <getopt.h>
#include <ipxe/command.h>
#include <ipxe/parseopt.h>

/** @file
 *
 * Numeric comparison commands
 *
 * The numeric counterpart to the string-only "iseq": islt/isgt/isle/isge
 * for "<", ">", "<=" and ">=".  Intended for RAM/CPU guards in boot menus,
 * e.g. "isge ${memsize} 4096 && item ...".  Negation needs no command:
 * "||" already inverts a command's exit status.
 */

/** Numeric comparison options */
struct isnum_options {};

/** Numeric comparison option list */
static struct option_descriptor isnum_opts[] = {};

/** Numeric comparison command descriptor */
static struct command_descriptor isnum_cmd =
	COMMAND_DESC ( struct isnum_options, isnum_opts, 2, 2,
		       "<value1> <value2>" );

/**
 * Parse two numeric arguments for numeric comparison commands
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @v first		First parsed value to fill in
 * @v second		Second parsed value to fill in
 * @ret rc		Return status code
 */
static int isnum_parse ( int argc, char **argv, unsigned int *first,
			 unsigned int *second ) {
	struct isnum_options opts;
	int rc;

	/* Parse options */
	if ( ( rc = parse_options ( argc, argv, &isnum_cmd, &opts ) ) != 0 )
		return rc;

	/* Parse numeric values */
	if ( ( rc = parse_integer ( argv[optind], first ) ) != 0 )
		return rc;
	if ( ( rc = parse_integer ( argv[ optind + 1 ], second ) ) != 0 )
		return rc;

	return 0;
}

/**
 * "islt" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int islt_exec ( int argc, char **argv ) {
	unsigned int first;
	unsigned int second;
	int rc;

	/* Parse numeric arguments */
	if ( ( rc = isnum_parse ( argc, argv, &first, &second ) ) != 0 )
		return rc;

	/* Return success iff first value is less than second */
	return ( ( first < second ) ? 0 : -ERANGE );
}

/** "islt" command */
COMMAND ( islt, islt_exec );

/**
 * "isgt" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int isgt_exec ( int argc, char **argv ) {
	unsigned int first;
	unsigned int second;
	int rc;

	/* Parse numeric arguments */
	if ( ( rc = isnum_parse ( argc, argv, &first, &second ) ) != 0 )
		return rc;

	/* Return success iff first value is greater than second */
	return ( ( first > second ) ? 0 : -ERANGE );
}

/** "isgt" command */
COMMAND ( isgt, isgt_exec );

/**
 * "isle" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int isle_exec ( int argc, char **argv ) {
	unsigned int first;
	unsigned int second;
	int rc;

	/* Parse numeric arguments */
	if ( ( rc = isnum_parse ( argc, argv, &first, &second ) ) != 0 )
		return rc;

	/* Return success iff first value is less than or equal to second */
	return ( ( first <= second ) ? 0 : -ERANGE );
}

/** "isle" command */
COMMAND ( isle, isle_exec );

/**
 * "isge" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int isge_exec ( int argc, char **argv ) {
	unsigned int first;
	unsigned int second;
	int rc;

	/* Parse numeric arguments */
	if ( ( rc = isnum_parse ( argc, argv, &first, &second ) ) != 0 )
		return rc;

	/* Return success iff first value is greater than or equal to second */
	return ( ( first >= second ) ? 0 : -ERANGE );
}

/** "isge" command */
COMMAND ( isge, isge_exec );
