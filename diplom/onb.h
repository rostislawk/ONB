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
void generate_pi(word *pi, size_t m);
void applyPi(word *a, word *b, word *pi, size_t m);
void applyF(word *a, word *mem, size_t n);

void generateONB2_A(word *a, size_t m);
void generateONB3_A(word *a, size_t m);

void sqr(word *res, word *a, size_t n, size_t m);
void mul(word *res, word *a, word *b, word *A, size_t n, size_t m);
void inv(word *res, word *a, word *A, size_t n, size_t m);
void div_onb(word *res, word *a, word *b, word *A, size_t n, size_t m);

void fromONB2ToStandard(word *onb2, word *st, word *b, word *pi, size_t m);
void fromONB3ToStandard(word *onb3, word *st, word *b, word *pi, size_t m);

void fromStandardToONB(word *a, word *b, size_t m);
