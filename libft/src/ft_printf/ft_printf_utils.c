/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 11:55:22 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/17 11:55:26 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		uitoa_buf(unsigned int ui, int base, int type, char *buf)
{
	int		i;
	int		size;
	int		nb[128];
	char	*lstr;
	char	*ustr;

	lstr = "0123456789abcdef";
	ustr = "0123456789ABCDEF";
	if (base < 2 || base > 16)
		return (0);
	size = 0;
	while (ui || !size)
	{
		nb[size++] = ui % base;
		ui /= base;
	}
	i = 0;
	while (size--)
		buf[size] = type ? ustr[nb[i++]] : lstr[nb[i++]];
	return (i);
}
