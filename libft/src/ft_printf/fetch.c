/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fetch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 13:55:58 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/24 13:04:01 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

/*
** fetch() uses the va_list macros to fetch arguments
** it is given an index n that if superior to 0 will
** look for the nth argument in the list by creating
** a temporary copy of the list given as a reference
** if it n is equal to 0 it will simply perform take
** the argument of the ref, therby modifying it
** the result is placed a the location given by ptr
*/

static void	arr_cast_int(void *tab, int n, int cast, void *ptr)
{
	if (cast == C_INT)
		*(int *)ptr = ((int *)(tab))[n];
	else if (cast == (C_INT | C_UNSIGNED))
		*(unsigned int *)ptr = ((unsigned int *)(tab))[n];
	else if (cast == (C_INT | C_LONG))
		*(long int *)ptr = ((long int *)(tab))[n];
	else if (cast == (C_INT | C_LONG | C_UNSIGNED))
		*(unsigned long int *)ptr = ((unsigned long int *)(tab))[n];
	else if (cast == (C_INT | C_SHORT))
		*(short int *)ptr = ((short int *)(tab))[n];
	else if (cast == (C_INT | C_SHORT | C_UNSIGNED))
		*(unsigned short int *)ptr = ((unsigned short int *)(tab))[n];
	else if (cast == (C_INT | C_LONG_LONG))
		*(unsigned long long int *)ptr = ((unsigned long long int *)(tab))[n];
}

static void	arr_get_val(void *tab, int n, int cast, void *ptr)
{
	if (cast & C_INT)
		arr_cast_int(tab, n, cast, ptr);
	else if (cast == C_CHAR)
		*(char *)ptr = ((char *)(tab))[n];
	else if (cast == (C_CHAR | C_UNSIGNED))
		*(unsigned char *)ptr = ((unsigned char *)(tab))[n];
	else if (cast == C_SIZE_T)
		*(ssize_t *)ptr = ((ssize_t *)(tab))[n];
	else if (cast == (C_SIZE_T | C_UNSIGNED))
		*(size_t *)ptr = ((size_t *)(tab))[n];
	else if (cast == C_INTMAX_T)
		*(intmax_t *)ptr = ((intmax_t *)(tab))[n];
	else if (cast == (C_INTMAX_T | C_UNSIGNED))
		*(uintmax_t *)ptr = ((uintmax_t *)(tab))[n];
	else if (cast == C_WINT_T)
		*(wint_t *)ptr = ((wint_t *)(tab))[n];
	else if (cast == C_WCHAR_T_P)
		*(wchar_t **)ptr = ((wchar_t **)(tab))[n];
	else if (cast == C_CHAR_P)
		*(char **)ptr = ((char **)(tab))[n];
	else if (cast == C_DOUBLE)
		*(double *)ptr = ((double *)(tab))[n];
	else if (cast == (C_DOUBLE | C_LONG))
		*(long double *)ptr = ((long double *)(tab))[n];
}

static void	va_cast_int(va_list tmp, int cast, void *ptr)
{
	if (cast == C_INT)
		*(int *)ptr = va_arg(tmp, int);
	else if (cast == (C_INT | C_UNSIGNED))
		*(unsigned int *)ptr = va_arg(tmp, unsigned int);
	else if (cast == (C_INT | C_LONG))
		*(long int *)ptr = va_arg(tmp, long int);
	else if (cast == (C_INT | C_LONG | C_UNSIGNED))
		*(unsigned long int *)ptr = va_arg(tmp, unsigned long int);
	else if (cast == (C_INT | C_SHORT))
		*(int *)ptr = va_arg(tmp, int);
	else if (cast == (C_INT | C_SHORT | C_UNSIGNED))
		*(unsigned int *)ptr = va_arg(tmp, unsigned int);
	else if (cast == (C_INT | C_LONG_LONG))
		*(long long int *)ptr = va_arg(tmp, long long int);
	else if (cast == (C_INT | C_LONG_LONG | C_UNSIGNED))
		*(unsigned long long int *)ptr = va_arg(tmp, unsigned long long int);
}

static void	va_get_val(va_list tmp, int cast, void *ptr)
{
	if (cast & C_INT)
		va_cast_int(tmp, cast, ptr);
	else if (cast == C_CHAR)
		*(char *)ptr = va_arg(tmp, int);
	else if (cast == (C_CHAR | C_UNSIGNED))
		*(unsigned char *)ptr = va_arg(tmp, int);
	else if (cast == C_SIZE_T)
		*(ssize_t *)ptr = va_arg(tmp, ssize_t);
	else if (cast == (C_SIZE_T | C_UNSIGNED))
		*(size_t *)ptr = va_arg(tmp, size_t);
	else if (cast == C_INTMAX_T)
		*(intmax_t *)ptr = va_arg(tmp, intmax_t);
	else if (cast == (C_INTMAX_T | C_UNSIGNED))
		*(uintmax_t *)ptr = va_arg(tmp, uintmax_t);
	else if (cast == C_WINT_T)
		*(wint_t *)ptr = va_arg(tmp, wint_t);
	else if (cast == C_WCHAR_T_P)
		*(wchar_t **)ptr = va_arg(tmp, wchar_t *);
	else if (cast == C_CHAR_P)
		*(char **)ptr = va_arg(tmp, char *);
	else if (cast == C_DOUBLE)
		*(double *)ptr = va_arg(tmp, double);
	else if (cast == (C_DOUBLE | C_LONG))
		*(long double *)ptr = va_arg(tmp, long double);
}

void		fetch(t_farg *args, int n, int cast, void *ptr)
{
	va_list tmp;

	if (args->arr && n == 0)
	{
		arr_get_val(args->arr, (args->i)++, cast, ptr);
		(args->j)++;
	}
	else if (n > 0)
	{
		va_copy(tmp, args->ref);
		while (--n)
			va_arg(tmp, int);
		va_get_val(tmp, cast, ptr);
		va_end(tmp);
		if (args->arr)
			(args->j)++;
	}
	else if (n == 0)
		va_get_val(args->cur, cast, ptr);
}
