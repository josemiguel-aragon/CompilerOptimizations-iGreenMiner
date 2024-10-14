/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* atax.c: this file is part of PolyBench/C */

/* Include polybench common header. */
/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* polybench.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <math.h>


/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/*
 * polybench.h: this file is part of PolyBench/C
 *
 * Polybench header for instrumentation.
 *
 * Programs must be compiled with `-I utilities utilities/polybench.c'
 *
 * Optionally, one can define:
 *
 * -DPOLYBENCH_TIME, to report the execution time,
 *   OR (exclusive):
 * -DPOLYBENCH_PAPI, to use PAPI H/W counters (defined in polybench.c)
 *
 *
 * See README or utilities/polybench.c for additional options.
 *
 */
#ifndef POLYBENCH_H
# define POLYBENCH_H

# include <stdlib.h>

/* Array padding. By default, none is used. */
# ifndef POLYBENCH_PADDING_FACTOR
/* default: */
#  define POLYBENCH_PADDING_FACTOR 0
# endif

/* Inter-array padding, for use with . By default, none is used. */
# ifndef POLYBENCH_INTER_ARRAY_PADDING_FACTOR
/* default: */
#  define POLYBENCH_INTER_ARRAY_PADDING_FACTOR 0
#  undef POLYBENCH_ENABLE_INTARRAY_PAD
# else
#  define POLYBENCH_ENABLE_INTARRAY_PAD
# endif


/* C99 arrays in function prototype. By default, do not use. */
# ifdef POLYBENCH_USE_C99_PROTO
#  define POLYBENCH_C99_SELECT(x,y) y
# else
/* default: */
#  define POLYBENCH_C99_SELECT(x,y) x
# endif


/* Scalar loop bounds in SCoPs. By default, use parametric loop bounds. */
# ifdef POLYBENCH_USE_SCALAR_LB
#  define POLYBENCH_LOOP_BOUND(x,y) x
# else
/* default: */
#  define POLYBENCH_LOOP_BOUND(x,y) y
# endif

/* Use the 'restrict' keyword to declare that the different arrays do not
 * alias. By default, we do not use it as it is only supported in C99 and
 * even here several compilers do not properly get it.
 */
# ifdef POLYBENCH_USE_RESTRICT
#  define POLYBENCH_RESTRICT restrict
# else
/* default: */
#  define POLYBENCH_RESTRICT
# endif

/* Macros to reference an array. Generic for heap and stack arrays
   (C99).  Each array dimensionality has his own macro, to be used at
   declaration or as a function argument.
   Example:
   int b[x] => POLYBENCH_1D_ARRAY(b, x)
   int A[N][N] => POLYBENCH_2D_ARRAY(A, N, N)
*/
# ifndef POLYBENCH_STACK_ARRAYS
#  define POLYBENCH_ARRAY(x) *x
#  ifdef POLYBENCH_ENABLE_INTARRAY_PAD
#   define POLYBENCH_FREE_ARRAY(x) polybench_free_data((void*)x);
#  else
#   define POLYBENCH_FREE_ARRAY(x) free((void*)x);
#  endif
#  define POLYBENCH_DECL_VAR(x) (*x)
# else
#  define POLYBENCH_ARRAY(x) x
#  define POLYBENCH_FREE_ARRAY(x)
#  define POLYBENCH_DECL_VAR(x) x
# endif
/* Macros for using arrays in the function prototypes. */
# define POLYBENCH_1D(var, dim1,ddim1) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_2D(var, dim1, dim2, ddim1, ddim2) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_3D(var, dim1, dim2, dim3, ddim1, ddim2, ddim3) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_4D(var, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_5D(var, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim5,ddim5) + POLYBENCH_PADDING_FACTOR]
/* Macros for using arrays within the functions. */
# define POLYBENCH_1D_F(var, dim1,ddim1) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_2D_F(var, dim1, dim2, ddim1, ddim2) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_3D_F(var, dim1, dim2, dim3, ddim1, ddim2, ddim3) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_4D_F(var, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_5D_F(var, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim5,ddim5) + POLYBENCH_PADDING_FACTOR]


/* Macros to allocate heap arrays.
   Example:
   polybench_alloc_2d_array(N, M, double) => allocates N x M x sizeof(double)
					  and returns a pointer to the 2d array
 */
# define POLYBENCH_ALLOC_1D_ARRAY(n1, type)	\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data (n1 + POLYBENCH_PADDING_FACTOR, sizeof(type))
# define POLYBENCH_ALLOC_2D_ARRAY(n1, n2, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR), sizeof(type))
# define POLYBENCH_ALLOC_3D_ARRAY(n1, n2, n3, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR][n3 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR) * (n3 + POLYBENCH_PADDING_FACTOR), sizeof(type))
# define POLYBENCH_ALLOC_4D_ARRAY(n1, n2, n3, n4, type)			\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR][n3 + POLYBENCH_PADDING_FACTOR][n4 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR) * (n3 + POLYBENCH_PADDING_FACTOR) * (n4 + POLYBENCH_PADDING_FACTOR), sizeof(type))
# define POLYBENCH_ALLOC_5D_ARRAY(n1, n2, n3, n4, n5, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR][n3 + POLYBENCH_PADDING_FACTOR][n4 + POLYBENCH_PADDING_FACTOR][n5 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR) * (n3 + POLYBENCH_PADDING_FACTOR) * (n4 + POLYBENCH_PADDING_FACTOR) * (n5 + POLYBENCH_PADDING_FACTOR), sizeof(type))

/* Macros for array declaration. */
# ifndef POLYBENCH_STACK_ARRAYS
#  define POLYBENCH_1D_ARRAY_DECL(var, type, dim1, ddim1)		\
  type POLYBENCH_1D_F(POLYBENCH_DECL_VAR(var), dim1, ddim1); \
  var = POLYBENCH_ALLOC_1D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), type);
#  define POLYBENCH_2D_ARRAY_DECL(var, type, dim1, dim2, ddim1, ddim2)	\
  type POLYBENCH_2D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, ddim1, ddim2); \
  var = POLYBENCH_ALLOC_2D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), type);
#  define POLYBENCH_3D_ARRAY_DECL(var, type, dim1, dim2, dim3, ddim1, ddim2, ddim3) \
  type POLYBENCH_3D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, ddim1, ddim2, ddim3); \
  var = POLYBENCH_ALLOC_3D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), POLYBENCH_C99_SELECT(dim3, ddim3), type);
#  define POLYBENCH_4D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) \
  type POLYBENCH_4D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4); \
  var = POLYBENCH_ALLOC_4D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), POLYBENCH_C99_SELECT(dim3, ddim3), POLYBENCH_C99_SELECT(dim4, ddim4), type);
#  define POLYBENCH_5D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) \
  type POLYBENCH_5D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5); \
  var = POLYBENCH_ALLOC_5D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), POLYBENCH_C99_SELECT(dim3, ddim3), POLYBENCH_C99_SELECT(dim4, ddim4), POLYBENCH_C99_SELECT(dim5, ddim5), type);
# else
#  define POLYBENCH_1D_ARRAY_DECL(var, type, dim1, ddim1)		\
  type POLYBENCH_1D_F(POLYBENCH_DECL_VAR(var), dim1, ddim1);
#  define POLYBENCH_2D_ARRAY_DECL(var, type, dim1, dim2, ddim1, ddim2)	\
  type POLYBENCH_2D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, ddim1, ddim2);
#  define POLYBENCH_3D_ARRAY_DECL(var, type, dim1, dim2, dim3, ddim1, ddim2, ddim3) \
  type POLYBENCH_3D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, ddim1, ddim2, ddim3);
#  define POLYBENCH_4D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) \
  type POLYBENCH_4D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4);
#  define POLYBENCH_5D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) \
  type POLYBENCH_5D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5);
# endif


/* Dead-code elimination macros. Use argc/argv for the run-time check. */
# ifndef POLYBENCH_DUMP_ARRAYS
#  define POLYBENCH_DCE_ONLY_CODE    if (argc > 42 && ! strcmp(argv[0], ""))
# else
#  define POLYBENCH_DCE_ONLY_CODE
# endif

