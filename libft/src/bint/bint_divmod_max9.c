/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_divmod_max9.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 15:00:29 by yforeau           #+#    #+#             */
/*   Updated: 2021/05/11 13:06:13 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

static void	substract_bintxu32(t_bint res, const t_bint b, uint32_t q)
{
	uint32_t	i;
	uint64_t	prod;
	uint64_t	diff;

	i = 0;
	prod = 0;
	diff = 0;
	while (++i <= BINT_LEN(b))
	{
		prod += (uint64_t)b[i] * (uint64_t)q;
		diff = (uint64_t)res[i] - (prod & 0xFFFFFFFF) - diff;
		res[i] = diff & 0xFFFFFFFF;
		prod >>= 32;
		diff = (diff >> 32) & 1;
	}
	while (BINT_LEN(res) && !res[BINT_LEN(res)])
		SET_BINT_LEN(res, BINT_LEN(res) - 1);
}

/*
** Divide dividend by divisor, return the quotient and update
** dividend to equal the remainder where:
** quotient = floor(dividend/divisor)
** remainder = dividend - quotient*divisor
**
** quotient is assumed to be in range [0,10)
** dividend does not contain more blocks than divisor (same length)
** dividend's highest (most significant) must be at least 8 for precision
** dividend and divisor are expected to be positive
**
** 10 is the error return
*/
uint32_t		bint_divmod_max9(t_bint dividend, const t_bint divisor)
{
	uint32_t	quotient;

	if (!BINT_LEN(divisor) || divisor[BINT_LEN(divisor)] < 8
		|| divisor[BINT_LEN(divisor)] > 0xFFFFFFFE
		|| BINT_LEN(dividend) > BINT_LEN(divisor)
		|| BINT_SIGN(dividend) || BINT_SIGN(divisor))
		return (10);
	if (BINT_LEN(divisor) > BINT_LEN(dividend))
		return (0);
	quotient = dividend[BINT_LEN(dividend)]
			/ ((divisor[BINT_LEN(divisor)]) + 1);
	if (quotient)
		substract_bintxu32(dividend, divisor, quotient);
	if (bintcmp(dividend, divisor) >= 0)
	{
		++quotient;
		substract_bintxu32(dividend, divisor, 1);
	}
	return (quotient);
}
