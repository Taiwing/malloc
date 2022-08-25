/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:05:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/08/25 19:25:46 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <unistd.h>
#include <sys/mman.h>

//TEMP
#include <stdio.h>
#include <string.h>
//TEMP

t_memory_zone	*zones = NULL;

enum e_block_type	block_type_from_size(size_t size)
{
	if (size >= LARGE_BLOCK_SIZE_MIN)
		return (E_LARGE_BLOCK);
	else if (size >= SMALL_BLOCK_SIZE_MIN)
		return (E_SMALL_BLOCK);
	else if (size >= TINY_BLOCK_SIZE_MIN)
		return (E_TINY_BLOCK);
	return (E_INVALID_BLOCK);
}

/*
** Utility boolean function returning 1 if the given block is splitable after
** the size is allocated. Does not check the errors regarding the size or the
** type of the block. It will simply return 0 in these cases.
*/
int			is_splitable_block(t_memory_block *block, size_t size)
{
	if (block->size == size)
		return (0);
	switch (block->type)
	{
		case E_TINY_BLOCK:
			return (block->size >= (ALIGN_EIGHT(size)
				+ TINY_FREE_BLOCK_SIZE_MIN));
			break;
		case E_SMALL_BLOCK:
			return (block->size >= (ALIGN_EIGHT(size)
				+ SMALL_FREE_BLOCK_SIZE_MIN));
			break;
		default:
			return (0);
			break;
	}
}

/*
** Looks for an available block of free memory in the zones list depending on
** the requested size. Invalid or large allocations always return NULL,
** otherwise it returns a pointer to the free block if there is one available.
*/
t_memory_block	*get_free_block(t_memory_zone *zones, size_t size)
{
	enum e_block_type	type;
	t_memory_block		*free_block = NULL;

	type = block_type_from_size(size);
	if (!zones || type == E_LARGE_BLOCK || type == E_INVALID_BLOCK)
		return (free_block);
	for (; zones; zones = zones->next)
	{
		if (zones->type != type)
			continue;
		for (t_memory_block *block = zones->blocks; block; block = block->next)
		{
			if (!block->free)
				continue;
			else if (block->size == size)
				return (block);
			else if (block->size > size)
				free_block = !free_block || free_block->size > block->size ?
					block : free_block;
		}
	}
	return (free_block);
}

t_memory_zone	*get_block_zone(t_memory_block *block)
{
	if (!block)
		return (NULL);
	while (block && block->prev)
		block = block->prev;
	return ((t_memory_zone *)block - sizeof(t_memory_zone));
}

/*
** Set a given free block as allocated.
*/
void		allocate_free_block(t_memory_block *block, size_t size)
{
	t_memory_block	new_block = { 0 };

	block->free = 0;
	if (is_splitable_block(block, size))
	{
		new_block.free = 1;
		new_block.size = block->size - ALIGN_EIGHT(size) - sizeof(new_block);
		new_block.prev = block;
		new_block.next = block->next;
		block->size = ALIGN_EIGHT(size);
		block->next = block + block->size;
		memcpy(block->next, &new_block, sizeof(new_block));
	}
}

size_t			get_zone_size(enum e_block_type type, size_t block_size)
{
	size_t	zone_size = 0, page_size = getpagesize();

	switch (type)
	{
		case E_INVALID_BLOCK:
			return (zone_size);
			break;
		case E_TINY_BLOCK:
			zone_size = TINY_ZONE_SIZE_MIN;
			break ;
		case E_SMALL_BLOCK:
			zone_size = SMALL_ZONE_SIZE_MIN;
			break ;
		case E_LARGE_BLOCK:
			zone_size = block_size + sizeof(t_memory_zone)
				+ sizeof(t_memory_block);
			break ;
	}
	if (zone_size % page_size)
		zone_size += page_size - (zone_size % page_size);
	return (zone_size);
}

t_memory_zone	*push_new_zone(t_memory_zone **zones, size_t size)
{
	t_memory_zone		*zone = NULL;
	enum e_block_type	type = block_type_from_size(size);
	size_t				zone_size = get_zone_size(type, size);
	t_memory_block		first_block = { 0 };

	if ((zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED || !zone)
		return (NULL);
	zone->type = type;
	zone->size = zone_size;
	zone->blocks = (void *)zone + sizeof(t_memory_zone);
	zone->prev = NULL;
	zone->next = NULL;
	first_block.type = type;
	first_block.size = zone_size - sizeof(t_memory_zone);
	first_block.free = 1;
	memcpy(zone->blocks, &first_block, sizeof(first_block));
	while (*zones && (*zones)->next)
		zones = &((*zones)->next);
	*zones = zone;
	return (zone);
}

void		delete_zone(t_memory_zone **zones, t_memory_zone *zone)
{
	if (zone->prev)
		zone->prev->next = zone->next;
	else
		*zones = zone->next;
	munmap(zone, zone->size);
}

int			is_free_zone(t_memory_zone *zone)
{
	if (!zone)
		return (0);
	for (t_memory_block *block = zone->blocks; block; block = block->next)
		if (!block->free)
			return (0);
	return (1);
}

int			is_full_zone(t_memory_zone *zone)
{
	if (!zone)
		return (0);
	for (t_memory_block *block = zone->blocks; block; block = block->next)
		if (block->free)
			return (0);
	return (1);
}

void		free(void *ptr)
{
	t_memory_zone	*zone;
	t_memory_block	*block;

	write(1, "free!\n", 6); //TEMP
	if (!ptr)
		return;
	block = ptr - sizeof(t_memory_block);
	block->free = 1;
	zone = get_block_zone(block);
	if (zone->type == E_LARGE_BLOCK)
		delete_zone(&zones, zone);
	else if (is_free_zone(zone))
	{
		for (t_memory_zone *ptr = zones; ptr; ptr = ptr->next)
		{
			if (ptr != zone && ptr->type == zone->type && !is_full_zone(ptr))
			{
				delete_zone(&zones, zone);
				break;
			}
		}
	}
	//TODO: defragment memory by merging empty adjacent blocks
}

void		*malloc(size_t size)
{
	t_memory_zone	*zone = NULL;
	t_memory_block	*block = NULL;

	write(1, "malloc!\n", 8); //TEMP
	if (!size)
		return (NULL);
	else if ((block = get_free_block(zones, size)))
		allocate_free_block(block, size);
	else if (!(zone = push_new_zone(&zones, size)))
		return (NULL);
	else
	{
		block = zone->blocks;
		allocate_free_block(block, size);
	}
	return ((void *)block + sizeof(t_memory_block));
}

void	*realloc(void *ptr, size_t size)
{
	//TODO
	(void)ptr;
	(void)size;
	write(1, "realloc!\n", 9); //TEMP
	//TEMP
	//TEMP
	return (NULL);
}
