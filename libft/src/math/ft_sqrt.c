/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 15:02:23 by yforeau           #+#    #+#             */
/*   Updated: 2021/09/01 15:03:37 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ft_sqrt: approximate square root of y with the Babylonian method
*/
double	ft_sqrt(double y)
{
	double	est;

	if (y <= 0.0)
		return (0.0);
	est = y / 2;
	for (int i = 0; i < 10; ++i)
		est = (est + y / est) / 2;
	return (est);
}
