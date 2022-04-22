
#ifndef RUBY_EVAL_INTERN_H
#define RUBY_EVAL_INTERN_H

#define PASS_PASSED_BLOCK_TH(th) do { \
    (th)->passed_block = GC_GUARDED_PTR_REF((rb_block_t *)(th)->cfp->lfp[0]); \
    (th)->cfp->flag |= VM_FRAME_FLAG_PASSED; \
} while (0)

#define PASS_PASSED_BLOCK() do { \
    rb_thread_t * const __th__ = GET_THREAD(); \
    PASS_PASSED_BLOCK_TH(__th__); \
} while (0)

#include "ruby/ruby.h"
#include "ruby/node.h"
#include "ruby/util.h"
#include "vm_core.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#include <stdio.h>
#include <setjmp.h>

#include "ruby/st.h"
#include "dln.h"

#ifdef __APPLE__
#include <crt_externs.h>
#endif

/* Make alloca work the best possible way.  */
#ifdef __GNUC__
# ifndef atarist
#  ifndef alloca
#   define alloca __builtin_alloca
#  endif
# endif	/* atarist */
#else
# ifdef HAVE_ALLOCA_H
#  include <alloca.h>
# else
#  ifdef _AIX
#pragma alloca
#  else
#   ifndef alloca		/* predefined by HP cc +Olibcalls */
void *alloca();
#   endif
#  endif /* AIX */
# endif	/* HAVE_ALLOCA_H */
#endif /* __GNUC__ */

#ifdef HAVE_STDARG_PROTOTYPES
#include <stdarg.h>
#define va_init_list(a,b) va_start(a,b)
#else
#include <varargs.h>
#define va_init_list(a,b) va_start(a)
#endif

#ifndef HAVE_STRING_H
char *strrchr(const char *, const char);
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_NET_SOCKET_H
#include <net/socket.h>
#endif

#ifdef __MACOS__
#include "macruby_private.h"
#endif

#ifdef __VMS
#include "vmsruby_private.h"
#endif

#define ruby_setjmp(env) RUBY_SETJMP(env)
#define ruby_longjmp(env,val) RUBY_LONGJMP(env,val)
#ifdef __CYGWIN__
int _setjmp(), _longjmp();
#endif

#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#if defined(__VMS)
#pragma nostandard
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

/*
  Solaris sys/select.h switches select to select_large_fdset to support larger
  file descriptors if FD_SETSIZE is larger than 1024 on 32bit environment.
  But Ruby doesn't change FD_SETSIZE because fd_set is allocated dynamically.
  So following definition is required to use select_large_fdset.
*/
#ifdef HAVE_SELECT_LARGE_FDSET
#define select(n, r, w, e, t) select_large_fdset(n, r, w, e, t)
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include <sys/stat.h>

#define SAVE_ROOT_JMPBUF(th, stmt) do \
  if (ruby_setjmp((th)->root_jmpbuf) == 0) { \
      stmt; \
  } \
  else { \
      rb_fiber_start(); \
  } while (0)

#define TH_PUSH_TAG(th) do { \
  rb_thread_t * const _th = th; \
  struct rb_vm_tag _tag; \
  _tag.tag = 0; \
  _tag.prev = _th->tag; \
  _th->tag = &_tag;

#define TH_POP_TAG() \
  _th->tag = _tag.prev; \
} while (0)

#define TH_POP_TAG2() \
  _th->tag = _tag.prev

#define PUSH_TAG() TH_PUSH_TAG(GET_THREAD())
#define POP_TAG()      TH_POP_TAG()

#define TH_EXEC_TAG() ruby_setjmp(_th->tag->buf)

#define EXEC_TAG() \
  TH_EXEC_TAG()

#define TH_JUMP_TAG(th, st) do { \
  ruby_longjmp(th->tag->buf,(st)); \
} while (0)

#define JUMP_TAG(st) TH_JUMP_TAG(GET_THREAD(), st)

