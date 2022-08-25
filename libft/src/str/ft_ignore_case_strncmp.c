/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ignore_case_strncmp.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 13:48:36 by yforeau           #+#    #+#             */
/*   Updated: 2021/09/21 13:49:21 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_ignore_case_strncmp(const char *s1, const char *s2, size_t n)
{
	t_uchar c1;
	t_uchar c2;

	if (!n)
		return (0);
	c1 = (t_uchar)*s1;
	c2 = (t_uchar)*s2;
	if (c1 && c2 && (c1 == c2 || (ft_isalpha(*s1) && ft_isalpha(*s2)
		&& (c1 + 32 == c2 || c1 - 32 == c2))))
		return (ft_ignore_case_strncmp(s1 + 1, s2 + 1, n - 1));
	return (c1 - c2);
}
