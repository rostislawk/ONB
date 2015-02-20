﻿/*!
*******************************************************************************
\file word.h
\brief Операции с двоичными словами
*//****************************************************************************
\author (С) Сергей Агиевич, http://apmi.bsu.by
\created 2012.04.18
\version 2014.10.21
*******************************************************************************
*/

#ifndef __WORD_H
#define __WORD_H

#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
*******************************************************************************
\file word.h

Реализованы операции с двоичными словами произвольной длины.

Двоичное слово задается массивом машинных слов: word w[n].

Разряды слова w[0] нумеруются от 0 (младший) до B_PER_W - 1 (старший),
разряды w[1] -- от B_PER_W (младший) до 2 * B_PER_W - 1 (старший) и т.д.

Запись [n]w означает, что слово w состоит из n машинных слов.

\pre Все входные указатели действительны.

\pre В функциях работы со словами по адресам памяти для слов
зарезервировано ясное из конекста либо уточняемое в описаниях функций
число машинных слов.

todo: safe = {wordCmp, wordCmp2, wordCmpW, wordIsZero, wordIsW, wordIsRepW, 
	wordWordSize?, wordOctetSize?, wordSetBit, wordLoZeroBits, wordHiZeroBits, 
	wordBitSize?, wordNAF?}
*******************************************************************************
*/

/*
*******************************************************************************
Псевдонимы
*******************************************************************************
*/

/*!	Корректное слово [n]a? */
#define wordIsValid(a, n) memIsValid((a), O_OF_W(n))

/*!	Буферы слов [n]a и [n]b не пересекаются? */
#define wordIsDisjoint(a, b, n) memIsDisjoint(a, b, O_OF_W(n))

/*!	Буферы слов [n]a и [n]b совпадают или не пересекаются? */
#define wordIsSameOrDisjoint(a, b, n) memIsSameOrDisjoint(a, b, O_OF_W(n))

/*!	Буферы слов [n]a и [m]b не пересекаются? */
#define wordIsDisjoint2(a, n, b, m) memIsDisjoint2(a, O_OF_W(n), b, O_OF_W(m))

/*!	Буферы слов [n]a, [m]b и [k]c не пересекаются? */
#define wordIsDisjoint3(a, n, b, m, c, k)\
	memIsDisjoint3(a, O_OF_W(n), b, O_OF_W(m), c, O_OF_W(k))

/*
*******************************************************************************
Копирование, логические операции
*******************************************************************************
*/

/*!	\brief Копирование слов

	Cлово [n]a переписывается в [n]b:
	\code
		b <- a.
	\endcode
	\pre Буфер b либо не пересекается, либо совпадает с буфером a.
*/
void wordCopy(
	word b[],		/*!< [out] приемник */
	const word a[],	/*!< [in] источник */
	size_t n		/*!< [in] длина a в машинных словах */
);

/*!	\brief Перестановка слов

	Cлова [n]a и [n]b меняются местами:
	\code
		a <-> b.
	\endcode
	\pre Буфер b не пересекается с буфером a.
*/
void wordSwap(
	word a[],		/*!< [in/out] первое слово */
	word b[],		/*!< [in/out] второе слово */
	size_t n		/*!< [in] длина a и b в машинных словах */
);

/*!	\brief Проверка совпадения слов

	Проверяется совпадение слов [n]a и [n]b.
	\return Признак сопадения.
	\safe Время выполнения функции зависит от значений слов.
	Имеется регулярная функция wordEqSafe().
*/
bool_t wordEq(
	const word a[],	/*!< [in] первое слово */
	const word b[],	/*!< [in] второе слово */
	size_t n		/*!< [in] длина a и b в машинных словах */
);

bool_t wordEqSafe(const word a[], const word b[], size_t n);

