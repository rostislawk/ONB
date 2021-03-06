/*!
*******************************************************************************
\file bit.h
\brief Манипуляции с битами
*//****************************************************************************
\author (С) Сергей Агиевич, http://apmi.bsu.by
\created 2014.07.18
\version 2014.10.09
*******************************************************************************
*/

#ifndef __BIT_H
#define __BIT_H

#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif


/*!
*******************************************************************************
\file bit.h

Реализованы быстрые манипуляции с битами машинных слов.

Использованы алгоритмы из следующих источников:
[1]	Уоррен Генри Мл. Алгоритмические трюки для программистов, 
	М.: Изательский дом <<Вильямс>>, 2003.
[2]	Andersen S.A. Bit Twidding Hacks. Avail. at:
	http://graphics.stanford.edu/~seander/bithacks.html, 1997-2005.

todo: safe = {bitCTZ, bitCLZ}
*******************************************************************************
*/

/*!	\brief Вес 

	Определяется число ненулевых битов в машинном слове w.
	\return Число ненулевых битов.
*/
size_t bitWeight(
	register word w		/*!< [in] машинное слово */
);

/*!	\brief Четность

	Определяется сумма по модулю 2 битов машинного слова w.
	\return Сумма битов.
*/
bool_t bitParity(
	register word w		/*!< [in] машинное слово */
);

/*!	\brief Число младших нулевых битов

	Определяется длина серии из нулевых битов в начале машинного слова w.
	\return Длина серии.
	\remark CTZ == Count of Trailing Zeros
*/
size_t bitCTZ(
	register word w		/*!< [in] машинное слово */
);

/*!	\brief Число старших нулевых битов

	Определяется длина серии из нулевых битов в конце машинного слова w.
	\return Длина серии.
	\remark CLZ == Count of Leading Zeros
*/
size_t bitCLZ(
	register word w		/*!< [in] машинное слово */
);

#endif /* __BIT_H */
