/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bint.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 12:01:14 by yforeau           #+#    #+#             */
/*   Updated: 2021/08/13 16:36:05 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINT_H
# define BINT_H

# include <stdint.h>

/*
** Bint numbers are signed integers of arbitrary length
**
** They are represented by an array of uint32_t numbers which are its digits
** stored in a 'small-endian' fashion. This means that the first number block
** (after the info block at index 0) is the smallest 32bit digit.
**
** The info block contains the number 'meta data'. It has its size encoded
** which is the maximum number of number blocks it can contain plus one for
** the info block (so it's simply its underlying array size).
**
** BINT_LEN - the number of blocks occupied by the number (0 means value 0)
** max: 7FFF = (2^15)-1 = 32767
** This means a bint number can be at most 1048544 bits long, and have an
** absolute value up to (2^1048544)-1. It can represent (2^1048545)-1 values
** (two times more than its length).
**
** BINT_SIGN - the sign (1 for negative, 0 for positive)
**
** BINT_SIZE - total size of the array (so total number of blocks + 1)
** max: FFFF = (2^16)-1 = 65535
** Of course the maximum useful value is BINT_LEN max + 1 (2^15 = 32768)
** because it is the maximum theoretical space needed to represent a bint.
**
** BINT_SIZE of 36 makes it possible to represent 1120 bits integers
** BINT_SIZE of 520 makes it possible to represent 16608 bits integers
**
** Every bint function will return BINT_FAILURE on failure and BINT_SUCCESS
** otherwise. The two exceptions are bintcmp which has a strcmp type return
** and bint_divmod_max9 which returns a quotient (returns 10 on error).
*/

/*
** TODO (maybe one day): Make the theoretical length of a bint infinite, which
** means the BINT_SIZE and BINT_LEN must also be able to be arbitrarily long,
** so that the only real limit are the system limits. The easiest way to do
** that would be to make the size and length "NULL" terminated, like so:
**
** BLOCK 0: SIGN
** BLOCK 1 ... BLOCK N: SIZE
** BLOCK N+1 ... BLOCK 2*N: LEN
** BLOCK (2*N)+1: START OF ABSOLUTE VALUE
**
** "NULL" terminated is obvioulsy impossible because 0 must be a possible
** valid size block, so the first bit would be reserved to that purpose. It
** would be always set to 1 except on the last size block (which could be
** first, meaning that the absolute value would start on block 3).
**
** The length would always take the same place in the array as the size and
** would be encoded in the same way (it could obvioulsy be way smaller than
** the size so it would not necessarily really need this space, but it would
** be taken for simplicity's sake).
**
** So the start of the absolute value of n would be:
** 1+(BLOCK_LEN(BINT_SIZE(n))*2)
**
** The minimum size of a bint would therefore be 3 (but the length could only
** be zero, and the value also).
**
** The maximum length of n would always be:
** BINT_SIZE(n) - (1+(BLOCK_LEN(BINT_SIZE(n))*2))
** ex: BINT_SIZE(n) = 3; BINT_LEN_MAX(n) = 3 - (1+(1*2)) = 3 - 3 = 0;
** ex: BINT_SIZE(n) = 10; BINT_LEN_MAX(n) = 10 - (1+(1*2)) = 10 - 3 = 7;
** ex: BINT_SIZE(n) = 520; BINT_LEN_MAX(n) = 520 - (1+(1*2)) = 520 - 3 = 517;
** ex: BINT_SIZE(n) = 2^32; BINT_LEN_MAX(n) = 2^32 - (1+(2*2)) = 2^32 - 5;
*/

# define BINT_LEN_MASK			0x00007FFF
# define BINT_SIGN_MASK			0x00008000
# define BINT_SIZE_MASK			0xFFFF0000
# define BINT_LEN(n)			(n[0] & BINT_LEN_MASK)
# define BINT_SIGN(n)			((n[0] & BINT_SIGN_MASK) >> 15)
# define BINT_SIZE(n)			((n[0] & BINT_SIZE_MASK) >> 16)
# define SET_BINT_LEN(n, len)	(n[0] = (n[0] & ~BINT_LEN_MASK)\
								| ((len) & BINT_LEN_MASK))
# define SET_BINT_SIGN(n, sign)	(n[0] = ((sign) ? n[0] | BINT_SIGN_MASK\
								: n[0] & ~BINT_SIGN_MASK))

# define BINT_SIZE_DEF			520
# define BINT_MAX_LEN			(BINT_SIZE_DEF - 1)
# define BINT_MAX_LOG2			(BINT_MAX_LEN * 32)
//# define BINT_MAX_LOG10			(BINT_MAX_LOG2)

# define BINT_FAILURE			1
# define BINT_SUCCESS			0

# define DEFLEN(len)			(len ? len : BINT_SIZE_DEF)
# define BINT_DEFAULT(len)		{[0] = (DEFLEN(len) << 16),\
								[ 1 ... DEFLEN(len) - 1] = 0} 

