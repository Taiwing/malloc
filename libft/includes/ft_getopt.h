/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 16:09:37 by yforeau           #+#    #+#             */
/*   Updated: 2019/01/17 13:38:09 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_H
# define FT_GETOPT_H

/*
** t_opt is the structure storing long options informations
** name is the option name (without '--').
** has_arg can have 3 values: 0, 1 and 2; which correspond
** respectively to NO_ARG, REQUIRED_ARG and OPTIONAL_ARG
** if flag not NULL, it is where val is stored (getopt returns
** 0 instead of val.
** val is the value identifying the option (it can be the character
** of the short option or another int, if it doesnt correspond to a
** short option for example).
*/

typedef struct		s_opt
{
	const char		*name;
	int				has_arg;
	int				*flag;
	int				val;
}					t_opt;

/*
** behaviour when a non-option argument is met
** DEFAULT: signals getopt to use the mode variable to choose an order
** LEAVE: getopt leaves as soon as non-option is met (is posix compliant)
** PERMUTE: getopt will move the arguments if they are non-options and that
** valid options are found after (or a "--" string)
** PASS: return 1 when a non-option is found and pass to the next argument
*/

enum				e_order {DEFAULT = 0, LEAVE, PERMUTE, PASS};

/*
** t_optdata stores different values needed for option parsing.
** user interface:
**  optind is the index of the current option in argv.
**  optopt is the character of an unknown short option.
**  opterr indicates to getopt if error messages must be printed
**  optarg is the argument of the current option.
**  longopts is the t_opt array of long options.
**  lopts_len is the length of longopts.
**  longindex is the index of the current long option in longopts.
** internals:
**  optstring is the one given by the user without the leading sign
**  order is the way getopt is gonna behave with non-option arguments
**  longonly tells getopt_internal if it has been called from longonly
**  first_nonopt is the index of the first non option argument
**  last_nonopt is the index of the last non option argument
**  nextchar is the next character to be treated by getopt
*/

typedef struct		s_optdata
{
	int				optind;
	int				optopt;
	int				opterr;
	char			*optarg;
	t_opt			*longopts;
	int				lopts_len;
	int				*longindex;
	char			*optstring;
	int				order;
	int				longonly;
	int				first_nonopt;
	int				last_nonopt;
	char			*nextchar;
}					t_optdata;

void				init_getopt(t_optdata *data, char *optstring,
								t_opt *longopts, int *longindex);
int					ft_getopt(int argc, char **argv, t_optdata *data);
int					ft_getopt_long(int argc, char **argv, t_optdata *data);
int					ft_getopt_long_only(int argc, char **argv, t_optdata *data);

#endif
