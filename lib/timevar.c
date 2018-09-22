/* Timing variables for measuring compiler performance.

   Copyright (C) 2000, 2002, 2004, 2006, 2009-2015, 2018 Free Software
   Foundation, Inc.

   Contributed by Alex Samuel <samuel@codesourcery.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <config.h>

/* This source file was taken from the GCC source code.  */

#include "../src/system.h"
#if HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
int timevar_report = 0;

#ifdef HAVE_SYS_TIMES_H
# include <sys/times.h>
#endif
#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif

#ifndef HAVE_CLOCK_T
typedef int clock_t;
#endif

#ifndef HAVE_STRUCT_TMS
struct tms
{
  clock_t tms_utime;
  clock_t tms_stime;
  clock_t tms_cutime;
  clock_t tms_cstime;
};
#endif

#if defined HAVE_DECL_GETRUSAGE && !HAVE_DECL_GETRUSAGE
int getrusage (int, struct rusage *);
#endif
#if defined HAVE_DECL_TIMES && !HAVE_DECL_TIMES
clock_t times (struct tms *);
#endif
#if defined HAVE_DECL_CLOCK && !HAVE_DECL_CLOCK
clock_t clock (void);
#endif

/* Calculation of scale factor to convert ticks to microseconds.
   We mustn't use CLOCKS_PER_SEC except with clock().  */
#if HAVE_SYSCONF && defined _SC_CLK_TCK
# define TICKS_PER_SECOND sysconf (_SC_CLK_TCK) /* POSIX 1003.1-1996 */
#elif defined CLK_TCK
# define TICKS_PER_SECOND CLK_TCK /* POSIX 1003.1-1988; obsolescent */
#elif defined HZ
# define TICKS_PER_SECOND HZ  /* traditional UNIX */
#else
# define TICKS_PER_SECOND 100 /* often the correct value */
#endif

/* Prefer times to getrusage to clock (each gives successively less
   information).  */
#if defined HAVE_TIMES
# define USE_TIMES
# define HAVE_USER_TIME
# define HAVE_SYS_TIME
# define HAVE_WALL_TIME
#elif defined HAVE_GETRUSAGE
# define USE_GETRUSAGE
# define HAVE_USER_TIME
# define HAVE_SYS_TIME
#elif defined HAVE_CLOCK
# define USE_CLOCK
# define HAVE_USER_TIME
#endif

/* libc is very likely to have snuck a call to sysconf() into one of
   the underlying constants, and that can be very slow, so we have to
   precompute them.  Whose wonderful idea was it to make all those
   _constants_ variable at run time, anyway?  */
#ifdef USE_TIMES
static float ticks_to_msec;
# define TICKS_TO_MSEC (1.0 / TICKS_PER_SECOND)
#elif defined USE_CLOCK
static float clocks_to_msec;
# define CLOCKS_TO_MSEC (1.0 / CLOCKS_PER_SEC)
#endif

#include "timevar.h"

/* See timevar.h for an explanation of timing variables.  */

/* This macro evaluates to nonzero if timing variables are enabled.  */
#define TIMEVAR_ENABLE (timevar_report)

/* A timing variable.  */

struct timevar_def
{
  /* Elapsed time for this variable.  */
  struct timevar_time_def elapsed;

  /* If this variable is timed independently of the timing stack,
     using timevar_start, this contains the start time.  */
  struct timevar_time_def start_time;

  /* The name of this timing variable.  */
  const char *name;

  /* Non-zero if this timing variable is running as a standalone
     timer.  */
  unsigned standalone : 1;

  /* Non-zero if this timing variable was ever started or pushed onto
     the timing stack.  */
  unsigned used : 1;
};

/* An element on the timing stack.  Elapsed time is attributed to the
   topmost timing variable on the stack.  */

struct timevar_stack_def
{
  /* The timing variable at this stack level.  */
  struct timevar_def *timevar;

  /* The next lower timing variable context in the stack.  */
  struct timevar_stack_def *next;
};

/* Declared timing variables.  Constructed from the contents of
   timevar.def.  */
static struct timevar_def timevars[TIMEVAR_LAST];

/* The top of the timing stack.  */
static struct timevar_stack_def *stack;

/* A list of unused (i.e. allocated and subsequently popped)
   timevar_stack_def instances.  */
static struct timevar_stack_def *unused_stack_instances;

/* The time at which the topmost element on the timing stack was
   pushed.  Time elapsed since then is attributed to the topmost
   element.  */