/*!	\brief Сравнение слов

	Cлова [n]a и [n]b сравниваются лексикографически.
	\remark a > b, если a[n - 1] == b[n - 1],..., a[i] == b[i], a[i] > b[i].
	\return 1, если a > b, или -1, если a < b, или 0, если a == b.
*/
int wordCmp(
	const word a[],	/*!< [in] первое слово */
	const word b[],	/*!< [in] второе слово */
	size_t n		/*!< [in] длина a и b в машинных словах */
);

/*!	\brief Сравнение слов разной длины

	Cлова [n]a и [m]b сравниваются лексикографически (см. wordCmp())
	после дополнения нулями до слов одинаковой длины.
	\return 1, если a > b, или -1, если a < b, или 0, если a == b.
*/
int wordCmp2(
	const word a[],	/*!< [in] первое слово */
	size_t n,		/*!< [in] длина a в машинных словах */
	const word b[],	/*!< [in] второе слово */
	size_t m		/*!< [in] длина b в машинных словах */
);

/*!	\brief Сравнение слова c машинным словом

	Число [n]a сравнивается с машинным словом w.
	\return 1, если a > w, или -1, если a < w, или 0, если a == w.
*/
int wordCmpW(
	const word a[],		/*!< [in] сравниваемое слово */
	size_t n,			/*!< [in] длина a в машинных словах */
	register word w		/*!< [in] машинное слово */
);

/*!	\brief Cложение слов по модулю 2

	Определяется поразрядная по модулю 2 сумма [n]с слов [n]a и [n]b:
	\code
		c <- a ^ b.
	\endcode
	\pre Буфер c либо не пересекается, либо совпадает с каждым из буферов a, b.
*/
void wordXor(
	word c[],		/*!< [out] сумма */
	const word a[],	/*!< [in] первое слагаемое */
	const word b[],	/*!< [in] второе слагаемое */
	size_t n		/*!< [in] длина a и b в машинных словах */
);

/*!	\brief Добавление слова по модулю 2

	К слову [n]b добавляется слово [n]a. Сложение выполняется поразрядно
	по модулю 2:
	\code
		b <- a ^ b.
	\endcode
	\pre Буфер b либо не пересекается, либо совпадает с буфером a.
*/
void wordXor2(
	word b[],		/*!< [in/out] второе слагаемое / сумма */
	const word a[],	/*!< [in] первое слагаемое */
	size_t n		/*!< [in] длина a в машинных словах */
);

/*!	\brief Обнуление слова

	Слово [n]a обнуляется:
	\code
		a <- 0.
	\endcode
*/
void wordSetZero(
	word a[],		/*!< [out] слово */
	size_t n		/*!< [in] длина a в машинных словах */
);

/*!	\brief Присвоение слову значения -- машинного слова

	Слову [n]a присваивается значение w, которое является машинным словом:
	\code
		a[0] <- w, a[1] <- 0, ..., a[n - 1] <- 0.
	\endcode
	\pre n > 0 или w == 0.
*/
void wordSetW(
	word a[],			/*!< [out] слово */
	size_t n,			/*!< [in] длина a в машинных словах */
	register word w		/*!< [in] значение */
);

/*!	\brief Заполнение слова машинным словом

	Всем машинным словам [n]a присваивается значение w:
	\code
		a[0] <- w, a[1] <- w, ..., a[n - 1] <- w.
	\endcode
	\pre n > 0 или w == 0.
*/
void wordRepW(
	word a[],			/*!< [out] слово */
	size_t n,			/*!< [in] длина a в машинных словах */
	register word w		/*!< [in] значение */
);

/*!	\brief Нулевое слово?

	Проверяется, что слово [n]a нулевое.
	\return TRUE, если a - нулевое, и FALSE в противном случае.
*/
bool_t wordIsZero(
	const word a[],	/*!< [in] проверяемое слово */
	size_t n		/*!< [in] длина a в машинных словах */
);

