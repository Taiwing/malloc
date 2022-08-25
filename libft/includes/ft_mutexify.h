/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mutexify.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 06:30:04 by yforeau           #+#    #+#             */
/*   Updated: 2021/11/05 06:30:05 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MUTEXFIY_H
# define FT_MUTEXFIY_H

/*
** Mutexify Macro
**
** Create a thread safe version of given function.
**
** More precisely, it will generate a wrapper function that will call the
** original function between mutex locks, store the return on the stack
** (if any) and return it when the mutex is released.
*/

// Fetch 10th argument since we need up to 8 arguments in the __VA_ARGS__ array
#  define _GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

// Depending on the size of __VA_ARGS__, the size is gonna be the 10th argument
#  define _COUNT_VARARGS(...) \
	_GET_NTH_ARG("", ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// Mutex format macros
#  define _TS_N
#  define _TS_C	,

// Calls for each possible size of __VA_ARGS__
#  define _FE_0(ACTION)
#  define _FE_1(ACTION, _1)\
	ACTION(_1, _TS_N)
#  define _FE_2(ACTION, _1, _2)\
	ACTION(_1, _2)
#  define _FE_3(ACTION, _1, _2, _3)\
	ACTION(_1, _2) _TS_C _FE_2(ACTION, _3, _TS_N)
#  define _FE_4(ACTION, _1, _2, _3, _4)\
	ACTION(_1, _2) _TS_C _FE_2(ACTION, _3, _4)
#  define _FE_5(ACTION, _1, _2, _3, _4, _5)\
	ACTION(_1, _2) _TS_C _FE_4(ACTION, _3, _4, _5, _TS_N)
#  define _FE_6(ACTION, _1, _2, _3, _4, _5, _6)\
	ACTION(_1, _2) _TS_C _FE_4(ACTION, _3, _4, _5, _6)
#  define _FE_7(ACTION, _1, _2, _3, _4, _5, _6, _7)\
	ACTION(_1, _2) _TS_C _FE_6(ACTION, _3, _4, _5, _6, _7, _TS_N)
#  define _FE_8(ACTION, _1, _2, _3, _4, _5, _6, _7, _8)\
	ACTION(_1, _2) _TS_C _FE_6(ACTION, _3, _4, _5, _6, _7, _8)

// Loop on __VA_ARGS__
#  define _EXEC_ACTION_TWO_BY_TWO(ACTION, ...)\
	_GET_NTH_ARG("", ##__VA_ARGS__, _FE_8, _FE_7, _FE_6, _FE_5, _FE_4,\
		_FE_3, _FE_2, _FE_1, _FE_0)(ACTION, ##__VA_ARGS__)

// Build arguments
#  define _TAKE_BOTH(_1, _2) _1 _2
#  define _TS_PROTOTYPE(...)\
	_EXEC_ACTION_TWO_BY_TWO(_TAKE_BOTH, __VA_ARGS__) 
#  define _TAKE_NAME(_1, _2) _2
#  define _TS_VARNAMES(...)\
	_EXEC_ACTION_TWO_BY_TWO(_TAKE_NAME, __VA_ARGS__) 

// MUTEXIFY: where the actual magic happens
#  define MUTEXIFY(RET, NAME, ...)\
RET	ts_##NAME(_TS_PROTOTYPE(__VA_ARGS__));\
pthread_mutex_t	NAME##_mutex = PTHREAD_MUTEX_INITIALIZER;\
RET	NAME(_TS_PROTOTYPE(__VA_ARGS__))\
{\
	RET	ret;\
	ft_mutex_lock(&NAME##_mutex);\
	ret = ts_##NAME(_TS_VARNAMES(__VA_ARGS__));\
	ft_mutex_unlock(&NAME##_mutex);\
	return (ret);\
}\
RET	ts_##NAME(_TS_PROTOTYPE(__VA_ARGS__))

// VOID_MUTEXIFY: where the actual magic happens for void functions
#  define VOID_MUTEXIFY(NAME, ...)\
void ts_##NAME(_TS_PROTOTYPE(__VA_ARGS__));\
pthread_mutex_t	NAME##_mutex = PTHREAD_MUTEX_INITIALIZER;\
void NAME(_TS_PROTOTYPE(__VA_ARGS__))\
{\
	ft_mutex_lock(&NAME##_mutex);\
	ts_##NAME(_TS_VARNAMES(__VA_ARGS__));\
	ft_mutex_unlock(&NAME##_mutex);\
}\
void ts_##NAME(_TS_PROTOTYPE(__VA_ARGS__))

#endif
