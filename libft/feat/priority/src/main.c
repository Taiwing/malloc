/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 14:51:11 by yforeau           #+#    #+#             */
/*   Updated: 2021/11/06 16:01:51 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "libft.h"

#define PRIORITY_LEVELS	2

pthread_mutex_t	g_priority[PRIORITY_LEVELS] = {
	[0 ... PRIORITY_LEVELS - 1] = PTHREAD_MUTEX_INITIALIZER
};

pthread_mutex_t	g_go_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t	g_natural_mutex = PTHREAD_MUTEX_INITIALIZER;

#define THREAD_COUNT	26

uint64_t		g_priority_val[THREAD_COUNT] = { 0 };

char			g_natural_order[THREAD_COUNT + 1] = { 0 };

typedef void	(*t_prio_action)(void *);

int	lowest_priority_action(pthread_mutex_t *prio, int nlevels,
		t_prio_action action, void *arg)
{
	if (!prio || !action || nlevels < 1)
		return (-1);
	for (int i = nlevels - 1; i >= 0; --i)
		ft_mutex_lock(prio + i);
	action(arg);
	for (int i = 0; i < nlevels; ++i)
		ft_mutex_unlock(prio + i);
	return (0);
}

int	highest_priority_action(pthread_mutex_t *prio, int nlevels,
		t_prio_action action, void *arg)
{
	if (!prio || !action || nlevels < 1)
		return (-1);
	ft_mutex_lock(prio);
	action(arg);
	ft_mutex_unlock(prio);
	return (0);
}

void	show(void *char_ptr)
{
	char	c = *(char *)char_ptr;

	/*
	if (c >= 'A' && c <= 'Z')
		sleep(1);
	*/
	if (c == 'A' || c == 'a')
		sleep(1);
	ft_putchar(*(char *)char_ptr);
}

void	*thread_worker(void *ptr)
{
	char		c = 'a' + ft_thread_self() - 1;
	uint64_t	priority = g_priority_val[ft_thread_self() - 1];

	(void)ptr;
	if (ft_thread_self() == THREAD_COUNT)
		ft_printf("priority order:\t");
	ft_mutex_lock(&g_go_mutex);
	ft_mutex_unlock(&g_go_mutex);
	if (priority >= 7)
		c -= 32;
	ft_mutex_lock(&g_natural_mutex);
	g_natural_order[ft_strlen(g_natural_order)] = c;
	ft_mutex_unlock(&g_natural_mutex);
	if (priority < 7)
		lowest_priority_action(g_priority, PRIORITY_LEVELS, show, (void *)&c);
	else
		highest_priority_action(g_priority, PRIORITY_LEVELS, show, (void *)&c);
	/*
	ft_printf("Worker %llu (%llx) exiting\n",
		ft_thread_self(), pthread_self());
	*/
	return (NULL);
}

int	main(void)
{
	int			ret;
	t_ft_thread	threads[THREAD_COUNT];

	ft_exitmsg("priority_test");
	ft_first_exit();
	ft_mutex_lock(&g_go_mutex);
	for (int i = 0; i < THREAD_COUNT; ++i)
		if (!ft_rand_uint64(g_priority_val + i, 0, 10))
			ft_exit(EXIT_FAILURE, "ft_rand_uint64: error");
	for (int i = 0; i < THREAD_COUNT; ++i)
		if ((ret = ft_thread_create(threads + i, NULL, thread_worker, NULL)))
			ft_exit(EXIT_FAILURE, "ft_thread_create: %s", strerror(ret));
	sleep(1);
	ft_mutex_unlock(&g_go_mutex);
	for (int i = 0; i < THREAD_COUNT; ++i)
		ft_thread_join(threads + i, NULL);
	ft_putchar('\n');
	ft_printf("natural order:\t%s\n", g_natural_order);
	return (EXIT_SUCCESS);
}