/*! \brief	Принимает значение -- машинное слово?

	Проверяется, что слово [n]a принимает значение w, которое 
	является машинным словом:
	\code
		a[0] == w && a[1] == ... == a[n - 1] == 0?
	\endcode
	\remark Пустое слово (n == 0) принимает значение 0.
	\return TRUE, если a = w, и FALSE в противном случае.
*/
bool_t wordIsW(
	const word a[],		/*!< [in] проверяемое слово */
	size_t n,			/*!< [in] длина a в машинных словах */
	register word w		/*!< [in] значение */
);

/*!	\brief Повтор машинного слова?

	Проверяется, что все машинные слова [n]a принимают значение w:
	\code
		a[0] == w && a[1] == w && ... && a[n - 1] == w?
	\endcode
	\remark В пустом слове (n == 0) повторяется значение 0.
	\return TRUE, если a составлено из w, и FALSE в противном случае.
*/
bool_t wordIsRepW(
	const word a[],		/*!< [in] проверяемое слово */
	size_t n,			/*!< [in] длина a в машинных словах */
	register word w		/*!< [in] значение */
);

/*!	\brief Размер значащей части слова в машинных словах

	Определяется размер значащей части слова [n]a. Размер полагается равным
	индексу последнего ненулевого машинного слова, увеличенному на единицу.
	\remark Размер пустого (n == 0) или нулевого слов равняется 0.
*/
size_t wordWordSize(
	const word a[],	/*!< [in] слово */
	size_t n		/*!< [in] длина a в машинных словах */
);

/*!	\brief Размер значащей части слова в октетах

	Определяется размер значащей части слова [n]a. Размер полагается равным
	индексу последнего ненулевого октета, увеличенному на единицу.
	\remark Размер пустого (n == 0) или нулевого слов равняется 0.
*/
size_t wordOctetSize(
	const word a[],	/*!< [in] слово */
	size_t n		/*!< [in] длина a в машинных словах */
);

/*!	\brief Выгрузить слово в буфер памяти

	Слово [n]src выгружается в буфер dest.
	\pre По адресу dest зарезервировано O_OF_W(n) октетов.
*/
void wordToMem(
	void* dest,			/*!< [out] приемник */
	const word* src,	/*!< [in] источник */
	size_t n			/*!< [in] длина src в машинных словах */
);

/*!	\brief Загрузить слово из буфера памяти

	Слово [n]dest загружается из буфера src.
	\pre По адресу src зарезервировано O_OF_W(n) октетов.
*/
void wordFromMem(
	word* dest,			/*!< [out] приемник */
	size_t n,			/*!< [in] длина dest в машинных словах */
	const void* src		/*!< [in] источник */
);

/*
*******************************************************************************
Операции с битами, кодирование
*******************************************************************************
*/

/*!	\brief Проверить разряд слова

	В слове a определяется разряд с номером pos.
	\pre По адресу a зарезервировано W_OF_B(pos + 1) машинных слов.
	\return TRUE, если бит ненулевой, и FALSE в противном случае.
*/
bool_t wordTestBit(
	const word a[],	/*!< [in] слово */
	size_t pos		/*!< [in] номер разряда */
);

/*!	\brief Получить разряды слова

	В слове a определяется разряды с номерами pos,..., pos + width - 1.
	\pre По адресу a зарезервировано W_OF_B(pos + width) машинных слов.
	\pre width <= B_PER_W.
	\return Машинное слово, составленное из разрядов a с номерами
	pos (младший), pos + 1,..., pos + width -1 (старший).
*/
word wordGetBits(
	const word a[],	/*!< [in] слово */
	size_t pos,		/*!< [in] номер первого разряда */
	size_t width	/*!< [in] число разрядов */
);

/*!	\brief Установить разряд слова

	В слове a разряду с номером pos присваивается значение val.
	\pre По адресу a зарезервировано W_OF_B(pos + 1) машинных слов.
*/
void wordSetBit(
	word a[],		/*!< [in/out] обрабатываемое слово */
	size_t pos,		/*!< [in] номер разряда */
	bool_t val		/*!< [in] устанавливаемое значение */
);

