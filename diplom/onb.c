#include "onb.h"
#include "utils.h"

static octet f[256] = {
	0x00, 0x01, 0x8A, 0x8B, 0x44, 0x45, 0xCE, 0xCF, 0xA8, 0xA9, 0x22, 0x23, 0xEC, 0xED, 0x66, 0x67,
	0x10, 0x11, 0x9A, 0x9B, 0x54, 0x55, 0xDE, 0xDF, 0xB8, 0xB9, 0x32, 0x33, 0xFC, 0xFD, 0x76, 0x77,
	0xA0, 0xA1, 0x2A, 0x2B, 0xE4, 0xE5, 0x6E, 0x6F, 0x08, 0x09, 0x82, 0x83, 0x4C, 0x4D, 0xC6, 0xC7,
	0xB0, 0xB1, 0x3A, 0x3B, 0xF4, 0xF5, 0x7E, 0x7F, 0x18, 0x19, 0x92, 0x93, 0x5C, 0x5D, 0xD6, 0xD7,
	0x40, 0x41, 0xCA, 0xCB, 0x04, 0x05, 0x8E, 0x8F, 0xE8, 0xE9, 0x62, 0x63, 0xAC, 0xAD, 0x26, 0x27,
	0x50, 0x51, 0xDA, 0xDB, 0x14, 0x15, 0x9E, 0x9F, 0xF8, 0xF9, 0x72, 0x73, 0xBC, 0xBD, 0x36, 0x37,
	0xE0, 0xE1, 0x6A, 0x6B, 0xA4, 0xA5, 0x2E, 0x2F, 0x48, 0x49, 0xC2, 0xC3, 0x0C, 0x0D, 0x86, 0x87,
	0xF0, 0xF1, 0x7A, 0x7B, 0xB4, 0xB5, 0x3E, 0x3F, 0x58, 0x59, 0xD2, 0xD3, 0x1C, 0x1D, 0x96, 0x97,
	0x80, 0x81, 0x0A, 0x0B, 0xC4, 0xC5, 0x4E, 0x4F, 0x28, 0x29, 0xA2, 0xA3, 0x6C, 0x6D, 0xE6, 0xE7,
	0x90, 0x91, 0x1A, 0x1B, 0xD4, 0xD5, 0x5E, 0x5F, 0x38, 0x39, 0xB2, 0xB3, 0x7C, 0x7D, 0xF6, 0xF7,
	0x20, 0x21, 0xAA, 0xAB, 0x64, 0x65, 0xEE, 0xEF, 0x88, 0x89, 0x02, 0x03, 0xCC, 0xCD, 0x46, 0x47,
	0x30, 0x31, 0xBA, 0xBB, 0x74, 0x75, 0xFE, 0xFF, 0x98, 0x99, 0x12, 0x13, 0xDC, 0xDD, 0x56, 0x57,
	0xC0, 0xC1, 0x4A, 0x4B, 0x84, 0x85, 0x0E, 0x0F, 0x68, 0x69, 0xE2, 0xE3, 0x2C, 0x2D, 0xA6, 0xA7,
	0xD0, 0xD1, 0x5A, 0x5B, 0x94, 0x95, 0x1E, 0x1F, 0x78, 0x79, 0xF2, 0xF3, 0x3C, 0x3D, 0xB6, 0xB7,
	0x60, 0x61, 0xEA, 0xEB, 0x24, 0x25, 0xAE, 0xAF, 0xC8, 0xC9, 0x42, 0x43, 0x8C, 0x8D, 0x06, 0x07,
	0x70, 0x71, 0xFA, 0xFB, 0x34, 0x35, 0xBE, 0xBF, 0xD8, 0xD9, 0x52, 0x53, 0x9C, 0x9D, 0x16, 0x17};

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

void generate_pi(word *pi, size_t m)
{
	size_t index = 0;
	size_t p = 2 * m + 1;
	pi[0] = 1;
	for (index = 1; index < m; ++index) {
		pi[index] = (pi[index-1] * 2) % p;
		if (pi[index] > m) {
			pi[index] = p - pi[index];
		}
		pi[index - 1] -= 1;
	}
	pi[m - 1] -= 1;
}

void applyPi(word *a, word *b, word *pi, size_t m)
{
	size_t index = 0;
	BOOL bit = FALSE;
	for (index = 0; index < m; ++index) {
		bit = wordGetBits(a, index, 1) == 1 ? TRUE : FALSE;
		wordSetBit(b, pi[index], bit); 
	}
}

//word apply_F_for_word(word a)
//{
//
//}
//
//void applyF(word *a, word *mem, size_t n)
//{
//	wordC
//}

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

void fromONB2ToStandard(word *onb2, word *st, word *b, word *pi, size_t m)
{
	size_t ext = next_power_of_two(m);
	word *x = malloc(ext * sizeof(word));
	wordSetZero(x, ext);
	applyPi(onb2, x, pi, m);
	free(x);
}