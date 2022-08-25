/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 17:36:36 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/27 06:45:37 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_EXIT_H
# define FT_EXIT_H

# define EXIT_MSG_MAX	4096

enum e_fe_status	{ FIRST_EXIT_OFF = 0, FIRST_EXIT_ON, FIRST_EXIT_FILLED };

/*
** t_exit_args: exit arguments
**
** status: defines if and how the first exit is going to be used
** ret: corresponds to ft_exit argument
** err: buffer storing the error message
*/
typedef struct			s_exit_args
{
	enum e_fe_status	status;
	int					ret;
	char				err[EXIT_MSG_MAX + 1];
}						t_exit_args;

/*
** Global instance of t_exit_args for ft_get_first_exit()
*/
extern t_exit_args	g_first_exit;

typedef void		(*t_atexitf)(void);

char				*ft_exitmsg(char *str);
void				ft_atexit(t_atexitf handler);
void				ft_exit(int ret, const char *errfmt, ...);
void				ft_first_exit(void);
void				ft_set_first_exit(int ret, char *err);
void				ft_get_first_exit(int *ret, char **err);

#endif
