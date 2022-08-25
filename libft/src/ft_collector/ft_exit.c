/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 11:16:55 by yforeau           #+#    #+#             */
/*   Updated: 2021/10/27 07:00:51 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf_internal.h"
#ifdef NO_COLLEC
# include "ft_collector.h"
#endif

/*
** This is the maximum allowed of functions to execute at exit time. In practice
** this limit should never be reached, since atexit functions are supposed to
** each get rid of a type of object requiring a specific function to do the job
** (like "close" for open files). Anyway, if this is really needed, ATEXIT_MAX
** can always be increased.
*/

#define ATEXIT_MAX	1024

/*
** ft_exitmsg stores the begining of the exit message (typically the name
** of the program) and returns it.
*/
#ifdef THREAD_SAFE
MUTEXIFY(char*, ft_exitmsg, char*, str)
#else
char	*ft_exitmsg(char *str)
#endif
{
	static char	*msg = NULL;

	msg = str ? str : msg;
	return (msg);
}

/*
** ft_atexit stores the list of function to execute at the end of the program.
** This should typically be wrappers for simple call to ft_collector with the
** instruction FT_COLLEC_FREE on each instances of "objects", but it can be
** something else since the prototype is really not restrictive (void input,
** void output).
*/
void	ft_atexit(t_atexitf handler)
{
#ifdef THREAD_SAFE
	static __thread t_atexitf	farr[ATEXIT_MAX];
	static __thread size_t		i = 0;
#else
	static t_atexitf			farr[ATEXIT_MAX];
	static size_t				i = 0;
#endif

	if (!handler)
	{
		while (i > 0)
			farr[--i]();
	}
	else if (i < ATEXIT_MAX)
		farr[i++] = handler;
	else
	{
		handler();
		ft_exit(EXIT_FAILURE, "%s: no space left", __func__);
	}
}

static void	print_exit(const char *err)
{
	char	*msg;

	if ((msg = ft_exitmsg(NULL)))
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

static void	set_exiterr(char *errbuf, const char *errfmt, t_farg *args)
{
	t_pdata	errpdata;

	pdata_init(&errpdata, PMODE_BLNOALLOC, -1);
	pdata_set_buf(&errpdata, errbuf, EXIT_MSG_MAX);
	if (ft_printf_internal(&errpdata, errfmt, args) < 0)
		errbuf[0] = 0;
}

void	ft_exit(int ret, const char *errfmt, ...)
{
	char	errbuf[EXIT_MSG_MAX] = { 0 };
	char	*err = errbuf;
	t_farg	args = { 0 };

#ifdef THREAD_SAFE
	ft_set_thread_error(ret);
#endif
	if (errfmt)
	{
		va_start(args.cur, errfmt);
		va_copy(args.ref, args.cur);
		set_exiterr(errbuf, errfmt, &args);
		va_end(args.cur);
		va_end(args.ref);
	}
	ft_set_first_exit(ret, errbuf);
	ft_atexit(NULL);
	ft_get_first_exit(&ret, &err);
	if (err && *err)
		print_exit(err);
	ft_heap_collector(NULL, FT_COLLEC_FREE);
	exit(ret);
}

#ifdef THREAD_SAFE
/*
** ft_thread_exit: simple ft_atexit compatible exit call
*/
void	ft_thread_exit(void)
{
	pthread_exit(NULL);
}
#endif
