#include "tests.h"
#include "onb.h"
#include "utils.h"
#include "math\zz.h"
#include "math\gf2.h"
#include "math\poly.h"
#include "core\mem.h"

void mega_test()
{
	size_t onb2_count = 5;
	size_t onb3_count = 9;
	size_t index = 0;
	printf("TEST ONB2 generation b\n");
	for (index = 0; index < onb2_count; ++index) {
		test_generation_b(onb2_sizes[index]);
	}
	printf("TEST ONB3 generation b\n");
	for (index = 0; index < onb3_count; ++index) {
		test_generation_b(onb3_sizes[index]);
	}

	printf("\nTEST ONB2 generation A\n");
	for (index = 0; index < onb2_count; ++index) {
		test_generationONB2_A(onb2_sizes[index]);
	}
	printf("TEST ONB3 generation A\n");
	for (index = 0; index < onb3_count; ++index) {
		test_generationONB3_A(onb3_sizes[index]);
	}

	printf("\nTEST ONB2 multiplication\n");
	for (index = 0; index < onb2_count; ++index) {
		test_mulONB2(onb2_sizes[index]);
	}
	printf("TEST ONB3 multiplication\n");
	for (index = 0; index < onb3_count; ++index) {
		test_mulONB3(onb3_sizes[index]);
	}

	printf("\nTEST multiplication\n");
	for (index = 0; index < onb2_count; ++index) {
		test_mul(onb2_sizes[index]);
	}
	printf("TEST multiplication\n");
	for (index = 0; index < onb3_count; ++index) {
		test_mul(onb3_sizes[index]);
	}

	printf("\nTEST ONB sqr\n");
	for (index = 0; index < onb2_count; ++index) {
		test_sqrONB(onb2_sizes[index]);
	}
	printf("TEST sqr\n");
	for (index = 0; index < onb3_count; ++index) {
		test_sqrONB(onb3_sizes[index]);
	}

	printf("\nTEST sqr\n");
	for (index = 0; index < onb2_count; ++index) {
		test_sqr(onb2_sizes[index]);
	}
	for (index = 0; index < onb3_count; ++index) {
		test_sqr(onb3_sizes[index]);
	}

	printf("\nTEST convert bases\n");
	for (index = 0; index < onb2_count; ++index) {
		test_convertONB(onb2_sizes[index]);
	}
	for (index = 0; index < onb3_count; ++index) {
		test_convertONB(onb3_sizes[index]);
	}
}

void test_shift()
{
	word *a = (word *)malloc(3 * sizeof(word));
	printf("TEST SHIFT\n");
	a[0] = 15;
	a[1] = 31;
	a[2] = 1;
	//wordSetBit(a,32,TRUE);
	printBinaryRepresentation2(a, 3, 3 * B_PER_W);
	printf("\n");
	shiftRight(a, 3, 36, 33);
	printBinaryRepresentation2(a, 3, 3 * B_PER_W);
	free(a);
}

void test_generation_b(size_t m)
{
	size_t w_size = size_in_words(m), index;
	word *b = (word *)memAlloc(w_size * 2 * sizeof(word));
	uint64 start, overhead, freq1, freq2;
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
        generate_b(b, m);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("%u, %u\n", m, freq1/1000);
	//printf("TEST GENERATION B\n");
	
	//printBinaryRepresentation2(b, w_size, m + 1);
	
	memFree(b);
}

void test_generation_pi(size_t m)
{
	size_t index = 0;
	word *pi = (word *)malloc(m * sizeof(word));
	printf("TEST GENERATION PI\n");
	generate_pi(pi, m);
	for (index = 0; index < m; ++index) {
		printf("%d ", pi[index]);
	}
	printf("\n");
}

