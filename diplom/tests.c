#include "tests.h"
#include "onb.h"
#include "utils.h"

static const size_t field_size = 14;
static const size_t test_arr_size = 3;

void test_shift()
{
	word *a = (word *)malloc(test_arr_size * sizeof(word));
	printf("TEST SHIFT\n");
	a[0] = 15;
	a[1] = 31;
	a[2] = 1;
	//wordSetBit(a,32,TRUE);
	printBinaryRepresentation2(a, test_arr_size, field_size);
	printf("\n");
	shiftRight(a, test_arr_size, 36, 33);
	printBinaryRepresentation2(a, test_arr_size, field_size);
	free(a);
}

void test_generation_b(size_t m)
{
	size_t b_size = bits_in_number(m);
	size_t w_size = size_in_words(m);
	word *b = (word *)malloc(b_size * sizeof(word));
	printf("TEST GENERATION B\n");
	generate_b(b, m);
	printBinaryRepresentation2(b, w_size, m + 1);
	
	free(b);
}

void test_generationONB2_A()
{
	word *a = (word *)malloc((2 * field_size - 1) * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
		generateONB2_A(a, field_size);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("Processor ticks for multiplication in the field with m = %u equals = %u\n", field_size, freq1/1000);
	for (index = 0; index < 2 * field_size - 1; ++index) {
		printf("%u -> %u\n", (index + 1) / 2, a[index]);
	}
	free(a);
}

void test_mulONB2()
{
	size_t field_size_word_length = size_in_words(field_size);
	word *A = (word *)malloc((2 * field_size - 1) * sizeof(word));
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * 2 * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	generateONB2_A(A, field_size);
	wordSetZero(a, field_size_word_length);
	wordSetZero(b, field_size_word_length);
	a[0] = 1;
	b[0] = 1;
	normalize(a, field_size_word_length, field_size);
	printBinaryRepresentation2(a, field_size_word_length, field_size);
	printf("\n");
	normalize(b, field_size_word_length, field_size);
	printBinaryRepresentation2(b, field_size_word_length, field_size);
	printf("\n");
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
        mul(res, a, b, A, field_size_word_length, field_size);
	}
    freq1 = RDTSC() - start - overhead;	
	printBinaryRepresentation2(res, field_size_word_length, field_size);
	printf("\n");
	printf("Processor ticks for multiplication in the field with m = %u equals = %u\n", field_size, freq1/1000);
	free(A);
	free(a);
	free(b);
	free(res);
}

void test_invONB2()
{
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

void test_divONB2()
{
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
