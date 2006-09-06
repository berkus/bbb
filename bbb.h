/*
 * Copyright (c) 1998-2003 Bart Massey
 * All Rights Reserved
 * Please see end of file for license information
 */

extern unsigned int *bb_its;
extern int bb_nits;

#if defined(__STDC__) || defined(__GNUC__)
#define BB_PROTO
#endif

#define NO_ITEM 0
typedef unsigned int wtid;  /* XXX 4e9 outstanding writes??? */

#ifdef BB_PROTO
struct _bboard;
struct _item;
struct _callback_rec;
#endif
typedef struct _item *item;
typedef struct _bboard *bboard;
typedef struct _callback_rec *callback_rec;

typedef void *datap;
typedef int datasiz;

typedef enum _notice {
  INSERT_NOTICE,
  SET_NOTICE,
  UNLINK_NOTICE,
  UNLINK_PARENT_NOTICE
} notice;

typedef void callback(
#ifdef BB_PROTO
 bboard bb, item d, item c, datasiz ndata, datap data, wtid t, notice no
#endif 
);

extern bboard bb_new(
#ifdef BB_PROTO
 bb_names bnm
#endif
);

extern bboard bb_find(
#ifdef BB_PROTO
 bb_names bnm
#endif
);

extern void bb_init(
#ifdef BB_PROTO
 bboard bb, item p
#endif
);

extern item bb_new_area(
#ifdef BB_PROTO
 bboard bb, item_names nm
#endif
);

extern item bb_new_datum(
#ifdef BB_PROTO
 bboard bb, item_names nm, item_types tp, datap data
#endif
);

extern item bb_lookup(
#ifdef BB_PROTO
 bboard bb, item a, item_names nm
#endif
);

extern void bb_prepend(
#ifdef BB_PROTO
 bboard bb, wtid t, item a, item p
#endif
);

extern void bb_append(
#ifdef BB_PROTO
 bboard bb, wtid t, item a, item p
#endif
);

extern void bb_insert_before(
#ifdef BB_PROTO
 bboard bb, wtid t, item q, item p
#endif
);

extern void bb_insert_after(
#ifdef BB_PROTO
 bboard bb, wtid t, item q, item p
#endif
);

extern void bb_unlink(
#ifdef BB_PROTO
 bboard bb, wtid t, item p
#endif
);

extern void bb_delete(
#ifdef BB_PROTO
 bboard bb, item p
#endif
);

extern wtid bb_set(
#ifdef BB_PROTO
 bboard bb, wtid t, item d, datap data
#endif
);

extern callback_rec bb_watch(
#ifdef BB_PROTO
 bboard bb, item a, callback *c, datasiz ndata, datap data
#endif 
);

extern void bb_unwatch(
#ifdef BB_PROTO
bboard bb, callback_rec cb
#endif 
);

extern datap bb_datum(
#ifdef BB_PROTO
 bboard bb, item d
#endif 
);

extern item bb_parent(
#ifdef BB_PROTO
 bboard bb, item p
#endif
);

extern item bb_first(
#ifdef BB_PROTO
 bboard bb, item a
#endif
);

extern item bb_next(
#ifdef BB_PROTO
 bboard bb, item d
#endif
);

extern item bb_last(
#ifdef BB_PROTO
 bboard bb, item a
#endif
);

/*
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
