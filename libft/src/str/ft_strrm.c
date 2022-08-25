/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 19:23:10 by yforeau           #+#    #+#             */
/*   Updated: 2019/04/09 22:19:45 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrm(char **str, size_t start, size_t n)
{
	size_t	len;
	char	*res;

	if (!str || !*str)
		return (NULL);
	else if (!n || start >= (len = ft_strlen(*str)))
		return (*str);
	else if (!start)
		return (ft_strcut(str, n, len > n ? len - n : 0));
	else if (start + n >= len)
		return (ft_strcut(str, 0, start));
	res = ft_strsub(*str, 0, start);
	res = ft_stradd(&res, *str + start + n, len - start - n);
	ft_memdel((void **)str);
	return (res);
}
