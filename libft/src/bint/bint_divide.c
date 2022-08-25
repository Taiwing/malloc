/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_divide.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 19:22:38 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/04 19:25:43 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

/*
** Divide dividend by divisor, return the quotient and the remainder where:
** quotient = floor(dividend/divisor)
** remainder = dividend - (divisor * floor(dividend/divisor))
**
** quotient can be NULL if we dont need it
**
** This computation is congruent with modular arithmetic.
*/
int	bint_divmod(t_bint quotient, t_bint remainder,
	const t_bint dividend, const t_bint divisor)
{
	if (!remainder)
		return (BINT_FAILURE);
	if (bint_divide(quotient, remainder, dividend, divisor) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (BINT_SIGN(dividend) == BINT_SIGN(divisor) || !BINT_LEN(remainder))
		return (BINT_SUCCESS);
	if (quotient)
		bint_sub_u64(quotient, quotient, 1);
	bint_add(remainder, remainder, divisor);
	return (BINT_SUCCESS);
}

static int	first_digit_divide(t_bint quotient, const t_bint numerator,
		const t_bint denominator)
{
	uint32_t	carry;
	uint32_t	len;
	uint32_t	d;
	uint64_t	n;

	bintcpy(quotient, BINT_ZERO);
	if (bintcmp_abs(numerator, denominator) < 0)
		return (BINT_SUCCESS);
	len = BINT_LEN(numerator) - BINT_LEN(denominator) + 1;
	if (BINT_SIZE(quotient) <= len)
		return (BINT_FAILURE);
	carry = 0;
	d = denominator[BINT_LEN(denominator)];
	for (uint32_t i = 0; i < len; ++i)
	{
		n = ((uint64_t)carry << 32) + numerator[BINT_LEN(numerator) - i];
		quotient[len - i] = n / d;
		carry = n % d;
	}
	SET_BINT_LEN(quotient, len);
	bintclean(quotient);
	SET_BINT_SIGN(quotient, BINT_SIGN(numerator) != BINT_SIGN(denominator));
	return (BINT_SUCCESS);
}

static int	init_fast_divide(t_bint n, t_bint d,
	const t_bint dividend, const t_bint divisor)
{
	if (bintcpy(n, dividend) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (bintcpy(d, divisor) == BINT_FAILURE)
		return (BINT_FAILURE);
	SET_BINT_SIGN(n, 0);
	SET_BINT_SIGN(d, 0);
	return (BINT_SUCCESS);
}

static int	fast_divide(t_bint q, t_bint r,
	const t_bint dividend, const t_bint divisor)
{
	int			ret;
	uint32_t	n[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	d[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	ret = init_fast_divide(n, d, dividend, divisor);
	ret = ret == BINT_SUCCESS ? first_digit_divide(q, n, d) : ret;
	while (bintcmp_abs(r, divisor) >= 0 && ret == BINT_SUCCESS)
	{
		ret = bint_mult(tmp, q, d);
		ret = ret == BINT_SUCCESS ? bint_sub(r, n, tmp) : ret;
		ret = ret == BINT_SUCCESS ? first_digit_divide(tmp, r, d) : ret;
		if (ret == BINT_SUCCESS && (BINT_LEN(tmp) > 1
			|| (BINT_LEN(tmp) == 1 && tmp[1] >= 2)))
			ret = bint_shiftright(tmp, 1);
		ret = ret == BINT_SUCCESS ? bint_add(q, q, tmp) : ret;
	}
	if (ret == BINT_SUCCESS && BINT_LEN(r) && BINT_SIGN(r))
	{
		ret = bint_sub_u64(q, q, 1);
		ret = ret == BINT_SUCCESS ? bint_add(r, r, d) : ret;
	}
	return (ret);
}

/*
** Divide dividend by divisor, return the quotient and the remainder where:
** quotient = dividend/divisor = quotient > 0 ? floor(quotient) : ceil(quotient)
** remainder = dividend - (divisor * quotient)
**
** quotient or remainder can be NULL if we dont need one or the other
**
** This computation is equivalent to the modulo operation of ANSI C.
*/
int	bint_divide(t_bint quotient, t_bint remainder,
	const t_bint dividend, const t_bint divisor)
{
	uint32_t	local_bint[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (!BINT_LEN(divisor) || (!quotient && !remainder))
		return (BINT_FAILURE);
	if (!quotient || !remainder)
	{
		quotient = !quotient ? local_bint : quotient;
		remainder = !remainder ? local_bint : remainder;
	}
	if (bintcpy(remainder, dividend) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (quotient)
		bintclr(quotient);
	if (bintcmp_abs(remainder, divisor) < 0)
		return (BINT_SUCCESS);
	if (fast_divide(quotient, remainder, dividend, divisor) == BINT_FAILURE)
		return (BINT_FAILURE);
	SET_BINT_SIGN(remainder, BINT_SIGN(dividend));
	SET_BINT_SIGN(quotient, BINT_SIGN(dividend) != BINT_SIGN(divisor));
	/*
	if (quotient != local_bint)
		bintclean(quotient);
	if (remainder != local_bint)
		bintclean(remainder);
	*/
	return (BINT_SUCCESS);
}