#define POLYBENCH_DUMP_TARGET stderr
#define POLYBENCH_DUMP_START    fprintf(POLYBENCH_DUMP_TARGET, "==BEGIN DUMP_ARRAYS==\n")
#define POLYBENCH_DUMP_FINISH   fprintf(POLYBENCH_DUMP_TARGET, "==END   DUMP_ARRAYS==\n")
#define POLYBENCH_DUMP_BEGIN(s) fprintf(POLYBENCH_DUMP_TARGET, "begin dump: %s", s)
#define POLYBENCH_DUMP_END(s)   fprintf(POLYBENCH_DUMP_TARGET, "\nend   dump: %s\n", s)

# define polybench_prevent_dce(func)		\
  POLYBENCH_DCE_ONLY_CODE			\
  func


/* Performance-related instrumentation. See polybench.c */
# define polybench_start_instruments
# define polybench_stop_instruments
# define polybench_print_instruments

/* By default, collect PAPI counters on thread 0. */
#ifndef POLYBENCH_THREAD_MONITOR
# define POLYBENCH_THREAD_MONITOR 0
#endif

/* Total LLC cache size. By default 32+MB.. */
#ifndef POLYBENCH_CACHE_SIZE_KB
# define POLYBENCH_CACHE_SIZE_KB 32770
#endif


int polybench_papi_counters_threadid = POLYBENCH_THREAD_MONITOR;
double polybench_program_total_flops = 0;


/*
 * Allocation table, to enable inter-array padding. All data allocated
 * with polybench_alloc_data should be freed with polybench_free_data.
 *
 */
#define NB_INITIAL_TABLE_ENTRIES 512
struct polybench_data_ptrs
{
  void** user_view;
  void** real_ptr;
  int nb_entries;
  int nb_avail_entries;
};
static struct polybench_data_ptrs* _polybench_alloc_table = NULL;
static size_t polybench_inter_array_padding_sz = 0;

/* Timer code (gettimeofday). */
double polybench_t_start, polybench_t_end;
/* Timer code (RDTSC). */
unsigned long long int polybench_c_start, polybench_c_end;

static
double rtclock()
{
    return 0;
}



void polybench_flush_cache()
{
  int cs = POLYBENCH_CACHE_SIZE_KB * 1024 / sizeof(double);
  double* flush = (double*) calloc (cs, sizeof(double));
  int i;
  double tmp = 0.0;
  for (i = 0; i < cs; i++)
    tmp += flush[i];
  assert (tmp <= 10.0);
  free (flush);
}



void polybench_prepare_instruments()
{
  polybench_flush_cache ();
}


void polybench_timer_start()
{
  polybench_prepare_instruments ();
  polybench_t_start = rtclock ();
}


void polybench_timer_stop()
{
  polybench_t_end = rtclock ();
}


void polybench_timer_print()
{
      printf ("%0.6f\n", polybench_t_end - polybench_t_start);
}


static
void*
xmalloc(size_t alloc_sz)
{
  void* ret = NULL;
  /* By default, post-pad the arrays. Safe behavior, but likely useless. */
  polybench_inter_array_padding_sz += POLYBENCH_INTER_ARRAY_PADDING_FACTOR;
  size_t padded_sz = alloc_sz + polybench_inter_array_padding_sz;
  int err = posix_memalign (&ret, 4096, padded_sz);
  if (! ret || err)
    {
      fprintf (stderr, "[PolyBench] posix_memalign: cannot allocate memory");
      exit (1);
    }
  /* Safeguard: this is invoked only if polybench.c has been compiled
     with inter-array padding support from polybench.h. If so, move
     the starting address of the allocation and return it to the
     user. The original pointer is registered in an allocation table
     internal to polybench.c. Data must then be freed using
     polybench_free_data, which will inspect the allocation table to
     free the original pointer.*/

  return ret;
}


void polybench_free_data(void* ptr)
{
  free (ptr);
}


void* polybench_alloc_data(unsigned long long int n, int elt_size)
{

  /// FIXME: detect overflow!
  size_t val = n;
  val *= elt_size;
  void* ret = xmalloc (val);

  return ret;
}

#endif


/* Include benchmark-specific header. */
/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _ATAX_H
# define _ATAX_H


#define M_ATAX 38
#define N_ATAX 42


# define _PB_M POLYBENCH_LOOP_BOUND(M_ATAX,m)
# define _PB_N POLYBENCH_LOOP_BOUND(N_ATAX,n)


/* Default data type */
#define DATA_TYPE_IS_DOUBLE

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_ATAX_H */


/* Array initialization. */
static
void init_array (int m, int n,
		 DATA_TYPE POLYBENCH_2D(A,M_ATAX,N_ATAX,m,n),
		 DATA_TYPE POLYBENCH_1D(x,N_ATAX,n))
{
  int i, j;
  DATA_TYPE fn;
  fn = (DATA_TYPE)n;

  for (i = 0; i < n; i++)
      x[i] = 1 + (i / fn);
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      A[i][j] = (DATA_TYPE) ((i+j) % n) / (5*m);
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE POLYBENCH_1D(y,N_ATAX,n))

{
  int i;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("y");
  for (i = 0; i < n; i++) {
    if (i % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
    fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, y[i]);
  }
  POLYBENCH_DUMP_END("y");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_atax(int m, int n,
		 DATA_TYPE POLYBENCH_2D(A,M_ATAX,N_ATAX,m,n),
		 DATA_TYPE POLYBENCH_1D(x,N_ATAX,n),
		 DATA_TYPE POLYBENCH_1D(y,N_ATAX,n),
		 DATA_TYPE POLYBENCH_1D(tmp,M_ATAX,m))
{
  int i, j;

#pragma scop
  for (i = 0; i < _PB_N; i++)
    y[i] = 0;
  for (i = 0; i < _PB_M; i++)
    {
      tmp[i] = SCALAR_VAL(0.0);
      for (j = 0; j < _PB_N; j++)
	tmp[i] = tmp[i] + A[i][j] * x[j];
      for (j = 0; j < _PB_N; j++)
	y[j] = y[j] + A[i][j] * tmp[i];
    }
#pragma endscop

}


int main_atax()
{
  /* Retrieve problem size. */
  int m = M_ATAX;
  int n = N_ATAX;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, M_ATAX, N_ATAX, m, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N_ATAX, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N_ATAX, n);
  POLYBENCH_1D_ARRAY_DECL(tmp, DATA_TYPE, M_ATAX, m);

  /* Initialize array(s). */
  init_array (m, n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(x));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_atax (m, n,
	       POLYBENCH_ARRAY(A),
	       POLYBENCH_ARRAY(x),
	       POLYBENCH_ARRAY(y),
	       POLYBENCH_ARRAY(tmp));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(y)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);
  POLYBENCH_FREE_ARRAY(tmp);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _CORRELATION_H
# define _CORRELATION_H


#define M_correlation 28
#define N_correlation 32

# define _PB_M_correlation POLYBENCH_LOOP_BOUND(M_correlation,m)
# define _PB_N_correlation POLYBENCH_LOOP_BOUND(N_correlation,n)

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_CORRELATION_H */



/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* correlation.c: this file is part of PolyBench/C */

/* Array initialization. */
static
void init_array_correlation (int m,
		 int n,
		 DATA_TYPE *float_n,
		 DATA_TYPE POLYBENCH_2D(data,N_correlation,M_correlation,n,m))
{
  int i, j;

  *float_n = (DATA_TYPE)N_correlation;

  for (i = 0; i < N_correlation; i++)
    for (j = 0; j < M_correlation; j++)
      data[i][j] = (DATA_TYPE)(i*j)/M_correlation + i;

}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array_correlation(int m,
		 DATA_TYPE POLYBENCH_2D(corr,M_correlation,M_correlation,m,m))

