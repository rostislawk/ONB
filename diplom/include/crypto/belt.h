/*!
*******************************************************************************
\file belt.h
\brief Алгоритмы СТБ 34.101.31 (belt)
*//****************************************************************************
\author (С) Сергей Агиевич, http://apmi.bsu.by
\created 2012.12.18
\version 2014.10.28
*******************************************************************************
*/

#ifndef __BELT_H
#define __BELT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "defs.h"

/*!
*******************************************************************************
\file belt.h

Реализованы алгоритмы СТБ 34.101.31 (belt). При ссылках на алгоритмы
подразумеваются разделы СТБ 34.101.31-2011, в которых эти алгоритмы определены.

Дополнительно реализованы алгоритм ключезависимого хэширования (HMAC),
основанный на функции хэширования belt-hash, и алгоритм построения ключа
по паролю, основанный на HMAC(belt-hash). Эти алгоритмы определены
соответственно в СТБ 34.101.47 и в СТБ 34.101.45.

Основные алгоритмы объединяются в группы, которые определяют следующие
криптографические <i>механизмы</i>:
-	ECB ---  шифрование в режиме простой замены;
-	CBC ---  шифрование в режиме сцепления блоков;
-	CFB ---  шифрование в режиме гаммирования с обратной связью;
-	CTR ---  шифрование в режиме счетчика;
-	MAC ---  имитозащита на основе шифрования;
-	DWP ---  шифрование и имитозащита данных;
-	KWP ---  шифрование и имитозащита ключей;
-	Hash --- хэширование;
-	KRP ---  преобразование ключей;
-	HMAC --- имитозащита на основе хэширования.

В механизмах ECB, CBC, CFB, CTR, MAC, DWP, KWP, KRP используется ключ
из 32 октетов. Ключ такой длины может быть построен по ключу из 16
или 24 октетов с помощью функции beltExpand().

В механизме HMAC используется ключ произвольной длины. Рекомендуется
использовать ключ из 32 октетов.

В механизмах ECB, CBC, CFB, CTR, MAC, DWP, KWP данные обрабатываются
блоками по 16 октетов.

В механизмах Hash, HMAC данные обрабатываются блоками по 32 октета.

На вход основных и вспомогательных функций данные и ключи подаются
либо как void-массивы (неформатированные данные, по умолчанию),
либо как массивы uint32 (форматированные данные). Внутри функций
неформатированные данные форматируются перед использованием.
На платформах с соглашениями BIG_ENDIAN форматирование требует
дополнительного времени.

Каждый механизм реализуется связкой из нескольких функций. Эти функции 
используют общее состояние, указатель на которое передается в функции 
как дополнительный параметр. Имеются функция определения объема памяти 
для состояния (_keep). Память для состояния готовит вызывающая программа 
и она же отвечает за очистку памяти.

Состояние можно копировать как фрагмент памяти.

В связке обязательно имеется функция инициализации алгоритма (Start)
и одна или несколько функций обработки фрагментов данных и получения
результатов обработки (StepX).

Логика суффиксов функций StepX:
-	E -- encrypt (зашифровать);
-	D -- decrypt (расшифровать);
-	A -- authenticate (имитозащита);
-	I -- имитозащита открытых данных в режиме DWP;
-	H -- hashing (хэширование);
-	G -- get (получить имитовставку, хэш-значение или новый ключ).

Функции связки спроектированы как максимально простые и эффективные.
В частности, в этих функциях не проверяются входные данные.

Каждая связка покрывается высокоуровневой функцией, которая
обрабатывает все данные целиком. В высокоуровневых функциях есть
проверка входных данных.

\expect Общее состояние связки функций не изменяется вне этих функций.

\expect{ERR_BAD_INPUT} Все входные указатели действительны.

\pre Если не оговорено противное, то входные буферы функций связки 
не пересекаются.
*******************************************************************************
*/

/*
*******************************************************************************
Долговременные параметры
*******************************************************************************
*/

/*!	\brief Блок подстановки H

	Возвращается таблица значений блока подстановки H.
	\return Указатель на таблицу H.
	\remark Таблица используется для формирования начальных значений
	переменных некоторых алгоритмов, а также в тестовых примерах.
*/
const octet* beltGetH();

/*
*******************************************************************************
Расширение ключа (алгоритм 7.1.2)
*******************************************************************************
*/

/*!	\brief Расширение ключа

	Ключ [len]theta расширяется до ключа key.
	\pre len == 16 || len == 24 || len == 32.
	\remark Буферы key и theta могут пересекаться.
*/
void beltKeyExpand(
	octet key[32],			/*!< [out] расширенный ключ */
	const octet theta[],	/*!< [in] первоначальный ключ */
	size_t len				/*!< [in] длина theta в октетах */
);

