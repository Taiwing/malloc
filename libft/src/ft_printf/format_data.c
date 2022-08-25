/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 13:10:09 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:30:32 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

static void	apply_fw(t_pdata *l, t_pconv *conv, int fdat[4], int *size)
{
	char	*t;

	if ((t = ft_strchr(FTP_NUMERIC_CONV, conv->type)) && *size < conv->fw
		&& conv->flags & F_ZERO && !(conv->flags & F_MINUS)
		&& (conv->prec < 0 || ft_strchr(FTP_FLOAT_CONV, conv->type)))
	{
		fdat[ZPAD] += conv->fw - *size - (l->buf[0] == '-');
		if (l->buf[0] == '-')
		{
			fdat[SIGN] = '-';
			l->buf[0] = '0';
		}
	}
	else if (!t && *size < conv->fw && conv->flags & F_ZERO
			&& !(conv->flags & F_MINUS) && conv->type != 't')
		fdat[ZPAD] = conv->fw - *size;
	else if (*size < conv->fw && conv->type != 't')
	{
		fdat[SPAD] = conv->fw - *size;
		fdat[SPAD] *= conv->flags & F_MINUS ? -1 : 1;
	}
	else
		return ;
	*size = conv->fw;
}

static void	apply_sign(t_pdata *l, t_pconv *conv, int fdat[4], int *size)
{
	if (ft_strchr(FTP_NUM_SIGNED_CONV, conv->type) && l->buf[0] != '-'
		&& fdat[SIGN] != '-' && (conv->flags & F_PLUS || conv->flags & F_SPACE))
	{
		fdat[SIGN] = conv->flags & F_PLUS ? '+' : ' ';
		(*size)++;
	}
}

static void	apply_prec(t_pdata *l, t_pconv *conv, int *size, int fdat[4])
{
	fdat[ZPAD] = conv->prec - *size;
	if (l->buf[0] == '-')
	{
		fdat[SIGN] = '-';
		l->buf[0] = '0';
		(*size)++;
	}
	*size = *size + fdat[ZPAD];
}

static void	apply_hash(t_pdata *l, char t, int *size, int fdat[4])
{
	if ((t == 'o' || t == 'O') && !fdat[ZPAD] && (l->buf[0] != '0' || !l->n))
	{
		fdat[ZPAD]++;
		(*size)++;
	}
	else if ((ft_strchr("bBxX", t) && !(l->buf[0] == '0' && *size == 1)
			&& *size > 0) || t == 'p' || t == 'P')
	{
		*size = *size + 2;
		if (t == 'b' || t == 'B')
			fdat[HEX] = t == 'b' ? 3 : 4;
		else
			fdat[HEX] = t == 'x' || t == 'p' ? 1 : 2;
	}
}

static void	int_format(t_pconv *conv, t_pdata *l, int fdat[4], int *size)
{
	if (conv->prec > *size - (l->buf[0] == '-'))
		apply_prec(l, conv, size, fdat);
	else if (!conv->prec && *size == 1 && l->buf[0] == '0')
	{
		(*size)--;
		(l->n)--;
		l->buf[0] = 0;
	}
	if (conv->type != 'd' && (conv->flags & F_HASH
		|| conv->type == 'p' || conv->type == 'P'))
		apply_hash(l, conv->type, size, fdat);
}

void		format_data(t_pdata *d, t_pdata *l, t_pconv *conv)
{
	int		size;
	int		fdat[4];

	size = l->n;
	fdat[SPAD] = 0;
	fdat[ZPAD] = 0;
	fdat[SIGN] = 0;
	fdat[HEX] = 0;
	if (ft_strchr(FTP_INT_CONV, conv->type))
		int_format(conv, l, fdat, &size);
	apply_sign(l, conv, fdat, &size);
	apply_fw(l, conv, fdat, &size);
	if (fdat[SPAD] > 0)
		pdata_add(d, NULL, ' ', fdat[SPAD]);
	if (fdat[SIGN])
		pdata_add(d, NULL, fdat[SIGN], 1);
	if (fdat[HEX] && fdat[HEX] < 3)
		pdata_add(d, fdat[HEX] == 1 ? "0x" : "0X", 0, 2);
	if (fdat[HEX] > 2)
		pdata_add(d, fdat[HEX] == 3 ? "0b" : "0B", 0, 2);
	if (fdat[ZPAD] > 0)
		pdata_add(d, NULL, '0', fdat[ZPAD]);
	pdata_add(d, l->buf, 0, l->n);
	if (fdat[SPAD] < 0)
		pdata_add(d, NULL, ' ', fdat[SPAD] * -1);
}
