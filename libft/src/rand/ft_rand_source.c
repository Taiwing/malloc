/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand_source.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:39:34 by yforeau           #+#    #+#             */
/*   Updated: 2021/09/23 13:09:33 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

static char	*ft_flush_fd(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
	return (NULL);
}

/*
** ft_rand_source: fill dst buffer with random data
**
** dst: destination buffer
** size: bytes to read from source
** src: source file, "/dev/urandom" if NULL
** flush: close opened fd if any (boolean 1 == true)
**
** By default ft_rand_source will read from /dev/urandom if the
** current randomness source is exhausted, meaning that it cannot
** read  'size' bytes from source, or if it is unset (fd == -1).
**
** To set a new randomness source the preceding one must be exhausted,
** flushed or unset.
**
** If /dev/urandom is the only used source then it is more efficient to
** set it at the start of the program with a call like this:
** ft_rand_source(NULL, 0, "/dev/urandom", 0)
** and to set an atexit handler to flush ft_rand_source.
**
** return: dst is NULL or size is 0, the return is src (so NULL on error)
** and ft_rand_source does not read from source. Otherwise ft_rand_source
** will try to read 'size' bytes from source and return 'dst' on success
** or NULL on failure.
*/

#ifdef THREAD_SAFE
# include "thread_safe.h"
MUTEXIFY(void*, ft_rand_source, void*, dst, size_t, size,
	const char*, src, int, flush)
#else
void		*ft_rand_source(void *dst, size_t size,
	const char *src, int flush)
#endif
{
	int			rd;
	static int	fd = -1;
	
	if (flush)
		ft_flush_fd(&fd);
	if (fd == -1 && src && (fd = open(src, O_RDONLY)) < 0)
		return (ft_flush_fd(&fd));
	if (!dst || !size)
		return ((void *)src);
	rd = 0;
	if (fd >= 0 && (rd = read(fd, (char *)dst, size)) < 0)
		return (ft_flush_fd(&fd));
	else if (rd < (int)size)
	{
		ft_flush_fd(&fd);
		if ((fd = open("/dev/urandom", O_RDONLY)) < 0)
			return (NULL);
		rd += read(fd, (char *)dst + rd, size - rd);
		ft_flush_fd(&fd);
	}
	return (rd == (int)size ? dst : NULL);
}
