/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 14:41:13 by yforeau           #+#    #+#             */
/*   Updated: 2019/04/29 18:10:47 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	valid_type(int file_type, int type)
{
	if (file_type == DT_UNKNOWN)
		return (!type);
	if (file_type == DT_FIFO)
		return (type & FT_LS_FIFO);
	else if (file_type == DT_CHR)
		return (type & FT_LS_CHR);
	else if (file_type == DT_DIR)
		return (type & FT_LS_DIR);
	else if (file_type == DT_BLK)
		return (type & FT_LS_BLK);
	else if (file_type == DT_REG)
		return (type & FT_LS_REG);
	else if (file_type == DT_LNK)
		return (type & FT_LS_LNK);
	else if (file_type == DT_SOCK)
		return (type & FT_LS_SOCK);
	else if (file_type == DT_WHT)
		return (type & FT_LS_WHT);
	return (0);
}

#ifdef NO_COLLEC

t_list		*ft_ls_files(char *path, int type)
{
	struct dirent	*fp;
	t_list			*lst;
	DIR				*dirp;

	lst = NULL;
	if (!(dirp = opendir(path)))
		return (NULL);
	while ((fp = readdir(dirp)))
	{
		if (type == FT_LS_ALL || valid_type(fp->d_type, type))
			ft_lstadd(&lst, ft_lstnew((void *)fp->d_name,
				ft_strlen(fp->d_name) + 1));
	}
	closedir(dirp);
	return (lst);
}

#else

t_list		*ft_ls_files(char *path, int type)
{
	struct dirent	*fp;
	t_list			*lst;
	DIR				*dirp;

	lst = NULL;
	if (!(dirp = opendir(path)))
		return (NULL);
	ft_dir_collector(dirp, FT_COLLEC_ADD);
	while ((fp = readdir(dirp)))
	{
		if (type == FT_LS_ALL || valid_type(fp->d_type, type))
			ft_lstadd(&lst, ft_lstnew((void *)fp->d_name,
				ft_strlen(fp->d_name) + 1));
	}
	closedir(ft_dir_collector(dirp, FT_COLLEC_GET));
	return (lst);
}

#endif
