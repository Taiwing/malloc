/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint_mod.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 13:18:25 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/04 19:24:42 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "bint.h"

static int	bint_smod(t_bint res, const t_bint mod, t_bint tmp)
{
	if (bintcpy(tmp, res))
		return (BINT_FAILURE);
	return (bint_divide(NULL, res, tmp, mod));
}

/*
** Compute modular exponentiation: res = (a ^ b) % c
*/
int			bint_modexp(t_bint res, const t_bint a,
	const t_bint b, const t_bint c)
{
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	local_a[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	local_b[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (!bintcmp(c, BINT_ONE))
		return (bintcpy(res, BINT_ZERO));
	if (bintcpy(local_a, a) == BINT_FAILURE
		|| bintcpy(local_b, b) == BINT_FAILURE
		|| bintcpy(res, BINT_ONE) == BINT_FAILURE
		|| bint_smod(local_a, c, tmp) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (!bintcmp(local_a, BINT_ZERO) && bintcmp(b, BINT_ZERO))
		return (bintcpy(res, BINT_ZERO));
	while (bintcmp(local_b, BINT_ZERO))
	{
		if (bint_is_odd(local_b)
			&& (bint_mult(res, res, local_a) == BINT_FAILURE
			|| bint_smod(res, c, tmp) == BINT_FAILURE))
			return (BINT_FAILURE);
		if (bint_mult(local_a, local_a, local_a) == BINT_FAILURE
			|| bint_smod(local_a, c, tmp) == BINT_FAILURE
			|| bint_shiftright(local_b, 1) == BINT_FAILURE)
			return (BINT_FAILURE);
	}
	return (BINT_SUCCESS);
}

static int	bint_modinv_op(t_bint old, t_bint new,
	const t_bint q, t_bint tmp)
{
	if (bintcpy(tmp, new) == BINT_FAILURE
		|| bint_mult(new, new, q) == BINT_FAILURE
		|| bint_sub(new, old, new) == BINT_FAILURE
		|| bintcpy(old, tmp))
		return (BINT_FAILURE);
	return (BINT_SUCCESS);
}

/*
** Compute the modular inverse of a by b and put the result in t.
**
** gcd can be NULL, otherwise, as its name indicates, the gcd is
** stored in it as it is computed over the course of the inversion
*/
int			bint_modinv(t_bint t, t_bint gcd,
	const t_bint a, const t_bint b)
{
	uint32_t	q[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	r[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	newr[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	newt[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (bintcpy(t, BINT_ZERO) == BINT_FAILURE
		|| bintcpy(r, b) == BINT_FAILURE
		|| bintcpy(newr, a) == BINT_FAILURE
		|| bintcpy(newt, BINT_ONE) == BINT_FAILURE)
		return (BINT_FAILURE);
	while (bintcmp(newr, BINT_ZERO))
		if (bint_divide(q, NULL, r, newr) == BINT_FAILURE
			|| bint_modinv_op(t, newt, q, tmp) == BINT_FAILURE
			|| bint_modinv_op(r, newr, q, tmp) == BINT_FAILURE)
			return (BINT_FAILURE);
	if (gcd && bintcpy(gcd, r) == BINT_FAILURE)
		return (BINT_FAILURE);
	if (BINT_SIGN(t) && bint_add(t, t, b) == BINT_FAILURE)
		return (BINT_FAILURE);
	return (BINT_SUCCESS);
}

int			bint_modmul(t_bint res, const t_bint a,
	const t_bint b, const t_bint c)
{
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	tmp2[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	if (bint_divide(NULL, tmp, a, c) == BINT_FAILURE
		|| bint_divide(NULL, tmp2, b, c) == BINT_FAILURE
		|| bint_mult(tmp, tmp, tmp2) == BINT_FAILURE
		|| bint_divide(NULL, res, tmp, c) == BINT_FAILURE)
		return (BINT_FAILURE);
	return (BINT_SUCCESS);
}
