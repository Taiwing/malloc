/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap_p.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 19:50:13 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/23 21:11:12 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

/*
** This function takes two double pointers casted with (uintptr_t *).
** It swaps their value without temporary variable (yeah, thats pretty cool).
*/

void	ft_swap_p(uintptr_t *p1, uintptr_t *p2)
{
	if (p1 != p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
}
