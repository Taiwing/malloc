/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:16:23 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/18 23:19:08 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putendl_fd(char const *s, int fd)
{
	int	ret;

	if ((ret = ft_putstr_fd(s, fd)) >= 0 && ft_putchar_fd(10, fd) == 1)
		return (ret + 1);
	return (-1);
}
