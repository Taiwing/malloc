/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_conversion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/25 17:17:30 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/26 07:50:21 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

/*
** TODO:
** to be normified and further tested
** - if fw is negative or if F_MINUS is set, start at the of the array
** - handle no fw (NULL terminated arrays)
** - make all this shit secure
** - maybe handle litteral multidimensionnal arrays with a new T conv
**   or by using the cast modifiers for the t conversion
** MANUAL:
** write clear documentation for this shit
** # -> 	print a trailing character or conversion (takes the char after
**			the last conv, even '\0') at the end of each loop
** fw ->	length of the array (0 if not given)
** prec ->	number of conversions (1 if not given)
** It will loop on x elements of the array (fw), and apply the next y
** conversions (prec) in the format string to the elements of the array in a
** sequential fashion. If there are more elements than conversions
** (ie: if fw > prec), it will loop on the y conversions until x elements are
** printed, if there are more conversions than elements, it will print x
** elements and ignore the remaining ones. At the end, it will set the format
** string pointer "fmt" to the byte after the last conversion or after the
** trailing character. Also the characters between conversions will be printed
** in the order they appear in between the elements of the array.
** This conversion is able to handle multi dimensionnal arrays (meaning,
** that it works fine with nested t conversions).
** The conversions used for %t do not fetch the arguments with va_args, but in
** the array instead. Except if the conversion explicitly indicates a place to
** look for the argument with the $ flag (in this case the conversion doesn't
** count in the 'fw' counting since it doesnt take any element from the array),
** but it needs to be counted in the 'precision' count.
** NOTES:
** - litteral multidimensionnal arrays are not arrays of double pointers,
** ft_printf will crash if multi-dim arrays are given as such to %t
** example: char *tab[10][5] should be considered a char ** of length 50
*/

static void	apply_hash(t_pdata *l, t_pconv *conv, t_farg *tab, char **beg)
{
	if (conv->flags & F_HASH && l->n != -1)
	{
		if (**beg == '%')
			parser(l, beg, tab);
		else
			pdata_add(l, NULL, **beg ? *(*beg)++ : **beg, 1);
	}
}

static char	*tab_parsing(t_pdata *l, char *fmt, t_farg *tab, t_pconv *conv)
{
	t_pconv	cnv;
	char	*mod;
	char	*beg;

	while (tab->i < conv->fw && l->n != -1)
	{
		beg = fmt;
		tab->j = 0;
		while (tab->j < conv->prec && tab->i < conv->fw && *beg && l->n != -1)
			parser(l, &beg, tab);
		if (tab->j == conv->prec && tab->i == 0)
			break ;
		while (tab->i == conv->fw && tab->j < conv->prec && *beg && l->n != -1)
		{
			cnv = (t_pconv)CONV_INIT;
			if (!(mod = ft_strchr(beg, '%')) || !get_conv(&mod, tab, &cnv))
				break ;
			beg = mod;
			tab->j++;
		}
		apply_hash(l, conv, tab, &beg);
	}
	return (beg);
}

void		t_conversion(t_pdata *loc, t_farg *args, t_pconv *conv, char **fmt)
{
	t_farg		tab;

	fetch(args, conv->arg, C_INT | C_UNSIGNED | C_LONG, (void *)&(tab.arr));
	tab.i = 0;
	conv->prec = conv->prec < 0 ? 1 : conv->prec;
	va_copy(tab.ref, args->ref);
	*fmt = tab_parsing(loc, *fmt, &tab, conv);
	va_end(tab.ref);
}
