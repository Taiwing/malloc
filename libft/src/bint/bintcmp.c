/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bintcmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 11:42:05 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/16 11:45:33 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

/*
** Compare two bints' absolute value
*/
int		bintcmp_abs(const t_bint l, const t_bint r)
{
	int	i;

	if (BINT_LEN(l) != BINT_LEN(r))
		return (BINT_LEN(l) - BINT_LEN(r));
	i = BINT_LEN(l);
	while (i > 0 && l[i] == r[i])
		--i;
	if (i == 0)
		return (0);
	else
		return (l[i] > r[i] ? 1 : -1);
}

/*
** Compare two bints
*/
int		bintcmp(const t_bint l, const t_bint r)
{
	int	sign;

	sign = BINT_SIGN(l) ? -1 : 1;
	if (BINT_SIGN(l) != BINT_SIGN(r))
		return (sign);
	return (bintcmp_abs(l, r) * sign);
}
