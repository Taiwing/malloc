/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:05:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/08/23 19:27:52 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>

//TEMP
#include <unistd.h>
#include <stdio.h>
//TEMP

void	free(void *ptr)
{
	//TODO
	(void)ptr;
	write(1, "free!\n", 6); //TEMP
}

void	*malloc(size_t size)
{
	char	*mem;
	write(1, "malloc!\n", 8); //TEMP

	mem = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	//TEMP
	if (mem == MAP_FAILED)
	{
		write(1, "MAP_FAILED\n", 11);
		perror("mmap");
	}
	//TEMP
	return (mem == MAP_FAILED ? NULL : mem);
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
