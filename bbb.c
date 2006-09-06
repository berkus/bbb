/*
 * Copyright (c) 1998-2003 Bart Massey
 * All Rights Reserved
 * Please see end of file for license information
 */

typedef int bb_names;
typedef int item_names;
typedef int item_types;
#include "bbb.h"
#include "bbbpri.h"

#define ELEM _callback_rec
#include "ll.c"
#undef ELEM
#define ELEM _item
#include "ll.c"
#undef ELEM

extern void *malloc( unsigned int );
extern void free( void * );
#define BB_MAKE(T,V) T V = (T) malloc( sizeof( *V ) )

#ifdef BSD
extern void bcopy( void *, void *, int );
#define COPY(D,S,L) bcopy((S),(D),(L))
#else
extern void *memcpy( void *, void *, unsigned int );
#define COPY(D,S,L) memcpy((D),(S),(L))
#endif

static void do_callback( bboard bb, wtid t, item p, item c, notice no ) {
  callback_rec cp;

  for( cp = p->callbacks; cp; cp = _callback_rec_next( cp ) ) {
    if( cp->wt == t )
      abort();
    cp->wt = t;
    cp->cb( bb, p, c, cp->ndata, cp->data, t, no );
  }
}

static void do_callbacks( bboard bb, wtid t, item p, item c, notice no ) {
  do_callback( bb, t, p, c, no );
  if( p->parent )
    do_callbacks( bb, t, p->parent, c, no );
}

bboard bb_new( bb_names bnm ) {
  BB_MAKE(bboard,bb);

  bb->cur_wtid = 0;
  bb->contents = 0;
  return bb;
}

bboard bb_find( bb_names bnm ) {
  abort();
}

void bb_init( bboard bb, item p ) {
  if( bb->contents )
    abort();
  bb->contents = p;
}

item bb_new_area( bboard bb, item_names nm ) {
  BB_MAKE(item,a);

  a->next = 0;
  a->prev = 0;
  a->parent = 0;
  a->class = COMPOSITE_ITEM;
  a->callbacks = 0;
  a->contents.c.children = 0;
  return a;
}

item bb_new_datum( bboard bb, item_names nm, item_types tp, datap data ) {
  BB_MAKE(item,a);

  a->next = 0;
  a->prev = 0;
  a->parent = 0;
  a->class = ATOMIC_ITEM;
  a->callbacks = 0;
  a->contents.a.type = tp;
  COPY( a->contents.a.data, data, bb_its[tp] );
  return a;
}

item bb_lookup( bboard bb, item a, item_names nm ) {
  item p;

  if( a->class != COMPOSITE_ITEM )
    abort();
  for( p = a->contents.c.children; p; p = p->next )
    if( p->name == nm )
      return p;
  return NO_ITEM;
}

void bb_prepend( bboard bb, wtid t, item a, item p ) {
  if( a->class != COMPOSITE_ITEM )
    abort();

  _item_link( &a->contents.c.children, 0, p );
  p->parent = a;
  do_callbacks( bb, t, a, a, INSERT_NOTICE );
}

void bb_append( bboard bb, wtid t, item a, item p ) {
  if( a->class != COMPOSITE_ITEM )
    abort();

  _item_link( &a->contents.c.children, _item_tail( a ), p );
  p->parent = a;
  do_callbacks( bb, t, a, a, INSERT_NOTICE );
}

void bb_insert_before( bboard bb, wtid t, item q, item p ) {
  if( !q->parent )
    abort();
  if( q->parent->class != COMPOSITE_ITEM )
    abort();
    
  _item_link( &q->parent->contents.c.children, _item_prev( q ), p );
  p->parent = q->parent;
  do_callbacks( bb, t, q->parent, q->parent, INSERT_NOTICE );
}

void bb_insert_after( bboard bb, wtid t, item q, item p ) {
  if( !q->parent )
    abort();
  if( q->parent->class != COMPOSITE_ITEM )
    abort();
    
  _item_link( &q->parent->contents.c.children, q, p );
  p->parent = q->parent;
  do_callbacks( bb, t, q->parent, q->parent, INSERT_NOTICE );
}

static void do_unlink_callbacks( bboard bb, wtid t, item p ) {
  item a;

  if( p->class == COMPOSITE_ITEM ) {
    for( a = p->contents.c.children; a; a = _item_next( a ) )
      do_unlink_callbacks( bb, t, a );
    do_callback( bb, t, p, p, UNLINK_PARENT_NOTICE );
    return;
  }
  do_callback( bb, t, p, p, UNLINK_PARENT_NOTICE );
}

void bb_unlink( bboard bb, wtid t, item p ) {
  if( !p->parent ) {
    if( bb->contents == p ) {
      bb->contents = 0;
      return;
    }
    abort();
  }
  if( p->class == COMPOSITE_ITEM )
    do_unlink_callbacks( bb, t, p );
  _item_unlink( &p->parent->contents.c.children, p );
  do_callback( bb, t, p, p, UNLINK_NOTICE );
  p->parent = 0;
}

void bb_delete( bboard bb, item p ) {
  item np;
  
  if( p->class == COMPOSITE_ITEM ) {
    for( np = p->contents.c.children; np; np = _item_next( np ) )
      bb_delete( bb, np );
  } else
    free( p->contents.a.data );
  free( p );
}

wtid bb_set( bboard bb, wtid t, item d, datap data ) {
  if( d->class != ATOMIC_ITEM )
    abort();
  COPY( d->contents.a.data, data, bb_its[d->contents.a.type] );
  do_callbacks( bb, t, d, d, SET_NOTICE );
  return bb->cur_wtid++;
}

extern callback_rec bb_watch( bboard bb, item p, callback *c,
 datasiz ndata, datap data )
{
  BB_MAKE(callback_rec,cb);

  cb->data = data;
  cb->ndata = ndata;
  cb->cb = c;
  cb->wt = -1;
  cb->parent = p;
  _callback_rec_link( &p->callbacks, _callback_rec_tail( p->callbacks ), cb );
  return cb;
}

extern void bb_unwatch( bboard bb, callback_rec cb ) {
  _callback_rec_unlink( &cb->parent->callbacks, cb );
  free( cb );
}

extern datap bb_datum( bboard bb, item d ) {
  if( d->class != ATOMIC_ITEM )
    abort();
  return d->contents.a.data;
}

item bb_parent( bboard bb, item p) {
  return p->parent;
}

extern item bb_first( bboard bb, item a ) {
  if( a->class != COMPOSITE_ITEM )
    abort();
  return a->contents.c.children;
}

extern item bb_next( bboard bb, item d ) {
  return _item_next( d );
}

extern item bb_last( bboard bb, item a ) {
  if( a->class != COMPOSITE_ITEM )
    abort();
  return _item_tail( a->contents.c.children );
}

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
