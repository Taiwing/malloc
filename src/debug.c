/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 21:00:10 by yforeau           #+#    #+#             */
/*   Updated: 2022/10/20 09:42:42 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

char			*g_block_type_strings[4] = {
	"INVALID", "TINY", "SMALL", "LARGE"
};

static void		print_hex(void *mem, size_t size)
{
	if (size % 16)
		return;
	while (size)
	{
		ft_dprintf(2, "%1$p %1$8t %04hx\n", mem);
		size -= 16;
		mem += 16;
	}
}

static uint64_t	print_zone(t_memory_zone *zone, int show_free, int show_hex,
	uint64_t *maped_total)
{
	void		*alloc;
	uint64_t	total = 0;

	if (is_free_zone(zone) && !show_free)
		return (0);
	ft_dprintf(2, "%s : %p\n", g_block_type_strings[zone->type],
		(void *)zone + sizeof(t_memory_zone));
	for (t_memory_block *block = zone->blocks; block; block = block->next)
	{
		if (block->free && !show_free)
			continue;
		alloc = (void *)block + sizeof(t_memory_block);
		if (show_free)
			ft_dprintf(2, "%p - %p : %zu %s bytes\n",
				alloc, alloc + block->size, block->size,
				block->free ? "free" : "allocated");
		else
			ft_dprintf(2, "%p - %p : %zu bytes\n",
				alloc, alloc + block->size, block->size);
		if (!block->free)
		{
			total += block->size;
			if (show_hex)
				print_hex(alloc, block->size);
		}
		if (show_free && maped_total)
			*maped_total += block->size;
	}
	return (total);
}

void			show_alloc_mem(void)
{
	uint64_t	allocated = 0;

	for (t_memory_zone *zone = g_zones; zone; zone = zone->next)
		allocated += print_zone(zone, 0, 0, NULL);
	ft_dprintf(2, "Total : %llu bytes\n", allocated);
}

void			show_mem(void)
{
	uint64_t	allocated = 0, total = 0;

	for (t_memory_zone *zone = g_zones; zone; zone = zone->next)
		allocated += print_zone(zone, g_config.show_free,
			g_config.show_hex, &total);
	if (g_config.show_free)
	{
		ft_dprintf(2, "Allocated : %llu bytes\n", allocated);
		ft_dprintf(2, "Total : %llu bytes\n", total);
	}
	else
		ft_dprintf(2, "Total : %llu bytes\n", allocated);
}
