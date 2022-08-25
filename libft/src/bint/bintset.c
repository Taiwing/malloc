/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bintset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 11:55:38 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/04 19:24:09 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		bintset_u64(t_bint b, uint64_t u)
{
	uint32_t	i;

	i = 0;
	while (u)
	{
		if (i == BINT_SIZE(b) - 1)
			return (BINT_FAILURE);
		b[++i] = (uint32_t)(u & 0xFFFFFFFF);
		u >>= 32;
	}
	SET_BINT_LEN(b, i);
	SET_BINT_SIGN(b, 0);
	return (BINT_SUCCESS);
}

int		bintset_i64(t_bint b, int64_t i)
{
	if (bintset_u64(b, i < 0 ? -i : i) == BINT_FAILURE)
		return (BINT_FAILURE);
	SET_BINT_SIGN(b, i < 0);
	return (BINT_SUCCESS);
}

/*
** Compute 2^exp and put it into b
** exp is only limited by UINT32_MAX and BINT_SIZE
** sign sets b sign
*/
int		bintset_pow2(t_bint b, uint32_t exp, uint32_t sign)
{
	uint32_t	i;
	uint32_t	index;

	index = exp / 32;
	if (index + 1 >= BINT_SIZE(b))
		return (BINT_FAILURE);
	i = 0;
	while (++i <= index + 1)
		b[i] = 0;
	b[index + 1] |= (uint32_t)1 << (exp % 32);
	SET_BINT_LEN(b, index + 1);
	SET_BINT_SIGN(b, !!sign);
	return (BINT_SUCCESS);
}

static int	bintset_big_pow10(t_bint b, const t_bint cur, uint32_t exp)
{
	int			ret;
	uint32_t	base[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	next[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	temp[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	ret = bintset_pow10(next, exp, 0);
	ret = ret == BINT_SUCCESS
		? bint_mult(base, (t_bint)g_pow10_big[9], (t_bint)g_pow10_big[9])
		: ret;
	ret = ret == BINT_SUCCESS ? bint_mult(temp, next, base) : ret;
	ret = ret == BINT_SUCCESS ? bint_mult(b, temp, cur) : ret;
	return (ret);
}

/*
** Compute 10^exp and put it into b, with: exp <= (2^13)-1
** So exp <= 8191 (wich is: 2^0+2^1+2^2+2^3+...+2^11+2^12)
** If exp is greater than (2^13)-1, then the recursion is
** gonna kick in and the current result will be multiplied to
** the result of the next call times 10^(2^13). This means that
** every recursion will handle the first 12 bits of the exponent
** and recurse if any are left.
** So, as for pow2, the only limits are UINT32_MAX and BINT_SIZE.
** The sign can be set with the sign parameter.
*/
int		bintset_pow10(t_bint b, uint32_t exp, uint32_t sign)
{
	uint32_t	i;
	uint32_t	cur[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	next[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (bintset_u64(cur, (uint64_t)g_pow10_u32[exp & 0x7]) == BINT_FAILURE)
		return (BINT_FAILURE);
	SET_BINT_SIGN(cur, !!sign);
	exp >>= 3;
	i = 0;
	while (exp && i < 10)
	{
		if (exp & 1)
		{
			if (bint_mult(next, cur, (t_bint)g_pow10_big[i]) == BINT_FAILURE
				|| bintcpy(cur, next) == BINT_FAILURE)
				return (BINT_FAILURE);
		}
		++i;
		exp >>= 1;
	}
	if (exp && bintset_big_pow10(b, cur, exp) == BINT_SUCCESS)
		return (BINT_SUCCESS);
	return (exp ? BINT_FAILURE : bintcpy(b, cur));
}

/*
** Set a bint bit mask. The result will be a bint of log2 == bitlen
** where all the bits are set to 1. For example a call with
** bitlen == BINT_MAX_LOG2 and sign == 0 will be equal to BINT_MAX.
*/
int		bintset_mask(t_bint b, uint32_t bitlen, uint32_t sign)
{
	uint32_t	bits;
	uint32_t	blocklen;

	bits = bitlen % 32;
	blocklen = (bitlen / 32) + (bits != 0);
	if (blocklen >= BINT_SIZE(b))
		return (BINT_FAILURE);
	SET_BINT_LEN(b, blocklen);
	SET_BINT_SIGN(b, !!sign);
	ft_memset((void *)(b + 1), 0xff, blocklen * sizeof(uint32_t));
	if (bits)
		b[blocklen] >>= 32 - bits;
	bintclean(b);
	return (BINT_SUCCESS);
}
