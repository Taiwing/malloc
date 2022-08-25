/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdata_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 15:52:45 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 20:09:59 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

static void	pdata_realloc(t_pdata *data, size_t newsize)
{
	char	*newbuf;

	newsize = (newsize + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
	if (!(newbuf = (char *)ft_secmalloc(newsize)))
	{
		data->n = -1;
		return ;
	}
	ft_memcpy(newbuf, data->buf, data->n);
	ft_memdel((void **)&data->abuf);
	data->abuf = newbuf;
	data->bufsize = newsize;
	data->buf = data->abuf ? data->abuf : data->sbuf;
}

static void	pdata_flush(t_pdata *data, char **add, int c, size_t *size)
{
	char	*s = *add;

	pdata_print(data);
	if (data->n < 0)
		return ;
	data->flushed += data->n;
	data->n = 0;
	while (*size >= data->bufsize && !data->n)
	{
		if (s)
		{
			data->n = write(data->fd, s, data->bufsize) < 0 ? -1 : 0;
			s += data->bufsize;
		}
		else
		{
			ft_memset(data->buf, c, data->bufsize);
			data->n = write(data->fd, data->buf, data->bufsize) < 0 ? -1 : 0;
		}
		data->flushed += data->bufsize;
		*size -= data->bufsize;
	}
	*add = s;
}

void		pdata_add(t_pdata *data, char *add, int c, size_t size)
{
	size_t	newsize;

	if (data->n < 0)
		return ;
	if (!(data->flags & PDATA_NOLIMIT)
		&& (data->bufsize < (newsize = data->n + size + 1)))
	{
		if (data->flags & PDATA_STOP)
			size = (size_t)data->n < data->bufsize ?
				data->bufsize - (size_t)data->n - 1 : 0;
		else if (data->flags & PDATA_FLUSH)
			pdata_flush(data, &add, c, &size);
		else if (data->flags & PDATA_NOALLOC)
			data->n = -1;
		else
			pdata_realloc(data, newsize);
	}
	if (data->n >= 0 && size)
	{
		if (add)
			ft_memcpy(data->buf + data->n, add, size);
		else
			ft_memset(data->buf + data->n, c, size);
		data->n += size;
	}
}
