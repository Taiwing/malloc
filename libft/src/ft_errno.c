/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errno.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 05:21:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/23 05:27:42 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ft_errno global declaration
*/
#ifdef THREAD_SAFE
__thread int	ft_errno = 0;
#else
int				ft_errno = 0;
#endif

/*
** Every libft error string. Update this when adding an error to e_libft_errors
** in ft_errno.h to make sure every error code corresponds to a valid string.
*/
const char	*g_errors[E_FTERR_MAX] = {
	// Invalid error format string
	"Unknown error %d",

	// Network
	"invalid protocol",
	"invalid IP family",
	"missing IP address",
	"invalid port range",
	"invalid header level",
	"filter failure",
	"invalid scan handle",
	"scan list is full",
	"scan count is out of bounds",
	"invalid payload",
	"scan in progress",

	// Math
	"overflow",
	"not a number",
	"tried to divide by zero",

	// Other
	"randomness source failure",

	// Global
	"not found",
};

#define SIZE_FTERR_BUF	1024

/*
** ft_strerror: turn an error code (possibly from ft_errno) into an error string
**
** The resulting string is not to be modified by the user. Also it might be
** reset if the user calls ft_strerror again (only in the case where the error
** code is not known). If the error code is not a valid libft error it will
** return a formatted string with the erroneous error code inside.
**
** If a negative value is provided, ft_strerror assumes the error is from the
** libc. It just negates it back to a positive value and uses strerror_r on it.
** If it works, the resulting string is returned. ft_strerror fails otherwise.
**
** This function is thread-safe.
*/
char		*ft_strerror(int error)
{
#ifdef THREAD_SAFE
	static __thread char	error_buf[SIZE_FTERR_BUF] = { 0 };
#else
	static char				error_buf[SIZE_FTERR_BUF] = { 0 };
#endif
	
	if (error > E_FTERR_MIN && error < E_FTERR_MAX)
		return ((char *)g_errors[error]);
	else if (error < E_FTERR_MIN
		&& !strerror_r(-error, error_buf, SIZE_FTERR_BUF))
		return (error_buf);
	ft_snprintf(error_buf, SIZE_FTERR_BUF, g_errors[E_FTERR_MIN], error);
	return (error_buf);
}
