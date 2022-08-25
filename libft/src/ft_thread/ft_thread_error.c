/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 16:40:57 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/02 17:24:57 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef THREAD_SAFE

int				g_thread_error = 0;
pthread_mutex_t	g_thread_error_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
** ft_thread_error: returns the errcode of the last non-zero exited thread
*/
int		ft_thread_error(void)
{
	int	ret;

	ft_mutex_lock(&g_thread_error_mutex);
	ret = g_thread_error;
	ft_mutex_unlock(&g_thread_error_mutex);
	return (ret);
}

/*
** ft_set_thread_error: set thread error if non-zero and if thread is not main
*/
void	ft_set_thread_error(int err)
{
	if (!err || !ft_thread_self())
		return;
	ft_mutex_lock(&g_thread_error_mutex);
	g_thread_error = err;
	ft_mutex_unlock(&g_thread_error_mutex);
}

#endif