{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("corr");
  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++) {
      if ((i * m + j) % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
      fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, corr[i][j]);
    }
  POLYBENCH_DUMP_END("corr");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_correlation(int m, int n,
			DATA_TYPE float_n,
			DATA_TYPE POLYBENCH_2D(data,N_correlation,M_correlation,n,m),
			DATA_TYPE POLYBENCH_2D(corr,M_correlation,M_correlation,m,m),
			DATA_TYPE POLYBENCH_1D(mean,M_correlation,m),
			DATA_TYPE POLYBENCH_1D(stddev,M_correlation,m))
{
  int i, j, k;

  DATA_TYPE eps = SCALAR_VAL(0.1);


#pragma scop
  for (j = 0; j < _PB_M_correlation; j++)
    {
      mean[j] = SCALAR_VAL(0.0);
      for (i = 0; i < _PB_N_correlation; i++)
	mean[j] += data[i][j];
      mean[j] /= float_n;
    }


   for (j = 0; j < _PB_M_correlation; j++)
    {
      stddev[j] = SCALAR_VAL(0.0);
      for (i = 0; i < _PB_N_correlation; i++)
        stddev[j] += (data[i][j] - mean[j]) * (data[i][j] - mean[j]);
      stddev[j] /= float_n;
      stddev[j] = SQRT_FUN(stddev[j]);
      /* The following in an inelegant but usual way to handle
         near-zero std. dev. values, which below would cause a zero-
         divide. */
      stddev[j] = stddev[j] <= eps ? SCALAR_VAL(1.0) : stddev[j];
    }

  /* Center and reduce the column vectors. */
  for (i = 0; i < _PB_N_correlation; i++)
    for (j = 0; j < _PB_M_correlation; j++)
      {
        data[i][j] -= mean[j];
        data[i][j] /= SQRT_FUN(float_n) * stddev[j];
      }

  /* Calculate the m * m correlation matrix. */
  for (i = 0; i < _PB_M_correlation-1; i++)
    {
      corr[i][i] = SCALAR_VAL(1.0);
      for (j = i+1; j < _PB_M_correlation; j++)
        {
          corr[i][j] = SCALAR_VAL(0.0);
          for (k = 0; k < _PB_N_correlation; k++)
            corr[i][j] += (data[k][i] * data[k][j]);
          corr[j][i] = corr[i][j];
        }
    }
  corr[_PB_M_correlation-1][_PB_M_correlation-1] = SCALAR_VAL(1.0);
#pragma endscop

}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* 2mm.c: this file is part of PolyBench/C */

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _2MM_H
# define _2MM_H


/* Define sample dataset sizes. */
#define NI_2mm 16
#define NJ_2mm 18
#define NK_2mm 22
#define NL_2mm 24



#endif /* !(NI NJ NK NL) */

# define _PB_NI_2mm POLYBENCH_LOOP_BOUND(NI_2mm,ni)
# define _PB_NJ_2mm POLYBENCH_LOOP_BOUND(NJ_2mm,nj)
# define _PB_NK_2mm POLYBENCH_LOOP_BOUND(NK_2mm,nk)
# define _PB_NL_2mm POLYBENCH_LOOP_BOUND(NL_2mm,nl)



#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif


/* Array initialization. */
static
void init_array_2mm(int ni, int nj, int nk, int nl,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(A,NI_2mm,NK_2mm,ni,nk),
		DATA_TYPE POLYBENCH_2D(B,NK_2mm,NJ_2mm,nk,nj),
		DATA_TYPE POLYBENCH_2D(C,NJ_2mm,NL_2mm,nj,nl),
		DATA_TYPE POLYBENCH_2D(D,NI_2mm,NL_2mm,ni,nl))
{
  int i, j;

  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i][j] = (DATA_TYPE) ((i*j+1) % ni) / ni;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i][j] = (DATA_TYPE) (i*(j+1) % nj) / nj;
  for (i = 0; i < nj; i++)
    for (j = 0; j < nl; j++)
      C[i][j] = (DATA_TYPE) ((i*(j+3)+1) % nl) / nl;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++)
      D[i][j] = (DATA_TYPE) (i*(j+2) % nk) / nk;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array_2mm(int ni, int nl,
		 DATA_TYPE POLYBENCH_2D(D,NI_2mm,NL_2mm,ni,nl))
{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("D");
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++) {
	if ((i * ni + j) % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
	fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, D[i][j]);
    }
  POLYBENCH_DUMP_END("D");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_2mm(int ni, int nj, int nk, int nl,
		DATA_TYPE alpha,
		DATA_TYPE beta,
		DATA_TYPE POLYBENCH_2D(tmp,NI_2mm,NJ_2mm,ni,nj),
		DATA_TYPE POLYBENCH_2D(A,NI_2mm,NK_2mm,ni,nk),
		DATA_TYPE POLYBENCH_2D(B,NK_2mm,NJ_2mm,nk,nj),
		DATA_TYPE POLYBENCH_2D(C,NJ_2mm,NL_2mm,nj,nl),
		DATA_TYPE POLYBENCH_2D(D,NI_2mm,NL_2mm,ni,nl))
{
  int i, j, k;

#pragma scop
  /* D := alpha*A*B*C + beta*D */
  for (i = 0; i < _PB_NI_2mm; i++)
    for (j = 0; j < _PB_NJ_2mm; j++)
      {
	tmp[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NK_2mm; ++k)
	  tmp[i][j] += alpha * A[i][k] * B[k][j];
      }
  for (i = 0; i < _PB_NI_2mm; i++)
    for (j = 0; j < _PB_NL_2mm; j++)
      {
	D[i][j] *= beta;
	for (k = 0; k < _PB_NJ_2mm; ++k)
	  D[i][j] += tmp[i][k] * C[k][j];
      }
#pragma endscop

}


