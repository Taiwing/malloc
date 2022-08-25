/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_first_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 15:45:52 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 18:15:30 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_exit_args		g_first_exit = { 0 };

#ifdef THREAD_SAFE
pthread_mutex_t	g_first_exit_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

/*
** ft_first_exit:
**
** Set g_first_exit status to ON. It means that even if ft_exit is called
** multiple times only the first ft_exit set of arguments is going to be used.
** It works with single threaded and multi thread applications alike. It
** garantees that only one exit message is going to be printed.
*/
void				ft_first_exit(void)
{
#ifdef THREAD_SAFE
	ft_mutex_lock(&g_first_exit_mutex);
#endif
	if (g_first_exit.status == FIRST_EXIT_OFF)
		g_first_exit.status = FIRST_EXIT_ON;
#ifdef THREAD_SAFE
	ft_mutex_unlock(&g_first_exit_mutex);
#endif
}

void				ft_set_first_exit(int ret, char *err)
{
#ifdef THREAD_SAFE
	if (pthread_mutex_lock(&g_first_exit_mutex))
		return;
#endif
	if (g_first_exit.status == FIRST_EXIT_ON)
	{
		g_first_exit.status = FIRST_EXIT_FILLED;
		g_first_exit.ret = ret;
		if (err)
			ft_strncpy(g_first_exit.err, err, EXIT_MSG_MAX);
	}
#ifdef THREAD_SAFE
	pthread_mutex_unlock(&g_first_exit_mutex);
#endif
}

void				ft_get_first_exit(int *ret, char **err)
{
#ifdef THREAD_SAFE
	if (pthread_mutex_lock(&g_first_exit_mutex))
		return;
#endif
	if (g_first_exit.status == FIRST_EXIT_FILLED)
	{
		*ret = g_first_exit.ret;
		if (g_first_exit.err[0])
			*err = g_first_exit.err;
	}
#ifdef THREAD_SAFE
	pthread_mutex_unlock(&g_first_exit_mutex);
#endif
}
