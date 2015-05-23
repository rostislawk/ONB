#include "core/mem.h"
#include "core/util.h"
#include "crypto/belt.h"
#include "math/gf2.h"
#include "math\word.h"

#ifdef OS_WINDOWS
	#include <locale.h>
	#include <windows.h>
#endif

static const size_t onb2_sizes[5] = {173, 233, 281, 293, 509};
static const size_t onb3_sizes[9] = {179, 191, 239, 251, 359, 419, 431, 443, 491};

void mega_test();

void test_shift();
void test_generation_b(size_t m);
void test_generation_pi(size_t m);
void test_apply_pi();
void test_apply_f();
void test_onb_to_standard(size_t m);
void test_generationONB2_A(size_t m);
void test_generationONB3_A(size_t m);
void test_mulONB2(size_t m);
void test_mulONB3(size_t m);
void test_invONB2(size_t m);
void test_divONB2(size_t m);

void test_reverse();
void countTableF();
void test_mul();
void test_sqrONB(size_t m);
void test_sqr(size_t m);
