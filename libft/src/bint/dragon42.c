/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dragon42.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 14:57:21 by yforeau           #+#    #+#             */
/*   Updated: 2021/07/06 21:11:24 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "dragon4.h"
#include "log_and_ceil.h"

static int	scale_val(t_bint scale, t_bint scaled_val,
	const t_bint n, int *log10)
{
	int			ret;
	uint32_t	log2;
	int			digit_exp;
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	bintset_u64(scale, 2);
	log2 = logbase2_32(n[BINT_LEN(n)]) + ((BINT_LEN(n) - 1) * 32);
	digit_exp = ft_ceil(((int)log2) * LOG10_2 - 0.69);
	ret = BINT_SUCCESS;
	if (digit_exp > 0)
	{
		ret = bint_multpow10(tmp, scale, digit_exp);
		ret = ret == BINT_SUCCESS ? bintcpy(scale, tmp) : ret;
	}
	if (ret == BINT_SUCCESS && bintcmp(scaled_val, scale) < 0)
		ret = bint_smult10(scaled_val);
	else if (ret == BINT_SUCCESS)
		++digit_exp;
	*log10 = digit_exp;
	return (ret);
}

static void	init_binttostr(t_bint scale, t_bint scaled_val)
{
	uint32_t	shift;
	uint32_t	hi_block;

	hi_block = scale[BINT_LEN(scale)];
	if (hi_block < 8 || hi_block > 429496729)
	{
		shift = (32 + 27 - logbase2_32(hi_block)) % 32;
		bint_shiftleft(scale, shift);
		bint_shiftleft(scaled_val, shift);
	}
}

#define DIGIT_BUFLEN 128

static int	binttostr(t_bint scale, t_bint scaled_val, char *cur_digit, int fd)
{
	int			err;
	int			count;
	int			digit;
	char		*buf;
	char		static_buf[DIGIT_BUFLEN];

	err = 0;
	count = 0;
	buf = !cur_digit ? static_buf : cur_digit;
	while (BINT_LEN(scaled_val) && !err)
	{
		digit = bint_divmod_max9(scaled_val, scale);
		*buf++ = (char)(48 + digit);
		++count;
		err = digit == 10;
		if (!cur_digit && !(count % DIGIT_BUFLEN) && !err)
		{
			ft_dprintf(fd, "%.*s", DIGIT_BUFLEN, static_buf);
			buf = static_buf;
		}
		err = !err ? bint_smult10(scaled_val) == BINT_FAILURE : err;
	}
	if (!err && !cur_digit && (count % DIGIT_BUFLEN))
		ft_dprintf(fd, "%.*s", count % DIGIT_BUFLEN, static_buf);
	return (!err ? count : -1);
}

int			dragon42(char *buf, int fd, const t_bint n)
{
	uint32_t	scaled_val[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	scale[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	int			log10;
	int			ret;

	ret = 0;
	log10 = 1;
	if (BINT_LEN(n))
	{
		ret = bint_mult2(scaled_val, n) == BINT_FAILURE;
		ret = !ret ? scale_val(scale, scaled_val, n, &log10) == BINT_FAILURE
			: ret;
		if (ret)
			return (-1);
		init_binttostr(scale, scaled_val);
		ret = binttostr(scale, scaled_val, buf, fd);
	}
	if (ret >= 0 && ret < log10 && buf)
		ft_memset(buf, '0', log10 - ret);
	else if (ret >= 0 && ret < log10)
		ft_dprintf(fd, "%0*d", log10 - ret, 0);
	return (ret >= 0 ? log10 : ret);
}
