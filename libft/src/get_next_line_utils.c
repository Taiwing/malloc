/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 17:48:27 by yforeau           #+#    #+#             */
/*   Updated: 2019/04/29 17:59:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_isfd(void *fd, void *content)
{
	return (*(int *)fd != ((t_gnl *)content)->fd);
}

#ifdef NO_COLLEC

void	rm_cur(t_list **lst, t_gnl *cur)
{
	ft_lst_remove_if(lst, (void *)&(cur->fd), ft_isfd);
	free(cur);
}

#else

void	rm_cur(t_list **lst, t_gnl *cur)
{
	ft_lst_remove_if(lst, (void *)&(cur->fd), ft_isfd);
	free(ft_heap_collector(cur, FT_COLLEC_GET));
}

#endif
