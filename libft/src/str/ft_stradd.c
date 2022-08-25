/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stradd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:48:53 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/11 17:32:33 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_stradd(char **base, char *str, size_t l)
{
	char	*new;

	if ((new = ft_strnew(ft_strlen(*base) + l)))
		ft_strncat(ft_strcpy(new, *base), str, l);
	ft_memdel((void **)base);
	return (new);
}
