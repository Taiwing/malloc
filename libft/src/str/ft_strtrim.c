/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 13:28:03 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 18:58:24 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t	sz;

	if (!s)
		return (NULL);
	while (*s == ' ' || *s == '\n' || *s == '\t')
		s++;
	sz = *s ? ft_strlen(s) - 1 : 0;
	while (s[sz] == ' ' || s[sz] == '\n' || s[sz] == '\t')
		sz--;
	return (ft_strsub(s, 0, sz + 1));
}
