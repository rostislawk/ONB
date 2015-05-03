//#include "gf2_opt.h"
//#include "core/blob.h"
//#include "core/mem.h"
//#include "core/stack.h"
//#include "core/util.h"
//#include "math/gf2.h"
//#include "math/word.h"
//
//bool_t gf2_onbCreate(qr_o* f, const size_t p[4], void* stack)
//{
//	ASSERT(memIsValid(f, sizeof(qr_o)));
//	ASSERT(memIsValid(p, 4 * sizeof(size_t)));
//	// нормальный базис?
//	if (p[1] == 0)
//	{
//		size_t n1;
//		f->n = W_OF_B(p[0]);
//		f->no = O_OF_B(p[0]);
//		n1 = f->n + (p[0] % B_PER_W == 0);
//		/*f->mod = (word*)f->descr;
//		wordSetZero(f->mod, n1);
//		wordSetBit(f->mod, p[0], 1);
//		wordSetBit(f->mod, p[1], 1);
//		wordSetBit(f->mod, 0, 1);*/
//		// настроить интерфейсы
//		f->from = gf2_onbFrom;
//		f->to = gf2_onbTo;
//		f->add = gf2_onbAdd3;
//		f->sub = gf2_onbAdd3;
//		f->neg = gf2_onbNeg2;
//		f->mul = gf2_onbMul;
//		f->sqr = gf2_onbSqr;
//		f->inv = gf2_onbInv;
//		f->div = gf2_onbDiv;
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
//// -
//static bool_t gf2_onbFrom(word b[], const octet a[], const qr_o* f, void* stack)
//{
//	ASSERT(gf2IsOperable(f));
//	memToWord(b, a, f->no);
//	return gf2IsIn(b, f);
//}
//
//// -
//static void gf2_onbTo(octet b[], const word a[], const qr_o* f, void* stack)
//{
//	ASSERT(gf2IsOperable(f));
//	ASSERT(gf2IsIn(a, f));
//	memFromWord(b, f->no, a);
//}
//
//// -
//static void gf2_onbAdd3(word c[], const word a[], const word b[], const qr_o* f)
//{
//	ASSERT(gf2IsOperable(f));
//	ASSERT(gf2IsIn(a, f));
//	ASSERT(gf2IsIn(b, f));
//	wordXor(c, a, b, f->n);
//}
//
//// -
//static void gf2_onbNeg2(word b[], const word a[], const qr_o* f)
//{
//	ASSERT(gf2IsOperable(f));
//	ASSERT(gf2IsIn(a, f));
//	wordCopy(b, a, f->n);
//}
//
//// -
//static void gf2_onbMul(word c[], const word a[], const word b[], 
//	const qr_o* f, void* stack)
//{
//	word* prod = (word*)stack;
//	stack = prod + 2 * f->n;
//	ASSERT(gf2IsOperable(f));
//	ASSERT(gf2IsIn(a, f));
//	ASSERT(gf2IsIn(b, f));
//	polyMul(prod, a, f->n, b, f->n, stack);
//	wordCopy(c, prod, f->n);
//}
//
//// -
//static void gf2_onbSqr(word b[], const word a[], const qr_o* f, 
//	void* stack)
//{
//	word* prod = (word*)stack;
//	stack = prod + 2 * f->n;
//	ASSERT(gf2IsOperable(f));
//	ASSERT(gf2IsIn(a, f));
//	polySqr(prod, a, f->n, stack);
//	wordCopy(b, prod, f->n);
//}
//
////-
//static void gf2_onbInv(word b[], const word a[], const qr_o* f, void* stack)
//{
//	ASSERT(gf2IsOperable(f));
//	ASSERT(gf2IsIn(a, f));
//	if (gf2Deg(f) % B_PER_W == 0)
//	{
//		word* c = (word*)stack;
//		stack = c + f->n + 1;
//		polyInvMod(c, a, f->mod, f->n + 1, stack);
//		ASSERT(c[f->n] == 0);
//		wordCopy(b, c, f->n);
//	}
//	else
//		polyInvMod(b, a, f->mod, f->n, stack);
//}
//
//static size_t gf2Inv_deep(size_t n)
//{
//	return O_OF_W(n + 1) + ppInvMod_deep(n + 1);
//}
//
//static void gf2_onbDiv(word b[], const word divident[], const word a[], 
//	const qr_o* f, void* stack)
//{
//	ASSERT(gf2IsOperable(f));
//	ASSERT(gf2IsIn(divident, f));
//	ASSERT(gf2IsIn(a, f));
//	if (gf2Deg(f) % B_PER_W == 0)
//	{
//		word* c = (word*)stack;
//		stack = c + f->n + 1;
//		polyDivMod(c, divident, a, f->mod, f->n + 1, stack);
//		ASSERT(c[f->n] == 0);
//		wordCopy(b, c, f->n);
//	}
//	else
//		polyDivMod(b, divident, a, f->mod, f->n, stack);
//}
//
//static size_t gf2Div_deep(size_t n)
//{
//	return O_OF_W(n + 1) + ppDivMod_deep(n + 1);
//}