/**********************************************************************

  intern.h -

  $Author$
  $Date$
  created at: Thu Jun 10 14:22:17 JST 1993

  Copyright (C) 1993-2003 Yukihiro Matsumoto
  Copyright (C) 2000  Network Applied Communication Laboratory, Inc.
  Copyright (C) 2000  Information-technology Promotion Agency, Japan

**********************************************************************/

#ifndef RUBY_INTERN_H
#define RUBY_INTERN_H 1

#ifdef HAVE_STDARG_PROTOTYPES
# include <stdarg.h>
#else
# include <varargs.h>
#endif
#include <st.h>

/* 
 * Functions and variables that are used by more than one source file of
 * the kernel.
 */

#define ID_ALLOCATOR 1

/* array.c */
void rb_mem_clear(register VALUE*, register long);
VALUE rb_assoc_new(VALUE, VALUE);
VALUE rb_check_array_type(VALUE);
VALUE rb_ary_new(void);
VALUE rb_ary_new2(long);
VALUE rb_ary_new3(long,...);
VALUE rb_ary_new4(long, const VALUE *);
void rb_ary_free(VALUE);
VALUE rb_ary_freeze(VALUE);
VALUE rb_ary_aref(int, VALUE*, VALUE);
void rb_ary_store(VALUE, long, VALUE);
VALUE rb_ary_dup(VALUE);
VALUE rb_ary_to_ary(VALUE);
VALUE rb_ary_to_s(VALUE);
VALUE rb_ary_push(VALUE, VALUE);
VALUE rb_ary_pop(VALUE);
VALUE rb_ary_shift(VALUE);
VALUE rb_ary_unshift(VALUE, VALUE);
VALUE rb_ary_entry(VALUE, long);
VALUE rb_ary_each(VALUE);
VALUE rb_ary_join(VALUE, VALUE);
VALUE rb_ary_print_on(VALUE, VALUE);
VALUE rb_ary_reverse(VALUE);
VALUE rb_ary_sort(VALUE);
VALUE rb_ary_sort_bang(VALUE);
VALUE rb_ary_delete(VALUE, VALUE);
VALUE rb_ary_delete_at(VALUE, long);
VALUE rb_ary_clear(VALUE);
VALUE rb_ary_plus(VALUE, VALUE);
VALUE rb_ary_concat(VALUE, VALUE);
VALUE rb_ary_assoc(VALUE, VALUE);
VALUE rb_ary_rassoc(VALUE, VALUE);
VALUE rb_ary_includes(VALUE, VALUE);
VALUE rb_ary_cmp(VALUE, VALUE);
VALUE rb_check_array_value(VALUE);
VALUE rb_get_values_at(VALUE, long, int, VALUE*, VALUE(*)(VALUE,long));
/* bignum.c */
VALUE rb_big_clone(VALUE);
void rb_big_2comp(VALUE);
VALUE rb_big_norm(VALUE);
VALUE rb_uint2big(VALUE);
VALUE rb_int2big(SIGNED_VALUE);
VALUE rb_uint2inum(VALUE);
VALUE rb_int2inum(SIGNED_VALUE);
VALUE rb_cstr_to_inum(const char*, int, int);
VALUE rb_str_to_inum(VALUE, int, int);
VALUE rb_cstr2inum(const char*, int);
VALUE rb_str2inum(VALUE, int);
VALUE rb_big2str(VALUE, int);
VALUE rb_big2str0(VALUE, int, int);
SIGNED_VALUE rb_big2long(VALUE);
#define rb_big2int(x) rb_big2long(x)
VALUE rb_big2ulong(VALUE);
#define rb_big2uint(x) rb_big2ulong(x)
#if HAVE_LONG_LONG
VALUE rb_ll2inum(LONG_LONG);
VALUE rb_ull2inum(unsigned LONG_LONG);
LONG_LONG rb_big2ll(VALUE);
unsigned LONG_LONG rb_big2ull(VALUE);
#endif  /* HAVE_LONG_LONG */
void rb_quad_pack(char*,VALUE);
VALUE rb_quad_unpack(const char*,int);
VALUE rb_dbl2big(double);
double rb_big2dbl(VALUE);
VALUE rb_big_cmp(VALUE, VALUE);
VALUE rb_big_eq(VALUE, VALUE);
VALUE rb_big_plus(VALUE, VALUE);
VALUE rb_big_minus(VALUE, VALUE);
VALUE rb_big_mul(VALUE, VALUE);
VALUE rb_big_div(VALUE, VALUE);
VALUE rb_big_modulo(VALUE, VALUE);
VALUE rb_big_divmod(VALUE, VALUE);
VALUE rb_big_pow(VALUE, VALUE);
VALUE rb_big_and(VALUE, VALUE);
VALUE rb_big_or(VALUE, VALUE);
VALUE rb_big_xor(VALUE, VALUE);
VALUE rb_big_lshift(VALUE, VALUE);
/* class.c */
VALUE rb_class_boot(VALUE);
VALUE rb_class_new(VALUE);
VALUE rb_mod_init_copy(VALUE, VALUE);
VALUE rb_class_init_copy(VALUE, VALUE);
VALUE rb_singleton_class_clone(VALUE);
void rb_singleton_class_attached(VALUE,VALUE);
VALUE rb_make_metaclass(VALUE, VALUE);
void rb_check_inheritable(VALUE);
VALUE rb_class_inherited(VALUE, VALUE);
VALUE rb_define_class_id(ID, VALUE);
VALUE rb_module_new(void);
VALUE rb_define_module_id(ID);
VALUE rb_mod_included_modules(VALUE);
VALUE rb_mod_include_p(VALUE, VALUE);
VALUE rb_mod_ancestors(VALUE);
VALUE rb_class_instance_methods(int, VALUE*, VALUE);
VALUE rb_class_public_instance_methods(int, VALUE*, VALUE);
VALUE rb_class_protected_instance_methods(int, VALUE*, VALUE);
VALUE rb_class_private_instance_methods(int, VALUE*, VALUE);
VALUE rb_class_local_methods(VALUE);
VALUE rb_obj_singleton_methods(int, VALUE*, VALUE);
void rb_define_method_id(VALUE, ID, VALUE (*)(ANYARGS), int);
void rb_frozen_class_p(VALUE);
void rb_undef(VALUE, ID);
void rb_define_protected_method(VALUE, const char*, VALUE (*)(ANYARGS), int);
void rb_define_private_method(VALUE, const char*, VALUE (*)(ANYARGS), int);
void rb_define_singleton_method(VALUE, const char*, VALUE(*)(ANYARGS), int);
VALUE rb_singleton_class(VALUE);
/* compar.c */
int rb_cmpint(VALUE, VALUE, VALUE);
NORETURN(void rb_cmperr(VALUE, VALUE));
/* enum.c */
/* enumerator.c */
VALUE rb_enumeratorize(VALUE, VALUE, int, VALUE *);
#define RETURN_ENUMERATOR(obj, argc, argv) do {				\
	if (!rb_block_given_p())					\
	    return rb_enumeratorize(obj, ID2SYM(rb_frame_this_func()),	\
				    argc, argv);			\
    } while (0)
