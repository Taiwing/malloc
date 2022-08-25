/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdata.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 13:11:40 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 19:45:17 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

void	pdata_print(t_pdata *data)
{
	if (data->n > 0)
		data->n = write(data->fd, data->buf, data->n);
	ft_memdel((void **)&data->abuf);
	if (data->n >= 0 && data->flags & PDATA_FLUSH)
		data->n += data->flushed;
}

/*
** pdata_local_set_buf:
**
** The NOLIMIT and FLUSH flags cannot be used in a local buffer. This is because
** this buffer is separated from the main one since the data inside must be
** formatted and some characters can be added before it. So the contents cannot
** be printed (flushed) before the conversion is fully done, and the buffer has
** a defined size so NOLIMT cannot apply. Then the only thing we can do, if the
** sub buffer is not large enough and the function is in NOALLOC mode (meaning
** FLUSH has been set and ALLOC is not set), is to stop and return an error.
**
** For now this means that NOALLOC mode will make the ft_printf call fail in
** this case. This is not really a big problem since NOALLOC mode will mainly
** be used for debugging/error printing and that gigantic conversions are quite
** rare.
**
** TODO: (maybe) Fix this by using only one buffer and creating a "pdata_insert"
** function. The buffer would still need to hold a boolean prohibiting printing
** before formatting is done. An other way would be to somehow format before the
** conversion. This would be difficult as most of the formatting depends on the
** data being formatted (it makes sense right ?).
*/
void	pdata_local_set_buf(t_pdata *loc)
{
	loc->flags &= PDATA_NOALLOC;
	pdata_set_buf(loc, NULL, 0);
}

void	pdata_set_buf(t_pdata *data, char *userbuf, size_t userbuf_size)
{
	data->buf = NULL;
	data->sbuf = NULL;
	data->abuf = NULL;
	data->bufsize = data->flags & PDATA_NOLIMIT ? 0
		: userbuf ? userbuf_size : BLOCK_SIZE;
	if (data->flags & PDATA_ALLOC)
	{
		data->flags &= ~PDATA_NOALLOC;
		if (!(data->abuf = (char *)ft_secmalloc(BLOCK_SIZE)))
		{
			data->n = -1;
			return ;
		}
		data->buf = data->abuf;
	}
	else if (userbuf)
		data->buf = data->sbuf = userbuf;
	else
		data->buf = data->sbuf = data->dbuf;
}

void	pdata_init(t_pdata *data, enum e_pmodes pmode, int fd)
{
	data->flags = (uint8_t)pmode;
	data->fd = fd;
	data->n = 0;
	data->flushed = 0;
}
