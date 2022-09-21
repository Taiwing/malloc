/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:05:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/09/21 15:45:05 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

t_memory_zone	*g_zones = NULL;
t_malloc_config	g_config = { 0 };

void __attribute__ ((constructor))	init_malloc(void)
{
	g_config.history = !!getenv("MALLOC_HISTORY");
	g_config.show = !!getenv("MALLOC_SHOW");
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
	if (g_config.history)
		ft_dprintf(2, "%cfree(ptr = %p)\n", g_config.show ? '\n' : 0, ptr);
	free_internal(ptr);
	if (g_config.show)
		show_alloc_mem();
}

static void	*malloc_internal(size_t size)
{
	t_memory_zone	*zone = NULL;
	t_memory_block	*block = NULL;

	if (!size)
		return (NULL);
	else if ((block = get_free_block(g_zones, size)))
		allocate_free_block(block, size);
	else if (!(zone = push_new_zone(&g_zones, size)))
		return (NULL);
	else
	{
		block = zone->blocks;
		allocate_free_block(block, size);
	}
	return ((void *)block + sizeof(t_memory_block));
}

void		*malloc(size_t size)
{
	void	*ret;

	if (g_config.history)
		ft_dprintf(2, "%cmalloc(size = %zu)\n", g_config.show ? '\n' : 0, size);
	ret = malloc_internal(size);
	if (g_config.show)
		show_alloc_mem();
	return (ret);
}

void		*realloc(void *ptr, size_t size)
{
	t_memory_block		*block;
	void				*new_allocation;

	if (g_config.history)
		ft_dprintf(2, "%crealloc(ptr = %p, size = %zu)\n",
			g_config.show ? '\n' : 0, ptr, size);
	if (!ptr)
		return (malloc(size));
	block = ptr - sizeof(t_memory_block);
	if (!get_block_zone(g_zones, block))
	{
		ft_dprintf(2, "realloc(): invalid pointer\n");
		abort();
	}
	//TODO: remove this condition and optimize this to defragment memory
	if (block->size >= size)
		return (ptr);
	//TODO: optimize for when the block type remains the same and try to find
	//a big enough block or create one by merging eventual successive free block
	if (!(new_allocation = malloc_internal(size)))
		return (NULL);
	ft_memcpy(new_allocation, ptr, block->size);
	free_internal(ptr);
	if (g_config.show)
		show_alloc_mem();
	return (new_allocation);
}

void		*calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (g_config.history)
		ft_dprintf(2, "%ccalloc(nmemb = %zu, size = %zu)\n",
			g_config.show ? '\n' : 0, nmemb, size);
	if (SIZE_MAX / nmemb < size || !(ptr = malloc_internal(nmemb * size)))
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	if (g_config.show)
		show_alloc_mem();
	return (ptr);
}