void test_apply_pi()
{
	size_t index = 0, m = 5;
	word *pi = (word *)malloc(m * sizeof(word));
	size_t word_size = size_in_words(m);
	word *a = (word *)malloc(word_size * sizeof(word));
	word *b = (word *)malloc(word_size * sizeof(word));
	a[0] = 0x19;
	printf("TEST APLYING PI\n");
	generate_pi(pi, m);
	applyPi(a, b, pi, m);
	printBinaryRepresentation2(a, word_size, m);
	printBinaryRepresentation2(b, word_size, m);
}

void test_apply_f()
{
	word *a = malloc(sizeof(word) * 4);
	word *b = malloc(sizeof(word) * 4);
	wordSetZero(b, 4);
	a[0] = 0x96FAED11;
	a[1] = 0x00000000;
	a[2] = 0xCCCCCCCC;
	a[3] = 0xF0F0F0F0;
	printBinaryRepresentation2(a, 4, 4 * B_PER_W);
	applyF(a, b, 4);
	printBinaryRepresentation2(a, 4, 4 * B_PER_W);

	free(a);
	free(b);
}

void test_apply_f1()
{
	word *a = malloc(sizeof(word));
	word *b = malloc(sizeof(word));
	a[0] = 0x17;
	b[0] = 0;
	printBinaryRepresentation2(a, 1, B_PER_W);
	applyF(a, b, 1);
	printBinaryRepresentation2(a, 1, B_PER_W);
	free(a);
	free(b);
}

void test_onb_to_standard(size_t m)
{
	size_t words_in_m = (m / B_PER_W > 1) ? m / B_PER_W : 1;
	size_t index = 0;
	word *a = malloc(words_in_m * sizeof(word));
	word *b = malloc(words_in_m * sizeof(word));
	word *x = malloc(words_in_m * sizeof(word));
	word *pi = malloc(m * sizeof(word));
	for (index = 0; index < words_in_m; ++index) {
		a[index] = 0x1B;
	}
	generate_b(b, m);
	generate_pi(pi, m);
	fromONB2ToStandard(a, x, b, pi, m);
	printBinaryRepresentation2(a, words_in_m, m);
	printBinaryRepresentation2(x, words_in_m, m);
	free(a);
	//free(b);
	//free(x);
	//free(pi);
}

void test_generationONB2_A(size_t m)
{
	word *a = (word *)malloc((2 * m - 1) * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
		generateONB2_A(a, m);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("%u, %u\n", m, freq1/1000);
	/*for (index = 0; index < 2 * m - 1; ++index) {
		printf("%u -> %u\n", (index + 1) / 2, a[index]);
	}*/
	//free(a);
}

void test_generationONB3_A(size_t m)
{
	word *a = (word *)malloc((2 * m - 1) * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
		generateONB3_A(a, m);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("%u, %u\n", m, freq1/1000);
	/*for (index = 0; index < 2 * m - 1; ++index) {
		printf("%u -> %u\n", (index + 1) / 2, a[index]);
	}*/
	//free(a);
}

void test_mulONB2(size_t m)
{
	size_t field_size_word_length = size_in_words(m);
	word *A = (word *)malloc((2 * m - 1) * sizeof(word));
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * 2 * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	generateONB2_A(A, m);
	wordSetZero(a, field_size_word_length);
	wordSetZero(b, field_size_word_length);
	for (index = 0; index < field_size_word_length; ++index) {
		a[index] = 1647462394;
		b[index] = 1620632094;
	}
	normalize(a, field_size_word_length, m);
	//printBinaryRepresentation2(a, field_size_word_length, m);
	//printf("\n");
	normalize(b, field_size_word_length, m);
	//printBinaryRepresentation2(b, field_size_word_length, m);
	//printf("\n");
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
        mul(res, a, b, A, field_size_word_length, m);
	}
    freq1 = RDTSC() - start - overhead;	
	//printBinaryRepresentation2(res, field_size_word_length, m);
	printf("%u, %u\n", m, freq1/1000);
	free(A);
	free(a);
	free(b);
	free(res);
}

void test_mulONB3(size_t m)
{
	size_t field_size_word_length = size_in_words(m);
	word *A = (word *)malloc((2 * m - 1) * sizeof(word));
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * 2 * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	generateONB3_A(A, m);
	wordSetZero(a, field_size_word_length);
	wordSetZero(b, field_size_word_length);
	for (index = 0; index < field_size_word_length; ++index) {
		a[index] = 1647462394;
		b[index] = 1620632094;
	}
	normalize(a, field_size_word_length, m);
	//printBinaryRepresentation2(a, field_size_word_length, m);
	//printf("\n");
	normalize(b, field_size_word_length, m);
	//printBinaryRepresentation2(b, field_size_word_length, m);
	//printf("\n");
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
        mul(res, a, b, A, field_size_word_length, m);
	}
    freq1 = RDTSC() - start - overhead;	
	//printBinaryRepresentation2(res, field_size_word_length, m);
	//printf("\n");
	printf("%u, %u\n", m, freq1/1000);
	free(A);
	free(a);
	free(b);
	free(res);
}