static struct timevar_time_def start_time;

static void get_time (struct timevar_time_def *);
static void timevar_accumulate (struct timevar_time_def *,
                                struct timevar_time_def *,
                                struct timevar_time_def *);

/* Fill the current times into TIME.  The definition of this function
   also defines any or all of the HAVE_USER_TIME, HAVE_SYS_TIME, and
   HAVE_WALL_TIME macros.  */

static void
get_time (struct timevar_time_def *now)
{
  now->user = 0;
  now->sys  = 0;
  now->wall = 0;

  if (!TIMEVAR_ENABLE)
    return;

  {
#ifdef USE_TIMES
    struct tms tms;
    now->wall = times (&tms)  * ticks_to_msec;
    now->user = (tms.tms_utime + tms.tms_cutime) * ticks_to_msec;
    now->sys  = (tms.tms_stime + tms.tms_cstime) * ticks_to_msec;
#elif defined USE_GETRUSAGE
    struct rusage rusage;
    getrusage (RUSAGE_CHILDREN, &rusage);
    now->user = rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec * 1e-6;
    now->sys  = rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec * 1e-6;
#elif defined USE_CLOCK
    now->user = clock () * clocks_to_msec;
#endif
  }
}

/* Add the difference between STOP and START to TIMER.  */

static void
timevar_accumulate (struct timevar_time_def *timer,
                    struct timevar_time_def *start,
                    struct timevar_time_def *stop)
{
  timer->user += stop->user - start->user;
  timer->sys += stop->sys - start->sys;
  timer->wall += stop->wall - start->wall;
}

void
timevar_init ()
{
  if (!TIMEVAR_ENABLE)
    return;

  /* Zero all elapsed times.  */
  memset ((void *) timevars, 0, sizeof (timevars));

  /* Initialize the names of timing variables.  */
#define DEFTIMEVAR(identifier__, name__) \
  timevars[identifier__].name = name__;
#include "timevar.def"
#undef DEFTIMEVAR

#if defined USE_TIMES
  ticks_to_msec = TICKS_TO_MSEC;
#elif defined USE_CLOCK
  clocks_to_msec = CLOCKS_TO_MSEC;
#endif
}

void
timevar_push (timevar_id_t timevar)
{
  struct timevar_def *tv = &timevars[timevar];
  struct timevar_stack_def *context;
  struct timevar_time_def now;

  if (!TIMEVAR_ENABLE)
    return;

  /* Mark this timing variable as used.  */
  tv->used = 1;

  /* Can't push a standalone timer.  */
  if (tv->standalone)
    abort ();

  /* What time is it?  */
  get_time (&now);

  /* If the stack isn't empty, attribute the current elapsed time to
     the old topmost element.  */
  if (stack)
    timevar_accumulate (&stack->timevar->elapsed, &start_time, &now);

  /* Reset the start time; from now on, time is attributed to
     TIMEVAR.  */
  start_time = now;

  /* See if we have a previously-allocated stack instance.  If so,
     take it off the list.  If not, malloc a new one.  */
  if (unused_stack_instances != NULL)
    {
      context = unused_stack_instances;
      unused_stack_instances = unused_stack_instances->next;
    }
  else
    context = (struct timevar_stack_def *)
      xmalloc (sizeof (struct timevar_stack_def));

  /* Fill it in and put it on the stack.  */
  context->timevar = tv;
  context->next = stack;
  stack = context;
}

void
timevar_pop (timevar_id_t timevar)
{
  struct timevar_time_def now;
  struct timevar_stack_def *popped = stack;

  if (!TIMEVAR_ENABLE)
    return;

  if (&timevars[timevar] != stack->timevar)
    abort ();

  /* What time is it?  */
  get_time (&now);

  /* Attribute the elapsed time to the element we're popping.  */
  timevar_accumulate (&popped->timevar->elapsed, &start_time, &now);

  /* Reset the start time; from now on, time is attributed to the
     element just exposed on the stack.  */
  start_time = now;

  /* Take the item off the stack.  */
  stack = stack->next;

  /* Don't delete the stack element; instead, add it to the list of
     unused elements for later use.  */
  popped->next = unused_stack_instances;
  unused_stack_instances = popped;
}

