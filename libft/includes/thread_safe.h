/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 16:55:03 by yforeau           #+#    #+#             */
/*   Updated: 2021/11/06 12:18:27 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_H
# define THREAD_SAFE_H
# ifdef THREAD_SAFE
#  include <pthread.h>
#  include <stdint.h>
#  include "ft_mutexify.h"

/*
** t_ft_thread: libft thread structure
**
** id: is the libft assigned id (returned by ft_thread_self)
** thread: is the pthread thread structure (used for pthread_join)
*/

typedef uint64_t	t_thread_id;

typedef struct		s_ft_thread
{
	t_thread_id		id;
	pthread_t		thread;
}					t_ft_thread;

typedef void		*(*t_threadf)(void *);

/*
** ft_thread globals
*/

extern __thread t_thread_id	g_thread_id;

/*
** ft_thread functions
*/
t_thread_id	ft_thread_self(void);
uint64_t	ft_thread_count(void);
int			ft_thread_create(t_ft_thread *thread, const pthread_attr_t *attr,
				t_threadf start_routine, void *arg);
int			ft_thread_join(t_ft_thread *thread, void **retval);
int			ft_thread_error(void);
void		ft_set_thread_error(int errcode);
void		ft_thread_exit(void);

/*
** Mutex functions
*/
void		ft_mutex_lock(pthread_mutex_t *mutex);
void		ft_mutex_unlock(pthread_mutex_t *mutex);

# endif
#endif
