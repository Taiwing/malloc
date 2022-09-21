/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 21:07:55 by yforeau           #+#    #+#             */
/*   Updated: 2022/09/21 11:39:58 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"
#include <sys/mman.h>

static size_t		get_zone_size(enum e_block_type type, size_t block_size)
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
	t_memory_zone		*zone = NULL, *ptr = NULL;
	enum e_block_type	type = block_type_from_size(size);
	size_t				zone_size = get_zone_size(type, size);
	t_memory_block		first_block = { 0 };

	//ft_printf("push_new_zone(zones = %p, size = %zu)\n", zones, size); //TEMP
	if ((zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED || !zone)
		return (NULL);
	zone->type = type;
	zone->size = zone_size;
	zone->blocks = (void *)zone + sizeof(t_memory_zone);
	zone->next = NULL;
	first_block.type = type;
	first_block.size = zone_size - sizeof(t_memory_zone) - sizeof(t_memory_block);
	first_block.free = 1;
	ft_memcpy(zone->blocks, &first_block, sizeof(first_block));
	if (!*zones)
		*zones = zone;
	else
	{
		for (ptr = *zones; ptr && ptr->next; ptr = ptr->next);
		ptr->next = zone;
	}
	return (zone);
}

void		delete_zone(t_memory_zone **zones, t_memory_zone *zone)
{
	t_memory_zone	*ptr;

	if (zone == *zones)
		*zones = zone->next;
	else
	{
		for (ptr = *zones; ptr && ptr->next != zone; ptr = ptr->next);
		ptr->next = zone->next;
	}
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
