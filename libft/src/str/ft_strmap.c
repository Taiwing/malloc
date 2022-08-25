/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:24:09 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 18:34:47 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	int		l;
	char	*map;

	l = s ? ft_strlen(s) : 0;
	map = ft_strnew(l);
	while (map && s && l--)
		map[l] = f(s[l]);
	return (map);
}
