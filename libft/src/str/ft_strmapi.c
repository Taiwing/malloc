/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 18:24:52 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 18:35:03 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*map;

	if (!s || !f)
		return (NULL);
	i = 0;
	map = ft_strnew(ft_strlen(s));
	while (map && s[i++])
		map[i - 1] = f(i - 1, s[i - 1]);
	return (map);
}
