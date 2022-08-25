/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait_lock.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 12:16:52 by yforeau           #+#    #+#             */
/*   Updated: 2021/11/06 12:17:57 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** t_mslock: master/slave lock
**
** main_mutex: locked by master and waited for by slave
** sync_mutex: locked by slave and waited for by master
*/
typedef struct		s_mslock
{
	pthread_mutex_t	main_mutex;
	pthread_mutex_t	sync_mutex;
}					t_mslock;

/*
** mslock functions
*/

void		ft_mslock_init(t_mslock *lock);
void		ft_lock_slave(t_mslock *lock);
void		ft_wait_master(t_mslock *lock);
void		ft_unlock_slave(t_mslock *lock);
