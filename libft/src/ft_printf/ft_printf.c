/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 14:43:15 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 20:07:10 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

int	ft_printf(const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	pdata_init(&data, PMODE_CLASSIC, 1);
	pdata_set_buf(&data, NULL, 0);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	pdata_init(&data, PMODE_CLASSIC, fd);
	pdata_set_buf(&data, NULL, 0);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_sprintf(char *str, const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	pdata_init(&data, PMODE_NOLIMIT, -1);
	pdata_set_buf(&data, str, 0);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_snprintf(char *str, int size, const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	if (!size)
		return (0);
	pdata_init(&data, PMODE_BUFLIMIT, -1);
	pdata_set_buf(&data, str, (size_t)size);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_asprintf(char **str, const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	pdata_init(&data, PMODE_BUFALLOC, -1);
	pdata_set_buf(&data, NULL, 0);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	*str = data.abuf;
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_zprintf(const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	pdata_init(&data, PMODE_NOALLOC, 1);
	pdata_set_buf(&data, NULL, 0);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_dzprintf(int fd, const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	pdata_init(&data, PMODE_NOALLOC, fd);
	pdata_set_buf(&data, NULL, 0);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_szprintf(char *str, const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	pdata_init(&data, PMODE_NLNOALLOC, -1);
	pdata_set_buf(&data, str, 0);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}

int	ft_snzprintf(char *str, int size, const char *format, ...)
{
	t_farg	args = { 0 };
	t_pdata	data;
	int		ret;

	if (!size)
		return (0);
	pdata_init(&data, PMODE_BLNOALLOC, -1);
	pdata_set_buf(&data, str, (size_t)size);
	va_start(args.cur, format);
	va_copy(args.ref, args.cur);
	ret = ft_printf_internal(&data, format, &args);
	va_end(args.cur);
	va_end(args.ref);
	return (ret);
}