void test_invONB2(size_t m)
{
	size_t field_size = m;
	size_t field_size_word_length = size_in_words(field_size);
	word *A = (word *)malloc((2 * field_size - 1) * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * sizeof(word));
	word *unity = (word *)malloc(field_size_word_length * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	generateONB2_A(A, field_size);
	wordSetZero(b, field_size_word_length);
	b[0] = 1;
	printBinaryRepresentation2(b, field_size_word_length, field_size);
	normalize(b, field_size_word_length, field_size);
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
        inv(res, b, A, field_size_word_length, field_size);
	}
    freq1 = RDTSC() - start - overhead;	
	
	printBinaryRepresentation2(res, field_size_word_length, field_size);
	mul(unity, b, res, A, field_size_word_length, field_size);
	printBinaryRepresentation2(unity, field_size_word_length, field_size);
	printf("\nProcessor ticks for multiplication in the field with m = %u equals = %u\n", field_size, freq1/1000);
	free(A);
	free(unity);
	free(b);
	free(res);
}

void test_divONB2(size_t m)
{
	size_t field_size = m;
	size_t field_size_word_length = size_in_words(field_size);
	word *A = (word *)malloc((2 * field_size - 1) * sizeof(word));
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * sizeof(word));
	generateONB2_A(A, field_size);
	wordSetZero(a, field_size_word_length);
	wordSetZero(b, field_size_word_length);
	a[0] = 2;
	b[0] = 1;
	normalize(a, field_size_word_length, field_size);
	normalize(b, field_size_word_length, field_size);
	div_onb(res, a, b, A, field_size_word_length, field_size);
	printBinaryRepresentation2(res, field_size_word_length, field_size);
	free(A);
	free(a);
	free(b);
	free(res);
}

void test_reverse()
{
	word *a = malloc(sizeof(word) * 4);
	word *b = malloc(sizeof(word) * 4);
	wordSetZero(b, 4);
	a[0] = 0x96FAED11;
	a[1] = 0x00000000;
	a[2] = 0xCCCCCCCC;
	a[3] = 0xF0F0F0F0;
	reverse(a, b, 4);
	printBinaryRepresentation2(a, 4, 4 * B_PER_W);
	printBinaryRepresentation2(b, 4, 4 * B_PER_W);

	free(a);
	free(b);
}

