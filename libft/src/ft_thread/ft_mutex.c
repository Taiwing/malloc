/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mutex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 14:22:04 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/27 06:52:17 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef THREAD_SAFE

void			ft_mutex_lock(pthread_mutex_t *mutex)
{
	int		ret;

	if (!(ret = pthread_mutex_lock(mutex)))
		return;
	ft_exit(EXIT_FAILURE, "%s: %s", "pthread_mutex_lock", strerror(ret));
}

void			ft_mutex_unlock(pthread_mutex_t *mutex)
{
	int		ret;

	if (!(ret = pthread_mutex_unlock(mutex)))
		return;
	ft_exit(EXIT_FAILURE, "%s: %s", "pthread_mutex_lock", strerror(ret));
}

#endif