int main_2mm()
{
  /* Retrieve problem size. */
  int ni = NI_2mm;
  int nj = NJ_2mm;
  int nk = NK_2mm;
  int nl = NL_2mm;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(tmp,DATA_TYPE,NI_2mm,NJ_2mm,ni,nj);
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,NI_2mm,NK_2mm,ni,nk);
  POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,NK_2mm,NJ_2mm,nk,nj);
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,NJ_2mm,NL_2mm,nj,nl);
  POLYBENCH_2D_ARRAY_DECL(D,DATA_TYPE,NI_2mm,NL_2mm,ni,nl);

  /* Initialize array(s). */
  init_array_2mm (ni, nj, nk, nl, &alpha, &beta,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_2mm (ni, nj, nk, nl,
	      alpha, beta,
	      POLYBENCH_ARRAY(tmp),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(ni, nl,  POLYBENCH_ARRAY(D)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(tmp);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(D);

  return 0;
}


int main_correlation()
{
  /* Retrieve problem size. */
  int n = N_correlation;
  int m = M_correlation;

  /* Variable declaration/allocation. */
  DATA_TYPE float_n;
  POLYBENCH_2D_ARRAY_DECL(data,DATA_TYPE,N_correlation,M_correlation,n,m);
  POLYBENCH_2D_ARRAY_DECL(corr,DATA_TYPE,M_correlation,M_correlation,m,m);
  POLYBENCH_1D_ARRAY_DECL(mean,DATA_TYPE,M_correlation,m);
  POLYBENCH_1D_ARRAY_DECL(stddev,DATA_TYPE,M_correlation,m);

  /* Initialize array(s). */
  init_array_correlation (m, n, &float_n, POLYBENCH_ARRAY(data));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_correlation (m, n, float_n,
		      POLYBENCH_ARRAY(data),
		      POLYBENCH_ARRAY(corr),
		      POLYBENCH_ARRAY(mean),
		      POLYBENCH_ARRAY(stddev));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(m, POLYBENCH_ARRAY(corr)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(data);
  POLYBENCH_FREE_ARRAY(corr);
  POLYBENCH_FREE_ARRAY(mean);
  POLYBENCH_FREE_ARRAY(stddev);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */


/* Define sample dataset sizes. */
#define NI_3mm 16
#define NJ_3mm 18
#define NK_3mm 20
#define NL_3mm 22
#define NM_3mm 24


# define _PB_NI_3mm POLYBENCH_LOOP_BOUND(NI_3mm,ni)
# define _PB_NJ_3mm POLYBENCH_LOOP_BOUND(NJ_3mm,nj)
# define _PB_NK_3mm POLYBENCH_LOOP_BOUND(NK_3mm,nk)
# define _PB_NL_3mm POLYBENCH_LOOP_BOUND(NL_3mm,nl)
# define _PB_NM_3mm POLYBENCH_LOOP_BOUND(NM_3mm,nm)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif




/* Array initialization. */
static
void init_array_3mm(int ni, int nj, int nk, int nl, int nm,
		DATA_TYPE POLYBENCH_2D(A,NI_3mm,NK_3mm,ni,nk),
		DATA_TYPE POLYBENCH_2D(B,NK_3mm,NJ_3mm,nk,nj),
		DATA_TYPE POLYBENCH_2D(C,NJ_3mm,NM_3mm,nj,nm),
		DATA_TYPE POLYBENCH_2D(D,NM_3mm,NL_3mm,nm,nl))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i][j] = (DATA_TYPE) ((i*j+1) % ni) / (5*ni);
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i][j] = (DATA_TYPE) ((i*(j+1)+2) % nj) / (5*nj);
  for (i = 0; i < nj; i++)
    for (j = 0; j < nm; j++)
      C[i][j] = (DATA_TYPE) (i*(j+3) % nl) / (5*nl);
  for (i = 0; i < nm; i++)
    for (j = 0; j < nl; j++)
      D[i][j] = (DATA_TYPE) ((i*(j+2)+2) % nk) / (5*nk);
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array_3mm(int ni, int nl,
		 DATA_TYPE POLYBENCH_2D(G,NI_3mm,NL_3mm,ni,nl))
{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("G");
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++) {
	if ((i * ni + j) % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
	fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, G[i][j]);
    }
  POLYBENCH_DUMP_END("G");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_3mm(int ni, int nj, int nk, int nl, int nm,
		DATA_TYPE POLYBENCH_2D(E,NI_3mm,NJ_3mm,ni,nj),
		DATA_TYPE POLYBENCH_2D(A,NI_3mm,NK_3mm,ni,nk),
		DATA_TYPE POLYBENCH_2D(B,NK_3mm,NJ_3mm,nk,nj),
		DATA_TYPE POLYBENCH_2D(F,NJ_3mm,NL_3mm,nj,nl),
		DATA_TYPE POLYBENCH_2D(C,NJ_3mm,NM_3mm,nj,nm),
		DATA_TYPE POLYBENCH_2D(D,NM_3mm,NL_3mm,nm,nl),
		DATA_TYPE POLYBENCH_2D(G,NI_3mm,NL_3mm,ni,nl))
{
  int i, j, k;

#pragma scop
  /* E := A*B */
  for (i = 0; i < _PB_NI_3mm; i++)
    for (j = 0; j < _PB_NJ_3mm; j++)
      {
	E[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NK_3mm; ++k)
	  E[i][j] += A[i][k] * B[k][j];
      }
  /* F := C*D */
  for (i = 0; i < _PB_NJ_3mm; i++)
    for (j = 0; j < _PB_NL_3mm; j++)
      {
	F[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NM_3mm; ++k)
	  F[i][j] += C[i][k] * D[k][j];
      }
  /* G := E*F */
  for (i = 0; i < _PB_NI_3mm; i++)
    for (j = 0; j < _PB_NL_3mm; j++)
      {
	G[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NJ_3mm; ++k)
	  G[i][j] += E[i][k] * F[k][j];
      }
#pragma endscop

}


int main_3mm()
{
  /* Retrieve problem size. */
  int ni = NI_3mm;
  int nj = NJ_3mm;
  int nk = NK_3mm;
  int nl = NL_3mm;
  int nm = NM_3mm;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(E, DATA_TYPE, NI_3mm, NJ_3mm, ni, nj);
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, NI_3mm, NK_3mm, ni, nk);
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, NK_3mm, NJ_3mm, nk, nj);
  POLYBENCH_2D_ARRAY_DECL(F, DATA_TYPE, NJ_3mm, NL_3mm, nj, nl);
  POLYBENCH_2D_ARRAY_DECL(C, DATA_TYPE, NJ_3mm, NM_3mm, nj, nm);
  POLYBENCH_2D_ARRAY_DECL(D, DATA_TYPE, NM_3mm, NL_3mm, nm, nl);
  POLYBENCH_2D_ARRAY_DECL(G, DATA_TYPE, NI_3mm, NL_3mm, ni, nl);

  /* Initialize array(s). */
  init_array_3mm (ni, nj, nk, nl, nm,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_3mm (ni, nj, nk, nl, nm,
	      POLYBENCH_ARRAY(E),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(F),
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(D),
	      POLYBENCH_ARRAY(G));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(ni, nl,  POLYBENCH_ARRAY(G)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(E);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);
  POLYBENCH_FREE_ARRAY(F);
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(D);
  POLYBENCH_FREE_ARRAY(G);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* bicg.c: this file is part of PolyBench/C */

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _BICG_H
# define _BICG_H


#define M_bicg 38
#define N_bicg 42

# define _PB_M_bicg POLYBENCH_LOOP_BOUND(M_bicg,m)
# define _PB_N_bicg POLYBENCH_LOOP_BOUND(N_bicg,n)

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_BICG_H */



/* Array initialization. */
static
void init_array_bicg (int m, int n,
		 DATA_TYPE POLYBENCH_2D(A,N_bicg,M_bicg,n,m),
		 DATA_TYPE POLYBENCH_1D(r,N_bicg,n),
		 DATA_TYPE POLYBENCH_1D(p,M_bicg,m))
{
  int i, j;

  for (i = 0; i < m; i++)
    p[i] = (DATA_TYPE)(i % m) / m;
  for (i = 0; i < n; i++) {
    r[i] = (DATA_TYPE)(i % n) / n;
    for (j = 0; j < m; j++)
      A[i][j] = (DATA_TYPE) (i*(j+1) % n)/n;
  }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array_bicg(int m, int n,
		 DATA_TYPE POLYBENCH_1D(s,M_bicg,m),
		 DATA_TYPE POLYBENCH_1D(q,N_bicg,n))

{
  int i;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("s");
  for (i = 0; i < m; i++) {
    if (i % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
    fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, s[i]);
  }
  POLYBENCH_DUMP_END("s");
  POLYBENCH_DUMP_BEGIN("q");
  for (i = 0; i < n; i++) {
    if (i % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
    fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, q[i]);
  }
  POLYBENCH_DUMP_END("q");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_bicg(int m, int n,
		 DATA_TYPE POLYBENCH_2D(A,N_bicg,M_bicg,n,m),
		 DATA_TYPE POLYBENCH_1D(s,M_bicg,m),
		 DATA_TYPE POLYBENCH_1D(q,N_bicg,n),
		 DATA_TYPE POLYBENCH_1D(p,M_bicg,m),
		 DATA_TYPE POLYBENCH_1D(r,N_bicg,n))
{
  int i, j;

#pragma scop
  for (i = 0; i < _PB_M_bicg; i++)
    s[i] = 0;
  for (i = 0; i < _PB_N_bicg; i++)
    {
      q[i] = SCALAR_VAL(0.0);
      for (j = 0; j < _PB_M_bicg; j++)
	{
	  s[j] = s[j] + r[i] * A[i][j];
	  q[i] = q[i] + A[i][j] * p[j];
	}
    }
#pragma endscop

}


int main_bicg()
{
  /* Retrieve problem size. */
  int n = N_bicg;
  int m = M_bicg;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N_bicg, M_bicg, n, m);
  POLYBENCH_1D_ARRAY_DECL(s, DATA_TYPE, M_bicg, m);
  POLYBENCH_1D_ARRAY_DECL(q, DATA_TYPE, N_bicg, n);
  POLYBENCH_1D_ARRAY_DECL(p, DATA_TYPE, M_bicg, m);
  POLYBENCH_1D_ARRAY_DECL(r, DATA_TYPE, N_bicg, n);

  /* Initialize array(s). */
  init_array_bicg (m, n,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(r),
	      POLYBENCH_ARRAY(p));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_bicg (m, n,
	       POLYBENCH_ARRAY(A),
	       POLYBENCH_ARRAY(s),
	       POLYBENCH_ARRAY(q),
	       POLYBENCH_ARRAY(p),
	       POLYBENCH_ARRAY(r));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(m, n, POLYBENCH_ARRAY(s), POLYBENCH_ARRAY(q)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(s);
  POLYBENCH_FREE_ARRAY(q);
  POLYBENCH_FREE_ARRAY(p);
  POLYBENCH_FREE_ARRAY(r);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _CHOLESKY_H
# define _CHOLESKY_H


#define N_cholesky 40

# define _PB_N_cholesky POLYBENCH_LOOP_BOUND(N_cholesky,n)

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_CHOLESKY_H */


/* Array initialization. */
static
void init_array_cholesky(int n,
		DATA_TYPE POLYBENCH_2D(A,N_cholesky,N_cholesky,n,n))
{
  int i, j;

  for (i = 0; i < n; i++)
    {
      for (j = 0; j <= i; j++)
	A[i][j] = (DATA_TYPE)(-j % n) / n + 1;
      for (j = i+1; j < n; j++) {
	A[i][j] = 0;
      }
      A[i][i] = 1;
    }

  /* Make the matrix positive semi-definite. */
  int r,s,t;
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, N_cholesky, N_cholesky, n, n);
  for (r = 0; r < n; ++r)
    for (s = 0; s < n; ++s)
      (POLYBENCH_ARRAY(B))[r][s] = 0;
  for (t = 0; t < n; ++t)
    for (r = 0; r < n; ++r)
      for (s = 0; s < n; ++s)
	(POLYBENCH_ARRAY(B))[r][s] += A[r][t] * A[s][t];
    for (r = 0; r < n; ++r)
      for (s = 0; s < n; ++s)
	A[r][s] = (POLYBENCH_ARRAY(B))[r][s];
  POLYBENCH_FREE_ARRAY(B);

}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array_cholesky(int n,
		 DATA_TYPE POLYBENCH_2D(A,N_cholesky,N_cholesky,n,n))

{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("A");
  for (i = 0; i < n; i++)
    for (j = 0; j <= i; j++) {
    if ((i * n + j) % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
    fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, A[i][j]);
  }
  POLYBENCH_DUMP_END("A");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_cholesky(int n,
		     DATA_TYPE POLYBENCH_2D(A,N_cholesky,N_cholesky,n,n))
{
  int i, j, k;


#pragma scop
  for (i = 0; i < _PB_N_cholesky; i++) {
     //j<i
     for (j = 0; j < i; j++) {
        for (k = 0; k < j; k++) {
           A[i][j] -= A[i][k] * A[j][k];
        }
        A[i][j] /= A[j][j];
     }
     // i==j case
     for (k = 0; k < i; k++) {
        A[i][i] -= A[i][k] * A[i][k];
     }
     A[i][i] = SQRT_FUN(A[i][i]);
  }
#pragma endscop

}


int main_cholesky()
{
  /* Retrieve problem size. */
  int n = N_cholesky;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N_cholesky, N_cholesky, n, n);

  /* Initialize array(s). */
  init_array_cholesky (n, POLYBENCH_ARRAY(A));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_cholesky (n, POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _DOITGEN_H
# define _DOITGEN_H


#define NQ_doitgen 8
#define NR_doitgen 10
#define NP_doitgen 12

# define _PB_NQ_doitgen POLYBENCH_LOOP_BOUND(NQ_doitgen,nq)
# define _PB_NR_doitgen POLYBENCH_LOOP_BOUND(NR_doitgen,nr)
# define _PB_NP_doitgen POLYBENCH_LOOP_BOUND(NP_doitgen,np)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_DOITGEN_H */


/* Array initialization. */
static
void init_array_doitgen(int nr, int nq, int np,
		DATA_TYPE POLYBENCH_3D(A,NR_doitgen,NQ_doitgen,NP_doitgen,nr,nq,np),
		DATA_TYPE POLYBENCH_2D(C4,NP_doitgen,NP_doitgen,np,np))
{
  int i, j, k;

  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++)
	A[i][j][k] = (DATA_TYPE) ((i*j + k)%np) / np;
  for (i = 0; i < np; i++)
    for (j = 0; j < np; j++)
      C4[i][j] = (DATA_TYPE) (i*j % np) / np;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
void kernel_doitgen(int nr, int nq, int np,
		    DATA_TYPE POLYBENCH_3D(A,NR_doitgen,NQ_doitgen,NP_doitgen,nr,nq,np),
		    DATA_TYPE POLYBENCH_2D(C4,NP_doitgen,NP_doitgen,np,np),
		    DATA_TYPE POLYBENCH_1D(sum,NP_doitgen,np))
{
  int r, q, p, s;

#pragma scop
  for (r = 0; r < _PB_NR_doitgen; r++)
    for (q = 0; q < _PB_NQ_doitgen; q++)  {
      for (p = 0; p < _PB_NP_doitgen; p++)  {
	sum[p] = SCALAR_VAL(0.0);
	for (s = 0; s < _PB_NP_doitgen; s++)
	  sum[p] += A[r][q][s] * C4[s][p];
      }
      for (p = 0; p < _PB_NP_doitgen; p++)
	A[r][q][p] = sum[p];
    }
#pragma endscop

}


int main_doitgen()
{
  /* Retrieve problem size. */
  int nr = NR_doitgen;
  int nq = NQ_doitgen;
  int np = NP_doitgen;

  /* Variable declaration/allocation. */
  POLYBENCH_3D_ARRAY_DECL(A,DATA_TYPE,NR_doitgen,NQ_doitgen,NP_doitgen,nr,nq,np);
  POLYBENCH_1D_ARRAY_DECL(sum,DATA_TYPE,NP_doitgen,np);
  POLYBENCH_2D_ARRAY_DECL(C4,DATA_TYPE,NP_doitgen,NP_doitgen,np,np);

  /* Initialize array(s). */
  init_array_doitgen (nr, nq, np,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(C4));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_doitgen (nr, nq, np,
		  POLYBENCH_ARRAY(A),
		  POLYBENCH_ARRAY(C4),
		  POLYBENCH_ARRAY(sum));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(nr, nq, np,  POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(sum);
  POLYBENCH_FREE_ARRAY(C4);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _GEMM_H
# define _GEMM_H


#define NI_gemm 20
#define NJ_gemm 25
#define NK_gemm 30


# define _PB_NI_gemm POLYBENCH_LOOP_BOUND(NI_gemm,ni)
# define _PB_NJ_gemm POLYBENCH_LOOP_BOUND(NJ_gemm,nj)
# define _PB_NK_gemm POLYBENCH_LOOP_BOUND(NK_gemm,nk)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_GEMM_H */



/* Array initialization. */
static
void init_array_gemm(int ni, int nj, int nk,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(C,NI_gemm,NJ_gemm,ni,nj),
		DATA_TYPE POLYBENCH_2D(A,NI_gemm,NK_gemm,ni,nk),
		DATA_TYPE POLYBENCH_2D(B,NK_gemm,NJ_gemm,nk,nj))
{
  int i, j;

  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++)
      C[i][j] = (DATA_TYPE) ((i*j+1) % ni) / ni;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i][j] = (DATA_TYPE) (i*(j+1) % nk) / nk;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i][j] = (DATA_TYPE) (i*(j+2) % nj) / nj;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gemm(int ni, int nj, int nk,
		 DATA_TYPE alpha,
		 DATA_TYPE beta,
		 DATA_TYPE POLYBENCH_2D(C,NI_gemm,NJ_gemm,ni,nj),
		 DATA_TYPE POLYBENCH_2D(A,NI_gemm,NK_gemm,ni,nk),
		 DATA_TYPE POLYBENCH_2D(B,NK_gemm,NJ_gemm,nk,nj))
{
  int i, j, k;

//BLAS PARAMS
//TRANSA = 'N'
//TRANSB = 'N'
// => Form C := alpha*A*B + beta*C,
//A is NIxNK
//B is NKxNJ
//C is NIxNJ
#pragma scop
  for (i = 0; i < _PB_NI_gemm; i++) {
    for (j = 0; j < _PB_NJ_gemm; j++)
	C[i][j] *= beta;
    for (k = 0; k < _PB_NK_gemm; k++) {
       for (j = 0; j < _PB_NJ_gemm; j++)
	  C[i][j] += alpha * A[i][k] * B[k][j];
    }
  }
#pragma endscop

}


int main_gemm()
{
  /* Retrieve problem size. */
  int ni = NI_gemm;
  int nj = NJ_gemm;
  int nk = NK_gemm;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,NI_gemm,NJ_gemm,ni,nj);
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,NI_gemm,NK_gemm,ni,nk);
  POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,NK_gemm,NJ_gemm,nk,nj);

  /* Initialize array(s). */
  init_array_gemm (ni, nj, nk, &alpha, &beta,
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_gemm (ni, nj, nk,
	       alpha, beta,
	       POLYBENCH_ARRAY(C),
	       POLYBENCH_ARRAY(A),
	       POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(ni, nj,  POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _GEMVER_H
# define _GEMVER_H


#   define N_gemver 40


# define _PB_N_gemver POLYBENCH_LOOP_BOUND(N_gemver,n)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_GEMVER_H */



/* Array initialization. */
static
void init_array_gemver (int n,
		 DATA_TYPE *alpha,
		 DATA_TYPE *beta,
		 DATA_TYPE POLYBENCH_2D(A,N_gemver,N_gemver,n,n),
		 DATA_TYPE POLYBENCH_1D(u1,N_gemver,n),
		 DATA_TYPE POLYBENCH_1D(v1,N_gemver,n),
		 DATA_TYPE POLYBENCH_1D(u2,N_gemver,n),
		 DATA_TYPE POLYBENCH_1D(v2,N_gemver,n),
		 DATA_TYPE POLYBENCH_1D(w,N_gemver,n),
		 DATA_TYPE POLYBENCH_1D(x,N_gemver,n),
		 DATA_TYPE POLYBENCH_1D(y,N_gemver,n),
		 DATA_TYPE POLYBENCH_1D(z,N_gemver,n))
{
  int i, j;

  *alpha = 1.5;
  *beta = 1.2;

  DATA_TYPE fn = (DATA_TYPE)n;

  for (i = 0; i < n; i++)
    {
      u1[i] = i;
      u2[i] = ((i+1)/fn)/2.0;
      v1[i] = ((i+1)/fn)/4.0;
      v2[i] = ((i+1)/fn)/6.0;
      y[i] = ((i+1)/fn)/8.0;
      z[i] = ((i+1)/fn)/9.0;
      x[i] = 0.0;
      w[i] = 0.0;
      for (j = 0; j < n; j++)
        A[i][j] = (DATA_TYPE) (i*j % n) / n;
    }
}





/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gemver(int n,
		   DATA_TYPE alpha,
		   DATA_TYPE beta,
		   DATA_TYPE POLYBENCH_2D(A,N_gemver,N_gemver,n,n),
		   DATA_TYPE POLYBENCH_1D(u1,N_gemver,n),
		   DATA_TYPE POLYBENCH_1D(v1,N_gemver,n),
		   DATA_TYPE POLYBENCH_1D(u2,N_gemver,n),
		   DATA_TYPE POLYBENCH_1D(v2,N_gemver,n),
		   DATA_TYPE POLYBENCH_1D(w,N_gemver,n),
		   DATA_TYPE POLYBENCH_1D(x,N_gemver,n),
		   DATA_TYPE POLYBENCH_1D(y,N_gemver,n),
		   DATA_TYPE POLYBENCH_1D(z,N_gemver,n))
{
  int i, j;

#pragma scop

  for (i = 0; i < _PB_N_gemver; i++)
    for (j = 0; j < _PB_N_gemver; j++)
      A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];

  for (i = 0; i < _PB_N_gemver; i++)
    for (j = 0; j < _PB_N_gemver; j++)
      x[i] = x[i] + beta * A[j][i] * y[j];

  for (i = 0; i < _PB_N_gemver; i++)
    x[i] = x[i] + z[i];

  for (i = 0; i < _PB_N_gemver; i++)
    for (j = 0; j < _PB_N_gemver; j++)
      w[i] = w[i] +  alpha * A[i][j] * x[j];

#pragma endscop
}


int main_gemver()
{
  /* Retrieve problem size. */
  int n = N_gemver;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N_gemver, N_gemver, n, n);
  POLYBENCH_1D_ARRAY_DECL(u1, DATA_TYPE, N_gemver, n);
  POLYBENCH_1D_ARRAY_DECL(v1, DATA_TYPE, N_gemver, n);
  POLYBENCH_1D_ARRAY_DECL(u2, DATA_TYPE, N_gemver, n);
  POLYBENCH_1D_ARRAY_DECL(v2, DATA_TYPE, N_gemver, n);
  POLYBENCH_1D_ARRAY_DECL(w, DATA_TYPE, N_gemver, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N_gemver, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N_gemver, n);
  POLYBENCH_1D_ARRAY_DECL(z, DATA_TYPE, N_gemver, n);


  /* Initialize array(s). */
  init_array_gemver (n, &alpha, &beta,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(u1),
	      POLYBENCH_ARRAY(v1),
	      POLYBENCH_ARRAY(u2),
	      POLYBENCH_ARRAY(v2),
	      POLYBENCH_ARRAY(w),
	      POLYBENCH_ARRAY(x),
	      POLYBENCH_ARRAY(y),
	      POLYBENCH_ARRAY(z));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_gemver (n, alpha, beta,
		 POLYBENCH_ARRAY(A),
		 POLYBENCH_ARRAY(u1),
		 POLYBENCH_ARRAY(v1),
		 POLYBENCH_ARRAY(u2),
		 POLYBENCH_ARRAY(v2),
		 POLYBENCH_ARRAY(w),
		 POLYBENCH_ARRAY(x),
		 POLYBENCH_ARRAY(y),
		 POLYBENCH_ARRAY(z));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(w)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(u1);
  POLYBENCH_FREE_ARRAY(v1);
  POLYBENCH_FREE_ARRAY(u2);
  POLYBENCH_FREE_ARRAY(v2);
  POLYBENCH_FREE_ARRAY(w);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);
  POLYBENCH_FREE_ARRAY(z);

  return 0;
}


/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _GESUMMV_H
# define _GESUMMV_H


#   define N_gesummv 30


# define _PB_N_gesummv POLYBENCH_LOOP_BOUND(N_gesummv,n)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_GESUMMV_H */



/* Array initialization. */
static
void init_array_gesummv(int n,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(A,N_gesummv,N_gesummv,n,n),
		DATA_TYPE POLYBENCH_2D(B,N_gesummv,N_gesummv,n,n),
		DATA_TYPE POLYBENCH_1D(x,N_gesummv,n))
{
  int i, j;

  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < n; i++)
    {
      x[i] = (DATA_TYPE)( i % n) / n;
      for (j = 0; j < n; j++) {
	A[i][j] = (DATA_TYPE) ((i*j+1) % n) / n;
	B[i][j] = (DATA_TYPE) ((i*j+2) % n) / n;
      }
    }
}




/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gesummv(int n,
		    DATA_TYPE alpha,
		    DATA_TYPE beta,
		    DATA_TYPE POLYBENCH_2D(A,N_gesummv,N_gesummv,n,n),
		    DATA_TYPE POLYBENCH_2D(B,N_gesummv,N_gesummv,n,n),
		    DATA_TYPE POLYBENCH_1D(tmp,N_gesummv,n),
		    DATA_TYPE POLYBENCH_1D(x,N_gesummv,n),
		    DATA_TYPE POLYBENCH_1D(y,N_gesummv,n))
{
  int i, j;

#pragma scop
  for (i = 0; i < _PB_N_gesummv; i++)
    {
      tmp[i] = SCALAR_VAL(0.0);
      y[i] = SCALAR_VAL(0.0);
      for (j = 0; j < _PB_N_gesummv; j++)
	{
	  tmp[i] = A[i][j] * x[j] + tmp[i];
	  y[i] = B[i][j] * x[j] + y[i];
	}
      y[i] = alpha * tmp[i] + beta * y[i];
    }
#pragma endscop

}


int main_gesummv()
{
  /* Retrieve problem size. */
  int n = N_gesummv;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N_gesummv, N_gesummv, n, n);
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, N_gesummv, N_gesummv, n, n);
  POLYBENCH_1D_ARRAY_DECL(tmp, DATA_TYPE, N_gesummv, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N_gesummv, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N_gesummv, n);


  /* Initialize array(s). */
  init_array_gesummv (n, &alpha, &beta,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B),
	      POLYBENCH_ARRAY(x));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_gesummv (n, alpha, beta,
		  POLYBENCH_ARRAY(A),
		  POLYBENCH_ARRAY(B),
		  POLYBENCH_ARRAY(tmp),
		  POLYBENCH_ARRAY(x),
		  POLYBENCH_ARRAY(y));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(y)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);
  POLYBENCH_FREE_ARRAY(tmp);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);

  return 0;
}


