/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcut.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:50:32 by yforeau           #+#    #+#             */
/*   Updated: 2018/11/08 15:37:34 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcut(char **s, size_t start, size_t l)
{
	char	*cut;

	cut = l > 0 ? ft_strsub(*s, start, l) : NULL;
	ft_memdel((void **)s);
	return (cut);
}
