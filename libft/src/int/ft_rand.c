/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 17:59:48 by yforeau           #+#    #+#             */
/*   Updated: 2022/01/06 04:49:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define BUF_SIZE	4096
#define	BUF_BYTES	(BUF_SIZE * sizeof(uint64_t))
#define START_STATE 0xACE1u

/*
** ft_rand: pseudo-randomness without randomness source
**
** This function just cycles through bitwise operations on an unsigned
** integer. The numbers look random but since it's entirely deterministic
** the values can be guessed (so this should *not* be used for security
** purposes, encryption, etc...).
*/
unsigned int	ft_rand(void)
{
#ifdef THREAD_SAFE
	static __thread unsigned int	lfsr = START_STATE;
#else
	static unsigned int				lfsr = START_STATE;
#endif
	unsigned int		bit;

	bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5));
	lfsr = (lfsr >> 1) | (bit << 15);
	return (lfsr);
}

/*
** ft_rand_uint64: get random 64-bit unsinged integer
**
** n: uint64_t destination pointer
** min: lowest possible value (inclusive)
** max: highest possible value (inclusive)
**
** return: store the value in the given n pointer
** or NULL on error or on bad input
*/
uint64_t		*ft_rand_uint64(uint64_t *n, uint64_t min, uint64_t max)
{
	uint64_t					dist;
#ifdef THREAD_SAFE
	static __thread int			i = BUF_SIZE;
	static __thread uint64_t	buf[BUF_SIZE];
#else
	static int					i = BUF_SIZE;
	static uint64_t				buf[BUF_SIZE];
#endif

	if (!n || max < min)
		return (NULL);
	else if (!(dist = max - min))
	{
		*n = min;
		return (n);
	}
	if (i == BUF_SIZE)
	{
		if (!ft_rand_source((void *)buf, BUF_BYTES, NULL, 0))
			return (NULL);
		i = 0;
	}
	*n = buf[i++];
	if (dist < UINT64_MAX)
		*n %= dist + 1;
	*n += min;
	return (n);
}
