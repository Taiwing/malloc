/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 21:07:25 by yforeau           #+#    #+#             */
/*   Updated: 2022/09/21 14:36:17 by yforeau          ###   ########.fr       */
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
** Get the given block memory zone
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
** Set a given free block as allocated.
*/
void		allocate_free_block(t_memory_block *block, size_t size)
{
	t_memory_block	new_block = { 0 };

	block->free = 0;
	if (is_splitable_block(block, size))
	{
		new_block.type = block->type;
		new_block.size = block->size - ALIGN_EIGHT(size) - sizeof(new_block);
		new_block.free = 1;
		new_block.next = block->next;
		block->size = ALIGN_EIGHT(size);
		block->next = (void *)block + sizeof(t_memory_block) + block->size;
		ft_memcpy(block->next, &new_block, sizeof(new_block));
	}
}

/*
** Defragment memory by merging adjacent free blocks.
*/
void		merge_free_blocks(t_memory_block *block)
{
	if (!block->free)
		return;
	while (block->next && block->next->free)
	{
		block->size += sizeof(t_memory_block) + block->next->size;
		block->next = block->next->next;
	}
}
