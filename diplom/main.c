//#include <stdio.h>
#include "core/mem.h"
#include "core/util.h"
#include "crypto/belt.h"
#include "math/gf2.h"
#include "math\word.h"

#ifdef OS_WINDOWS
	#include <locale.h>
	#include <windows.h>
#endif

static const size_t field_size = 14;

static const size_t test_arr_size = 3;

uint64 RDTSC()
{
    __asm _emit 0x0F __asm _emit 0x31
}

void printBinaryRepresentation1(word a);
void printBinaryRepresentation2(word *a, size_t arr_size, size_t m);
void shiftRight(word *a, size_t n, size_t m, size_t shift);
void normalize(word *a, size_t n, size_t m);

void generate_b(word *b, size_t m);
void generate_A(word *a, size_t m);
void sqr(word *res, word *a, size_t n, size_t m);
void mul(word *res, word *a, word *b, word *A, size_t n, size_t m);
void inv(word *res, word *a, word *A, size_t n, size_t m);
void div_onb(word *res, word *a, word *b, word *A, size_t n, size_t m);

void test_shift();
void test_generation_b();
void test_generation_A();
void test_mul();
void test_inv();
void test_div();

size_t bits_in_number(size_t number);
size_t size_in_words(size_t number);

int main()
{	
	//test_generation_b();
	test_generation_A();
	//test_mul();
	//test_inv();
	//test_div();
	return 0;
}

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

void test_generation_b()
{
	size_t b_size = bits_in_number(field_size);
	word *b = (word *)malloc(b_size * sizeof(word));
	printf("TEST GENERATION B\n");
	generate_b(b, field_size);
	printBinaryRepresentation2(b, test_arr_size, field_size + 1);
	
	free(b);
}