# define BINT_K_MAX				100

// TODO: check that it takes less time than the loop in long_multiplication
/*
# define BINT_DEFAULT(len)		{[0] = (DEFLEN(len) << 16)}
*/

typedef uint32_t *	t_bint;

/*
** bint utility functions
*/

void				bintclean(t_bint b);
void				bintclr(t_bint b);
int					bintcpy(t_bint dst, const t_bint src);
uint32_t			bintlog2(const t_bint b);
int					bint_is_odd(const t_bint b);
int					bint_is_even(const t_bint b);
int					bint_rand(t_bint n, const t_bint min,
	const t_bint max, uint64_t rand_sign);
int					bint_to_u64(uint64_t *res, const t_bint n);

int					bintset_u64(t_bint b, uint64_t u);
int					bintset_i64(t_bint b, int64_t i);
int					bintset_pow2(t_bint b, uint32_t exp, uint32_t sign);
int					bintset_pow10(t_bint b, uint32_t exp, uint32_t sign);
int					bintset_mask(t_bint b, uint32_t bitlen, uint32_t sign);

int					bintcmp(const t_bint l, const t_bint r);
int					bintcmp_abs(const t_bint l, const t_bint r);

int					dragon42(char *buf, int fd, const t_bint n);

/*
** bint operations
*/

int					bint_add(t_bint res, const t_bint l, const t_bint r);
int					bint_add_u64(t_bint res, const t_bint l, uint64_t r);
int					bint_add_abs(t_bint res, const t_bint l, const t_bint r);

int					bint_sub(t_bint res, const t_bint l, const t_bint r);
int					bint_sub_u64(t_bint res, const t_bint l, uint64_t r);
int					bint_sub_abs(t_bint res, const t_bint l, const t_bint r);

int					bint_mult(t_bint res, const t_bint l, const t_bint r);
int					bint_mult_u32(t_bint res, const t_bint lef, uint32_t rig);
int					bint_mult2(t_bint res, const t_bint in);
int					bint_multpow10(t_bint res, const t_bint in, uint32_t exp);
int					bint_smult2(t_bint res);
int					bint_smult10(t_bint res);

int					bint_divide(t_bint quotient, t_bint remainder,
	const t_bint dividend, const t_bint divisor);
int					bint_divmod(t_bint quotient, t_bint remainder,
	const t_bint dividend, const t_bint divisor);
uint32_t			bint_divmod_max9(t_bint dividend, const t_bint divisor);

int					bint_modexp(t_bint res, const t_bint a,
	const t_bint b, const t_bint c);
int					bint_modinv(t_bint t, t_bint gcd,
	const t_bint a, const t_bint b);
int					bint_modmul(t_bint res, const t_bint a,
	const t_bint b, const t_bint c);

int					bint_shiftleft(t_bint res, uint32_t shift);
int					bint_shiftright(t_bint res, uint32_t shift);
int					bint_shift_zeroes(t_bint res);

int					bint_is_prime(t_bint n, uint64_t k, uint64_t *ret);
int					bint_find_prime(t_bint n, uint64_t k,
	uint64_t size, uint64_t print);

/*
** Bint math constants
*/

extern const uint32_t	g_pow10_u32[8];
extern const uint32_t	*g_pow10_big[10];
extern const uint8_t	g_logtable[256];
extern const uint32_t	g_bint_zero[2];
extern const uint32_t	g_bint_one[2];
extern const uint32_t	g_bint_two[2];
extern const uint32_t	g_bint_three[2];
extern const uint32_t	g_bint_four[2];
extern const uint32_t	g_bint_five[2];
extern const uint32_t	g_bint_six[2];
extern const uint32_t	g_bint_seven[2];
extern const uint32_t	g_bint_eight[2];
extern const uint32_t	g_bint_nine[2];
extern const uint32_t	g_bint_ten[2];
extern const uint32_t	g_bint_max[BINT_SIZE_DEF];
extern const uint32_t	g_bint_min[BINT_SIZE_DEF];

# define	BINT_ZERO	((const t_bint)g_bint_zero)
# define	BINT_ONE	((const t_bint)g_bint_one)
# define	BINT_TWO	((const t_bint)g_bint_two)
# define	BINT_THREE	((const t_bint)g_bint_three)
# define	BINT_FOUR	((const t_bint)g_bint_four)
# define	BINT_FIVE	((const t_bint)g_bint_five)
# define	BINT_SIX	((const t_bint)g_bint_six)
# define	BINT_SEVEN	((const t_bint)g_bint_seven)
# define	BINT_EIGHT	((const t_bint)g_bint_eight)
# define	BINT_NINE	((const t_bint)g_bint_nine)
# define	BINT_TEN	((const t_bint)g_bint_ten)
# define	BINT_MIN	((const t_bint)g_bint_min)
# define	BINT_MAX	((const t_bint)g_bint_max)

#endif