enum ruby_tag_type {
    RUBY_TAG_RETURN	= 0x1,
    RUBY_TAG_BREAK	= 0x2,
    RUBY_TAG_NEXT	= 0x3,
    RUBY_TAG_RETRY	= 0x4,
    RUBY_TAG_REDO	= 0x5,
    RUBY_TAG_RAISE	= 0x6,
    RUBY_TAG_THROW	= 0x7,
    RUBY_TAG_FATAL	= 0x8,
    RUBY_TAG_MASK	= 0xf
};
#define TAG_RETURN	RUBY_TAG_RETURN
#define TAG_BREAK	RUBY_TAG_BREAK
#define TAG_NEXT	RUBY_TAG_NEXT
#define TAG_RETRY	RUBY_TAG_RETRY
#define TAG_REDO	RUBY_TAG_REDO
#define TAG_RAISE	RUBY_TAG_RAISE
#define TAG_THROW	RUBY_TAG_THROW
#define TAG_FATAL	RUBY_TAG_FATAL
#define TAG_MASK	RUBY_TAG_MASK

#define NEW_THROW_OBJECT(val, pt, st) \
  ((VALUE)NEW_NODE(NODE_LIT, (val), (pt), (st)))
#define SET_THROWOBJ_CATCH_POINT(obj, val) \
  (RNODE((obj))->u2.value = (val))
#define SET_THROWOBJ_STATE(obj, val) \
  (RNODE((obj))->u3.value = (val))

#define GET_THROWOBJ_VAL(obj)         ((VALUE)RNODE((obj))->u1.value)
#define GET_THROWOBJ_CATCH_POINT(obj) ((VALUE*)RNODE((obj))->u2.value)
#define GET_THROWOBJ_STATE(obj)       ((int)RNODE((obj))->u3.value)

#define SCOPE_TEST(f)  (vm_cref()->nd_visi & (f))
#define SCOPE_CHECK(f) (vm_cref()->nd_visi == (f))
#define SCOPE_SET(f)   (vm_cref()->nd_visi = (f))

#define CHECK_STACK_OVERFLOW(cfp, margin) do \
  if (((VALUE *)(cfp)->sp) + (margin) + sizeof(rb_control_frame_t) >= ((VALUE *)cfp)) { \
      rb_exc_raise(sysstack_error); \
  } \
while (0)

void rb_thread_cleanup(void);
void rb_thread_wait_other_threads(void);

enum {
    RAISED_EXCEPTION = 1,
    RAISED_STACKOVERFLOW = 2,
    RAISED_NOMEMORY = 4
};
int rb_thread_set_raised(rb_thread_t *th);
int rb_thread_reset_raised(rb_thread_t *th);
#define rb_thread_raised_set(th, f)   ((th)->raised_flag |= (f))
#define rb_thread_raised_reset(th, f) ((th)->raised_flag &= ~(f))
#define rb_thread_raised_p(th, f)     (((th)->raised_flag & (f)) != 0)
#define rb_thread_raised_clear(th)    ((th)->raised_flag = 0)

VALUE rb_f_eval(int argc, VALUE *argv, VALUE self);
VALUE rb_make_exception(int argc, VALUE *argv);

NORETURN(void rb_fiber_start(void));

NORETURN(void rb_print_undef(VALUE, ID, int));
NORETURN(void vm_localjump_error(const char *,VALUE, int));
NORETURN(void vm_jump_tag_but_local_jump(int, VALUE));

VALUE vm_make_jump_tag_but_local_jump(int state, VALUE val);
NODE *vm_cref(void);
rb_control_frame_t *vm_get_ruby_level_caller_cfp(rb_thread_t *th, rb_control_frame_t *cfp);
VALUE rb_obj_is_proc(VALUE);
VALUE rb_vm_call_cfunc(VALUE recv, VALUE (*func)(VALUE), VALUE arg, const rb_block_t *blockptr, VALUE filename);
void rb_thread_terminate_all(void);
VALUE rb_vm_top_self();
VALUE rb_vm_cbase(void);
void rb_trap_restore_mask(void);

#endif /* RUBY_EVAL_INTERN_H */
