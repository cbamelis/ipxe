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
 * The numeric counterpart to the string-only "iseq".  A numeric
 * comparison has three possible outcomes, so "numlt", "numeq" and
 * "numgt" form a complete set: every relation is one or two of those
 * outcomes.
 *
 *   a <  b	numlt a b
 *   a == b	numeq a b
 *   a >  b	numgt a b
 *   a <= b	numlt a b || numeq a b
 *   a >= b	numgt a b || numeq a b
 *   a != b	numlt a b || numgt a b
 *
 * Both sides of the "||" are positive tests, so a subsequent "&&" still
 * works as expected.  A "||" used as a negation (with the action in the
 * failure branch) must end the line: iPXE evaluates "&&" and "||"
 * strictly from left to right, so a trailing "&& action" would run in
 * both branches.
 */

/** Numeric comparison options */
struct num_options {};

/** Numeric comparison option list */
static struct option_descriptor num_opts[] = {};

/** Numeric comparison command descriptor */
static struct command_descriptor num_cmd =
	COMMAND_DESC ( struct num_options, num_opts, 2, 2,
		       "<value1> <value2>" );

/**
 * Parse numeric comparison arguments
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @v first		First parsed value to fill in
 * @v second		Second parsed value to fill in
 * @ret rc		Return status code
 */
static int num_parse ( int argc, char **argv, unsigned int *first,
		       unsigned int *second ) {
	struct num_options opts;
	int rc;

	/* Parse options */
	if ( ( rc = parse_options ( argc, argv, &num_cmd, &opts ) ) != 0 )
		return rc;

	/* Parse values */
	if ( ( rc = parse_integer ( argv[optind], first ) ) != 0 )
		return rc;
	if ( ( rc = parse_integer ( argv[ optind + 1 ], second ) ) != 0 )
		return rc;

	return 0;
}

/**
 * "numlt" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int numlt_exec ( int argc, char **argv ) {
	unsigned int first;
	unsigned int second;
	int rc;

	/* Parse values */
	if ( ( rc = num_parse ( argc, argv, &first, &second ) ) != 0 )
		return rc;

	/* Return success iff first value is less than second value */
	return ( ( first < second ) ? 0 : -ERANGE );
}

/** "numlt" command */
COMMAND ( numlt, numlt_exec );

/**
 * "numeq" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int numeq_exec ( int argc, char **argv ) {
	unsigned int first;
	unsigned int second;
	int rc;

	/* Parse values */
	if ( ( rc = num_parse ( argc, argv, &first, &second ) ) != 0 )
		return rc;

	/* Return success iff values are equal */
	return ( ( first == second ) ? 0 : -ERANGE );
}

/** "numeq" command */
COMMAND ( numeq, numeq_exec );

/**
 * "numgt" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int numgt_exec ( int argc, char **argv ) {
	unsigned int first;
	unsigned int second;
	int rc;

	/* Parse values */
	if ( ( rc = num_parse ( argc, argv, &first, &second ) ) != 0 )
		return rc;

	/* Return success iff first value is greater than second value */
	return ( ( first > second ) ? 0 : -ERANGE );
}

/** "numgt" command */
COMMAND ( numgt, numgt_exec );
