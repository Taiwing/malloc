/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:55:42 by yforeau           #+#    #+#             */
/*   Updated: 2019/04/29 17:48:15 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GET_NEXT_LINE_H
# define FT_GET_NEXT_LINE_H

# define GNL_BUFF_SIZE 32

# include "libft.h"

typedef struct	s_gnl
{
	int			fd;
	char		*trail;
}				t_gnl;

int				ft_isfd(void *fd, void *content);
void			rm_cur(t_list **lst, t_gnl *cur);

#endif
