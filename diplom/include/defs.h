/*!
*******************************************************************************
\file defs.h
\brief Базовые определения
*//****************************************************************************
\author (С) Сергей Агиевич, http://apmi.bsu.by
\created 2012.04.01
\version 2014.10.15
*******************************************************************************
*/

#ifndef __DEFS_H
#define __DEFS_H

#include <limits.h>
#include <stddef.h>

#ifndef SIZE_MAX
	#include <stdint.h>
#endif

/*!
*******************************************************************************
\file defs.h

\section defs-types Типы данных

\pre Длина машинного слова в битах (B_PER_W) равняется 16, 32 или 64.

\pre Длина слова типа size_t в битах не менее 16.

\pre Обязательно поддерживаются типы uint16, uint32.

Могут поддерживаться типы uint64, uint128.

\remark Поддержка B_PER_W == 16 полезна для организации тестирования
арифметики больших чисел и арифметики многочленов: как правило, ошибки в
функциях арифметики возникают с вероятностями, близкими к 1 / 2^B_PER_W.

\remark При разборе платформ для определения порядка октетов использован код
Брайана Гладмана (Brian Gladman, http://www.gladman.me.uk/).

\section defs-arrays Массивы

Массив элементов типа T, как правило, передается в функцию в виде пары 
(указатель in на элементы массива, длина массива in_len). Здесь in имеет тип 
const T*, а in_len -- тип size_t. При документировании массива используется 
запись [in_len]in. При T == void запись имеет такой же смысл, как при 
T == octet.

Если длина in_len заранее известна, то она может не передаваться в функцию.

Массив элементов типа T заранее известного размера возвращается из функций 
через буфер, на который ссылается указатель out типа T*. 

Если длина массива заранее неизвестна, то для его возврата используется пара 
(указатель out, размер out_len), где out_len имеет тип size_t*. 
Если обратиться к функции с нулевым out, то по адресу out_len будет 
размещена длина возвращаемого массива. При обращении с ненулевым out по 
адресу out_len должно быть указано число элементов типа T, зарезервированных
в буфере out. При документировании такой логики возврата массива используется 
запись [?out_len]out. При T == void запись имеет такой же смысл, 
как при T == octet. 

\section defs-seqs Последовательности вызовов

Ограничения на последовательность вызовов определенных функций документируются 
с помощью знаков "<", "*" и "<<".              ё

Запись "f1() < f2()" означает, что функция f2() должна вызываться после f1(). 

Запись "f()*" означает, что функция f() может вызываться последовательно 
произвольное число раз. 

Запись "f1()* < f2()" означает, что несколько вызовов f1() завершаются 
вызовом f2(). 

Запись "(f1()* < f2())*" означает, что каскад "несколько раз f1(), затем f2()" 
может повторяться произвольное число раз. Например, возможность 
инкрементального хэширования, при котором можно рассчитывать хэш-значение 
все более длинного фрагмента, обозначается следующим образом:
\code
(hash_step()* < hash_get())*.
\endcode

Знак "<<" используется при документировании протоколов. Запись "f1() << f2()" 
означает, что перед вызовом функции f2() стороной A сторона B должна вызвать 
функцию f1() и переслать результаты ее работы стороне B.
*******************************************************************************
*/

/*!
*******************************************************************************
\def OCTET_ORDER
\brief Порядок октетов в машинном слове

\def LITTLE_ENDIAN
\brief Порядок "от младших к старшим"

\def BIG_ENDIAN
\brief Порядок "от старших к младшим"
*******************************************************************************
*/

#ifndef LITTLE_ENDIAN
	#define LITTLE_ENDIAN 1234
#endif

#ifndef BIG_ENDIAN
	#define BIG_ENDIAN 4321
#endif

#if defined(__M_IX86) || defined(_X86_) || defined(i386) ||\
	defined(__i386__) || defined(_M_I86)  || defined(_M_IX86) ||\
	defined(_M_IA64) || defined(__ia64__) || defined(_M_X64) ||\
	defined(_M_AMD64) || defined(__amd64__) || defined(__amd64) ||\
	defined(__x86_64__) ||\
	defined(_M_ALPHA) || defined(__alpha__) || defined(__alpha) ||\
    defined(__arm__) ||\
    defined(__MIPS__) || defined(__mips__) || defined(__mips) ||\
	defined(__OS2__) || defined(sun386) || defined(__TURBOC__) ||\
	defined(vax) || defined(vms) || defined(VMS) || defined(__VMS)
	#define OCTET_ORDER LITTLE_ENDIAN
