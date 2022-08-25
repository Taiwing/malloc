/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_collector.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 09:41:53 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/02 17:38:50 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COLLECTOR_H
# define FT_COLLECTOR_H

# include <string.h>
# include <dirent.h>
# include "ft_exit.h"

void			*ft_secmalloc(size_t size);

/*
** The "t_stack" type allows to create a custom stack with an arbitrary
** type of object. This will be used with ft_collector which will act like
** ft_heap_collector with the heap_stack. This allows to keep track of
** resources used by the program and to free/close/end/clean them at the
** end of the program. For this, the function ft_atexit will have to be used
** with one function by stack. Also "container" functions with static "t_stack"
** pointers should be created to access them from everywhere in the program,
** or at least a big one to keep them all.
*/
typedef struct	s_stack
{
	void		**stack;
	size_t		size;
	size_t		i;
	void		(*cleanf)(void *);
}				t_stack;

/*
** Instructions for collector functions.
** FT_COLLEC_ADD: add an element to the stack
** FT_COLLEC_GET: remove an element from the stack
** FT_COLLEC_FREE: flush the entire stack
*/
enum			e_collec_do {FT_COLLEC_ADD, FT_COLLEC_GET, FT_COLLEC_FREE};

void			**ft_check_stack_size(void **stack, size_t *size, size_t i);
void			*ft_get_stack_ptr(void **stack, size_t *i, void *ptr);
void			*ft_heap_collector(void *ptr, int hs_do);
void			*ft_collector(t_stack *st, void *ptr, int gs_do);
void			ft_closedir(void *dirp);
t_stack			*ft_dir_container(void);
void			ft_close_dir_stack(void);
DIR				*ft_dir_collector(DIR *dirp, int ds_do);

#endif