/* error.c */
RUBY_EXTERN int ruby_nerrs;
VALUE rb_exc_new(VALUE, const char*, long);
VALUE rb_exc_new2(VALUE, const char*);
VALUE rb_exc_new3(VALUE, VALUE);
PRINTF_ARGS(NORETURN(void rb_loaderror(const char*, ...)), 1, 2);
PRINTF_ARGS(NORETURN(void rb_name_error(ID, const char*, ...)), 2, 3);
NORETURN(void rb_invalid_str(const char*, const char*));
PRINTF_ARGS(void rb_compile_error(const char*, ...), 1, 2);
PRINTF_ARGS(void rb_compile_error_append(const char*, ...), 1, 2);
NORETURN(void rb_load_fail(const char*));
NORETURN(void rb_error_frozen(const char*));
void rb_check_frozen(VALUE);
/* eval.c */
#if defined(NFDBITS) && defined(HAVE_RB_FD_INIT)
typedef struct {
    int maxfd;
    fd_set *fdset;
} rb_fdset_t;

void rb_fd_init(volatile rb_fdset_t *);
void rb_fd_term(rb_fdset_t *);
void rb_fd_zero(rb_fdset_t *);
void rb_fd_set(int, rb_fdset_t *);
void rb_fd_clr(int, rb_fdset_t *);
int rb_fd_isset(int, const rb_fdset_t *);
void rb_fd_copy(rb_fdset_t *, const fd_set *, int);
int rb_fd_select(int, rb_fdset_t *, rb_fdset_t *, rb_fdset_t *, struct timeval *);