void test_generation_A()
{
	word *a = (word *)malloc((2 * field_size - 1) * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	start = RDTSC();
    overhead = RDTSC() - start;
	for (index = 0; index < 1000; ++index) {
		generate_A(a, field_size);
	}
    freq1 = RDTSC() - start - overhead;	
	printf("Processor ticks for multiplication in the field with m = %u equals = %u\n", field_size, freq1/1000);
	for (index = 0; index < 2 * field_size - 1; ++index) {
		printf("%u -> %u\n", (index + 1) / 2, a[index]);
	}
	free(a);
}

void test_mul()
{
	size_t field_size_word_length = size_in_words(field_size);
	word *A = (word *)malloc((2 * field_size - 1) * sizeof(word));
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * 2 * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	generate_A(A, field_size);
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

void test_inv()
{
	size_t field_size_word_length = size_in_words(field_size);
	word *A = (word *)malloc((2 * field_size - 1) * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * sizeof(word));
	word *unity = (word *)malloc(field_size_word_length * sizeof(word));
	size_t index;
	uint64 start, overhead, freq1, freq2;
	generate_A(A, field_size);
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

void test_div()
{
	size_t field_size_word_length = size_in_words(field_size);
	word *A = (word *)malloc((2 * field_size - 1) * sizeof(word));
	word *a = (word *)malloc(field_size_word_length * sizeof(word));
	word *b = (word *)malloc(field_size_word_length * sizeof(word));
	word *res = (word *)malloc(field_size_word_length * sizeof(word));
	generate_A(A, field_size);
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

void printBinaryRepresentation2(word *a, size_t arr_size, size_t m) 
{
	size_t i, length = m;
	for (i = 0; i < arr_size; ++i) {
		if (length > B_PER_W) {
			length = length - B_PER_W;
			printBinaryRepresentation1(a[i], B_PER_W);
			printf(" ");
		}
		else {
			printBinaryRepresentation1(a[i], length);
			break;
		}
	}
	printf("end\n");
}

void printBinaryRepresentation1(word a, size_t m)
{
	size_t index = 0;
	if (m > B_PER_W) {
		m = B_PER_W;
	}
	for (index = 0; index < m; ++index) {
		printf("%u", (a>>index)&1);
	}
}

void shiftRight(word *a, size_t n, size_t m, size_t shift)
{
	word carry;
	word rest;
	word abc;
	//normalize(a, n, m);
	if (shift > B_PER_W) {
		carry = wordShHiCarry(a, n, B_PER_W, 0);
		wordSetBits(a, m - B_PER_W ,B_PER_W, carry);
		shiftRight(a, n, m, shift - B_PER_W);
	}
	else {
		abc = (m >= B_PER_W) ? m - B_PER_W : 0; 
		carry = wordShHiCarry(a, n, shift, 0);
		if (m < B_PER_W) {
			wordShHi(&carry, 1, B_PER_W - m);
		}
		rest = wordGetBits(a, abc, B_PER_W);
		carry = carry | rest;
		wordSetBits(a, abc, B_PER_W, carry);
	}
}

void normalize(word *a, size_t n, size_t m) 
{
	wordTrimHi(a, n, m);
}

// Возведение в квадрат в оптимальном нормальном базисе
void sqr(word *res, word *a, size_t n, size_t m)
{
	wordCopy(res, a, n);
	shiftRight(res, n, m, 1);
}

void generate_b(word *b, size_t m)
{
	size_t index = 0, k, bit_size = bits_in_number(m);
	word M, N;
	BOOL res;
	for (index = 0; index < m + 1; ++index) {
		k = (index + m) / 2;
		M = index;
		N = k;
		M = ~M;
		M = ~(M | N);
		normalize(&M, 1, bit_size);
		res = (M == 0) ? TRUE : FALSE;
		wordSetBit(b, index, res);
	}
	normalize(b, bit_size, m + 1);
}

void generate_A(word *a, size_t m)
{
	size_t p = 2 * m + 1;
	size_t ksigma, kmu;
	word *pi = (word *)malloc(p * sizeof(word));
	word *pi_inv = (word *)malloc(p * sizeof(word));
	word *sigma = (word *)malloc((p - 1) * sizeof(word));
	word *mu = (word *)malloc((p - 1) * sizeof(word));
	size_t index;
	pi[0] = 1;
	pi[p-1] = 1;
	pi_inv[0] = WORD_MAX;
	pi_inv[1] = 0;
	for (index = 1; index < p - 1; ++index) {
		pi[index] = (2 * pi[index - 1]) % p;
		pi_inv[pi[index]] = index;
	}
	sigma[0] = 1;
	mu[0] = WORD_MAX;
	for (index = 1; index < p; ++index) {
		if (index > 0) {
			ksigma = pi_inv[index - 1];
			if (ksigma != WORD_MAX && ksigma < m) {
				sigma[ksigma] = pi_inv[1+pi[ksigma]] % m;
			}
			else if (ksigma < m) {
				sigma[ksigma] = WORD_MAX;
			}
		}
		if (index < p - 1) {
			kmu = pi_inv[index + 1];
			if (kmu != WORD_MAX && kmu < m) {
				mu[kmu] = pi_inv[-1+pi[kmu]] % m;
			}
			else if (kmu < m) {
				mu[kmu] = WORD_MAX;
			}
		}
	}
	a[0] = sigma[0];
	for (index = 1; index < m; ++index) {
		a[index * 2 - 1] = sigma[index];
		a[index * 2] = mu[index];
	}
	/*for (index = 0; index < p; ++index) {
		printf("%u, ", pi[index]);
	}
	printf("\n");
	for (index = 0; index < p; ++index) {
		printf("%u, ", pi_inv[index]);
	}
	printf("\n");
	for (index = 0; index < m; ++index) {
		printf("%u, ", sigma[index]);
	}
	printf("\n");
	for (index = 0; index < m; ++index) {
		printf("%u, ", mu[index]);
	}*/
	free(sigma);
	free(mu);
	free(pi);
	free(pi_inv);
}

void mul(word *res, word *a, word *b, word *A, size_t n, size_t m)
{
	size_t step = 0;
	size_t index = 0;
	size_t apos;
	size_t bpos;
	word xi, yj;
	BOOL bit = FALSE;
	wordSetZero(res, n);
	for (step = 0; step < m; ++step) {
		bit = FALSE;
		for (index = 0; index < 2 * m - 1; ++index) {
			apos = ((index + 1) / 2 + step) % m;
			bpos = (A[index] + step) % m;
			xi = wordGetBits(a, apos, 1);
			yj = wordGetBits(b, bpos, 1);
			bit = (bit ^ (xi & yj)) & 1;
		}
		wordSetBit(res, step, bit);
	}
}

void inv(word *res, word *a, word *A, size_t n, size_t m) 
{
	word *stack1 = (word *)malloc(n * sizeof(word));
	word degree = m - 1;
	size_t bitSize = wordBitSize(&degree, 1);
	size_t index;
	sqr(stack1, a, n, m);
	for (index = bitSize - 1; index > 0; --index) { 
		if (wordGetBits(&degree, index - 1, 1) & 1) {
			mul(res, a, stack1, A, n, m);
		}
		else {
			wordCopy(res, stack1, n);
		}
		sqr(stack1, res, n, m);
	}
	free(stack1);
}

void div_onb(word *res, word *a, word *b, word *A, size_t n, size_t m)
{
	word *b_inv = (word *)malloc(n * sizeof(word));
	inv(b_inv, b, A, n, m);
	mul(res, a, b_inv, A, n, m);
	free(b_inv);
}

size_t bits_in_number(size_t number) 
{
	size_t index = 0;
	while (number != 0) {
		index++;
		number = number >> 1;
	}
	return index;
}

size_t size_in_words(size_t number)
{
	word rest = number % B_PER_W;
	size_t size_in_words = number / B_PER_W;
	return rest == 0 ? size_in_words : size_in_words + 1;
};