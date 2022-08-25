/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dir_collector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 13:58:47 by yforeau           #+#    #+#             */
/*   Updated: 2021/09/23 13:06:53 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_collector.h"

void	ft_closedir(void *dirp)
{
	closedir((DIR *)dirp);
}

#ifdef THREAD_SAFE
# include "thread_safe.h"
MUTEXIFY(t_stack*, ft_dir_container, void)
#else
t_stack	*ft_dir_container(void)
#endif
{
	static t_stack	*st = NULL;

	if (!st)
	{
		st = ft_secmalloc(sizeof(t_stack));
		st->stack = NULL;
		st->size = 0;
		st->i = 0;
		st->cleanf = ft_closedir;
	}
	return (st);
}

void	ft_close_dir_stack(void)
{
	ft_collector(ft_dir_container(), NULL, FT_COLLEC_FREE);
}

DIR		*ft_dir_collector(DIR *dirp, int ds_do)
{
	return ((DIR *)ft_collector(ft_dir_container(), dirp, ds_do));
}