#elif defined(AMIGA) || defined(applec) || defined(__AS400__) ||\
	defined(_CRAY) || defined(__hppa) || defined(__hp9000) ||\
	defined(ibm370) || defined(mc68000) || defined(m68k) ||\
	defined(__MRC__) || defined(__MVS__) || defined(__MWERKS__) ||\
	defined(sparc) || defined(__sparc) || defined(SYMANTEC_C) ||\
	defined(__VOS__) || defined(__TIGCC__) || defined(__TANDEM) ||\
	defined(THINK_C) || defined(__VMCMS__) || defined(_AIX)
	#define OCTET_ORDER BIG_ENDIAN
#else
	#error "Platform undefined"
#endif

/*!
*******************************************************************************
\def OS_WINDOWS
\brief Операционная система линейки Windows

\def OS_UNIX
\brief Операционная система линейки Unix
\remark Включает линейки Linux и MAC OS X

\def OS_LINUX
\brief Операционная система линейки Linux

\def OS_MAC
\brief Операционная система линейки MAC OS X
*******************************************************************************
*/

#if defined(_WIN32) || defined(_WIN64) || defined(_WINNT) ||\
	defined(__WIN32__) || defined(__WIN64__)  || defined(__WINNT__)
	#define OS_WINDOWS
	#undef OS_UNIX
	#undef OS_LINUX
	#undef OS_MAC
#elif defined(unix) || defined(_unix_) || defined(__unix__)
	#undef OS_WINDOWS
	#define OS_UNIX
	#if defined(__unix__)
		#define OS_LINUX
		#undef OS_MAC
	#elif defined(__APPLE__) || defined(__MACH__)
		#undef OS_LINUX
		#define OS_MAC
	#endif
#else
	#undef OS_WINDOWS
	#undef OS_UNIX
	#undef OS_LINUX
	#undef OS_MAC
#endif

/*!
*******************************************************************************
\typedef octet
\brief Октет

\typedef uint32
\brief 32-разрядное беззнаковое целое

\typedef uint16
\brief 16-разрядное беззнаковое целое

\typedef uint8
\brief 8-разрядное беззнаковое целое

\typedef int32
\brief 32-разрядное знаковое целое

\typedef int16
\brief 16-разрядное знаковое целое

\typedef int8
\brief 8-разрядное знаковое целое

\def B_PER_W
\brief Число битов в машинном слове

\def O_PER_W
\brief Число октетов в машинном слове

\def B_PER_S
\brief Число битов в size_t

\def O_PER_S
\brief Число октетов в size_t

\typedef dword
\brief Двойное машинное слово

\typedef word
\brief Машинное слово
*******************************************************************************
*/

#if (UCHAR_MAX == 255u)
	typedef unsigned char uint8;
	typedef signed char int8;
	typedef uint8 octet;
#else
	#error "Unsupported char size"
#endif

#if (USHRT_MAX == 65535u)
	typedef unsigned short uint16;
	typedef signed short int16;
#else
	#error "Unsupported short size"
#endif

#if (UINT_MAX == 65535u)
	#if (ULONG_MAX == 4294967295u)
		typedef unsigned long uint32;
		typedef signed long int32;
	#else
		#error "Unsupported int/long size"
	#endif
	#define B_PER_W 16
	typedef uint16 word;
	typedef uint32 dword;
#elif (UINT_MAX == 4294967295u)
	typedef unsigned int uint32;
	typedef signed int int32;
	#if (ULONG_MAX == 4294967295u)
		#if !defined(ULLONG_MAX) || (ULLONG_MAX == 4294967295u)
			#error "Unsupported int/long size"
		#elif (ULLONG_MAX == 18446744073709551615u)
			typedef unsigned long long uint64;
			typedef signed long long int64;
		#else
			#error "Unsupported int/long size"
		#endif
	#elif (ULONG_MAX == 18446744073709551615u)
		typedef unsigned long uint64;
		typedef signed long int64;
	#else
		#error "Unsupported int/long size"
	#endif
	#define B_PER_W 32
	typedef uint32 word;
	typedef uint64 dword;