/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _MVT_H
# define _MVT_H



#   define N_mvt 40


# define _PB_N_mvt POLYBENCH_LOOP_BOUND(N_mvt,n)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_MVT_H */


/* Array initialization. */
static
void init_array_mvt(int n,
		DATA_TYPE POLYBENCH_1D(x1,N_mvt,n),
		DATA_TYPE POLYBENCH_1D(x2,N_mvt,n),
		DATA_TYPE POLYBENCH_1D(y_1,N_mvt,n),
		DATA_TYPE POLYBENCH_1D(y_2,N_mvt,n),
		DATA_TYPE POLYBENCH_2D(A,N_mvt,N_mvt,n,n))
{
  int i, j;

  for (i = 0; i < n; i++)
    {
      x1[i] = (DATA_TYPE) (i % n) / n;
      x2[i] = (DATA_TYPE) ((i + 1) % n) / n;
      y_1[i] = (DATA_TYPE) ((i + 3) % n) / n;
      y_2[i] = (DATA_TYPE) ((i + 4) % n) / n;
      for (j = 0; j < n; j++)
	A[i][j] = (DATA_TYPE) (i*j % n) / n;
    }
}




/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_mvt(int n,
		DATA_TYPE POLYBENCH_1D(x1,N_mvt,n),
		DATA_TYPE POLYBENCH_1D(x2,N_mvt,n),
		DATA_TYPE POLYBENCH_1D(y_1,N_mvt,n),
		DATA_TYPE POLYBENCH_1D(y_2,N_mvt,n),
		DATA_TYPE POLYBENCH_2D(A,N_mvt,N_mvt,n,n))
{
  int i, j;

#pragma scop
  for (i = 0; i < _PB_N_mvt; i++)
    for (j = 0; j < _PB_N_mvt; j++)
      x1[i] = x1[i] + A[i][j] * y_1[j];
  for (i = 0; i < _PB_N_mvt; i++)
    for (j = 0; j < _PB_N_mvt; j++)
      x2[i] = x2[i] + A[j][i] * y_2[j];
#pragma endscop

}


