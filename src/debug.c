/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 21:00:10 by yforeau           #+#    #+#             */
/*   Updated: 2022/10/20 09:06:31 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

char			*g_block_type_strings[4] = {
	"INVALID", "TINY", "SMALL", "LARGE"
};

uint64_t		print_zone(t_memory_zone *zone, int show_free, int show_hex,
	uint64_t *maped_total)
{
	void		*alloc;
	uint64_t	total = 0;

	(void)show_hex; //TODO: implement this
	if (is_free_zone(zone) && !show_free)
		return (0);
	ft_dprintf(2, "%s : %p\n", g_block_type_strings[zone->type],
		(void *)zone + sizeof(t_memory_zone));
	for (t_memory_block *ptr = zone->blocks; ptr; ptr = ptr->next)
	{
		if (ptr->free && !show_free)
			continue;
		alloc = (void *)ptr + sizeof(t_memory_block);
		if (show_free)
			ft_dprintf(2, "%p - %p : %zu %s bytes\n",
				alloc, alloc + ptr->size, ptr->size,
				ptr->free ? "free" : "allocated");
		else
			ft_dprintf(2, "%p - %p : %zu bytes\n",
				alloc, alloc + ptr->size, ptr->size);
		if (!ptr->free)
			total += ptr->size;
		if (show_free && maped_total)
			*maped_total += ptr->size;
	}
	return (total);
}

void			show_alloc_mem(void)
{
	uint64_t	allocated = 0;

	for (t_memory_zone *ptr = g_zones; ptr; ptr = ptr->next)
		allocated += print_zone(ptr, 0, 0, NULL);
	ft_dprintf(2, "Total : %llu bytes\n", allocated);
}

void			show_mem(void)
{
	uint64_t	allocated = 0, total = 0;

	for (t_memory_zone *ptr = g_zones; ptr; ptr = ptr->next)
		allocated += print_zone(ptr, g_config.show_free,
			g_config.show_hex, &total);
	if (g_config.show_free)
	{
		ft_dprintf(2, "Allocated : %llu bytes\n", allocated);
		ft_dprintf(2, "Total : %llu bytes\n", total);
	}
	else
		ft_dprintf(2, "Total : %llu bytes\n", allocated);
}
