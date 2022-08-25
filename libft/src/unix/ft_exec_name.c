/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_name.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 12:26:39 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/30 12:55:51 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Search the executable name in a unix path and returns a pointer to it.
** It will return the word after the last forward slash if any.
*/
char	*ft_exec_name(const char *path)
{
	char		*ptr;
	const char	*exec_name;

	exec_name = path;
	while (*exec_name && (ptr = ft_strchr(exec_name, '/')))
		exec_name = ptr + 1;
	return ((char *)exec_name);
}