int main_mvt()
{
  /* Retrieve problem size. */
  int n = N_mvt;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N_mvt, N_mvt, n, n);
  POLYBENCH_1D_ARRAY_DECL(x1, DATA_TYPE, N_mvt, n);
  POLYBENCH_1D_ARRAY_DECL(x2, DATA_TYPE, N_mvt, n);
  POLYBENCH_1D_ARRAY_DECL(y_1, DATA_TYPE, N_mvt, n);
  POLYBENCH_1D_ARRAY_DECL(y_2, DATA_TYPE, N_mvt, n);


  /* Initialize array(s). */
  init_array_mvt (n,
	      POLYBENCH_ARRAY(x1),
	      POLYBENCH_ARRAY(x2),
	      POLYBENCH_ARRAY(y_1),
	      POLYBENCH_ARRAY(y_2),
	      POLYBENCH_ARRAY(A));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_mvt (n,
	      POLYBENCH_ARRAY(x1),
	      POLYBENCH_ARRAY(x2),
	      POLYBENCH_ARRAY(y_1),
	      POLYBENCH_ARRAY(y_2),
	      POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(x1), POLYBENCH_ARRAY(x2)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(x1);
  POLYBENCH_FREE_ARRAY(x2);
  POLYBENCH_FREE_ARRAY(y_1);
  POLYBENCH_FREE_ARRAY(y_2);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _SYMM_H
# define _SYMM_H


#define M_symm 20
#define N_symm 30


# define _PB_M_symm POLYBENCH_LOOP_BOUND(M_symm,m)
# define _PB_N_symm POLYBENCH_LOOP_BOUND(N_symm,n)

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_SYMM_H */



/* Array initialization. */
static
void init_array_symm(int m, int n,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(C,M_symm,N_symm,m,n),
		DATA_TYPE POLYBENCH_2D(A,M_symm,M_symm,m,m),
		DATA_TYPE POLYBENCH_2D(B,M_symm,N_symm,m,n))
{
  int i, j;

  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {
      C[i][j] = (DATA_TYPE) ((i+j) % 100) / m;
      B[i][j] = (DATA_TYPE) ((n+i-j) % 100) / m;
    }
  for (i = 0; i < m; i++) {
    for (j = 0; j <=i; j++)
      A[i][j] = (DATA_TYPE) ((i+j) % 100) / m;
    for (j = i+1; j < m; j++)
      A[i][j] = -999; //regions of arrays that should not be used
  }
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_symm(int m, int n,
		 DATA_TYPE alpha,
		 DATA_TYPE beta,
		 DATA_TYPE POLYBENCH_2D(C,M_symm,N_symm,m,n),
		 DATA_TYPE POLYBENCH_2D(A,M_symm,M_symm,m,m),
		 DATA_TYPE POLYBENCH_2D(B,M_symm,N_symm,m,n))
{
  int i, j, k;
  DATA_TYPE temp2;

//BLAS PARAMS
//SIDE = 'L'
//UPLO = 'L'
// =>  Form  C := alpha*A*B + beta*C
// A is MxM
// B is MxN
// C is MxN
//note that due to Fortran array layout, the code below more closely resembles upper triangular case in BLAS
#pragma scop
   for (i = 0; i < _PB_M_symm; i++)
      for (j = 0; j < _PB_N_symm; j++ )
      {
        temp2 = 0;
        for (k = 0; k < i; k++) {
           C[k][j] += alpha*B[i][j] * A[i][k];
           temp2 += B[k][j] * A[i][k];
        }
        C[i][j] = beta * C[i][j] + alpha*B[i][j] * A[i][i] + alpha * temp2;
     }
#pragma endscop

}


int main_symm()
{
  /* Retrieve problem size. */
  int m = M_symm;
  int n = N_symm;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,M_symm,N_symm,m,n);
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,M_symm,M_symm,m,m);
  POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,M_symm,N_symm,m,n);

  /* Initialize array(s). */
  init_array_symm (m, n, &alpha, &beta,
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_symm (m, n,
	       alpha, beta,
	       POLYBENCH_ARRAY(C),
	       POLYBENCH_ARRAY(A),
	       POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(m, n, POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _SYR2K_H
# define _SYR2K_H


#   define M_syr2k 20
#   define N_syr2k 30


# define _PB_M_syr2k POLYBENCH_LOOP_BOUND(M_syr2k,m)
# define _PB_N_syr2k POLYBENCH_LOOP_BOUND(N_syr2k,n)

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_SYR2K_H */


/* Array initialization. */
static
void init_array_syr2k(int n, int m,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(C,N_syr2k,N_syr2k,n,n),
		DATA_TYPE POLYBENCH_2D(A,N_syr2k,M_syr2k,n,m),
		DATA_TYPE POLYBENCH_2D(B,N_syr2k,M_syr2k,n,m))
{
  int i, j;

  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < n; i++)
    for (j = 0; j < m; j++) {
      A[i][j] = (DATA_TYPE) ((i*j+1)%n) / n;
      B[i][j] = (DATA_TYPE) ((i*j+2)%m) / m;
    }
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      C[i][j] = (DATA_TYPE) ((i*j+3)%n) / m;
    }
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_syr2k(int n, int m,
		  DATA_TYPE alpha,
		  DATA_TYPE beta,
		  DATA_TYPE POLYBENCH_2D(C,N_syr2k,N_syr2k,n,n),
		  DATA_TYPE POLYBENCH_2D(A,N_syr2k,M_syr2k,n,m),
		  DATA_TYPE POLYBENCH_2D(B,N_syr2k,M_syr2k,n,m))
{
  int i, j, k;

//BLAS PARAMS
//UPLO  = 'L'
//TRANS = 'N'
//A is NxM
//B is NxM
//C is NxN
#pragma scop
  for (i = 0; i < _PB_N_syr2k; i++) {
    for (j = 0; j <= i; j++)
      C[i][j] *= beta;
    for (k = 0; k < _PB_M_syr2k; k++)
      for (j = 0; j <= i; j++)
	{
	  C[i][j] += A[j][k]*alpha*B[i][k] + B[j][k]*alpha*A[i][k];
	}
  }
#pragma endscop

}


int main_syr2k()
{
  /* Retrieve problem size. */
  int n = N_syr2k;
  int m = M_syr2k;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,N_syr2k,N_syr2k,n,n);
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,N_syr2k,M_syr2k,n,m);
  POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,N_syr2k,M_syr2k,n,m);

  /* Initialize array(s). */
  init_array_syr2k (n, m, &alpha, &beta,
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_syr2k (n, m,
		alpha, beta,
		POLYBENCH_ARRAY(C),
		POLYBENCH_ARRAY(A),
		POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _SYRK_H
# define _SYRK_H


#   define M_syrk 20
#   define N_syrk 30


# define _PB_M_syrk POLYBENCH_LOOP_BOUND(M_syrk,m)
# define _PB_N_syrk POLYBENCH_LOOP_BOUND(N_syrk,n)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_SYRK_H */


/* Array initialization. */
static
void init_array_syrk(int n, int m,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(C,N_syrk,N_syrk,n,n),
		DATA_TYPE POLYBENCH_2D(A,N_syrk,M_syrk,n,m))
{
  int i, j;

  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < n; i++)
    for (j = 0; j < m; j++)
      A[i][j] = (DATA_TYPE) ((i*j+1)%n) / n;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      C[i][j] = (DATA_TYPE) ((i*j+2)%m) / m;
}



/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_syrk(int n, int m,
		 DATA_TYPE alpha,
		 DATA_TYPE beta,
		 DATA_TYPE POLYBENCH_2D(C,N_syrk,N_syrk,n,n),
		 DATA_TYPE POLYBENCH_2D(A,N_syrk,M_syrk,n,m))
{
  int i, j, k;

//BLAS PARAMS
//TRANS = 'N'
//UPLO  = 'L'
// =>  Form  C := alpha*A*A**T + beta*C.
//A is NxM
//C is NxN
#pragma scop
  for (i = 0; i < _PB_N_syrk; i++) {
    for (j = 0; j <= i; j++)
      C[i][j] *= beta;
    for (k = 0; k < _PB_M_syrk; k++) {
      for (j = 0; j <= i; j++)
        C[i][j] += alpha * A[i][k] * A[j][k];
    }
  }
#pragma endscop

}


int main_syrk()
{
  /* Retrieve problem size. */
  int n = N_syrk;
  int m = M_syrk;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,N_syrk,N_syrk,n,n);
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,N_syrk,M_syrk,n,m);

  /* Initialize array(s). */
  init_array_syrk (n, m, &alpha, &beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_syrk (n, m, alpha, beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _TRISOLV_H
# define _TRISOLV_H

#   define N_trisolv 40


# define _PB_N_trisolv POLYBENCH_LOOP_BOUND(N_trisolv,n)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_TRISOLV_H */



/* Array initialization. */
static
void init_array_trisolv(int n,
		DATA_TYPE POLYBENCH_2D(L,N_trisolv,N_trisolv,n,n),
		DATA_TYPE POLYBENCH_1D(x,N_trisolv,n),
		DATA_TYPE POLYBENCH_1D(b,N_trisolv,n))
{
  int i, j;

  for (i = 0; i < n; i++)
    {
      x[i] = - 999;
      b[i] =  i ;
      for (j = 0; j <= i; j++)
	L[i][j] = (DATA_TYPE) (i+n-j+1)*2/n;
    }
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_trisolv(int n,
		    DATA_TYPE POLYBENCH_2D(L,N_trisolv,N_trisolv,n,n),
		    DATA_TYPE POLYBENCH_1D(x,N_trisolv,n),
		    DATA_TYPE POLYBENCH_1D(b,N_trisolv,n))
{
  int i, j;

#pragma scop
  for (i = 0; i < _PB_N_trisolv; i++)
    {
      x[i] = b[i];
      for (j = 0; j <i; j++)
        x[i] -= L[i][j] * x[j];
      x[i] = x[i] / L[i][i];
    }
#pragma endscop

}


int main_trisolv()
{
  /* Retrieve problem size. */
  int n = N_trisolv;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(L, DATA_TYPE, N_trisolv, N_trisolv, n, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N_trisolv, n);
  POLYBENCH_1D_ARRAY_DECL(b, DATA_TYPE, N_trisolv, n);


  /* Initialize array(s). */
  init_array_trisolv (n, POLYBENCH_ARRAY(L), POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(b));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_trisolv (n, POLYBENCH_ARRAY(L), POLYBENCH_ARRAY(x), POLYBENCH_ARRAY(b));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(x)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(L);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(b);

  return 0;
}

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _TRMM_H
# define _TRMM_H


#   define M_trmm 20
#   define N_trmm 30


# define _PB_M_trmm POLYBENCH_LOOP_BOUND(M_trmm,m)
# define _PB_N_trmm POLYBENCH_LOOP_BOUND(N_trmm,n)


#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_TRMM_H */


/* Array initialization. */
static
void init_array_trmm(int m, int n,
		DATA_TYPE *alpha,
		DATA_TYPE POLYBENCH_2D(A,M_trmm,M_trmm,m,m),
		DATA_TYPE POLYBENCH_2D(B,M_trmm,N_trmm,m,n))
{
  int i, j;

  *alpha = 1.5;
  for (i = 0; i < m; i++) {
    for (j = 0; j < i; j++) {
      A[i][j] = (DATA_TYPE)((i+j) % m)/m;
    }
    A[i][i] = 1.0;
    for (j = 0; j < n; j++) {
      B[i][j] = (DATA_TYPE)((n+(i-j)) % n)/n;
    }
 }

}



/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_trmm(int m, int n,
		 DATA_TYPE alpha,
		 DATA_TYPE POLYBENCH_2D(A,M_trmm,M_trmm,m,m),
		 DATA_TYPE POLYBENCH_2D(B,M_trmm,N_trmm,m,n))
{
  int i, j, k;

//BLAS parameters
//SIDE   = 'L'
//UPLO   = 'L'
//TRANSA = 'T'
//DIAG   = 'U'
// => Form  B := alpha*A**T*B.
// A is MxM
// B is MxN
#pragma scop
  for (i = 0; i < _PB_M_trmm; i++)
     for (j = 0; j < _PB_N_trmm; j++) {
        for (k = i+1; k < _PB_M_trmm; k++)
           B[i][j] += A[k][i] * B[k][j];
        B[i][j] = alpha * B[i][j];
     }
#pragma endscop

}


int main_trmm()
{
  /* Retrieve problem size. */
  int m = M_trmm;
  int n = N_trmm;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,M_trmm,M_trmm,m,m);
  POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,M_trmm,N_trmm,m,n);

  /* Initialize array(s). */
  init_array_trmm (m, n, &alpha, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_trmm (m, n, alpha, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(m, n, POLYBENCH_ARRAY(B)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}

int main_all()
{
    int i;
    for(i=0;i<5;i++)
    {
        /* Minería de datos */
        main_correlation();
            
        main_2mm();
        main_3mm();
        main_atax();
        main_bicg();
        main_cholesky();
        main_doitgen();
        main_gemm();
        main_gemver();
        main_gesummv();
        
        main_mvt();
        main_symm();
        main_syr2k();
        main_syrk();
        main_trisolv();
        main_trmm();
    }
    return 0;
}
