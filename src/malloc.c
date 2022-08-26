/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:05:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/08/26 14:14:09 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

t_memory_zone	*g_zones = NULL;

void	free(void *ptr)
{
	t_memory_zone	*zone;
	t_memory_block	*block;

	ft_printf("\nfree(ptr = %p)\n", ptr); //TEMP
	if (!ptr)
		return;
	block = ptr - sizeof(t_memory_block);
	block->free = 1;
	zone = get_block_zone(block);
	if (zone->type == E_LARGE_BLOCK)
		delete_zone(&g_zones, zone);
	else if (is_free_zone(zone))
	{
		for (t_memory_zone *ptr = g_zones; ptr; ptr = ptr->next)
		{
			if (ptr != zone && ptr->type == zone->type && !is_full_zone(ptr))
			{
				delete_zone(&g_zones, zone);
				break;
			}
		}
	}
	show_mem(); //TEMP
	//TODO: defragment memory by merging empty adjacent blocks
}

void	*malloc(size_t size)
{
	t_memory_zone	*zone = NULL;
	t_memory_block	*block = NULL;

	ft_printf("\nmalloc(size = %zu)\n", size); //TEMP
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
	show_mem(); //TEMP
	//TEMP
	ft_printf("COME ON!\n");
	//ft_bzero((void *)block + sizeof(t_memory_block), block->size);
	ft_printf("YAY!!!\n");
	//TEMP
	return ((void *)block + sizeof(t_memory_block));
}

void	*realloc(void *ptr, size_t size)
{
	t_memory_block		*block;
	void				*new_allocation;

	ft_printf("\nrealloc(ptr = %p, size = %zu)\n", ptr, size); //TEMP
	if (!ptr)
		return (malloc(size));
	block = ptr - sizeof(t_memory_block);
	//TODO: remove this condition and optimize this to defragment memory
	if (block->size >= size)
		return (ptr);
	//TODO: optimize for when the block type remains the same and try to find
	//a big enough block or create one by merging eventual successive free block
	ft_printf("NIQUE 0\n");
	if (!(new_allocation = malloc(size)))
		return (NULL);
	ft_printf("NIQUE 1\n");
	ft_printf(
		"size = %zu\n"
		"new_allocation = %p\n"
		"ptr = %p\n"
		"block->size = %zu\n"
		"SIZE_MAX = %zu\n",
		size, new_allocation, ptr, block->size, SIZE_MAX
	);
	ft_memcpy(new_allocation, ptr, block->size);
	ft_printf("NIQUE 2\n");
	free(ptr);
	ft_printf("NIQUE 3\n");
	show_mem(); //TEMP
	ft_printf("NIQUE 4\n");
	return (new_allocation);
}

void	*calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ft_printf("\ncalloc(nmemb = %zu, size = %zu)\n", nmemb, size); //TEMP
	if (SIZE_MAX / nmemb < size || !(ptr = malloc(nmemb * size)))
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	show_mem(); //TEMP
	return (ptr);
}
