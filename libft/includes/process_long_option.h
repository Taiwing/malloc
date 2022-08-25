/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_long_option.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 22:20:23 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/10 23:12:31 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_LONG_OPTION_H
# define PROCESS_LONG_OPTION_H

# include "ft_getopt.h"

int	process_long_option(int argc, char **argv, t_optdata *d, char *prefix);

#endif
