/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_secatoi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 23:50:36 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/23 05:37:29 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_secatoi: like atoi but with overflow detection and input check
**
** Returns -1 on error and 0 on success. Sets ft_errno if needed.
*/
int	ft_secatoi(int *dest, int min, int max, const char *nptr)
{
	long long int	nb = 0;
	long long int	sign = 1;
	long long int	result = 0;

	if (nptr && (*nptr == '-' || *nptr == '+'))
		sign = *nptr++ == '-' ? -1 : sign;
	while (nptr && *nptr > 47 && *nptr < 58 && nb <= max)
		nb = (nb * 10) + ((*nptr++) - 48);
	if (!nptr || (*nptr && (*nptr <= 47 || *nptr >= 58)))
	{
		ft_errno = E_FTERR_NOT_A_NUMBER;
		return (-1);
	}
	else if ((result = nb * sign) > max || result < min)
	{
		ft_errno = E_FTERR_OVERFLOW;
		return (-1);
	}
	*dest = (int)result;
	return (0);
}
