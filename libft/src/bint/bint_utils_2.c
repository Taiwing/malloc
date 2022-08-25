/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 14:20:22 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/13 16:35:45 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			bint_is_odd(const t_bint b)
{
	return (BINT_LEN(b) && (b[1] & 1));
}

int			bint_is_even(const t_bint b)
{
	return (!bint_is_odd(b));
}

static int	bint_rand_internal(t_bint n, const t_bint min, const t_bint max)
{
	uint32_t	dist[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	remainder[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (bint_sub_abs(dist, max, min) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (!ft_rand_source((void *)(n + 1),
		BINT_LEN(dist) * sizeof(uint32_t), NULL, 0))
		return (BINT_FAILURE);
	SET_BINT_LEN(n, BINT_LEN(dist));
	bintclean(n);
	if (bintcmp_abs(dist, BINT_MAX) < 0)
	{
		if (bint_add_u64(dist, dist, 1) == BINT_FAILURE)
			return (BINT_FAILURE);
		if (bint_divide(NULL, remainder, n, dist) == BINT_FAILURE
			|| bintcpy(n, remainder) == BINT_FAILURE)
			return (BINT_FAILURE);
	}
	return (bint_add_abs(n, n, min));
}

/*
** bint_rand:
**
** Generate a random number between min and max (inclusive). The min and
** max values will be treated as positive numbers (in other words, only
** their absolute value will be used). The number generated will be
** positive except if rand_sign is not zero, then the number will get a
** randomly selected sign.
*/
int			bint_rand(t_bint n, const t_bint min,
	const t_bint max, uint64_t rand_sign)
{
	int	cmp;
	int	sign;

	if ((cmp = bintcmp_abs(max, min)) < 0 || BINT_SIZE(n) < BINT_SIZE(max))
		return (BINT_FAILURE);
	sign = rand_sign ? 1 : 0;
	if (sign && !ft_rand_source((void *)&sign, sizeof(sign), NULL, 0))
		return (BINT_FAILURE);
	SET_BINT_SIGN(n, sign % 2);
	if (!cmp && bintcpy(n, min) == BINT_FAILURE)
		return (BINT_FAILURE);
	else if (!cmp)
		return (BINT_SUCCESS);
	return (bint_rand_internal(n, min, max));
}

int			bint_to_u64(uint64_t *res, const t_bint n)
{
	if (BINT_SIGN(n) || BINT_LEN(n) > 2)
		return (BINT_FAILURE);
	*res = BINT_LEN(n) ? (uint64_t)n[1] : 0;
	if (BINT_LEN(n) == 2)
		*res = *res + ((uint64_t)n[2] << 32);
	return (BINT_SUCCESS);
}