/*!	\brief Установить разряды слова

	В слове a определяется разряды с номерами pos,..., pos + width - 1
	устанавливаются равными последовательным разрядам val
	(от младшего к старшему).
	\pre По адресу a зарезервировано W_OF_B(pos + width) машинных слов.
	\pre width <= B_PER_W.
*/
void wordSetBits(
	word a[],		/*!< [out] слово */
	size_t pos,		/*!< [in] номер первого разряда */
	size_t width,	/*!< [in] число разрядов */
	word val		/*!< [in] значение разрядов */
);

/*!	\brief Инвертировать разряд слова

	В слове a инвертируется разряд с номером pos.
	\pre По адресу a зарезервировано W_OF_B(pos + 1) машинных слов.
*/
void wordFlipBit(
	word a[],		/*!< [out] слово */
	size_t pos		/*!< [in] номер разряда */
);

/*!	\brief Количество первых (младших) нулевых битов

	Определяется длина серии из нулевых битов в начале слова [n]a.
	\return Длина серии.
*/
size_t wordLoZeroBits(
	const word a[],		/*!< [in] слово */
	size_t n			/*!< [in] длина a в машинных словах */
);

/*!	\brief Количество последних (старших) нулевых битов

	Определяется длина серии из нулевых битов в конце слова [n]a.
	\return Длина серии.
*/
size_t wordHiZeroBits(
	const word a[],		/*!< [in] слово */
	size_t n			/*!< [in] длина a в машинных словах */
);

/*!	\brief Размер значащей части слова в битах

	Определяется размер значащей части слова [n]a. Размер полагается
	равным индексу последнего ненулевого разряда, увеличенному на единицу.
	\remark Размер пустого (n == 0) или нулевого слов равняется 0.
	\remark Если wordBitSize(a, n) == m > 0, то для a как числа выполняется
		2^{m - 1} < a <= 2^m - 1.
	\return Размер значащей части.
*/
size_t wordBitSize(
	const word a[],		/*!< [in] слово */
	size_t n			/*!< [in] длина a в машинных словах */
);

/*!	\brief Расчет NAF

	В [2n + 1]naf помещается кодированное представление оптимальной знаковой
	формы (NAF) слова [n]a. Рассчитывается оконная NAF с размером окна w.
	\remark NAF(a, w) представляет собой последовательность символов
	(a_0, a_1,..., a_{l-1}) такую, что
	-	a_i \in {0, \pm 1, \pm 3, ..., \pm 2^{w-1} - 1};
	-	если a != 0, то a_{l-1} != 0;
	-	a как число равняется \sum {i=0}^{l-1} a_i 2^i;
	-	среди любых w последовательных символов a_i только один ненулевой.
	.
	\remark Известно, что l - длина NAF - не превосходит wordBitSize(a) + 1.
	\remark Кодирование символов состоит в следующем
	(\<b> -- двоичная запись числа b):
	-	нулевые a_i представляются одним двоичным символом 0;
	-	положительные a_i представляются w двоичными символами 0\<a_i>;
	-	отрицательные a_i представляются w двоичными символами 1\<|a_i|>;
	-	кодированное представление - это конкатенация кода a_{l-1}
		(первые символы), ...., кода a_1, кода a_0 (последние символы).
	.
	\remark Для кодирования w последовательных элементов a_i потребуется
	не более (w - 1) * 1 + 1 * w = 2 * w - 1 битов. Поэтому при w < B_PER_W
	для хранения всего кодового представления потребуется
	не более 2 * n + 1 слов.
	\remark Если при расчете NAF получен суффикс \alpha, 0,..., 0, 1,
	в котором w - 1 нулей и \alpha < 0, то этот суффикс заменяется на
	\beta, 0,..., 0, 1, в котором w - 2 нулей и
	\beta = 2^{w - 1} + \alpha > 0.
	Суффиксы описывают одинаковые числа:
		2^w + \alpha = 2^{w - 1} + \beta.
	Длина второго суффикса и соответствующей NAF на единицу меньше.
	\pre 2 <= w < B_PER_W.
	\pre Буфер naf не пересекается с буфером a.
	\return Размер naf (число символов l).
*/
size_t wordNAF(
	word naf[],				/*!< [out] знаковая форма */
	const word a[],			/*!< [in] слово */
	size_t n,				/*!< [in] длина a в машинных словах */
	size_t w				/*!< [in] длина окна */
);

