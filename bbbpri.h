/*
 * Copyright (c) 1998-2003 Bart Massey
 * All Rights Reserved
 * Please see end of file for license information
 */

typedef enum _item_classes {
  ATOMIC_ITEM,
  COMPOSITE_ITEM
} item_classes;

struct _atomic {
  item_types type;
  datap data;
} atomic;

struct _composite {
  struct _item *children;
};

struct _callback_rec {
  struct _callback_rec *next, *prev;
  wtid wt;
  datap data;
  int ndata;
  struct _item *parent;
  callback *cb;
};

struct _item {
  item_names name;
  struct _item *next, *prev;
  struct _item *parent;
  item_classes class;
  struct _callback_rec *callbacks;
  union {
    struct _atomic a;
    struct _composite c;
  } contents;
};

struct _bboard {
  wtid cur_wtid;
  struct _item *contents;
};

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
