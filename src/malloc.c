/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:05:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/08/23 17:02:27 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

#include <unistd.h> //TEMP

void	free(void *ptr)
{
	//TODO
	(void)ptr;
	write(1, "free!\n", 6); //TEMP
}

void	*malloc(size_t size)
{
	//TODO
	(void)size;
	write(1, "malloc!\n", 8); //TEMP
	return (NULL);
}

void	*realloc(void *ptr, size_t size)
{
	//TODO
	(void)ptr;
	(void)size;
	write(1, "realloc!\n", 9); //TEMP
	return (NULL);
}
