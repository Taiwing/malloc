/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_collector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 10:28:41 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/27 06:53:25 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#ifdef NO_COLLEC
# include "ft_collector.h"
#endif

const size_t	g_gs_sizes[4] = {
	0xffff, 0xffffffff, 0xffffffffffff
};

static void		**ft_alloc_stack(size_t *size)
{
	void	**ptr;
	int		j;

	j = -1;
	while (++j < 3 && g_gs_sizes[j] <= *size)
		;
	if (j == 3)
		return (NULL);
	if (!(ptr = (void **)malloc(g_gs_sizes[j] * sizeof(void *))))
		return (NULL);
	*size = g_gs_sizes[j];
	return (ptr);
}

void			**ft_check_stack_size(void **stack, size_t *size, size_t i)
{
	void	**ptr;
	size_t	old_size;

	if (i < *size)
		return (stack);
	else if (!*size)
		return (ft_alloc_stack(size));
	else
	{
		old_size = *size;
		ptr = ft_alloc_stack(size);
		if (ptr && stack)
		{
			ft_memcpy(ptr, stack, old_size * sizeof(void *));
			free(stack);
		}
		return (ptr);
	}
}

void			*ft_get_stack_ptr(void **stack, size_t *i, void *ptr)
{
	size_t	j;

	if (stack && ptr && *i)
	{
		j = *i;
		while (j > 0 && stack[--j] != ptr)
			;
		if (ptr == stack[j])
		{
			--(*i);
			stack[j] = j == *i ? NULL : stack[*i];
			return (ptr);
		}
	}
	if (ptr)
		ft_exit(EXIT_FAILURE, "%s: invalid pointer", __func__);
	return (NULL);
}

void			*ft_collector(t_stack *st, void *ptr, int gs_do)
{
	if (gs_do == FT_COLLEC_ADD)
	{
		if (!(st->stack = ft_check_stack_size(st->stack, &(st->size), st->i)))
		{
			st->cleanf(ptr);
			ft_exit(EXIT_FAILURE, "%s: no space left", __func__);
		}
		else
			st->stack[st->i++] = ptr;
	}
	else if (gs_do == FT_COLLEC_GET)
		return (ft_get_stack_ptr(st->stack, &(st->i), ptr));
	else if (gs_do == FT_COLLEC_FREE)
	{
		while (st->i > 0)
			st->cleanf(st->stack[--st->i]);
		free(st->stack);
	}
	return (NULL);
}
