#include "math\word.h"

#ifdef OS_WINDOWS
	#include <locale.h>
	#include <windows.h>
#endif

uint64 RDTSC();

void printBinaryRepresentation1(word a);
void printBinaryRepresentation2(word *a, size_t arr_size, size_t m);
void shiftRight(word *a, size_t n, size_t m, size_t shift);
void normalize(word *a, size_t n, size_t m);

size_t bits_in_number(size_t number);
size_t size_in_words(size_t number);

word reverseWord(word a);
void reverse(word *a, word *b, size_t n);
