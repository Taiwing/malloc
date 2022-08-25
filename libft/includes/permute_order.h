/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permute_order.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 18:25:39 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/15 14:31:06 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERMUTE_ORDER_H
# define PERMUTE_ORDER_H

# include "ft_getopt.h"
# include <stdint.h>

void	permute(uintptr_t *argv, t_optdata *d);
void	permute_order(int argc, char **argv, t_optdata *d);

#endif
