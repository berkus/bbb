/*
 * Copyright (c) 1998-2003 Bart Massey
 * All Rights Reserved
 * Please see end of file for license information
 */

/*
 * doubly linked lists
 * circular in the reverse direction,
 * but not in the forward direction -- this
 * works pretty nicely, actually.
 * define ELEM to be the structure type of list elements.
 */

#ifdef __STDC__
/* this is insane */
#define __ELEMIZE(EL,ID) EL ## _ ## ID
#define _ELEMIZE(EL,ID) __ELEMIZE(EL,ID)
#define ELEMIZE(ID) _ELEMIZE(ELEM,ID)
#else
#define ELEMIZE(ID) ELEM/**/_/**/ID
#endif

static void ELEMIZE(unlink)( hd, dp )
struct ELEM **hd;
struct ELEM *dp;
{
  if( !*hd )
    abort();
  if( *hd == dp ) {
    *hd = dp->next;
    if( *hd )
      (*hd)->prev = dp->prev;
    return;
  }
  if( dp->next )
    dp->next->prev = dp->prev;
  else
    (*hd)->prev = dp->prev;
  dp->prev->next = dp->next;
}

static struct ELEM *ELEMIZE(tail)( hd )
struct ELEM *hd;
{
  if( !hd )
    return 0;
  return hd->prev;
}

static struct ELEM *ELEMIZE(next)( p )
struct ELEM *p;
{
  return p->next;
}

static struct ELEM *ELEMIZE(prev)( p )
struct ELEM *p;
{
  if( !p->prev->next )
    return 0;
  return p->prev;
}

static void ELEMIZE(link)( hd, atdp, dp )
struct ELEM **hd;
struct ELEM *atdp, *dp;
{
  if( !*hd ) {
    if( atdp )
      abort();
    *hd = dp;
    dp->next = 0;
    dp->prev = dp;
    return;
  }
  if( !atdp ) {
    dp->next = *hd;
    dp->prev = (*hd)->prev;
    dp->next->prev = dp;
    *hd = dp;
    return;
  }
  dp->prev = atdp;
  dp->next = atdp->next;
  if( atdp->next )
    atdp->next->prev = dp;
  else
    (*hd)->prev = dp;
  atdp->next = dp;
}

#undef ELEMIZE
#undef _ELEMIZE
#undef __ELEMIZE

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
