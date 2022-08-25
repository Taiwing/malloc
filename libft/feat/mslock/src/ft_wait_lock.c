/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait_lock.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:18:28 by yforeau           #+#    #+#             */
/*   Updated: 2021/11/06 12:21:23 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wait_lock.h"

#ifdef THREAD_SAFE

//in master
void	ft_mslock_init(t_mslock *lock)
{
	int	err;
	if ((err = pthread_mutex_init(&lock->main_mutex, NULL))
		|| (err = pthread_mutex_init(&lock->sync_mutex, NULL)))
		ft_exit(EXIT_FAILURE, "%s: pthread_mutex_init: %s", __func__,
			strerror(err));
}

//in master
void	ft_lock_slave(t_mslock *lock)
{
	ft_mutex_lock(&lock->main_mutex); //1 initializing lock
}

//in slave
void	ft_wait_master(t_mslock *lock)
{
	ft_mutex_lock(&lock->sync_mutex); //

	ft_mutex_lock(&lock->main_mutex); //2 waiting for master
	ft_mutex_unlock(&lock->main_mutex); //4 giving the lock back

	ft_mutex_unlock(&lock->sync_mutex); //
}

//in master
void	ft_unlock_slave(t_mslock *lock)
{
	ft_mutex_unlock(&lock->main_mutex); //3 unlocking slave
	ft_mutex_lock(&lock->sync_mutex); //
	ft_mutex_lock(&lock->main_mutex); //5 reclaimng the lock
	ft_mutex_unlock(&lock->sync_mutex); //
}

#endif