#define rb_fd_ptr(f)	((f)->fdset)
#define rb_fd_max(f)	((f)->maxfd)

#else

typedef fd_set rb_fdset_t;
#define rb_fd_zero(f)	FD_ZERO(f)
#define rb_fd_set(n, f)	FD_SET(n, f)
#define rb_fd_clr(n, f)	FD_CLR(n, f)
#define rb_fd_isset(n, f) FD_ISSET(n, f)
#define rb_fd_copy(d, s, n) (*(d) = *(s))
#define rb_fd_ptr(f)	(f)
#define rb_fd_init(f)	FD_ZERO(f)
#define rb_fd_term(f)	(f)
#define rb_fd_max(f)	FD_SETSIZE
#define rb_fd_select(n, rfds, wfds, efds, timeout)	select(n, rfds, wfds, efds, timeout)

#endif

RUBY_EXTERN struct RNode *ruby_current_node;
void ruby_set_current_source(void);
NORETURN(void rb_exc_raise(VALUE));
NORETURN(void rb_exc_fatal(VALUE));
VALUE rb_f_exit(int,VALUE*);
VALUE rb_f_abort(int,VALUE*);
void rb_remove_method(VALUE, const char*);
#define rb_disable_super(klass, name) ((void)0)
#define rb_enable_super(klass, name) ((void)0)
#define HAVE_RB_DEFINE_ALLOC_FUNC 1
void rb_define_alloc_func(VALUE, VALUE (*)(VALUE));
void rb_undef_alloc_func(VALUE);
void rb_clear_cache(void);
void rb_clear_cache_by_class(VALUE);
void rb_alias(VALUE, ID, ID);
void rb_attr(VALUE,ID,int,int,int);
int rb_method_boundp(VALUE, ID, int);
VALUE rb_dvar_defined(ID);
VALUE rb_dvar_curr(ID);
VALUE rb_dvar_ref(ID);
void rb_dvar_asgn(ID, VALUE);
void rb_dvar_push(ID, VALUE);
VALUE *rb_svar(int);
VALUE rb_eval_cmd(VALUE, VALUE, int);
int rb_obj_respond_to(VALUE, ID, int);
int rb_respond_to(VALUE, ID);
void rb_interrupt(void);
VALUE rb_apply(VALUE, ID, VALUE);
void rb_backtrace(void);
ID rb_frame_this_func(void);
VALUE rb_obj_instance_eval(int, VALUE*, VALUE);
VALUE rb_obj_instance_exec(int, VALUE*, VALUE);
VALUE rb_mod_module_eval(int, VALUE*, VALUE);
VALUE rb_mod_module_exec(int, VALUE*, VALUE);
void rb_load(VALUE, int);
void rb_load_protect(VALUE, int, int*);
NORETURN(void rb_jump_tag(int));
int rb_provided(const char*);
void rb_provide(const char*);
VALUE rb_f_require(VALUE, VALUE);
VALUE rb_require_safe(VALUE, int);
void rb_obj_call_init(VALUE, int, VALUE*);
VALUE rb_class_new_instance(int, VALUE*, VALUE);
VALUE rb_block_proc(void);
VALUE rb_f_lambda(void);
VALUE rb_proc_new(VALUE (*)(ANYARGS/* VALUE yieldarg[, VALUE procarg] */), VALUE);
VALUE rb_proc_call(VALUE, VALUE);
int rb_proc_arity(VALUE);
VALUE rb_obj_method(VALUE, VALUE);
VALUE rb_method_call(int, VALUE*, VALUE);
int rb_mod_method_arity(VALUE, ID);
int rb_obj_method_arity(VALUE, ID);
VALUE rb_protect(VALUE (*)(VALUE), VALUE, int*);
void rb_set_end_proc(void (*)(VALUE), VALUE);
void rb_mark_end_proc(void);
void rb_exec_end_proc(void);
void ruby_finalize(void);
NORETURN(void ruby_stop(int));
int ruby_cleanup(int);
int ruby_exec(void);
void rb_gc_mark_threads(void);
void rb_thread_start_timer(void);
void rb_thread_stop_timer(void);
void rb_thread_schedule(void);
void rb_thread_wait_fd(int);
int rb_thread_fd_writable(int);
void rb_thread_fd_close(int);
int rb_thread_alone(void);
void rb_thread_polling(void);
void rb_thread_sleep(int);
void rb_thread_sleep_forever(void);
VALUE rb_thread_stop(void);
VALUE rb_thread_wakeup(VALUE);
VALUE rb_thread_run(VALUE);
VALUE rb_thread_kill(VALUE);
VALUE rb_thread_create(VALUE (*)(ANYARGS), void*);
void rb_thread_interrupt(void);
void rb_thread_trap_eval(VALUE, int, int);
void rb_thread_signal_raise(const char*); /* should pass literal */
void rb_thread_signal_exit(void);
int rb_thread_select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
void rb_thread_wait_for(struct timeval);
VALUE rb_thread_current(void);
VALUE rb_thread_main(void);
VALUE rb_thread_local_aref(VALUE, ID);
VALUE rb_thread_local_aset(VALUE, ID, VALUE);
void rb_thread_atfork(void);
VALUE rb_exec_recursive(VALUE(*)(VALUE, VALUE, int),VALUE,VALUE);
/* file.c */
VALUE rb_file_s_expand_path(int, VALUE *);
VALUE rb_file_expand_path(VALUE, VALUE);
void rb_file_const(const char*, VALUE);
int rb_find_file_ext(VALUE*, const char* const*);
VALUE rb_find_file(VALUE);
char *rb_path_next(const char *);
char *rb_path_skip_prefix(const char *);
char *rb_path_last_separator(const char *);
char *rb_path_end(const char *);
VALUE rb_file_directory_p(VALUE,VALUE);
/* gc.c */
void ruby_set_stack_size(size_t);
NORETURN(void rb_memerror(void));
int ruby_stack_check(void);
int ruby_stack_length(VALUE**);
char *rb_source_filename(const char*);
void rb_gc_mark_locations(VALUE*, VALUE*);
void rb_mark_tbl(struct st_table*);
void rb_mark_hash(struct st_table*);
void rb_gc_mark_maybe(VALUE);
void rb_gc_mark(VALUE);
void rb_gc_force_recycle(VALUE);
void rb_gc(void);
void rb_gc_copy_finalizer(VALUE,VALUE);
void rb_gc_finalize_deferred(void);
void rb_gc_call_finalizer_at_exit(void);
VALUE rb_gc_enable(void);
VALUE rb_gc_disable(void);
VALUE rb_gc_start(void);
/* hash.c */
void st_foreach_safe(struct st_table *, int (*)(ANYARGS), st_data_t);
void rb_hash_foreach(VALUE, int (*)(ANYARGS), VALUE);
VALUE rb_hash(VALUE);
VALUE rb_hash_new(void);
VALUE rb_hash_freeze(VALUE);
VALUE rb_hash_aref(VALUE, VALUE);
VALUE rb_hash_aset(VALUE, VALUE, VALUE);
VALUE rb_hash_delete_if(VALUE);
VALUE rb_hash_delete(VALUE,VALUE);
int rb_path_check(const char*);
int rb_env_path_tainted(void);
/* io.c */
#define rb_defout rb_stdout
RUBY_EXTERN VALUE rb_fs;
RUBY_EXTERN VALUE rb_output_fs;
RUBY_EXTERN VALUE rb_rs;
RUBY_EXTERN VALUE rb_default_rs;
RUBY_EXTERN VALUE rb_output_rs;
VALUE rb_io_write(VALUE, VALUE);
VALUE rb_io_gets(VALUE);
VALUE rb_io_getc(VALUE);
VALUE rb_io_ungetc(VALUE, VALUE);
VALUE rb_io_close(VALUE);
VALUE rb_io_flush(VALUE);
VALUE rb_io_eof(VALUE);
VALUE rb_io_binmode(VALUE);
VALUE rb_io_addstr(VALUE, VALUE);
VALUE rb_io_printf(int, VALUE*, VALUE);
VALUE rb_io_print(int, VALUE*, VALUE);
VALUE rb_io_puts(int, VALUE*, VALUE);
VALUE rb_file_open(const char*, const char*);
VALUE rb_gets(void);
void rb_write_error(const char*);
void rb_write_error2(const char*, long);
/* marshal.c */
VALUE rb_marshal_dump(VALUE, VALUE);
VALUE rb_marshal_load(VALUE);
/* numeric.c */
void rb_num_zerodiv(void);
VALUE rb_num_coerce_bin(VALUE, VALUE);
VALUE rb_num_coerce_cmp(VALUE, VALUE);
VALUE rb_num_coerce_relop(VALUE, VALUE);
VALUE rb_float_new(double);
VALUE rb_num2fix(VALUE);
VALUE rb_fix2str(VALUE, int);
VALUE rb_dbl_cmp(double, double);
/* object.c */
int rb_eql(VALUE, VALUE);
VALUE rb_any_to_s(VALUE);
VALUE rb_inspect(VALUE);
VALUE rb_obj_is_instance_of(VALUE, VALUE);
VALUE rb_obj_is_kind_of(VALUE, VALUE);
VALUE rb_obj_alloc(VALUE);
VALUE rb_obj_clone(VALUE);
VALUE rb_obj_dup(VALUE);
VALUE rb_obj_init_copy(VALUE,VALUE);
VALUE rb_obj_taint(VALUE);
VALUE rb_obj_tainted(VALUE);
VALUE rb_obj_untaint(VALUE);
VALUE rb_obj_freeze(VALUE);
VALUE rb_obj_id(VALUE);
VALUE rb_obj_class(VALUE);
VALUE rb_class_real(VALUE);
VALUE rb_class_inherited_p(VALUE, VALUE);
VALUE rb_convert_type(VALUE,int,const char*,const char*);
VALUE rb_check_convert_type(VALUE,int,const char*,const char*);
VALUE rb_check_to_integer(VALUE, const char *);
VALUE rb_to_int(VALUE);
VALUE rb_Integer(VALUE);
VALUE rb_Float(VALUE);
VALUE rb_String(VALUE);
VALUE rb_Array(VALUE);
double rb_cstr_to_dbl(const char*, int);
double rb_str_to_dbl(VALUE, int);
/* parse.y */
RUBY_EXTERN int   ruby_sourceline;
RUBY_EXTERN char *ruby_sourcefile;
ID rb_id_attrset(ID);
void rb_gc_mark_parser(void);
int rb_is_const_id(ID);
int rb_is_instance_id(ID);
int rb_is_instance2_id(ID);
int rb_is_class_id(ID);
int rb_is_local_id(ID);
int rb_is_junk_id(ID);
int rb_symname_p(const char*);
void rb_gc_mark_symbols(void);
VALUE rb_backref_get(void);
void rb_backref_set(VALUE);
VALUE rb_lastline_get(void);
void rb_lastline_set(VALUE);
VALUE rb_sym_all_symbols(void);
ID rb_compose_ivar2(ID, VALUE);
ID rb_decompose_ivar2(ID, VALUE*);
/* process.c */
struct rb_exec_arg {
    int argc;
    VALUE *argv;
    const char *prog;
};
int rb_proc_exec_n(int, VALUE*, const char*);
int rb_proc_exec(const char*);
VALUE rb_check_argv(int, VALUE*);
int rb_exec(const struct rb_exec_arg*);
int rb_fork(int*, int (*)(void*), void*);
VALUE rb_f_exec(int,VALUE*);
int rb_waitpid(int,int*,int);
void rb_syswait(int);
int rb_spawn(int, VALUE*);
VALUE rb_proc_times(VALUE);
VALUE rb_detach_process(int);
/* range.c */
VALUE rb_range_new(VALUE, VALUE, int);
VALUE rb_range_beg_len(VALUE, long*, long*, long, int);
VALUE rb_length_by_each(VALUE);
/* random.c */
unsigned long genrand_int32(void);
double genrand_real(void);
/* re.c */
int rb_memcmp(const void*,const void*,long);
int rb_memcicmp(const void*,const void*,long);
long rb_memsearch(const void*,long,const void*,long);
VALUE rb_reg_nth_defined(int, VALUE);
VALUE rb_reg_nth_match(int, VALUE);
VALUE rb_reg_last_match(VALUE);
VALUE rb_reg_match_pre(VALUE);
VALUE rb_reg_match_post(VALUE);
VALUE rb_reg_match_last(VALUE);
VALUE rb_reg_new(const char*, long, int);
VALUE rb_reg_compile(const char*, long, int);
VALUE rb_reg_match(VALUE, VALUE);
VALUE rb_reg_match2(VALUE);
int rb_reg_options(VALUE);
void rb_set_kcode(const char*);
const char* rb_get_kcode(void);
/* ruby.c */
RUBY_EXTERN VALUE rb_argv;
RUBY_EXTERN VALUE rb_argv0;
void rb_load_file(const char*);
void ruby_script(const char*);
void ruby_prog_init(void);
void ruby_set_argv(int, char**);
void ruby_process_options(int, char**);
void ruby_load_script(void);
void ruby_init_loadpath(void);
void ruby_incpush(const char*);
/* signal.c */
VALUE rb_f_kill(int, VALUE*);
void rb_gc_mark_trap_list(void);
#ifdef POSIX_SIGNAL
#define posix_signal ruby_posix_signal
void posix_signal(int, RETSIGTYPE (*)(int));
#ifdef HAVE_NATIVETHREAD
#define posix_nativethread_signal ruby_posix_nativethread_signal
void posix_nativethread_signal(int, RETSIGTYPE (*)(int));
#endif
#endif
void rb_trap_exit(void);
void rb_trap_exec(void);
const char *ruby_signal_name(int);
/* sprintf.c */
VALUE rb_f_sprintf(int, const VALUE*);
PRINTF_ARGS(VALUE rb_sprintf(const char*, ...), 1, 2);
VALUE rb_vsprintf(const char*, va_list);
VALUE rb_str_format(int, const VALUE *, VALUE);
/* string.c */
VALUE rb_str_new(const char*, long);
VALUE rb_str_new2(const char*);
VALUE rb_str_new3(VALUE);
VALUE rb_str_new4(VALUE);
VALUE rb_str_new5(VALUE, const char*, long);
VALUE rb_tainted_str_new(const char*, long);
VALUE rb_tainted_str_new2(const char*);
VALUE rb_str_buf_new(long);
VALUE rb_str_buf_new2(const char*);
void rb_str_free(VALUE);
VALUE rb_str_buf_append(VALUE, VALUE);
VALUE rb_str_buf_cat(VALUE, const char*, long);
VALUE rb_str_buf_cat2(VALUE, const char*);
VALUE rb_obj_as_string(VALUE);
VALUE rb_check_string_type(VALUE);
VALUE rb_str_dup(VALUE);
VALUE rb_str_locktmp(VALUE);
VALUE rb_str_unlocktmp(VALUE);
VALUE rb_str_dup_frozen(VALUE);
VALUE rb_str_plus(VALUE, VALUE);
VALUE rb_str_times(VALUE, VALUE);
VALUE rb_str_substr(VALUE, long, long);
void rb_str_modify(VALUE);
VALUE rb_str_freeze(VALUE);
void rb_str_set_len(VALUE, long);
VALUE rb_str_resize(VALUE, long);
VALUE rb_str_cat(VALUE, const char*, long);
VALUE rb_str_cat2(VALUE, const char*);
VALUE rb_str_append(VALUE, VALUE);
VALUE rb_str_concat(VALUE, VALUE);
int rb_memhash(const void *ptr, long len);
int rb_str_hash(VALUE);
int rb_str_cmp(VALUE, VALUE);
VALUE rb_str_upto(VALUE, VALUE, int);
void rb_str_update(VALUE, long, long, VALUE);
VALUE rb_str_inspect(VALUE);
VALUE rb_str_dump(VALUE);
VALUE rb_str_split(VALUE, const char*);
void rb_str_associate(VALUE, VALUE);
VALUE rb_str_associated(VALUE);
void rb_str_setter(VALUE, ID, VALUE*);
VALUE rb_str_intern(VALUE);
VALUE rb_sym_to_s(VALUE);
/* struct.c */
VALUE rb_struct_new(VALUE, ...);
VALUE rb_struct_define(const char*, ...);
VALUE rb_struct_alloc(VALUE, VALUE);
VALUE rb_struct_aref(VALUE, VALUE);
VALUE rb_struct_aset(VALUE, VALUE, VALUE);
VALUE rb_struct_getmember(VALUE, ID);
VALUE rb_struct_iv_get(VALUE, const char*);
VALUE rb_struct_s_members(VALUE);
VALUE rb_struct_members(VALUE);
/* time.c */
VALUE rb_time_new(time_t, time_t);
/* variable.c */
VALUE rb_mod_name(VALUE);
VALUE rb_class_path(VALUE);
void rb_set_class_path(VALUE, VALUE, const char*);
VALUE rb_path2class(const char*);
void rb_name_class(VALUE, ID);
VALUE rb_class_name(VALUE);
void rb_autoload(VALUE, ID, const char*);
VALUE rb_autoload_load(VALUE, ID);
VALUE rb_autoload_p(VALUE, ID);
void rb_gc_mark_global_tbl(void);
VALUE rb_f_trace_var(int, VALUE*);
VALUE rb_f_untrace_var(int, VALUE*);
VALUE rb_f_global_variables(void);
void rb_alias_variable(ID, ID);
struct st_table* rb_generic_ivar_table(VALUE);
void rb_copy_generic_ivar(VALUE,VALUE);
void rb_mark_generic_ivar(VALUE);
void rb_mark_generic_ivar_tbl(void);
void rb_free_generic_ivar(VALUE);
VALUE rb_ivar_get(VALUE, ID);
VALUE rb_ivar_set(VALUE, ID, VALUE);
VALUE rb_ivar_defined(VALUE, ID);
VALUE rb_iv_set(VALUE, const char*, VALUE);
VALUE rb_iv_get(VALUE, const char*);
VALUE rb_attr_get(VALUE, ID);
VALUE rb_obj_instance_variables(VALUE);
VALUE rb_obj_remove_instance_variable(VALUE, VALUE);
void *rb_mod_const_at(VALUE, void*);
void *rb_mod_const_of(VALUE, void*);
VALUE rb_const_list(void*);
VALUE rb_mod_constants(int, VALUE *, VALUE);
VALUE rb_mod_remove_const(VALUE, VALUE);
int rb_const_defined(VALUE, ID);
int rb_const_defined_at(VALUE, ID);
int rb_const_defined_from(VALUE, ID);
int rb_const_defined_fallback(VALUE, ID, struct RNode *);
VALUE rb_const_get(VALUE, ID);
VALUE rb_const_get_at(VALUE, ID);
VALUE rb_const_get_from(VALUE, ID);
VALUE rb_const_get_fallback(VALUE, ID, struct RNode *);
void rb_const_set(VALUE, ID, VALUE);
VALUE rb_mod_const_missing(VALUE,VALUE);
void rb_cvar_set(VALUE, ID, VALUE);
VALUE rb_cvar_defined(VALUE, ID);
void rb_cvar_set(VALUE, ID, VALUE);
VALUE rb_cvar_get(VALUE, ID);
void rb_cv_set(VALUE, const char*, VALUE);
VALUE rb_cv_get(VALUE, const char*);
void rb_define_class_variable(VALUE, const char*, VALUE);
VALUE rb_mod_class_variables(VALUE);
VALUE rb_mod_remove_cvar(VALUE, VALUE);
/* version.c */
void ruby_show_version(void);
void ruby_show_copyright(void);

#endif /* RUBY_INTERN_H */