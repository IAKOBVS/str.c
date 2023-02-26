#ifndef JSTR_H_DEF
#define JSTR_H_DEF

#include <stddef.h>
#include "/home/james/c/vargc.h"

#if defined(__PRETTY_FUNCTION__)
	#define CURR_FUNC __PRETTY_FUNCTION__
#elif defined(__FUNCTION__)
	#define CURR_FUNC __FUNCTION__
#elif defined(__func__)
	#define CURR_FUNC __func__
#else
	#define CURR_FUNC ""
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    #define ALWAYS_INLINE __forceinline inline
#else
    #define ALWAYS_INLINE inline
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# define RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__)
# define RESTRICT __restrict__
#elif defined(_MSC_VER)
# define RESTRICT __restrict
#else
# define RESTRICT
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 3)) || (defined(__clang__) && __has_builtin(__builtin_expect))
  #define likely(x) __builtin_expect(!!(x), 1)
  #define unlikely(x) __builtin_expect(!!(x), 0)
#else
  #define likely(x) (x)
  #define unlikely(x) (x)
#endif

#define JSTR_MIN_SIZE 8
#define MAX(a,b) ((a)>(b)?(a):(b))

typedef struct jstring {
	char *data;
	size_t capacity;
	size_t size; 
} jstring;

int private_jstr_cat(jstring *RESTRICT dest, ...);
#define jstr_cat(JSTR, ...) private_jstr_cat(JSTR, __VA_ARGS__, NULL)

void jstr_init(jstring *RESTRICT dest);

void jstr_delete_fast(jstring *RESTRICT dest);
void jstr_delete(jstring *RESTRICT dest);

int jstr_new(jstring *RESTRICT dest, const char *RESTRICT src, const size_t src_size);
#define jstr_new_auto(dest, src) jstr_new(dest, src, strlen(src))

int jstr_pushback(jstring *dest, const char c);

void jstr_popback(jstring *RESTRICT dest);

int jstr_append(jstring *dest, const char *RESTRICT src, const size_t src_size);
#define jstr_append_auto(dest, src) jstr_append(dest, src, strlen(src))

/* swaps dest with src and updates the jstring struct members */
void jstr_swap(jstring *RESTRICT dest, jstring *RESTRICT src);
void jstr_swap_str(jstring *RESTRICT dest, char **RESTRICT src, size_t *src_size, size_t *src_capacity);

int jstr_shrink(jstring *RESTRICT dest);

int jstr_reserve(jstring *RESTRICT dest, size_t capacity);

/* replaces dest->data with dest and reallocs if needed */
int jstr_replace(jstring *RESTRICT dest, char *RESTRICT src, const size_t src_size);
#define jstr_replace_auto(dest, src) jstr_replace(dest, src, strlen(src))

/* compares two Jstr, and if equal, returns 0 */
int jstr_cmp(jstring *RESTRICT dest, jstring *RESTRICT src);

#undef ALWAYS_INLINE
#undef RESTRICT

#endif