void
timevar_start (timevar_id_t timevar)
{
  struct timevar_def *tv = &timevars[timevar];

  if (!TIMEVAR_ENABLE)
    return;

  /* Mark this timing variable as used.  */
  tv->used = 1;

  /* Don't allow the same timing variable to be started more than
     once.  */
  if (tv->standalone)
    abort ();
  tv->standalone = 1;

  get_time (&tv->start_time);
}

void
timevar_stop (timevar_id_t timevar)
{
  struct timevar_def *tv = &timevars[timevar];
  struct timevar_time_def now;

  if (!TIMEVAR_ENABLE)
    return;

  /* TIMEVAR must have been started via timevar_start.  */
  if (!tv->standalone)
    abort ();

  get_time (&now);
  timevar_accumulate (&tv->elapsed, &tv->start_time, &now);
}

void
timevar_get (timevar_id_t timevar,
             struct timevar_time_def *elapsed)
{
  struct timevar_def *tv = &timevars[timevar];
  struct timevar_time_def now;

  *elapsed = tv->elapsed;

  /* Is TIMEVAR currently running as a standalone timer?  */
  if (tv->standalone)
    {
      get_time (&now);
      timevar_accumulate (elapsed, &tv->start_time, &now);
    }
  /* Or is TIMEVAR at the top of the timer stack?  */
  else if (stack->timevar == tv)
    {
      get_time (&now);
      timevar_accumulate (elapsed, &start_time, &now);
    }
}

void
timevar_print (FILE *fp)
{
  /* Only print stuff if we have some sort of time information.  */
#if defined HAVE_USER_TIME || defined HAVE_SYS_TIME || defined HAVE_WALL_TIME
  unsigned /* timevar_id_t */ id;
  struct timevar_time_def *total = &timevars[TV_TOTAL].elapsed;
  struct timevar_time_def now;

  if (!TIMEVAR_ENABLE)
    return;

  /* Update timing information in case we're calling this from GDB.  */

  if (fp == 0)
    fp = stderr;

  /* What time is it?  */
  get_time (&now);

  /* If the stack isn't empty, attribute the current elapsed time to
     the old topmost element.  */
  if (stack)
    timevar_accumulate (&stack->timevar->elapsed, &start_time, &now);

  /* Reset the start time; from now on, time is attributed to
     TIMEVAR.  */
  start_time = now;

  fputs (_("\nExecution times (seconds)\n"), fp);
  for (id = 0; id < (unsigned) TIMEVAR_LAST; ++id)
    {
      struct timevar_def *tv = &timevars[(timevar_id_t) id];
      const float tiny = 5e-3;

      /* Don't print the total execution time here; that goes at the
         end.  */
      if ((timevar_id_t) id == TV_TOTAL)
        continue;

      /* Don't print timing variables that were never used.  */
      if (!tv->used)
        continue;

      /* Don't print timing variables if we're going to get a row of
         zeroes.  */
      if (tv->elapsed.user < tiny
          && tv->elapsed.sys < tiny
          && tv->elapsed.wall < tiny)
        continue;

      /* The timing variable name.  */
      fprintf (fp, " %-22s:", tv->name);

# ifdef HAVE_USER_TIME
      /* Print user-mode time for this process.  */
      fprintf (fp, "%7.2f (%2.0f%%) usr",
               tv->elapsed.user,
               (total->user == 0 ? 0 : tv->elapsed.user / total->user) * 100);
# endif

# ifdef HAVE_SYS_TIME
      /* Print system-mode time for this process.  */
      fprintf (fp, "%7.2f (%2.0f%%) sys",
               tv->elapsed.sys,
               (total->sys == 0 ? 0 : tv->elapsed.sys / total->sys) * 100);
# endif

# ifdef HAVE_WALL_TIME
      /* Print wall clock time elapsed.  */
      fprintf (fp, "%7.2f (%2.0f%%) wall",
               tv->elapsed.wall,
               (total->wall == 0 ? 0 : tv->elapsed.wall / total->wall) * 100);
# endif

      putc ('\n', fp);
    }

  /* Print total time.  */
  fputs (_(" TOTAL                 :"), fp);
#ifdef HAVE_USER_TIME
  fprintf (fp, "%7.2f          ", total->user);
#endif
#ifdef HAVE_SYS_TIME
  fprintf (fp, "%7.2f          ", total->sys);
#endif
#ifdef HAVE_WALL_TIME
  fprintf (fp, "%7.2f\n", total->wall);
#endif

#endif /* defined HAVE_USER_TIME || defined HAVE_SYS_TIME || defined HAVE_WALL_TIME */
}
