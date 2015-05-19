#include "onb.h"
#include "utils.h"

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

void generateONB2_A(word *a, size_t m)
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
	free(sigma);
	free(mu);
	free(pi);
	free(pi_inv);
}

void generateONB3_A(word *a, size_t m)
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
	for (index = 0; index < p; ++index) {
		pi_inv[index] = WORD_MAX;
	}
	pi_inv[1] = 0;
	for (index = 1; index < m; ++index) {
		pi[index] = (2 * pi[index - 1]) % p;
		pi[index + m];
		pi_inv[pi[index]] = index;
	}
	sigma[0] = 1;
	mu[0] = WORD_MAX;
	
	for (index = 1; index < m; ++index) {
		ksigma = pi[index] + 1;
		if (pi_inv[ksigma] == WORD_MAX) {
			ksigma = p - pi[index] - 1;
		}
		sigma[index] = pi_inv[ksigma];
		kmu = pi[index] - 1;
		if (pi_inv[kmu] == WORD_MAX) {
			kmu = p - pi[index] + 1;
		}
		mu[index] = pi_inv[kmu];
	}
	a[0] = sigma[0];
	for (index = 1; index < m; ++index) {
		a[index * 2 - 1] = sigma[index];
		a[index * 2] = mu[index];
	}
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