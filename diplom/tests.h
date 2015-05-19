#include "core/mem.h"
#include "core/util.h"
#include "crypto/belt.h"
#include "math/gf2.h"
#include "math\word.h"

#ifdef OS_WINDOWS
	#include <locale.h>
	#include <windows.h>
#endif

void test_shift();
void test_generation_b(size_t m);
void test_generationONB2_A();
void test_mulONB2();
void test_invONB2();
void test_divONB2();