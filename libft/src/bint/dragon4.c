/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dragon4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 20:05:11 by yforeau           #+#    #+#             */
/*   Updated: 2021/07/06 21:12:15 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "dragon4.h"
#include "log_and_ceil.h"

static int	scale_val(t_bint scale, t_bint scaled_val, t_fltinf *info)
{
	int			digit_exp;
	uint32_t	tmp[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	bintset_u64(scaled_val, info->mantissa);
	bint_shiftleft(scaled_val, info->exp > 0 ? info->exp + 1 : 1);
	bintset_pow2(scale, info->exp > 0 ? 1 : 1 - info->exp, 0);
	digit_exp = ft_ceil(((int)info->log2 + info->exp) * LOG10_2 - 0.69);
	if (info->conv == 'f' || info->conv == 'F')
		digit_exp = digit_exp <= -info->prec ? 1 - info->prec : digit_exp;
	if (digit_exp > 0)
		bint_multpow10(tmp, scale, digit_exp);
	else if (digit_exp < 0)
		bint_multpow10(tmp, scaled_val, -digit_exp);
	if (digit_exp)
		bintcpy(digit_exp > 0 ? scale : scaled_val, tmp);
	if (bintcmp(scaled_val, scale) >= 0)
		++digit_exp;
	else
		bint_smult10(scaled_val);
	return (digit_exp);
}

static int	init_ftostr(t_bint scale, t_bint scaled_val, t_fltinf *info)
{
	int			prec;
	uint32_t	hi_block;
	int			cutoff_exp;

	prec = info->prec + info->digit_exp > 16383 ?
		16383 - info->digit_exp : info->prec;
	cutoff_exp = info->conv == 'f' || info->conv == 'F' ?
		-prec : info->digit_exp - prec - 1;
	hi_block = scale[BINT_LEN(scale)];
	if (hi_block < 8 || hi_block > 429496729)
	{
		prec = (32 + 27 - logbase2_32(hi_block)) % 32;
		bint_shiftleft(scale, (uint32_t)prec);
		bint_shiftleft(scaled_val, (uint32_t)prec);
	}
	return (cutoff_exp);
}

static char	*ftostr(t_bint scale, t_bint scaled_val,
		t_fltinf *info, char *cur_digit)
{
	int			cutoff_exp;

	cutoff_exp = init_ftostr(scale, scaled_val, info);
	while ((info->digit = bint_divmod_max9(scaled_val, scale)) < 10
		&& --info->digit_exp > cutoff_exp)
	{
		*cur_digit++ = (char)(48 + info->digit);
		if (!BINT_LEN(scaled_val))
			break ;
		bint_smult10(scaled_val);
	}
	if (info->digit_exp > cutoff_exp)
	{
		ft_memset((void *)cur_digit, '0', --info->digit_exp - cutoff_exp);
		info->digit = 0;
	}
	return (cur_digit + info->digit_exp - cutoff_exp);
}

static char	*round_up9(char *cur_digit, char *buf, int *exp10, int conv)
{
	char	*last_digit;

	last_digit = cur_digit;
	while (1)
	{
		if (cur_digit == buf)
		{
			*cur_digit++ = '1';
			++(*exp10);
			break ;
		}
		--cur_digit;
		if (*cur_digit != '9')
		{
			*cur_digit = *cur_digit + 1;
			++cur_digit;
			break ;
		}
		else
			*cur_digit = '0';
	}
	return (conv == 'g' || conv == 'G' ? last_digit : cur_digit);
}

int			dragon4(t_fltinf *info, char *buf)
{
	uint32_t	scaled_val[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	scale[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	char		*cur_digit;
	int			round_down;
	int			cmp;

	if (!info->mantissa)
	{
		*buf = '0';
		info->exp10 = 0;
		return (1);
	}
	info->digit_exp = scale_val(scale, scaled_val, info);
	info->exp10 = info->digit_exp - 1;
	cur_digit = ftostr(scale, scaled_val, info, buf);
	bint_smult2(scaled_val);
	cmp = bintcmp(scaled_val, scale);
	round_down = !cmp ? !(info->digit & 1) : cmp < 0;
	if (round_down || info->digit < 9)
		*cur_digit++ = (char)(48 + info->digit + !round_down);
	else
		cur_digit = round_up9(cur_digit, buf, &(info->exp10), info->conv);
	return ((int)(cur_digit - buf));
}
