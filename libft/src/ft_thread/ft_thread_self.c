/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread_self.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 14:22:39 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/02 14:53:47 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifdef THREAD_SAFE

__thread t_thread_id	g_thread_id = 0;

/*
** Simply return the current's thread id.
**
** Main thread is always 0. An already used thread id cannot be used again, so
** the maximum allowed thread id is UINT64_MAX as t_thread_id is really an
** uint64_t.
*/
t_thread_id	ft_thread_self(void)	
{
	return (g_thread_id);
}

#endif
