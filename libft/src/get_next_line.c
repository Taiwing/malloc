/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:55:35 by yforeau           #+#    #+#             */
/*   Updated: 2021/09/24 17:33:29 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_gnl	*ft_get_trail(int fd, t_list **lst)
{
	t_list	*ref;
	t_list	*ptr;
	t_gnl	new;

	ref = *lst;
	ptr = ft_lst_find(*lst, (void *)&fd, ft_isfd);
	if (!ptr)
	{
		new.fd = fd;
		new.trail = NULL;
		ft_lst_push_front(lst, (void *)&new, sizeof(t_gnl));
		ptr = *lst;
		if (ptr == ref)
			return (NULL);
	}
	else
		ft_lst_move_front(lst, ptr);
	return ((t_gnl *)ptr->content);
}

static int		ft_read_file(t_gnl *cur, char **line, t_list **lst)
{
	int		r;
	size_t	l;
	char	buf[GNL_BUFF_SIZE];

	l = cur->trail ? ft_strlen(cur->trail) : 0;
	if (!(r = read(cur->fd, buf, GNL_BUFF_SIZE)))
	{
		if (!(*line = ft_strcut(&(cur->trail), 0, l)) && l > 0)
			r = -1;
		if (!*line)
			rm_cur(lst, cur);
	}
	else if (r > 0)
	{
		if (cur->trail)
			cur->trail = ft_stradd(&(cur->trail), buf, r);
		else
			cur->trail = ft_strndup(buf, r);
		if (cur->trail && l + r > ft_strlen(cur->trail))
			ft_memdel((void **)&cur->trail);
		r = !cur->trail ? -1 : r;
	}
	return (r);
}

#ifdef THREAD_SAFE
MUTEXIFY(int, get_next_line, const int, fd, char**, line)
#else
int				get_next_line(const int fd, char **line)
#endif
{
	int				r;
	size_t			l;
	char			*eol;
	t_gnl			*cur;
	static t_list	*lst = NULL;

	cur = ft_get_trail(fd, &lst);
	r = cur ? 0 : -1;
	if (r != -1 && (eol = cur->trail ? ft_strchr(cur->trail, 10) : NULL))
	{
		l = ft_strlen(eol) - 1;
		if (!(*line = ft_strsub(cur->trail, 0, eol - cur->trail)) ||
			(!(cur->trail = ft_strcut(&(cur->trail), eol - cur->trail + 1, l))
			&& l > 0))
			r = -1;
	}
	else if (r != -1 && (r = ft_read_file(cur, line, &lst)) > 0)
#ifdef THREAD_SAFE
		return (ts_get_next_line(fd, line));
#else
		return (get_next_line(fd, line));
#endif
	if (r == -1 && cur)
		rm_cur(&lst, cur);
	return (r == -1 ? -1 : *line != NULL);
}