void countTableF()
{
	octet x, f;
	octet x1, x2, x3, x4, x5, x6, x7, x8;
	octet f1, f2, f3, f4, f5, f6, f7, f8;
	size_t index = 1;
	for (x1 = 0; x1 < 2; ++x1) {
		for (x2 = 0; x2 < 2; ++x2) {
			for (x3 = 0; x3 < 2; ++x3) {
				for (x4 = 0; x4 < 2; ++x4) {
					for (x5 = 0; x5 < 2; ++x5) {
						for (x6 = 0; x6 < 2; ++x6) {
							for (x7 = 0; x7 < 2; ++x7) {
								for (x8 = 0; x8 < 2; ++x8) {
									f8 = x8 ^ x6 ^ x4 ^ x2;
									f7 = x7 ^ x3;
									f6 = x6 ^ x4;
									f5 = x5;
									f4 = x4 ^ x2;
									f3 = x3;
									f2 = x2;
									f1 = x1;
									x = x1 * 0x80 + x2 * 0x40 + x3 * 0x20 + x4 * 0x10 + x5 * 0x08 + x6 * 0x04 + x7 * 0x02 + x8;
									f = f1 * 0x80 + f2 * 0x40 + f3 * 0x20 + f4 * 0x10 + f5 * 0x08 + f6 * 0x04 + f7 * 0x02 + f8;
									printf("0x%X, ", f);
									if (index == 16) {
										printf("\n");
										index = 0;
									}
									index++;
								}
							}
						}	
					}
				}
			}
		}
	}
}

void test_mul(size_t m)
{
	size_t field_size_word_length = size_in_words(m);
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *c = (word *)malloc(field_size_word_length * 2 * sizeof(word));
	word *mod = (word *)malloc((field_size_word_length + 1) * sizeof(word));
	size_t deep = polyMulMod_deep(field_size_word_length);
	word *stack = (word *)malloc(sizeof(word) * deep);
	size_t index;
	uint64 start, overhead, freq1, freq2;
	wordSetZero(a, field_size_word_length);
	wordSetZero(b, field_size_word_length);
	generate_b(mod, m);
	for (index = 0; index < field_size_word_length; ++index) {
		a[index] = 1647462394;
		b[index] = 1620632094;
	}
	normalize(a, field_size_word_length, m);
	normalize(b, field_size_word_length, m);
	normalize(mod, field_size_word_length + 1, m + 1);
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
        polyMulMod(c, a, b, mod, field_size_word_length, stack);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("%u, %u\n", m, freq1/1000);

	free(a);
	free(b);
	free(c);
	free(mod);
	free(stack);
}

void test_sqrONB(size_t m)
{
	size_t field_size_word_length = size_in_words(m);
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * 2 * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	wordSetZero(a, field_size_word_length);
	for (index = 0; index < field_size_word_length; ++index) {
		a[index] = 1647462394;
	}
	normalize(a, field_size_word_length, m);
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
        sqr(res, a, field_size_word_length, m);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("%u, %u\n", m, freq1/1000);
	free(a);
	free(res);
}

void test_sqr(size_t m)
{
	size_t field_size_word_length = size_in_words(m);
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * 2 * sizeof(word));
	word *mod = (word *)malloc((field_size_word_length + 1) * sizeof(word));
	size_t deep = polyMulMod_deep(field_size_word_length);
	word *stack = (word *)malloc(sizeof(word) * deep);
	size_t index;
	uint64 start, overhead, freq1, freq2;
	wordSetZero(a, field_size_word_length);
	for (index = 0; index < field_size_word_length; ++index) {
		a[index] = 1647462394;
	}
	generate_b(mod, m);
	normalize(a, field_size_word_length, m);
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
		polySqrMod(res, a, mod, field_size_word_length, stack);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("%u, %u\n", m, freq1/1000);
	free(a);
	free(res);
	free(mod);
	free(stack);
}

void test_convertONB(size_t m)
{
	size_t field_size_word_length = size_in_words(m);
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *st = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc((field_size_word_length + 1) * sizeof(word));
	word *pi = (word *)malloc(m * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	wordSetZero(a, field_size_word_length);
	for (index = 0; index < field_size_word_length; ++index) {
		a[index] = 1647462394;
	}
	generate_b(b, m);
	generate_pi(pi, m);
	normalize(a, field_size_word_length, m);
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
		fromONB2ToStandard(a, st, b, pi, m);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("%u, %u\n", m, freq1/1000);
	
	free(a);
	free(b);
	free(st);
	free(pi);
}