/*!	\brief Расширение и форматирование ключа

	Ключ [len]theta форматируется и расширяется до ключа key.
	\pre len == 16 || len == 24 || len == 32.
	\remark Буферы key и theta могут пересекаться.
*/
void beltKeyExpand2(
	uint32 key[8],			/*!< [out] расширенный форматированный ключ */
	const octet theta[],	/*!< [in] первоначальный ключ */
	size_t len				/*!< [in] длина ключа в октетах */
);

/*
*******************************************************************************
Шифрование блока (алгоритмы 6.1.3, 6.1.4)
*******************************************************************************
*/

/*!	\brief Зашифрование блока

	Выполняется зашифрование блока данных block на форматированном ключе key.
	Результат зашифрования возвращается по адресу block.
*/
void beltBlockEncr(
	octet block[16],		/*!< [in/out] блок */
	const uint32 key[8]		/*!< [in] ключ */
);

/*!	\brief Зашифрование форматированного блока

	Выполняется зашифрование форматированного блока данных block
	на форматированном ключе key. Результат зашифрования возвращается
	по адресу block.
*/
void beltBlockEncr2(
	uint32 block[4],		/*!< [in/out] блок */
	const uint32 key[8]		/*!< [in] ключ */
);

/*!	\brief Расшифрование блока

	Выполняется расшифрование блока данных block на форматированном ключе key.
	Результат расшифрования возвращается по адресу block.
*/
void beltBlockDecr(
	octet block[16],		/*!< [in/out] блок */
	const uint32 key[8]		/*!< [in] ключ */
);

/*!	\brief Расшифрование форматированного блока

	Выполняется расшифрование форматированного блока данных block
	на форматированном ключе key. Результат расшифрования возвращается
	по адресу block.
*/
void beltBlockDecr2(
	uint32 block[4],		/*!< [in/out] блок */
	const uint32 key[8]		/*!< [in] ключ */
);

/*
*******************************************************************************
Шифрование в режиме простой замены (ECB, алгоритмы 6.2.3, 6.2.4)
*******************************************************************************
*/

/*!	\brief Длина состояния функций ECB

	Возвращается длина состояния (в октетах) функций шифрования в режиме ECB.
	\return Длина состояния.
*/
size_t beltECB_keep();

/*!	\brief Инициализация шифрования в режиме ECB

	По ключу [len]theta в state формируются структуры данных, необходимые для 
	шифрования в режиме ECB.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltECB_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltECBStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа в октетах */
);