#elif (UINT_MAX == 18446744073709551615u)
	#if (ULONG_MAX == 18446744073709551615u)
		#if !defined(ULLONG_MAX) || (ULLONG_MAX == 18446744073709551615u)
			#error "Unsupported int/long size"
		#elif (ULLONG_MAX == 340282366920938463463374607431768211455u)
			typedef unsigned long long uint128;
			typedef signed long long int128;
		#else
			#error "Unsupported int/long size"
		#endif
	#elif (ULONG_MAX == 340282366920938463463374607431768211455u)
		typedef unsigned long uint128;
		typedef signed long int128;
	#else
		#error "Unsupported int/long size"
	#endif
	#define B_PER_W 64
	typedef uint64 word;
	typedef uint128 dword;
#else
	#error "Unsupported int size"
#endif

#if (B_PER_W != 16 && B_PER_W != 32 && B_PER_W != 64)
	#error "Unsupported word size"
#endif

#if (SIZE_MAX == 65535u)
	#define B_PER_S 16
#elif (SIZE_MAX == 4294967295u)
	#define B_PER_S 32
#elif (SIZE_MAX == 18446744073709551615u)
	#define B_PER_S 64
#else
	#error "Unsupported size_t size"
#endif

#define O_PER_W (B_PER_W / 8)
#define O_PER_S (B_PER_S / 8)

#ifndef U32_1
	#define U32_1 ((uint32)1)
#endif

#ifndef U32_MAX
	#define U32_MAX ((uint32)(0 - U32_1))
#endif

#ifndef WORD_1
	#define WORD_1 ((word)1)
#endif

#ifndef WORD_MAX
	#define WORD_MAX ((word)(0 - WORD_1))
#endif

#define WORD_BIT_POS(pos) (WORD_1 << (pos))
#define WORD_BIT_HI WORD_BIT_POS(B_PER_W - 1)
#define WORD_BIT_HALF WORD_BIT_POS(B_PER_W / 2)

#ifndef SIZE_1
	#define SIZE_1 ((size_t)1)
#endif

/*
*******************************************************************************
Макросы конвертации
*******************************************************************************
*/

/*!	\brief Число октетов для размещения nb битов */
#define O_OF_B(nb) (((nb) + 7) / 8)

/*!	\brief Число машинных слов для размещения nb битов */
#define W_OF_B(nb) (((nb) + B_PER_W - 1) / B_PER_W)

/*!	\brief Число битов для размещения no октетов */
#define B_OF_O(no) ((no) * 8)

/*!	\brief Число машинных слов для размещения no октетов */
#define W_OF_O(no) (((no) + O_PER_W - 1) / O_PER_W)

/*!	\brief Число октетов для размещения nw машинных слов */
#define O_OF_W(nw) ((nw) * O_PER_W)

/*!	\brief Число битов для размещения nw машинных слов */
#define B_OF_W(nw) ((nw) * B_PER_W)

/*
*******************************************************************************
Булевы данные
*******************************************************************************
*/

/*!	\brief Булев тип */
typedef int bool_t;

#ifndef TRUE
	#define TRUE ((bool_t)1)
#endif

#ifndef FALSE
	#define FALSE ((bool_t)0)
#endif

/*
*******************************************************************************
Ошибки
*******************************************************************************
*/

/*!	\brief Коды ошибок
	\remark Высокоуровневые функции возвращают значения типа err_t.
	Возврат ERR_OK означает, что функция завершена успешно. Код ERR_MAX 
	зарезервирован для описания специальных особых ситуаций.
	Возврат других значений означает ошибку при выполнении функции.
*/
typedef uint32 err_t;

/*!	\brief Код успешного завершения */
#define ERR_OK	((err_t)0)

/*!	\brief Максимальный код ошибки */
#define ERR_MAX	((err_t)(0 - (err_t)1))

