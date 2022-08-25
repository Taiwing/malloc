/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_sub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 14:15:59 by yforeau           #+#    #+#             */
/*   Updated: 2021/07/06 21:10:28 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

/*
** Substract r from l, or l from r and put the result in res without
** considering the sign. The smallest value is always gonna be substracted
** from the biggest one.
*/
int			bint_sub_abs(t_bint res, const t_bint l, const t_bint r)
{
	uint32_t	i;
	int			cmp;
	uint64_t	diff;
	t_bint		small;
	t_bint		large;

	cmp = bintcmp_abs(l, r);
	small = cmp > 0 ? r : l;
	large = cmp > 0 ? l : r;
	for (i = 1, diff = 0; i <= BINT_LEN(large); ++i)
	{
		diff = (uint64_t)large[i] - diff;
		diff -= i <= BINT_LEN(small) ? (uint64_t)small[i] : 0;
		if (i < BINT_SIZE(res))
			res[i] = diff & 0xFFFFFFFF;
		else if (diff)
			return (BINT_FAILURE);
		diff = (diff >> 32) & 1;
	}
	SET_BINT_LEN(res, BINT_LEN(large) < BINT_SIZE(res)
		? BINT_LEN(large) : BINT_SIZE(res) - 1);
	bintclean(res);
	return (BINT_SUCCESS);
}

/*
** Substract r from l and put the result in res
*/
int			bint_sub(t_bint res, const t_bint l, const t_bint r)
{
	int	cmp;

	if (BINT_SIGN(l) == BINT_SIGN(r))
	{
		if (!(cmp = bintcmp_abs(l, r)))
		{
			bintclr(res);
			return (BINT_SUCCESS);
		}
		else if (bint_sub_abs(res, l, r) == BINT_FAILURE)
			return (BINT_FAILURE);
		SET_BINT_SIGN(res, cmp > 0 ? BINT_SIGN(l) : !BINT_SIGN(r));
		return (BINT_SUCCESS);
	}
	else if (bint_add_abs(res, l, r) == BINT_FAILURE)
		return (BINT_FAILURE);
	SET_BINT_SIGN(res, BINT_SIGN(l));
	return (BINT_SUCCESS);
}

/*
** Substract r from l and put the result in res
*/
int			bint_sub_u64(t_bint res, const t_bint l, uint64_t r)
{
	uint32_t	cpy[3] = BINT_DEFAULT(3);

	if (!r)
		return (res != l ? bintcpy(res, l) : BINT_SUCCESS);
	else if (!BINT_LEN(l))
	{
		if (bintset_u64(res, r) == BINT_FAILURE)
			return (BINT_FAILURE);
		SET_BINT_SIGN(res, 1);
		return (BINT_SUCCESS);
	}
	bintset_u64(cpy, r);
	return (bint_sub(res, l, cpy));
}
