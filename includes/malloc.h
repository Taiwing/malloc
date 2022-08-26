/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:10:38 by yforeau           #+#    #+#             */
/*   Updated: 2022/08/26 14:06:33 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <stdlib.h>

enum e_block_type	{
	E_INVALID_BLOCK,
	E_TINY_BLOCK,
	E_SMALL_BLOCK,
	E_LARGE_BLOCK,
};

# define	TINY_BLOCK_SIZE_MIN		1		// min size for a tiny block
# define	SMALL_BLOCK_SIZE_MIN	512		// min size for a small block
# define	LARGE_BLOCK_SIZE_MIN	4096	// min size for a large block

# define	TINY_BLOCK_SIZE_MAX		(SMALL_BLOCK_SIZE_MIN - 1)
# define	SMALL_BLOCK_SIZE_MAX	(LARGE_BLOCK_SIZE_MIN - 1)

# define	ZONE_LENGTH				128		// blocks per tiny/small zone

/*
** Memory Block: Individually allocated memory block or free memory
**
** type: size type of the memory block (tiny, small or large)
** size: size of the memory block (not counting this structure)
** free: boolean, 1 if the block is free 0 otherwise
** prev: previous block
** next: next block
*/
typedef struct				s_memory_block
{
	enum e_block_type		type;
	size_t					size;
	int						free;
	struct s_memory_block	*prev;
	struct s_memory_block	*next;
}							t_memory_block __attribute__ ((aligned(8)));

/*
** Memory Zone: mmap allocated memory zone
**
** type: what type of memory blocks it holds (tiny, small or large)
** size: total size of the zone counting this structure (mmap parameter)
** blocks: memory blocks list
** prev: previous memory zone
** next: next memory zone
*/
typedef struct				s_memory_zone
{
	enum e_block_type		type;
	size_t					size;
	t_memory_block			*blocks;
	struct s_memory_zone	*prev;
	struct s_memory_zone	*next;
}							t_memory_zone __attribute__ ((aligned(8)));

/*
** Global memory zone list
*/
extern t_memory_zone		*g_zones;

/*
** Block functions
*/
enum e_block_type	block_type_from_size(size_t size);
t_memory_block		*get_free_block(t_memory_zone *zones, size_t size);
t_memory_zone		*get_block_zone(t_memory_block *block);
void				allocate_free_block(t_memory_block *block, size_t size);

/*
** Zone functions
*/

t_memory_zone	*push_new_zone(t_memory_zone **zones, size_t size);
void			delete_zone(t_memory_zone **zones, t_memory_zone *zone);
int				is_free_zone(t_memory_zone *zone);
int				is_full_zone(t_memory_zone *zone);

/*
** Debug functions
*/
void	show_alloc_mem(void);
void	show_mem(void);

/*
** Malloc functions
*/
void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t nmemb, size_t size);

# define	TINY_ZONE_SIZE_MIN	(\
	sizeof(t_memory_zone) +\
	(TINY_BLOCK_SIZE_MAX + sizeof(t_memory_block)) * ZONE_LENGTH\
)

# define	SMALL_ZONE_SIZE_MIN	(\
	sizeof(t_memory_zone) +\
	(SMALL_BLOCK_SIZE_MAX + sizeof(t_memory_block)) * ZONE_LENGTH\
)

// Return a multiple of 8 from the given value (by increasing it if needed)
# define	ALIGN_EIGHT(VALUE)	(((VALUE + 7) >> 3) << 3)

// Mininum space a free tiny block must contain after a split
# define	TINY_FREE_BLOCK_SIZE_MIN	(\
	TINY_BLOCK_SIZE_MIN + sizeof(t_memory_block)\
)

// Mininum space a free small block must contain after a split
# define	SMALL_FREE_BLOCK_SIZE_MIN	(\
	SMALL_BLOCK_SIZE_MIN + sizeof(t_memory_block)\
)

#endif
