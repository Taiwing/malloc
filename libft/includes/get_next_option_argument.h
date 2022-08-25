/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_option_argument.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 18:23:36 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/10 23:00:45 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_OPTION_ARGUMENT_H
# define GET_NEXT_OPTION_ARGUMENT_H

# include "ft_getopt.h"

int	*get_next_option_argument(int argc, char **argv, t_optdata *d, int *c);

#endif