/*
*******************************************************************************
Сдвиги и очистка
*******************************************************************************
*/

/*!	\brief Сдвиг в сторону первых (младших) разрядов

	Слово [n]a сдвигается на shift позиций в сторону первых разрядов.
	Освободившиеся разряды заполняются нулями.
	\remark При интерпретации слов как чисел сдвиг означает деление на
	число 2^shift с приведением результата  mod 2^{n * B_PER_W}.
*/
void wordShLo(
	word a[],		/*!< [in/out] сдвигаемое слово */
	size_t n,		/*!< [in] длина a в машинных словах */
	size_t shift	/*!< [in] величина сдвига */
);

/*!	\brief Сдвиг в сторону первых (младших) разрядов с заемом

	Слово [n]a сдвигается на shift позиций в сторону первых разрядов.
	Освободившиеся разряды заполняются разрядами carry.
	\return Машинное слово, составленное из вытесненных последними разрядов.
*/
word wordShLoCarry(
	word a[],		/*!< [in/out] сдвигаемое слово */
	size_t n,		/*!< [in] длина a в машинных словах */
	size_t shift,	/*!< [in] величина сдвига */
	word carry		/*!< [in] машинное слово заема */
);

/*!	\brief Сдвиг в сторону последних (старших) разрядов

	Слово [n]a сдвигается на shift позиций в сторону последних разрядов.
	Освободившиеся разряды заполняются нулями.
	\remark При интерпретации слов как чисел сдвиг означает умножение на
	число 2^shift с приведением результата mod 2^{n * B_PER_W}.
*/
void wordShHi(
	word a[],		/*!< [in] сдвигаемое слово */
	size_t n,		/*!< [in] длина a в машинных словах */
	size_t shift	/*!< [in] величина сдвига */
);

/*!	\brief Сдвиг в сторону последних (старших) разрядов с заемом

	Слово [n]a сдвигается на shift позиций в сторону последних разрядов.
	Освободившиеся разряды заполняются разрядами carry.
	\return Машинное слово, составленное из вытесненных последними разрядов.
*/
word wordShHiCarry(
	word a[],		/*!< [in/out] сдвигаемое слово */
	size_t n,		/*!< [in] длина a в машинных словах */
	size_t shift,	/*!< [in] величина сдвига */
	word carry		/*!< [in] машинное слово заема */
);

/*!	\brief Отбросить первые (младшие) разряды слова

	В слове [n]a обнуляются разряды с номерами
	0, 1,..., min(pos, n * B_PER_W) - 1.
*/
void wordTrimLo(
	word a[],		/*!< [in/out] обрабатываемое слово */
	size_t n,		/*!< [in] длина a в машинных словах */
	size_t pos		/*!< [in] граница обнуляемых разрядов */
);

/*!	\brief Отбросить последние (старшие) разряды слова

	В слове [n]a обнуляются разряды с номерами
	pos, pos + 1,...., n * B_PER_W - 1.
	\remark При pos >= n * B_PER_W никаких действий не выполняется.
	\remark При интерпретации слов как чисел отбрасывание разрядов означает
	приведение mod 2^pos.
*/
void wordTrimHi(
	word a[],		/*!< [in/out] обрабатываемое слово */
	size_t n,		/*!< [in] длина a в машинных словах */
	size_t pos		/*!< [in] номер первого обнуляемого разряда */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __WORD_H */