/*!
*******************************************************************************
\brief Невозможное событие

Событие, вероятность наступления которого 2^{-B_PER_IMPOSSIBLE}, считается 
невозможным.

\remark Э. Борель: "событие, вероятность которого ниже 
10^{-50} \approx 2^{-166}, не произойдет никогда, сколько бы возможностей 
ни представилось" [Probability and Life, 1962]. 
*******************************************************************************
*/

#define B_PER_IMPOSSIBLE 64

/*!
*******************************************************************************
\brief Интерфейс генерации

Функция интерфейса gen_i генерирует count октетов и записывает их в буфер buf.
При генерации может использоваться и изменяться вспомогательная память 
(состояние) state. 

Функция интерфейса gen_i интерпретируется как генератор с состоянием state.
Используются генераторы двух типов:
-	rng (random number generator): генераторы случайных или 
	псевдослучайных чисел;
-	ang (arbitrary number generator): генераторы произвольных чисел, 
	которые реализуют принцип "выбрать произвольным образом". Генерируемые 
	числа (октеты) могут строиться по меткам времени, значениям монотонного 
	счетчика, случайным или псевдослучайным числам. Числа могут использоваться 
	в криптографических протоколах для построения синхропосылок, нонсов, 
	затравочных значений (seed), "соли" (salt).
.
\pre Буфер buf корректен.
\pre Состояние state корректно.
\expect Состояние state поддерживается постоянным между последовательными 
обращениями к функции.
\expect Октеты, формируемые генераторами rng, обладают минимальным 
статистическим качеством: каждое значение встречается с примерно равной 
частотой.
\expect Октеты, формируемые генераторами ang, почти не повторяются 
(повторяются только с пренебрежимо малыми вероятностями или только 
на недостижимо больших интервалах наблюдения).
\remark Функция интерфейса gen_i всегда генерирует все запрашиваемые октеты. 
Ошибки при генерации не предусмотрены.
*******************************************************************************
*/

typedef void (*gen_i)(
	void* buf,			/*!< [out] случайные числа */
	size_t count,		/*!< [in] число октетов */
	void* state			/*!< [in/out] cостояние */
);

/*!
*******************************************************************************
\brief Интерфейс чтения

Функция интерфейса read_i читает данные из файла file в буфер [count]buf. 
По адресу read возвращается число прочитанных октетов.
\pre Буфер buf корректен.
\pre Указатель read корректен.
\pre Файл file корректен.
\return ERR_OK, если прочитано определенное число октетов (возможно меньшее 
count и возможно нулевое) и конец файла не достигнут, ERR_MAX, если прочитано 
меньше чем count октетов и достигнут конец файла, или другой код ошибки.
\remark Файл -- это произвольный массив или поток данных на произвольном 
устройстве. В качестве файла может выступать обычный дисковый файл, сетевое
соединение, источник случайности и т.д.
\remark Для файлов некоторых устройств ошибкой не считается ситуация, 
когда прочитано меньше чем count октетов. Данная ситуация может быть связана 
с ожиданием данных в канале связи.
\remark Передавая count == 0, можно проверить наличие файла.
*******************************************************************************
*/

typedef err_t (*read_i)(
	size_t* read,		/*!< [out] число прочитанных октетов */
	void* buf,			/*!< [out] прочитанные данные */
	size_t count,		/*!< [in] длина buf в октетах */
	void* file			/*!< [in/out] описание файла */
);

/*!
*******************************************************************************
\brief Интерфейс записи

Функция интерфейса write_i записывает буфер [count]buf в файл file.
По адресу written возвращается число прочитанных октетов.
\pre Указатель written корректен.
\pre Буфер buf корректен.
\pre Файл file корректен.
\return ERR_OK, если записаны все октеты buf, и код ошибки в противном случае.
*******************************************************************************
*/

typedef err_t (*write_i)(
	size_t* written,	/*!< [out] число записанных октетов */
	const void* buf,	/*!< [in] записываемые данные */
	size_t count,		/*!< [in] длина buf в октетах */
	void* file			/*!< [in/out] описание файла */
);

#endif /* __DEFS_H */
