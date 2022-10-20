/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:05:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/10/19 19:09:32 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

t_memory_zone	*g_zones = NULL;
t_malloc_config	g_config = MALLOC_CONFIG_DEF;

void __attribute__ ((constructor))	init_malloc(void)
{
	g_config.show_hex = !!getenv("MALLOC_SHOW_HEX");
	g_config.show_free = !!getenv("MALLOC_SHOW_FREE");
	g_config.show = !!getenv("MALLOC_SHOW") || g_config.show_hex
		|| g_config.show_free;
	g_config.history = !!getenv("MALLOC_HISTORY") || g_config.show;
}

static void	free_internal(void *ptr)
{
	t_memory_zone	*zone;
	t_memory_block	*block;

	if (!ptr)
		return;
	block = ptr - sizeof(t_memory_block);
	if (!(zone = get_block_zone(g_zones, block)))
	{
		ft_dprintf(2, "free(): invalid pointer\n");
		abort();
	}
	block->free = 1;
	if (zone->type == E_LARGE_BLOCK)
		return (delete_zone(&g_zones, zone));
	else if (is_free_zone(zone))
	{
		for (t_memory_zone *ptr = g_zones; ptr; ptr = ptr->next)
		{
			if (ptr != zone && ptr->type == zone->type && !is_full_zone(ptr))
				return (delete_zone(&g_zones, zone));
		}
	}
	defragment_zone(zone);
}

void		free(void *ptr)
{
# ifdef THREAD_SAFE
	ft_mutex_lock(&g_config.mutex);
# endif
	if (g_config.history)
		ft_dprintf(2, "%cfree(ptr = %p)\n", g_config.show ? '\n' : 0, ptr);
	free_internal(ptr);
	if (g_config.show)
		show_mem();
# ifdef THREAD_SAFE
	ft_mutex_unlock(&g_config.mutex);
# endif
}

static void	*malloc_internal(size_t size)
{
	t_memory_zone	*zone = NULL;
	t_memory_block	*block = NULL;

	if (!size)
		return (NULL);
	else if ((block = get_free_block(g_zones, size)))
		allocate_block(block, size);
	else if (!(zone = push_new_zone(&g_zones, size)))
		return (NULL);
	else
	{
		block = zone->blocks;
		allocate_block(block, size);
	}
	return ((void *)block + sizeof(t_memory_block));
}

void		*malloc(size_t size)
{
	void	*ret;

# ifdef THREAD_SAFE
	ft_mutex_lock(&g_config.mutex);
# endif
	ret = malloc_internal(size);
	if (g_config.history)
		ft_dprintf(2, "%cmalloc(size = %zu) --> %p\n",
			g_config.show ? '\n' : 0, size, ret);
	if (g_config.show)
		show_mem();
# ifdef THREAD_SAFE
	ft_mutex_unlock(&g_config.mutex);
# endif
	return (ret);
}

static void	*realloc_internal(void *ptr, size_t size)
{
	t_memory_zone		*zone;
	t_memory_block		*block;
	void				*new_allocation = ptr;

	if (!ptr)
		return (malloc_internal(size));
	block = ptr - sizeof(t_memory_block);
	if (!(zone = get_block_zone(g_zones, block)))
	{
		ft_dprintf(2, "realloc(): invalid pointer\n");
		abort();
	}
	if ((zone = resize_zone(&g_zones, zone, size)))
		new_allocation = (void *)zone->blocks + sizeof(t_memory_block);
	else if (size < block->size)
		allocate_block(block, size);
	else if (size > block->size && !reallocate_block(block, size))
	{
		if (!(new_allocation = malloc_internal(size)))
			return (NULL);
		ft_memcpy(new_allocation, ptr, block->size);
		free_internal(ptr);
	}
	return (new_allocation);
}

void		*realloc(void *ptr, size_t size)
{
	void				*new_allocation;

# ifdef THREAD_SAFE
	ft_mutex_lock(&g_config.mutex);
# endif
	if (g_config.history)
		ft_dprintf(2, "%crealloc(ptr = %p, size = %zu)",
			g_config.show ? '\n' : 0, ptr, size);
	new_allocation = realloc_internal(ptr, size);
	if (g_config.history)
		ft_dprintf(2, " --> %p\n", new_allocation);
	if (g_config.show)
		show_mem();
# ifdef THREAD_SAFE
	ft_mutex_unlock(&g_config.mutex);
# endif
	return (new_allocation);
}

void		*calloc(size_t nmemb, size_t size)
{
	void	*ptr = NULL;

# ifdef THREAD_SAFE
	ft_mutex_lock(&g_config.mutex);
# endif
	if (size <= SIZE_MAX / nmemb && (ptr = malloc_internal(nmemb * size)))
		ft_bzero(ptr, nmemb * size);
	if (g_config.history)
		ft_dprintf(2, "%ccalloc(nmemb = %zu, size = %zu) --> %p\n",
			g_config.show ? '\n' : 0, nmemb, size, ptr);
	if (g_config.show)
		show_mem();
# ifdef THREAD_SAFE
	ft_mutex_unlock(&g_config.mutex);
# endif
	return (ptr);
}
