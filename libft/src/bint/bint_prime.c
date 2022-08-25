/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_prime.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 22:44:18 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/05 12:27:14 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const char		g_plus[BINT_K_MAX] = { [ 0 ... BINT_K_MAX - 1 ] = '+' };


/*
** bint_miller_test: use miller-rabin algorithm to primality
**
** If *ret ends up equal to 0 then the test is passed, but if it is set to
** one then n cannot be prime. If bint_miller_test returns BINT_FAILURE ret
** value must be ignored.
*/

static int	bint_miller_test(const t_bint n, const t_bint d, uint64_t *ret)
{
	uint32_t	a[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	x[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	d_cpy[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (bint_sub(tmp, n, BINT_TWO) == BINT_FAILURE
		|| bint_rand(a, BINT_TWO, tmp, 0) == BINT_FAILURE
		|| bint_modexp(x, a, d, n) == BINT_FAILURE
		|| bint_sub(tmp, n, BINT_ONE) == BINT_FAILURE
		|| bintcpy(d_cpy, d) == BINT_FAILURE)
		return (BINT_FAILURE);
	*ret = !bintcmp(x, BINT_ONE) || !bintcmp(x, tmp) ? 0 : 2;
	while (*ret == 2 && bintcmp(d_cpy, tmp))
	{
		if (bint_modexp(x, x, BINT_TWO, n) == BINT_FAILURE)
			return (BINT_FAILURE);
		if (!bintcmp(x, BINT_ONE))
			*ret = 1;
		else if (!bintcmp(x, tmp))
			*ret = 0;
		else if (bint_shiftleft(d_cpy, 1) == BINT_FAILURE)
			return (BINT_FAILURE);
	}
	return (BINT_SUCCESS);
}

/*
** bint_is_prime: test if bint is probably prime (Miller-Rabin)
**
** n: bint to test
** k: number of time to repeat the test (the bigger, the more probable
**    it is that n is prime, with a maximum of 100)
** ret: k + 1 if n does not pass initial tests, k otherwise, if k == 0
**  it means that n passed k tests successfully and is considered prime
**  according to the Miller-Rabin algorithm. If k > 0 then n is not prime
**  (also the number of passed tests can be obtained by substracting actual
**  k to original k, ex: if orig_k == 12 and k == 3 then n passed
**  orig_k - k = 12 - 3 = 9 tests successfully before failing)
**
** If bint_is_prime returns BINT_FAILURE, then ret value must be ignored.
*/

int			bint_is_prime(t_bint n, uint64_t k, uint64_t *ret)
{
	uint32_t	d[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	k = k > BINT_K_MAX ? BINT_K_MAX : k;
	*ret = k + 1;
	if (bintcmp(n, BINT_FOUR) < 0)
		*ret = bintcmp(n, BINT_TWO) < 0 ? *ret: 0;
	else if (bint_is_odd(n))
	{
		if (bint_sub(d, n, BINT_ONE) == BINT_FAILURE)
			return (BINT_FAILURE);
		if (bint_shift_zeroes(d) == BINT_FAILURE)
			return (BINT_FAILURE);
		for (*ret = 0, ++k; !*ret && --k;)
			if (bint_miller_test(n, d, ret) == BINT_FAILURE)
				return (BINT_FAILURE);
		*ret = k;
	}
	return (BINT_SUCCESS);
}

static int	set_first_bit(t_bint n, uint64_t size)
{
	uint32_t	bit[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (bintset_pow2(bit, size, 0) == BINT_FAILURE)
		return (BINT_FAILURE);
	return (bint_add(n, n, bit));
}

/*
** bint_find_prime: Search for a bint prime number
**
** n: destination
** k: number of time to repeat the test (the bigger, the more probable
**    it is that n is prime, with a maximum of 100)
** size: bit length of the prime number we want
** print: show progress boolean
*/

int			bint_find_prime(t_bint n, uint64_t k, uint64_t size, uint64_t print)
{
	uint64_t	ret;
	uint32_t	max[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	k = k > BINT_K_MAX ? BINT_K_MAX : k;
	if ((BINT_SIZE(n) - 1) < ((size / 32) + !!(size % 32))
		|| bintset_mask(max, size, 0) == BINT_FAILURE
		|| bint_rand(n, BINT_ZERO, max, 0) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (bintlog2(n) < size && set_first_bit(n, size) == BINT_FAILURE)
		return (BINT_FAILURE);
	while (bintcmp(n, BINT_TWO) > 0
		&& bint_is_prime(n, k, &ret) == BINT_SUCCESS)
	{
		if (ret <= k && print)
			ft_dprintf(2, ".%.*s%s", (int)(k - ret), g_plus, !ret ? "\n" : "");
		if (!ret)
			return (BINT_SUCCESS);
		if (bint_sub(n, n, BINT_TWO) == BINT_FAILURE)
			return (BINT_FAILURE);
	}
	return (BINT_FAILURE);
}
