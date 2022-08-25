/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 21:00:10 by yforeau           #+#    #+#             */
/*   Updated: 2022/08/25 21:30:57 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

char			*g_block_type_strings[4] = {
	"INVALID", "TINY", "SMALL", "LARGE"
};

static uint64_t	print_zone(t_memory_zone *zone)
{
	void		*alloc;
	uint64_t	total = 0;

	if (is_free_zone(zone))
		return (0);
	ft_printf("%s : 0x%llX\n", g_block_type_strings[zone->type],
		(void *)zone + sizeof(t_memory_zone));
	for (t_memory_block *ptr = zone->blocks; ptr; ptr = ptr->next)
	{
		if (ptr->free)
			continue;
		alloc = (void *)ptr + sizeof(t_memory_block);
		ft_printf("0x%llX - 0x%llX : %zu bytes\n", alloc, alloc + ptr->size, ptr->size);
		total += ptr->size;
	}
	return (total);
}

void			show_alloc_mem(void)
{
	uint64_t	total = 0;

	for (t_memory_zone *ptr = g_zones; ptr; ptr = ptr->next)
		total += print_zone(ptr);
	ft_printf("Total : %llu bytes\n", total);
}
