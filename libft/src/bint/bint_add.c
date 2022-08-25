/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 12:27:08 by yforeau           #+#    #+#             */
/*   Updated: 2021/07/06 21:12:46 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

/*
** Add l to r and put the result in res without considering the sign
*/
int			bint_add_abs(t_bint res, const t_bint l, const t_bint r)
{
	uint32_t	i;
	uint64_t	sum;
	t_bint		small;
	t_bint		large;

	small = BINT_LEN(l) > BINT_LEN(r) ? r : l;
	large = BINT_LEN(l) > BINT_LEN(r) ? l : r;
	if (BINT_LEN(large) >= BINT_SIZE(res))
		return (BINT_FAILURE);
	for (i = 1, sum = 0; i <= BINT_LEN(large); ++i)
	{
		sum += (uint64_t)large[i];
		sum += i <= BINT_LEN(small) ? (uint64_t)small[i] : 0;
		res[i] = sum & 0xFFFFFFFF;
		sum >>= 32;
	}
	if (sum && i >= BINT_SIZE(res))
		return (BINT_FAILURE);
	else if (sum)
		res[i] = sum;
	SET_BINT_LEN(res, i - (!sum));
	bintclean(res);
	return (BINT_SUCCESS);
}

/*
** Add l to r and put the result in res
*/
int			bint_add(t_bint res, const t_bint l, const t_bint r)
{
	int	cmp;

	if (BINT_SIGN(l) == BINT_SIGN(r))
	{
		if (bint_add_abs(res, l, r) == BINT_FAILURE)
			return (BINT_FAILURE);
		SET_BINT_SIGN(res, BINT_SIGN(l));
		return (BINT_SUCCESS);
	}
	else if (!(cmp = bintcmp_abs(l, r)))
	{
		bintclr(res);
		return (BINT_SUCCESS);
	}
	else if (bint_sub_abs(res, l, r) == BINT_FAILURE)
		return (BINT_FAILURE);
	SET_BINT_SIGN(res, cmp > 0 ? BINT_SIGN(l) : BINT_SIGN(r));
	return (BINT_SUCCESS);
}

/*
** Add r to l and put the result in res
*/
int			bint_add_u64(t_bint res, const t_bint l, uint64_t r)
{
	uint32_t	cpy[3] = BINT_DEFAULT(3);

	if (!r)
		return (res != l ? bintcpy(res, l) : BINT_SUCCESS);
	else if (!BINT_LEN(l))
		return (bintset_u64(res, r));
	bintset_u64(cpy, r);
	return (bint_add(res, l, cpy));
}
