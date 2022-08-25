/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespaces.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 17:06:54 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/03 13:12:47 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef NO_COLLEC

char	**ft_split_whitespaces(char *str)
{
#ifdef THREAD_SAFE
	static __thread int	size = 0;
#else
	static int			size = 0;
#endif
	int					l;
	char				*p;
	char				**tb;

	if (!str)
		return (NULL);
	p = NULL;
	tb = NULL;
	size = size == -1 ? 1 : size + 1;
	while (*str && (*str == 32 || (*str > 8 && *str < 12)))
		++str;
	l = 0;
	while (str[l] && !(str[l] == 32 || (str[l] > 8 && str[l] < 12)))
		++l;
	if (l && !(p = ft_strndup(str, l)))
		size = -1;
	if (size != -1 && !(tb = p ? ft_split_whitespaces(str + l) :
		(char **)ft_secmalloc(size * sizeof(char *))))
		size = -1;
	if (size == -1 && p)
		free(p);
	else if (size != -1)
		tb[--size] = p;
	return (tb);
}

#else

char	**ft_split_whitespaces(char *str)
{
#ifdef THREAD_SAFE
	static __thread int	size = 0;
#else
	static int			size = 0;
#endif
	int					l;
	char				*p;
	char				**tb;

	if (!str)
		return (NULL);
	p = NULL;
	tb = NULL;
	size = size == -1 ? 1 : size + 1;
	while (*str && (*str == 32 || (*str > 8 && *str < 12)))
		++str;
	l = 0;
	while (str[l] && !(str[l] == 32 || (str[l] > 8 && str[l] < 12)))
		++l;
	if (l && !(p = ft_strndup(str, l)))
		size = -1;
	if (size != -1 && !(tb = p ? ft_split_whitespaces(str + l) :
		(char **)ft_secmalloc(size * sizeof(char *))))
		size = -1;
	if (size == -1 && p)
		free(ft_heap_collector(p, FT_COLLEC_GET));
	else if (size != -1)
		tb[--size] = p;
	return (tb);
}

#endif
