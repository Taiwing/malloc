/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 14:55:23 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/02 21:35:59 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef THREAD_SAFE

uint64_t		g_thread_count = 0;
pthread_mutex_t	g_thread_count_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
** ft_thread_count: returns the number of threads started with ft_thread_create
*/
uint64_t	ft_thread_count(void)
{
	uint64_t	ret;

	ft_mutex_lock(&g_thread_count_mutex);
	ret = g_thread_count;
	ft_mutex_unlock(&g_thread_count_mutex);
	return (ret);
}

typedef struct	s_start_thread_args
{
	t_threadf	start_routine;
	void		*arg;
	t_thread_id	id;
}				t_start_thread_args;

static void	*start_thread(void *start_thread_args)
{
	t_start_thread_args	*args = start_thread_args;
	void				*arg = args->arg;
	t_threadf			start_routine = args->start_routine;

	ft_mutex_lock(&g_thread_count_mutex);
	g_thread_id = (t_thread_id)++g_thread_count;
	args->id = g_thread_id;
	ft_mutex_unlock(&g_thread_count_mutex);
	if (start_routine)
		return (start_routine(arg));
	return (NULL);
}

/*
** ft_thread_create: starts the thread and updates thread count and id
*/
int	ft_thread_create(t_ft_thread *thread, const pthread_attr_t *attr,
		t_threadf start_routine, void *arg)
{
	t_start_thread_args	args = { start_routine, arg, 0 };
	int					ret;

	ret = pthread_create(&thread->thread, attr, start_thread, (void *)&args);
	if (ret)
		return (ret);
	do
	{
		ft_mutex_lock(&g_thread_count_mutex);
		thread->id = args.id;
		ft_mutex_unlock(&g_thread_count_mutex);
	} while (!thread->id);
	return (ret);
}

#endif
