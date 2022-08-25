/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dragon4.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 16:12:44 by yforeau           #+#    #+#             */
/*   Updated: 2021/04/14 18:41:54 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAGON4_H
# define DRAGON4_H

# include <stdint.h>
# define LOG10_2		0.30102999566398119521373889472449
# define DBL_EXP_MAX	0x7FF
# define LDBL_EXP_MAX	0x7FFF
# define LDBL_INT_BIT	0x8000000000000000

/*
** Sign, exp and mantissa are defined by extracting theses values
** from the given float or double. Log2 is the numbers of bits of
** the mantissa, prec is the precision wanted to print the float.
** Digit_exp is the power of ten of the first digit of the float.
** Digit is the current digit during the conversion.
*/

typedef struct			s_bit64d
{
	uint64_t			mantissa: 52;
	uint32_t			exponent: 11;
	uint32_t			sign: 1;
}						t_bit64d;

typedef struct			s_bit80d
{
	uint64_t			mantissa;
	uint32_t			exponent: 15;
	uint32_t			sign: 1;
}						t_bit80d;

typedef union			u_bitd
{
	t_bit64d			sd;
	t_bit80d			ld;
}						t_bitd;

typedef struct			s_fltinf
{
	uint32_t			sign;
	int					exp;
	uint64_t			mantissa;
	uint32_t			log2;
	int					prec;
	int					conv;
	int					digit_exp;
	uint32_t			digit;
	int					exp10;
	int					flags;
}						t_fltinf;

int						dragon4(t_fltinf *info, char *buf);

#endif
