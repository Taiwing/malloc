/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 21:07:25 by yforeau           #+#    #+#             */
/*   Updated: 2022/10/18 12:57:48 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

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
static int		is_splitable_block(t_memory_block *block, size_t size)
{
	if (block->size == size)
		return (0);
	switch (block->type)
	{
		case E_TINY_BLOCK:
			return (block->size >= (ALIGN_SIXTEEN(size)
				+ TINY_FREE_BLOCK_SIZE_MIN));
			break;
		case E_SMALL_BLOCK:
			return (block->size >= (ALIGN_SIXTEEN(size)
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
	for (t_memory_zone *ptr = zones; ptr; ptr = ptr->next)
	{
		if (ptr->type != type)
			continue;
		for (t_memory_block *block = ptr->blocks; block; block = block->next)
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

/*
** Get the given block memory zone.
*/
t_memory_zone	*get_block_zone(t_memory_zone *zones, t_memory_block *block)
{
	for (t_memory_zone *zone = zones; zone; zone = zone->next)
		for (t_memory_block *ptr = zone->blocks; ptr; ptr = ptr->next)
			if (ptr == block)
				return (zone);
	return (NULL);
}

/*
** Set a given free block as allocated and split block if possible.
*/
void		allocate_block(t_memory_block *block, size_t size)
{
	t_memory_block	new_block = { 0 };

	block->free = 0;
	if (is_splitable_block(block, size))
	{
		new_block.type = block->type;
		new_block.size = block->size - ALIGN_SIXTEEN(size) - sizeof(new_block);
		new_block.free = 1;
		new_block.next = block->next;
		block->size = ALIGN_SIXTEEN(size);
		block->next = (void *)block + sizeof(t_memory_block) + block->size;
		ft_memcpy(block->next, &new_block, sizeof(new_block));
	}
}

/*
** Try to enlarge a tiny/small block of memory if there is a free one after it.
*/
t_memory_block	*reallocate_block(t_memory_block *block, size_t size)
{
	size_t	new_size;

	if (block->type == E_LARGE_BLOCK || !block->next || !block->next->free)
		return (NULL);
	if (block_type_from_size(size) != block->type)
		return (NULL);
	new_size = block->size + block->next->size + sizeof(t_memory_block);
	if (new_size < size)
		return (NULL);
	block->size = new_size;
	block->next = block->next->next;
	allocate_block(block, size);
	return (block);
}