/*!	\brief Зашифрование фрагмента в режиме ECB

	Буфер [count]buf зашифровывается в режиме ECB на ключе, размещенном 
	в state.
	\pre count >= 16.
	\expect beltECBStart() < beltECBStepE()*.
	\remark К функции можно обратиться несколько раз, передавая buf из полных
	блоков. Неполный блок можно передать только при последнем обращении.
	В этом случае выполняется "кража" блока (ciphertext stealing, CTS).
	\remark При организации последовательных обращений к функции следует
	учитывать, что буфер buf должен содержать не менее одного блока.
	Например для зашифрования 33 октетов можно зашифровать сначала 16 октетов,
	а затем еще 17. Но нельзя зашифровать сначала 32 октета, а затем еще 1.
*/
void beltECBStepE(
	void* buf,			/*!< [in/out] открытый текст / шифртекст */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Расшифрование в режиме ECB

	Буфер [count]buf расшифровывается в режиме ECB на ключе, размещенном 
	в state.
	\pre count >= 16.
	\expect beltECBStart() < beltECBStepD()*.
	\remark Сохраняются замечания по функции beltECBStepE().
*/
void beltECBStepD(
	void* buf,			/*!< [in/out] шифртекст / открытый текст */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Зашифрование в режиме ECB

	Буфер [count]src зашифровывается на ключе [len]theta октетов.
	Результат зашифрования размещается в буфере [count]dest.
	\expect{ERR_BAD_INPUT}
	-	len == 16 || len == 24 || len == 32;
	-	count >= 16.
	.
	\return ERR_OK, если данные успешно зашифрованы, и код ошибки
	в противном случае.
*/
err_t beltECBEncr(
	void* dest,				/*!< [out] шифртекст */
	const void* src,		/*!< [in] открытый текст */
	size_t count,			/*!< [in] число октетов текста */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа */
);

/*!	\brief Расшифрование в режиме ECB

	Буфер src из count октетов расшифровывается на ключе [len]theta.
	Результат расшифрования размещается в буфере dest.
	\expect{ERR_BAD_INPUT}
	-	len == 16 || len == 24 || len == 32;
	-	count >= 16.
	.
	\return ERR_OK, если данные успешно расшифрованы, и код ошибки
	в противном случае.
*/
err_t beltECBDecr(
	void* dest,				/*!< [out] открытый текст */
	const void* src,		/*!< [in] шифртекст */
	size_t count,			/*!< [in] число октетов текста */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа */
);

/*
*******************************************************************************
Шифрование в режиме сцепления блоков (CBC, алгоритмы 6.3.3, 6.3.4)
*******************************************************************************
*/

/*!	\brief Длина состояния функций CBC

	Возвращается длина состояния (в октетах) функций шифрования в режиме CBC.
	\return Длина состояние.
*/
size_t beltCBC_keep();

/*!	\brief Инициализация шифрования в режиме CBC

	По ключу [len]theta и синхропосылке iv в state формируются
	структуры данных, необходимые для шифрования в режиме CBC.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltCBC_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltCBCStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа в октетах */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*!	\brief Зашифрование в режиме CBC

	Буфер [count]buf зашифровывается в режиме CBC на ключе, размещенном 
	в state.
	\pre count >= 16.
	\expect beltCBCStart() < beltCBCStepE()*.
	\remark К функции можно обратиться несколько раз, передавая buf из полных
	блоков. Неполный блок можно передать только при последнем обращении.
	В этом случае выполняется "кража" блока (ciphertext stealing, CTS).
	\remark При организации последовательных обращений к функции следует
	учитывать, что буфер buf должен содержать не менее одного блока.
	Например для зашифрования 33 октетов можно зашифровать сначала 16 октетов,
	а затем еще 17. Но нельзя зашифровать сначала 32 октета, а затем еще 1.
*/
void beltCBCStepE(
	void* buf,			/*!< [in/out] открытый текст / шифртекст */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Расшифрование в режиме CBC

	Буфер [count]buf расшифровывается в режиме CBC на ключе, размещенном 
	в state.
	\pre count >= 16.
	\expect beltCBCStart() < beltCBCStepD()*.
	\remark Сохраняются замечания по функции beltCBCStepE().
*/
void beltCBCStepD(
	void* buf,			/*!< [in/out] шифртекст / открытый текст */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Зашифрование в режиме CBC

	Буфер [count]src зашифровывается на ключе [len]theta с использованием 
	синхропосылки iv. Результат зашифрования размещается в буфере [count]dest.
	\expect{ERR_BAD_INPUT}
	-	len == 16 || len == 24 || len == 32;
	-	count >= 16.
	.
	\return ERR_OK, если данные успешно зашифрованы, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltCBCEncr(
	void* dest,				/*!< [out] шифртекст */
	const void* src,		/*!< [in] открытый текст */
	size_t count,			/*!< [in] число октетов текста */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*!	\brief Расшифрование в режиме CBC

	Буфер [count]src расшифровывается на ключе [len]theta с использованием 
	синхропосылки iv. Результат расшифрования размещается в буфере [count]dest.
	\expect{ERR_BAD_INPUT}
	-	len == 16 || len == 24 || len == 32;
	-	count >= 16.
	.
	\return ERR_OK, если данные успешно расшифрованы, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltCBCDecr(
	void* dest,				/*!< [out] открытый текст */
	const void* src,		/*!< [in] шифртекст */
	size_t count,			/*!< [in] число октетов текста */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*
*******************************************************************************
Шифрование в режиме гаммирования с обратной связью (CFB,
алгоритмы 6.4.2, 6.4.3)
*******************************************************************************
*/

/*!	\brief Длина состояния функций CFB

	Возвращается длина состояния (в октетах) функций шифрования в режиме CFB.
	\return Длина состояния.
*/
size_t beltCFB_keep();

/*!	\brief Инициализация шифрования в режиме CFB

	По ключу [len]theta и синхропосылке iv в state формируются
	структуры данных, необходимые для шифрования в режиме CFB.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltCFB_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltCFBStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа в октетах */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*!	\brief Зашифрование в режиме CFB

	Буфер [count]buf зашифровывается в режиме CFB на ключе, размещенном 
	в state.
	\expect beltCFBStart() < beltCFBStepE()*.
*/
void beltCFBStepE(
	void* buf,			/*!< [in/out] открытый текст / шифртекст */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Расшифрование в режиме CFB

	Буфер [count]buf расшифровывается в режиме CFB на ключе, размещенном 
	в state.
	\expect beltCFBStart() < beltCFBStepD()*.
*/
void beltCFBStepD(
	void* buf,			/*!< [in/out] шифртекст / открытый текст */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Зашифрование в режиме CFB

	Буфер [count]src зашифровывается на ключе [len]theta с использованием 
	синхропосылки iv. Результат зашифрования размещается в буфере [count]dest.
	\expect{ERR_BAD_INPUT} len == 16 || len == 24 || len == 32.
	\return ERR_OK, если данные успешно зашифрованы, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltCFBEncr(
	void* dest,				/*!< [out] шифртекст */
	const void* src,		/*!< [in] открытый текст */
	size_t count,			/*!< [in] число октетов текста */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*!	\brief Расшифрование в режиме CFB

	Буфер [count]src расшифровывается на ключе [len]theta с использованием 
	синхропосылки iv. Результат расшифрования размещается в буфере [count]dest.
	\expect{ERR_BAD_INPUT} len == 16 || len == 24 || len == 32.
	\return ERR_OK, если данные успешно расшифрованы, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltCFBDecr(
	void* dest,				/*!< [out] открытый текст */
	const void* src,		/*!< [in] шифртекст */
	size_t count,			/*!< [in] число октетов текста */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*
*******************************************************************************
Шифрование в режиме счетчика (CTR, алгоритм 6.5.3)
*******************************************************************************
*/

/*!	\brief Длина состояния функций CTR

	Возвращается длина состояния (в октетах) функций шифрования в режиме CTR.
	\return Длина состояния.
*/
size_t beltCTR_keep();

/*!	\brief Инициализация шифрования в режиме CTR

	По ключу [len]theta и синхропосылке iv в state формируются
	структуры данных, необходимые для шифрования в режиме CTR.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltCTR_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltCTRStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа в октетах */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*!	\brief Зашифрование фрагмента в режиме CTR

	Буфер [count]buf зашифровывается в режиме CTR на ключе, размещенном 
	в state.
	\expect beltCTRStart() < beltCTRStepE()*.
*/
void beltCTRStepE(
	void* buf,			/*!< [in/out] открытый текст / шифртекст */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Расшифрование фрагмента в режиме CTR
	\remark Зашифрование в режиме CTR не отличается от расшифрования.
*/
#define beltCTRStepD beltCTRStepE

/*!	\brief Шифрование в режиме CTR

	Буфер [count]src зашифровывается или расшифровывается на ключе
	[len]theta с использованием синхропосылки iv. Результат шифрования 
	размещается в буфере [count]dest.
	\expect{ERR_BAD_INPUT} len == 16 || len == 24 || len == 32.
	\return ERR_OK, если шифрование завершено успешно, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltCTR(
	void* dest,				/*!< [out] шифртекст / открытый текст */
	const void* src,		/*!< [in] открытый текст / шифртекст */
	size_t count,			/*!< [in] число октетов текста */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*
*******************************************************************************
Имитозащита (MAC, алгоритм 6.6.3)
*******************************************************************************
*/

/*!	\brief Длина состояния функций MAC

	Возвращается длина состояния (в октетах) функций имитозащиты в режиме MAC.
	\return Длина состояния.
*/
size_t beltMAC_keep();

/*!	\brief Инициализация функций MAC

	По ключу [len]theta в state формируются структуры данных, необходимые для 
	имитозащиты в режиме MAC.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltMAC_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltMACStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа в октетах */
);

/*!	\brief Имитозащита фрагмента данных в режиме MAC

	Текущая имитовставка, размещенная в state, пересчитывается с учетом нового
	фрагмента данных [count]buf. Пересчет выполняется на ключе, также 
	размещенном в state.
	\expect beltMACStart() < beltMACStepA()*.
*/
void beltMACStepA(
	const void* buf,	/*!< [in] данные */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Определение имитовставки в режиме MAC

	Определяется окончательная имитовставка mac всех данных,
	обработанных до этого функцией beltMACStepA().
	\expect (beltMACStepA()* < beltMACStepG())*.
	\remark Если продолжение имитозащиты не предполагается, то буферы 
	mac и state могут пересекаться.
*/
void beltMACStepG(
	octet mac[8],		/*!< [out] имитовставка */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Имитозащита в режиме MAC

	На ключе [len]theta определяется имитовставка mac буфера [count]src.
	\expect{ERR_BAD_INPUT} len == 16 || len == 24 || len == 32.
	\return ERR_OK, если имитовставка успешно вычислена, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltMAC(
	octet mac[8],			/*!< [out] имитовставка */
	const void* src,		/*!< [in] данные */
	size_t count,			/*!< [in] число октетов данных */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа */
);

/*
*******************************************************************************
Шифрование и имитозащита данных (datawrap, DWP, алгоритмы 6.7.3, 6.7.4)
*******************************************************************************
*/

/*!	\brief Длина состояния функций DWP

	Возвращается длина состояния (в октетах) функций DWP.
	\return Длина состояния.
*/
size_t beltDWP_keep();

/*!	\brief Инициализация функций DWP

	По ключу [len]theta и по синхропосылке iv в state формируются
	структуры данных, необходимые для шифрования и имитозащиты в режиме DWP.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltDWP_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltDWPStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа в октетах */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*!	\brief Зашифрование критического фрагмента в режиме DWP

	Фрагмент критических данных [count]buf зашифровывается на ключе,
	размещенном в state. Результат зашифрования сохраняется в buf.
	\expect beltDWPStart() < beltDWPStepE()*.
*/
void beltDWPStepE(
	void* buf,			/*!< [in/out] критические данные */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Имитозащита открытого фрагмента в режиме DWP

	Текущая имитовставка, размещенная в state, пересчитывается с учетом нового
	фрагмента открытых данных [count]buf. Пересчет выполняется на ключе, 
	также размещенном в state.
	\expect beltDWPStart() < beltDWPStepI()*.
*/
void beltDWPStepI(
	const void* buf,	/*!< [in] открытые данные */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Имитозащита критического фрагмента в режиме DWP

	Текущая имитовставка, размещенная в state, пересчитывается с учетом нового
	фрагмента зашифрованных критических данных [count]buf. Пересчет выполняется 
	на ключе, также размещенном в state.
	\expect По адресу state зарезервировано beltDWP_keep() октетов.
	\expect beltDWPStepI()* < beltDWPStepA()*.
	\expect beltDWPStepE()* < beltDWPStepA()*.
*/
void beltDWPStepA(
	const void* buf,	/*!< [in/out] критические данные */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Определение имитовставки в режиме DWP

	Определяется окончательная имитовставка mac всех данных,
	обработанных до этого функциями beltMACStepI(), beltMACStepA().
	\expect beltDWPStepI()* < beltDWPStepG().
	\expect beltDWPStepA()* < beltDWPStepG().
*/
void beltDWPStepG(
	octet mac[8],		/*!< [out] имитовставка */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Расшифрование критического фрагмента в режиме DWP

	Фрагмент зашифрованных критических данных [count]buf расшифровывается 
	на ключе, размещенном в state.
	Результат расшифрования сохраняется в buf.
	\expect beltDWPStepG() < beltDWPStepD().
*/
void beltDWPStepD(
	void* buf,			/*!< [in/out] критические данные */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Установка защиты в режиме DWP

	На ключе [len]theta с использованием имитовставки iv устанавливается 
	защита критических данных [count1]src1 и открытых данных [count2]src1. 
	При установке защиты критические данные зашифровываются и сохраняются 
	в буфере [count1]dest. Кроме этого определяется имитовставка mac 
	пары (src1, src2).
	\expect{ERR_BAD_INPUT}
	-	len == 16 || len == 24 || len == 32;
	-	буферы dest и mac не пересекаются.
	.
	\return ERR_OK, если защита успешно установлена, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться, за исключением пересечения dest и mac.
*/
err_t beltDWPWrap(
	void* dest,				/*!< [out] зашифрованные критические данные */
	octet mac[8],			/*!< [out] имитовставка */
	const void* src1,		/*!< [in] критические данные */
	size_t count1,			/*!< [in] число октетов критических данных */
	const void* src2,		/*!< [in] открытые данные */
	size_t count2,			/*!< [in] число октетов открытых данных */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*!	\brief Снятие защиты в режиме DWP

	На ключе [len]theta октетов с использованием имитовставки iv
	снимается защита зашифрованных критических данных [count1]src1 
	и открытых данных [count2]src1. При снятии защиты проверяется
	целостность пары (src1, src2) с помощью имитовставки mac. Если
	целостность не нарушена, то данные src1 расшифровываются в буфер 
	[count1]dest.
	\expect{ERR_BAD_INPUT} len == 16 || len == 24 || len == 32.
	\return ERR_OK, если защита успешно снята, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltDWPUnwrap(
	void* dest,				/*!< [out] расшифрованные критические данные */
	const void* src1,		/*!< [in] зашифрованные критические данные */
	size_t count1,			/*!< [in] число октетов критических данных */
	const void* src2,		/*!< [in] открытые данные */
	size_t count2,			/*!< [in] число октетов открытых данных */
	const octet mac[8],		/*!< [in] имитовставка */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа */
	const octet iv[16]		/*!< [in] синхропосылка */
);

/*
*******************************************************************************
Шифрование и имитозащита ключей (keywrap, KWP, алгоритмы 6.8.3, 6.8.4)
*******************************************************************************
*/

/*!	\brief Длина состояния функций KWP

	Возвращается длина состояния (в октетах) функций KWP.
	\return Длина состояния.
*/
size_t beltKWP_keep();

/*!	\brief Инициализация функций KWP

	По ключу [len]theta в state формируются структуры данных,
	необходимые для шифрования и имитозащиты в режиме KWP.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltKWP_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltKWPStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа в октетах */
);

/*!	\brief Зашифрование в режиме KWP

	Буфер [count]buf зашифровывается на ключе, размещенном в state.
	Зашифрование определяется шагом 2 алгоритма 6.8.3 и состоит в 
	преобразовании переменной r, которая представляет buf. Результат
	зашифрования сохраняется в buf.
	\pre count >= 32.
	\expect beltKWPStart() < beltKWPStepE()*.
	\remark Зашифрование в режиме KWP рассматривается как отдельный алгоритм.
	Этот алгоритм может использоваться для защиты ключа key, если
	buf = key || header, где header -- заголовок key из 16 октетов.
	Этот алгоритм может использоваться и по-другому, например, для генерации
	одноразового личного ключа в СТБ 34.101.45.
	\remark Зашифрование в режиме KWP можно применять многократно. При первом
	обращении счетчик i меняется от 1 до 2n, при втором --- от 2n + 1 до 4n,
	и так далее (см. описание алгоритма 6.8.3). Многократное зашифрование
	в режиме KWP используется для генерации одноразового ключа в СТБ 34.101.45.
	\remark Расшифрование в режиме KWP корректно работает, только если
	зашифрование выполнено однократно.
*/
void beltKWPStepE(
	void* buf,			/*!< [in/out] открытый текст / шифртекст  */
	size_t count,		/*!< [in] число октетов текста */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Расшифрование в режиме KWP

	Буфер [count]buf расшифровывается на ключе, размещенном в state.
	Расшифрование определяется шагом 3 алгоритма 6.9.3 и состоит
	в преобразовании переменной r, которая представляет buf. Результат
	расшифрования сохраняется в buf.
	\pre count >= 32.
	\expect beltKWPStart() < beltKWPStepD().
	\remark Сохраняются замечания к функции beltKWPStepE() с точностью до
	направления шифрования.
*/
void beltKWPStepD(
	void* buf,			/*!< [in/out] шифртекст / открытый текст */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Расшифрование в режиме KWP составного блока

	Буфер buf1 || buf2, где buf1 состоит из (count - 16) октетов, а buf2 
	состоит из 16 октетов, расшифровывается на ключе, размещенном в state.
	Расшифрование определяется шагом 3 алгоритма 6.9.3 и состоит 
	в преобразовании переменной r, которая представляет buf1 || buf2. 
	Результат расшифрования сохраняется в buf1 || buf2.
	\pre count >= 32.
	\pre Буферы buf1 и buf2 не пересекаются.
	\expect beltKWPStart() < beltKWPStepD2().
	\remark С помощью beltKWPStepD2() снятие защиты с ключа может быть 
	организовано по следующей схеме:
	-	первая часть ключа помещается в buf1;
	-	вторая часть ключа помещается в buf2;
	-	применяется beltKWPStepD2();
	-	если buf2 совпадает с заголовком ключ, то buf1 принимается как ключ.
	.
*/
void beltKWPStepD2(
	void* buf1,		/*!< [in/out] первая часть шифртекста / открытого текста */
	void* buf2,		/*!< [in/out] вторая часть шифртекста / открытый текста */
	size_t count,	/*!< [in] число октетов данных */
	void* state		/*!< [in/out] состояние */
);

/*!	\brief Установка защиты в режиме KWP

	На ключе [len]theta устанавливается защита ключа [count]src с заголовком 
	header. В результате определяется защищенный ключ [count + 16]dest.
	\expect{ERR_BAD_INPUT}
	-	len == 16 || len == 24 || len == 32;
	-	count >= 16.
	.
	\return ERR_OK, если защита успешно установлена, и код ошибки 
	в противном случае.
	\remark При нулевом указателе header используется нулевой заголовок.
	\remark Буферы могут пересекаться.
*/
err_t beltKWPWrap(
	octet dest[],			/*!< [out] защищенный ключ */
	const octet src[],		/*!< [in] защищаемый ключ */
	size_t count,			/*!< [in] длина src в октетах */
	const octet header[16],	/*!< [in] заголовок ключа */
	const octet theta[],	/*!< [in] ключ защиты */
	size_t len				/*!< [in] длина theta в октетах */
);

/*!	\brief Снятие защиты в режиме KWP

	На ключе [len]theta снимается защита с ключа [count]src. В результате 
	определяется первоначальный ключ [count - 16]dest с заголовком header.
	\expect{ERR_BAD_INPUT} 
	-	len == 16 || len == 24 || len == 32;
	-	count >= 32.
	.
	\return ERR_OK, если защита успешно снята, и код ошибки в противном случае.
	\remark При нулевом указателе header используется нулевой заголовок.
	\remark Буферы могут пересекаться.
*/
err_t beltKWPUnwrap(
	octet dest[],			/*!< [out] ключ */
	const octet src[],		/*!< [in] защищенный ключ */
	size_t count,			/*!< [in] длина src в октетах */
	const octet header[16],	/*!< [in] заголовок ключа */
	const octet theta[],	/*!< [in] ключ защиты */
	size_t len				/*!< [in] длина theta в октетах */
);

/*
*******************************************************************************
Хэширование (Hash, алгоритм 6.9.3)
*******************************************************************************
*/

/*!	\brief Длина состояния функций хэширования

	Возвращается длина состояния (в октетах) функций хэширования.
	\return Длина состояния.
*/
size_t beltHash_keep();

/*!	\brief Инициализация функций хэширования

	В state формируются структуры данных, необходимые для хэширования.
	\pre По адресу state зарезервировано beltHash_keep() октетов.
*/
void beltHashStart(
	void* state			/*!< [out] состояние */
);

/*!	\brief Хэширование фрагмента данных

	Текущее хэш-значение, размещенное в state, пересчитывается с учетом нового
	фрагмента данных [count]buf.
	\expect beltHashStart() < beltHashStepH()*.
*/
void beltHashStepH(
	const void* buf,	/*!< [in] данные */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Определение хэш-значения

	Определяется окончательное хэш-значение hash всех данных,
	обработанных до этого функцией beltHashStepH().
	\expect (beltHashStepH()* < beltHashStepG())*.
	\remark Если продолжение хэширования не предполагается, то буферы 
	hash и state могут пересекаться.
*/
void beltHashStepG(
	octet hash[32],		/*!< [out] хэш-значение */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Определение хэш-значения заданной длины

	Определяется окончательное хэш-значение [hash_len]hash 
	всех данных, обработанных до этого функцией beltHashStepH().
	Возвращаемое хэш-значение состоит из первых hash_len октетов 
	полного хэш-значения.
	\pre hash_len <= 32.
	\expect (beltHashStepH()* < beltHashStepG2())*.
	\remark Если продолжение хэширования не предполагается, то буферы 
	hash и state могут пересекаться.
*/
void beltHashStepG2(
	octet hash[],		/*!< [out] хэш-значение */
	size_t hash_len,	/*!< [out] длина hash */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Хэширование

	Определяется хэш-значение hash буфера [count]src.
	\return ERR_OK, если хэширование успешно завершено, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltHash(
	octet hash[32],		/*!< [out] хэш-значение */
	const void* src,	/*!< [in] данные */
	size_t count		/*!< [in] число октетов данных */
);

/*
*******************************************************************************
Преобразование ключа (KeyRep, алгоритм 7.2.3)
*******************************************************************************
*/

/*!	\brief Длина состояния функций преобразования ключа

	Возвращается длина состояния (в октетах) функций преобразования ключа.
	\return Длина состояния.
*/
size_t beltKRP_keep();

/*!	\brief Инициализация функций преобразования ключа

	По ключу [len]theta, который принадлежит уровню level, в state формируются 
	структуры данных, необходимые для преобразования этого ключа.
	\pre len == 16 || len == 24 || len == 32.
	\pre По адресу state зарезервировано beltKRP_keep() октетов.
	\remark Буферы theta и state могут пересекаться.
*/
void beltKRPStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len,				/*!< [in] длина ключа в октетах */
	const octet level[12]	/*!< [in] уровень */
);

/*!	\brief Тиражирование ключа

	Ключ [len]theta, размещенный в state функцией beltKRPStart(), 
	преобразуется в ключ [ke_len]key, который имеет заголовок header.
	\pre (key_len == 16 || key_len == 24 || key_len == 32) && key_len <= len.
	\expect beltKRPStart() < beltKRPStepG()*.
*/
void beltKRPStepG(
	octet key[],			/*!< [out] преобразованный ключ */
	size_t key_len,			/*!< [in] длина key в октетах */
	const octet header[16],	/*!< [in] заголовок key */
	void* state				/*!< [in/out] состояние */
);

/*!	\brief Преобразование ключа

	По ключу [n]src, который имеет уровень level, строится ключ [m]dest, 
	который имеет заголовок header.
	\expect{ERR_BAD_INPUT} 
	-	n == 16 || n == 24 || n == 32;
	-	m == 16 || m == 24 || m == 32;
	-	m <= n.
	.
	\return ERR_OK, если преобразование успешно завершено, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltKRP(
	octet dest[],				/*!< [out] преобразованный ключ */
	size_t m,					/*!< [in] длина dest в октетах */
	const octet src[],			/*!< [in] исходный ключ */
	size_t n,					/*!< [in] длина src в октетах */
	const octet level[12],		/*!< [in] уровень ключа src */
	const octet header[16]		/*!< [in] заголовок ключа dest */
);

/*
*******************************************************************************
Ключезавимое хэширование (HMAC, алгоритм 6.1.4 СТБ 34.101.47)
*******************************************************************************
*/

/*!	\brief Длина состояния функций HMAC

	Возвращается Длина состояния (в октетаъ) функций имитозащиты в режиме MAC.
	\return Длина состояния.
*/
size_t beltHMAC_keep();

/*!	\brief Инициализация функций HMAC

	По ключу [len]theta в state формируются структуры данных, необходимые для 
	имитозащиты в режиме HMAC.
	\pre По адресу state зарезервировано beltHMAC_keep() октетов.
	\remark Рекомендуется использовать ключ из 32 октетов.
*/
void beltHMACStart(
	void* state,			/*!< [out] состояние */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа в октетах */
);

/*!	\brief Имитозащита фрагмента данных в режиме HMAC

	Текущая имитовставка, размещенная в state, пересчитывается с учетом нового
	фрагмента данных [count]buf. Пересчет выполняется на ключе,
	также размещенном в state.
	\expect beltHMACStart() < beltHMACStepA()*.
*/
void beltHMACStepA(
	const void* buf,	/*!< [in] данные */
	size_t count,		/*!< [in] число октетов данных */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Определение имитовставки в режиме HMAC

	Определяется окончательная имитовставка mac всех данных,
	обработанных до этого функцией beltHMACStepA().
	\expect (beltHMACStepA()* < beltHMACStepG())*.
	\remark Рекомендуется использовать ключ из 32 октетов.
*/
void beltHMACStepG(
	octet mac[32],		/*!< [out] имитовставка */
	void* state			/*!< [in/out] состояние */
);

/*!	\brief Имитозащита в режиме HMAC

	На ключе [len]theta определяется имитовставка mac буфера [count]src.
	\return ERR_OK, если имитовставка успешно вычислена, и код ошибки
	в противном случае.
	\remark Буферы могут пересекаться.
*/
err_t beltHMAC(
	octet mac[32],			/*!< [out] имитовставка */
	const void* src,		/*!< [in] данные */
	size_t count,			/*!< [in] число октетов данных */
	const octet theta[],	/*!< [in] ключ */
	size_t len				/*!< [in] длина ключа */
);

/*
*******************************************************************************
Построение ключа по паролю (алгоритм E.2.3 СТБ 34.101.45)
*******************************************************************************
*/

/*!	\brief Построение ключа по паролю

	По паролю [pwd_len]pwd строится ключ theta. Для усложнения словарных атак 
	используется синхропосылка [salt_len]salt. Для усложнения перебора паролей 
	ключ пересчитывается iter > 0 раз.
	\expect{ERR_BAD_INPUT} iter != 0.
	\return ERR_OK, если ключ успешно построен, и код ошибки
	в противном случае.
	\remark Рекомендуется использовать iter >= 10000 и salt_len >= 8.
	\remark Реализованный алгоритм соответствует PKCS#5 (механизм PBKDF2).
*/
err_t beltKeyDerive(
	octet theta[32],		/*!< [out] ключ */
	const octet pwd[],		/*!< [in] пароль */
	size_t pwd_len,			/*!< [in] длина пароля (в октетах) */
	size_t iter,			/*!< [in] число итераций */
	const octet salt[],		/*!< [in] синхропосылка ("соль") */
	size_t salt_len			/*!< [in] длина синхропосылки (в октетах) */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BELT_H */
