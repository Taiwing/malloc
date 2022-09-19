/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 21:00:10 by yforeau           #+#    #+#             */
/*   Updated: 2022/09/19 13:07:03 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

char			*g_block_type_strings[4] = {
	"INVALID", "TINY", "SMALL", "LARGE"
};

static uint64_t	print_zone(t_memory_zone *zone, int show_free, uint64_t *maped_total)
{
	void		*alloc;
	uint64_t	total = 0;

	if (is_free_zone(zone) && !show_free)
		return (0);
	ft_printf("%s : %p\n", g_block_type_strings[zone->type],
		(void *)zone + sizeof(t_memory_zone));
	for (t_memory_block *ptr = zone->blocks; ptr; ptr = ptr->next)
	{
		if (ptr->free && !show_free)
			continue;
		alloc = (void *)ptr + sizeof(t_memory_block);
		if (show_free)
			ft_printf("%p - %p : %zu %s bytes\n",
				alloc, alloc + ptr->size, ptr->size,
				ptr->free ? "free" : "allocated");
		else
			ft_printf("%p - %p : %zu bytes\n",
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
		allocated += print_zone(ptr, 0, NULL);
	ft_printf("Total : %llu bytes\n", allocated);
}

void			show_mem(void)
{
	uint64_t	allocated = 0, total = 0;

	for (t_memory_zone *ptr = g_zones; ptr; ptr = ptr->next)
		allocated += print_zone(ptr, 1, &total);
	ft_printf("Allocated : %llu bytes\n", allocated);
	ft_printf("Total : %llu bytes\n", total);
}
