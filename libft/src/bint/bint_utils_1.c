/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 14:20:03 by yforeau           #+#    #+#             */
/*   Updated: 2021/07/06 21:16:26 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bint.h"

int			bintcpy(t_bint dst, const t_bint src)
{
	uint32_t	i;

	if (BINT_LEN(src) > BINT_SIZE(dst) - 1)
		return (BINT_FAILURE);
	i = 0;
	while (i < BINT_LEN(src))
	{
		++i;
		dst[i] = src[i];
	}
	SET_BINT_LEN(dst, i);
	SET_BINT_SIGN(dst, BINT_SIGN(src));
	return (BINT_SUCCESS);
}

void		bintclean(t_bint b)
{
	uint32_t	len;

	for (len = BINT_LEN(b); len > 0 && !b[len]; --len);
	SET_BINT_LEN(b, len);
}

void		bintclr(t_bint b)
{
	SET_BINT_LEN(b, 0);
	SET_BINT_SIGN(b, 0);
}

uint32_t	bintlog2(t_bint b)
{
	int			log2;
	uint32_t	hi_block;

	if (!BINT_LEN(b))
		return (1);
	log2 = ((BINT_LEN(b) - 1) * 32);
	hi_block = b[BINT_LEN(b)];
	while (hi_block)
	{
		++log2;
		hi_block >>= 1;
	}
	return (log2);
}
