/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_mult.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 13:12:45 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/04 19:36:53 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

/*
** Standard multiplication algorithm
**
** s is the smaller number and l the larger one
*/
static int	long_multiplication(t_bint res, const t_bint s, const t_bint l)
{
	uint64_t	prod;
	uint32_t	max;
	t_bint		r;

	if ((max = BINT_LEN(s) + BINT_LEN(l)) >= BINT_SIZE(res))
		return (BINT_FAILURE);
	r = res;
	for (uint32_t i = 1, j; i <= BINT_LEN(s); ++i, ++r)
	{
		if (!s[i])
			continue;
		prod = 0;
		for (j = 1; j <= BINT_LEN(l); ++j)
		{
			prod += r[j] + (l[j] * (uint64_t)s[i]);
			r[j] = prod & 0xFFFFFFFF;
			prod >>= 32;
		}
		r[j] = (uint32_t)(prod & 0xFFFFFFFF);
	}
	SET_BINT_LEN(res, (max > 0 && res[max] == 0 ? max - 1 : max));
	SET_BINT_SIGN(res, BINT_SIGN(s) != BINT_SIGN(l));
	return (BINT_SUCCESS);
}

#ifdef KARATSUBA_MULT
static int	bint_split_at(t_bint high, t_bint low,
	const t_bint orig, uint32_t at)
{
	if (at > BINT_LEN(orig) || at >= BINT_SIZE(low)
		|| BINT_LEN(orig) - at >= BINT_SIZE(high))
		return (BINT_FAILURE);
	bintclr(low);
	bintclr(high);
	if (at == BINT_LEN(orig))
		return (bintcpy(low, orig));
	SET_BINT_LEN(low, at);
	SET_BINT_LEN(high, BINT_LEN(orig) - at);
	for (uint32_t i = 1; i <= at; ++i)
		low[i] = orig[i];
	for (uint32_t i = at + 1; i <= BINT_LEN(orig); ++i)
		high[i - at] = orig[i];
	return (BINT_SUCCESS);
}

static int	karatsuba_result(t_bint res, uint32_t z[3][BINT_SIZE_DEF],
	uint32_t m, uint32_t sign)
{
	if (bint_sub(z[1], z[1], z[2]) == BINT_FAILURE
		|| bint_sub(z[1], z[1], z[0]) == BINT_FAILURE
		|| bint_shiftleft(z[1], m * 32) == BINT_FAILURE
		|| bint_shiftleft(z[2], m * 2 * 32) == BINT_FAILURE
		|| bint_add(z[1], z[0], z[1]) == BINT_FAILURE
		|| bint_add(res, z[1], z[2]) == BINT_FAILURE)
		return (BINT_FAILURE);
	SET_BINT_SIGN(res, sign);
	return (BINT_SUCCESS);
}

/*
** Karatsuba multiplication algorithm: recursive divide and conquer
**
** This is not used as of yet because this way too slow, actually
** it is slower than the original which is obviously not the goal.
** I really don't know how I could optimize this function... Maybe
** with multithreading... Who knows ? Anyway, it seems way out of
** scope for what I need it for, so this will have to wait.
*/
static int	karatsuba(t_bint res, const t_bint s, const t_bint l)
{
	uint32_t	high[2][BINT_SIZE_DEF] = { [ 0 ... 1 ] = BINT_DEFAULT(0) };
	uint32_t	low[2][BINT_SIZE_DEF] = { [ 0 ... 1 ] = BINT_DEFAULT(0) };
	uint32_t	z[3][BINT_SIZE_DEF] = { [ 0 ... 2 ] = BINT_DEFAULT(0) };
	int			cmp;
	uint32_t	m;

	if (BINT_LEN(l) < 10 || BINT_LEN(s) < 2)
		return (long_multiplication(res, s, l));
	m = (BINT_LEN(s) / 2) + (BINT_LEN(s) % 2);
	if (bint_split_at(high[0], low[0], s, m) == BINT_FAILURE
		|| bint_split_at(high[1], low[1], l, m) == BINT_FAILURE)
		return (BINT_FAILURE);
	cmp = bintcmp_abs(low[1], low[0]);
	if (karatsuba(z[0], low[cmp <= 0], low[cmp > 0]) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (karatsuba(z[2], high[0], high[1]) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (bint_add_abs(low[0], low[0], high[0]) == BINT_FAILURE
		|| bint_add_abs(low[1], low[1], high[1]) == BINT_FAILURE)
		return (BINT_FAILURE);
	cmp = bintcmp_abs(low[1], low[0]);
	if (karatsuba(z[1], low[cmp <= 0], low[cmp > 0]) == BINT_FAILURE)
		return (BINT_FAILURE);
	return (karatsuba_result(res, z, m, BINT_SIGN(s) != BINT_SIGN(l)));
}

#endif

/*
** Multiply l by r and put the result in res
*/
int			bint_mult(t_bint res, const t_bint l, const t_bint r)
{
	int				cmp;
	t_bint			small;
	t_bint			large;
	uint32_t		tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	cmp = bintcmp_abs(l, r);
	small = cmp < 0 ? l : r;
	large = cmp >= 0 ? l : r;
#ifndef KARATSUBA_MULT
	if (long_multiplication(tmp, small, large) == BINT_FAILURE)
		return (BINT_FAILURE);
#else
	if (karatsuba(tmp, small, large) == BINT_FAILURE)
		return (BINT_FAILURE);
#endif
	return (bintcpy(res, tmp));
}
