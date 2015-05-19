#include "utils.h"

uint64 RDTSC()
{
    __asm _emit 0x0F __asm _emit 0x31
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