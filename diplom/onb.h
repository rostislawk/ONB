#include "core/mem.h"
#include "core/util.h"
#include "crypto/belt.h"
#include "math/gf2.h"
#include "math\word.h"

#ifdef OS_WINDOWS
	#include <locale.h>
	#include <windows.h>
#endif

void generate_b(word *b, size_t m);

void generateONB2_A(word *a, size_t m);
void generateONB3_A(word *a, size_t m);

void sqr(word *res, word *a, size_t n, size_t m);
void mul(word *res, word *a, word *b, word *A, size_t n, size_t m);
void inv(word *res, word *a, word *A, size_t n, size_t m);
void div_onb(word *res, word *a, word *b, word *A, size_t n, size_t m);
