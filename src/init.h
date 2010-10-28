/* File: init.h */

/*
 * Copyright (c) 2000 Robert Ruehlmann
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.
 */

#ifndef INCLUDED_INIT_H
#define INCLUDED_INIT_H

#include "h-basic.h"
#include "z-bitflag.h"
#include "z-file.h"
#include "z-rand.h"
#include "parser.h"

typedef struct header header;

typedef errr (*parse_info_txt_func)(char *buf, header *head);
typedef errr (*eval_info_post_func)(header *head);
typedef errr (*emit_info_txt_index_func)(ang_file *fp, header *head, int i);
typedef errr (*emit_info_txt_always_func)(ang_file *fp, header *head);

/*
 * Template file header information (see "init.c").  16 bytes.
 *
 * Note that the sizes of many of the "arrays" are between 32768 and
 * 65535, and so we must use "unsigned" values to hold the "sizes" of
 * these arrays below.  Normally, I try to avoid using unsigned values,
 * since they can cause all sorts of bizarre problems, but I have no
 * choice here, at least, until the "race" array is split into "normal"
 * and "unique" monsters, which may or may not actually help.
 *
 * Note that, on some machines, for example, the Macintosh, the standard
 * "read()" and "write()" functions cannot handle more than 32767 bytes
 * at one time, so we need replacement functions, see "util.c" for details.
 *
 * Note that, on some machines, for example, the Macintosh, the standard
 * "malloc()" function cannot handle more than 32767 bytes at one time,
 * but we may assume that the "ralloc()" function can handle up to 65535
 * butes at one time.  We should not, however, assume that the "ralloc()"
 * function can handle more than 65536 bytes at a time, since this might
 * result in segmentation problems on certain older machines, and in fact,
 * we should not assume that it can handle exactly 65536 bytes at a time,
 * since the internal functions may use an unsigned short to specify size.
 *
 * In general, these problems occur only on machines (such as most personal
 * computers) which use 2 byte "int" values, and which use "int" for the
 * arguments to the relevent functions.
 */
struct header
{
	byte v_major;		/* Version -- major */
	byte v_minor;		/* Version -- minor */
	byte v_patch;		/* Version -- patch */
	byte v_extra;		/* Version -- extra */


	u16b info_num;		/* Number of "info" records */

	u16b info_len;		/* Size of each "info" record */


	u32b head_size;		/* Size of the "header" in bytes */

	u32b info_size;		/* Size of the "info" array in bytes */

	u32b name_size;		/* Size of the "name" array in bytes */

	u32b text_size;		/* Size of the "text" array in bytes */

	void *info_ptr;
	char *name_ptr;
	char *text_ptr;

	parse_info_txt_func parse_info_txt;
	eval_info_post_func eval_info_post;	/* Evaluate after parsing */
	emit_info_txt_index_func emit_info_txt_index;
	emit_info_txt_always_func emit_info_txt_always;
	
};

extern errr init_info_txt(ang_file *fp, char *buf, header *head,
                          parse_info_txt_func parse_info_txt_line);
extern errr eval_info(eval_info_post_func eval_info_process, header *head);

extern errr emit_info_txt(ang_file *fp, ang_file *template, char *buf, header *head,
   emit_info_txt_index_func emit_info_txt_index, emit_info_txt_always_func emit_info_txt_always);

#ifdef TEST
extern struct parser *init_parse_a(void);
extern struct parser *init_parse_c(void);
extern struct parser *init_parse_e(void);
extern struct parser *init_parse_f(void);
extern struct parser *init_parse_h(void);
extern struct parser *init_parse_k(void);
extern struct parser *init_parse_p(void);
extern struct parser *init_parse_r(void);
extern struct parser *init_parse_v(void);
extern struct parser *init_parse_z(void);
extern struct parser *init_parse_flavor(void);
extern struct parser *init_parse_names(void);
#endif

extern errr parse_file(struct parser *p, const char *filename);

extern errr parse_s_info(char *buf, header *head);

extern errr emit_r_info_index(ang_file *fp, header *head, int i);


/*
 * Error tracking
 */
extern int error_idx;
extern int error_line;

extern header flavor_head;

#endif /* INCLUDED_INIT_H */
