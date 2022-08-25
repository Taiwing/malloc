/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 12:14:57 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:33:33 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INTERNAL_H
# define FT_PRINTF_INTERNAL_H

# include <sys/types.h>
# include <stdarg.h>
# include <wchar.h>
# include "t_pdata.h"
# include "dragon4.h"

/*
** t_farg: Fetch Arguments (maybe change the name, this is not really clear)
**
** arr: array for the t conversion
** i: index for the t conversion
** j: second index for the t conversion
** cur: current argument
** ref: first argument for reference
*/

typedef struct	s_farg
{
	void		*arr;
	int			i;
	int			j;
	va_list		cur;
	va_list		ref;
}				t_farg;

/*
** Conversion Flags
*/
# define F_HASH			0x0001
# define F_ZERO			0x0002
# define F_MINUS		0x0004
# define F_SPACE		0x0008
# define F_PLUS			0x0010

/*
** Parameter Type Casts
*/
# define C_UNSIGNED		0x0001
# define C_SHORT		0x0002
# define C_LONG			0x0004
# define C_LONG_LONG	0x0008
# define C_INT			0x0010
# define C_CHAR			0x0020
# define C_SIZE_T		0x0040
# define C_INTMAX_T		0x0080
# define C_WINT_T		0x0100
# define C_WCHAR_T_P	0x0200
# define C_CHAR_P		0x0400
# define C_DOUBLE		0x0800

/*
** t_pconv: Conversion Data
**
** type: kind of conversion
** flags: characters given with the conversion (after the %)
** cast: type cast of the parameter (for fetch)
** arg: positional parameter (integer vlaue of $ flag)
** fw: field width
** prec: precision
*/
typedef struct			s_pconv
{
	char				type;
	char				flags;
	int					cast;
	int					arg;
	int					fw;
	int					prec;
}						t_pconv;

# define CONV_INIT		{ 0, 0, 0, 0, 0, -1 }

/*
** Conversions
*/

# define TO_STR_NX(x)			#x
# define TO_STR(x)				TO_STR_NX(x)
# define PPCAT_NX(a, b)			a ## b
# define PPCAT(a, b)			PPCAT_NX(a, b)

# define RAW_SINT_CONV			idD
# define RAW_UINT_CONV			bBoOuUxXpP
# define RAW_FLOAT_CONV			eEfFgG
# define RAW_TEXT_CONV			cCsS
# define RAW_OTHER_CONV			t%

# define RAW_INT_CONV			PPCAT(RAW_SINT_CONV, RAW_UINT_CONV)
# define RAW_NUMERIC_CONV		PPCAT(RAW_INT_CONV, RAW_FLOAT_CONV)
# define RAW_NUM_SIGNED_CONV	PPCAT(RAW_SINT_CONV, RAW_FLOAT_CONV)
# define RAW_ALPHANUM_CONV		PPCAT(RAW_NUMERIC_CONV, RAW_TEXT_CONV)
# define RAW_CONVERSIONS		PPCAT(RAW_ALPHANUM_CONV, RAW_OTHER_CONV)

# define FTP_SINT_CONV			TO_STR(RAW_SINT_CONV)
# define FTP_UINT_CONV			TO_STR(RAW_UINT_CONV)
# define FTP_FLOAT_CONV			TO_STR(RAW_FLOAT_CONV)
# define FTP_TEXT_CONV			TO_STR(RAW_TEXT_CONV)
# define FTP_OTHER_CONV			TO_STR(RAW_OTHER_CONV)

# define FTP_INT_CONV			TO_STR(RAW_INT_CONV)
# define FTP_NUMERIC_CONV		TO_STR(RAW_NUMERIC_CONV)
# define FTP_NUM_SIGNED_CONV	TO_STR(RAW_NUM_SIGNED_CONV)
# define FTP_CONVERSIONS		TO_STR(RAW_CONVERSIONS)


typedef void	(*t_convf)(t_pdata *l, t_farg *a, t_pconv *c, char **f);

void	itoa_cast(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);
void	efg_conversions(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);
void	p_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);
void	c_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);
void	lc_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);
void	s_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);
void	ls_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);
void	t_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt);

/*
** Format Data Enumeration (for format_data function)
*/
enum	e_fdat {SPAD, ZPAD, SIGN, HEX};

/*
** Int functions
*/

int		uitoa_buf(unsigned int ui, int base, int type, char *buf);
void	itoa_int(t_pdata *loc, t_farg *args, t_pconv *conv);
void	itoa_long(t_pdata *loc, t_farg *args, t_pconv *conv);
void	itoa_long_long(t_pdata *loc, t_farg *args, t_pconv *conv);
void	itoa_size_t(t_pdata *loc, t_farg *args, t_pconv *conv);
void	itoa_intmax_t(t_pdata *loc, t_farg *args, t_pconv *conv);
void	itoa_sint(t_pdata *loc, int si);
void	itoa_slint(t_pdata *loc, long int si);
void	itoa_sllint(t_pdata *loc, long long int si);
void	itoa_ssize_t(t_pdata *loc, ssize_t si);
void	itoa_sintmax_t(t_pdata *loc, intmax_t si);
void	itoa_uint(t_pdata *loc, unsigned int ui, int base, int type);
void	itoa_ulint(t_pdata *loc, unsigned long int ui, int base, int type);
void	itoa_ullint(t_pdata *loc, unsigned long long int ui,
					int base, int type);
void	itoa_usize_t(t_pdata *loc, size_t ui, int base, int type);
void	itoa_uintmax_t(t_pdata *loc, uintmax_t ui, int base, int type);

/*
** Float functions
*/

void	e_format(t_pdata *loc, char *buf, int size, t_fltinf *info);
void	f_format(t_pdata *loc, char *buf, int size, t_fltinf *info);
void	g_format(t_pdata *loc, char *buf, int size, t_fltinf *info);

/*
** ft_printf internal functions
*/

int		ft_printf_internal(t_pdata *data, const char *format, t_farg *args);
void	parser(t_pdata *data, char **fmt, t_farg *args);
int		get_flags(char **fmt, t_farg *args, t_pconv *conv);
int		get_conv(char **fmt, t_farg *args, t_pconv *conv);
void	fetch(t_farg *args, int n, int cast, void *ptr);
void	format_data(t_pdata *d, t_pdata *l, t_pconv *conv);

#endif
