#include <stdio.h>
#include <float.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include "libft.h"
#include "ft_printf.h"
#include "modexp_tests.h"
#include "modinv_tests.h"
#include "modmul_tests.h"

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_RESET   "\x1b[0m"

#define	CONVERSIONS	"dDibBoOuUxXeEfFgGcCsSpPt%"
#define	FLAGS		"#0-+ "
#define	CAST		"hljzL"
const char			g_spaces[256] = { [0 ... 255] = ' ' };
#define SPACES		((char *)g_spaces)
#define LINE_WIDTH	80

int		ret_orig;
int		ret_mine;
char	buf_orig[16384];
char	buf_mine[16384];
int		margin;
int		show;
int		binterr;
int		namelen;

#define PRINT_TITLE(fmtl, fmtr, name, reslen) {\
	namelen = ft_strlen(fmtl) + ft_strlen(fmtr) + ft_strlen(name) + reslen;\
	if (namelen - 4 > LINE_WIDTH)\
		margin = ft_printf("%s%.*s...%s", fmtl, namelen - ft_strlen(fmtl)\
			- ft_strlen(fmtr) - reslen - 7, name, fmtr);\
	else\
		margin = ft_printf("%s%s%s", fmtl, name, fmtr);\
	ft_printf("%.*s", LINE_WIDTH - reslen - margin, SPACES);\
}

#define	PRINTF_TEST(format, ...) {\
	ret_orig = sprintf(buf_orig, format, __VA_ARGS__);\
	ret_mine = ft_sprintf(buf_mine, format, __VA_ARGS__);\
	PRINT_TITLE("TEST:  [", "]", #format, 2);\
	if (ret_orig == ret_mine\
			&& (ret_orig == -1 || !strncmp(buf_orig, buf_mine, ret_orig)))\
	{\
		ft_printf(C_GREEN"OK\n"C_RESET);\
		if (show && ret_orig != -1)\
			ft_printf("%.*s\n", ret_orig, buf_orig);\
		else if (show)\
			ft_printf("ERROR\n");\
	}\
	else\
	{\
		ft_printf(C_RED "KO\n" C_RESET);\
		ft_printf("ret_orig = %d\n", ret_orig);\
		ft_printf("ret_mine = %d\n", ret_mine);\
		ft_printf("ORIG>\t");\
		if (ret_orig != -1)\
		{\
			ft_printf("%.*s", ret_orig, buf_orig);\
			ft_printf("\n");\
		}\
		else\
			ft_printf("ERROR\n");\
		ft_printf("MINE>\t");\
		if (ret_mine != -1)\
		{\
			ft_printf("%.*s", ret_mine, buf_mine);\
			ft_printf("\n");\
		}\
		else\
			ft_printf("ERROR\n");\
	}\
}

#define	FT_PRINTF_TEST(format, ...) {\
	ret_mine = ft_sprintf(buf_mine, format, __VA_ARGS__);\
	PRINT_TITLE("TEST:  [", "]", #format, 2);\
	if (ret_mine != -1)\
	{\
		ft_printf(C_GREEN"OK\n"C_RESET);\
		if (show)\
			ft_printf("%.*s\n", ret_mine, buf_mine);\
	}\
	else\
	{\
		ft_printf(C_RED "KO\n" C_RESET);\
		ft_printf("ret_mine = %d\n", ret_mine);\
	}\
}

#define BINT_TEST(title, functions, ops) {\
	if (show)\
	{\
		ft_printf("\n//////// BEFORE /////////\n");\
		bint_print(a, 16, 1);\
		bint_print(b, 16, 1);\
		bint_print(c, 16, 1);\
		bint_print(d, 16, 1);\
	}\
	binterr = 0;\
	ops;\
	if (show || binterr)\
	{\
		ft_printf("//////// AFTER /////////\n");\
		bint_print(a, 16, 1);\
		bint_print(b, 16, 1);\
		bint_print(c, 16, 1);\
		bint_print(d, 16, 1);\
	}\
	if (show)\
		ft_printf("functions: %s\n", functions);\
	PRINT_TITLE("TEST:  ", "", title, 2);\
	if (!binterr)\
		ft_printf(C_GREEN"OK\n"C_RESET);\
	else\
		ft_printf(C_RED "KO\n" C_RESET);\
}

#define BINT_ASSERT(name, test, ops) {\
	if (!binterr)\
	{\
		ops;\
		binterr = !(test);\
		if (show || binterr)\
		{\
			PRINT_TITLE("ASSERT:  ", "", name, 7);\
			if (!binterr)\
				ft_printf(C_GREEN"SUCCESS\n"C_RESET);\
			else\
				ft_printf(C_RED "FAILURE\n" C_RESET);\
		}\
	}\
}

int		decimal_to_bint(t_bint res, const char *str);

int		bint_print(const t_bint n, uint32_t base, uint32_t info)
{
	base = !base ? 10 : base;
	if (base != 2 && base != 10 && base != 16)
		return (BINT_FAILURE);
	if (!info && BINT_SIGN(n))
		ft_printf("-");
	if (base == 10 && (dragon42(NULL, 1, n) == -1 || !ft_printf("\n")))
		return (BINT_FAILURE);
	ft_printf(base == 2 ? "0b " : base == 16 ? "0x " : "");
	for (int i = BINT_LEN(n); base != 10 && i >= 0; --i)
	{
		if (!i)
			ft_printf("%s", BINT_LEN(n) ? "" : "0\n");
		else if (base == 16)
			ft_printf("%08x%c", n[i], i > 1 ? ' ' : '\n');
		else if (base == 2)
			ft_printf("%032b%c", n[i], i > 1 ? ' ' : '\n');
	}
	if (info)
		ft_printf("info block: size = %u | sign = %u | len = %u\n",
			BINT_SIZE(n), BINT_SIGN(n), BINT_LEN(n));
	return (BINT_SUCCESS);
}

void	test_mandatory(int ac, char **av)
{
	ft_printf("implemented in original funcion:\n");
	PRINTF_TEST("%f", 42.00);
	PRINTF_TEST("{%f}", 1.42);
	PRINTF_TEST("%f", FLT_MIN);
	PRINTF_TEST("%f", FLT_MAX);
	PRINTF_TEST("%f", DBL_MIN);
	PRINTF_TEST("%f", DBL_MAX);
	PRINTF_TEST("%Lf", LDBL_MIN);
	PRINTF_TEST("%e", 1.000);
	PRINTF_TEST("%f", 1000.00);
	PRINTF_TEST("%g", 1000.00);
	PRINTF_TEST("%e", 4242.42424242);
	PRINTF_TEST("%e", 0.0004242);
	PRINTF_TEST("%.3e", 0.0004242);
	PRINTF_TEST("%10.2e", 0.0004242);
	PRINTF_TEST("%#5.12e", 0.0004242);
	PRINTF_TEST("%#5.12e", 0.0004242);
	PRINTF_TEST("%e", FLT_MIN);
	PRINTF_TEST("%e", FLT_MAX);
	PRINTF_TEST("%e", DBL_MIN);
	PRINTF_TEST("%e", DBL_MAX);
	PRINTF_TEST("%Le", LDBL_MIN);
	PRINTF_TEST("%Le", LDBL_MAX);
	PRINTF_TEST("%g", 42.000);
	PRINTF_TEST("%g", 42.420);
	PRINTF_TEST("%g", FLT_MIN);
	PRINTF_TEST("%g", FLT_MAX);
	PRINTF_TEST("%g", DBL_MIN);
	PRINTF_TEST("%g", DBL_MAX);
	PRINTF_TEST("%Lg", LDBL_MIN);
	PRINTF_TEST("%Lg", LDBL_MAX);

	ft_printf("\n\nnot implemented in original function:\n");
	ft_printf("\nt conversion:\n");

	FT_PRINTF_TEST("%#*t\t\t%s\n", ac, av);

	unsigned int		int_tab[10] = {
		0, 1, UINT_MAX, UINT_MAX, INT_MIN, INT_MIN, 6, 654676, 42, 9
	};
	FT_PRINTF_TEST("%#10.2t\t\tfirst field: %08x\tsecond field: %u\n", int_tab);

	ft_printf("\nb conversion:\n");
	FT_PRINTF_TEST("dec: %#3t%-32u \n", int_tab + 5);
	FT_PRINTF_TEST("bin: %#3t%.32b \n", int_tab + 5);

	ft_printf("\nBINT tests:\n");

	uint32_t	i;
	int			ret;
	uint32_t	a[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	b[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	c[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t	d[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	BINT_TEST(
		"pow mult test setup",
		"bintset_u64",
		{
			BINT_ASSERT("set b to 1", ret == BINT_SUCCESS,
				ret = bintset_u64(b, 1));
			BINT_ASSERT("set d to 1", ret == BINT_SUCCESS,
				ret = bintset_u64(d, 1));
		}
	);

	char	assert_name[256];
	char	test_name[256];
	int		step = 16;
	for (i = 16; i <= 128; i += step)
	{
		ft_sprintf(test_name, "set vs smult (pow = %u)", i);
		BINT_TEST(
			test_name,
			"bintset_pow2, bintset_pow10, bint_smult2, bint_smult10",
			{
				ft_sprintf(assert_name, "set a to 2^%u", i);
				BINT_ASSERT(assert_name, ret == BINT_SUCCESS,
					ret = bintset_pow2(a, i, 0));

				ft_sprintf(assert_name, "mult b by 2 (%d times)", step);
				BINT_ASSERT(
					assert_name,
					ret == BINT_SUCCESS,
					{
						for (int j = 0; j < step && ret == BINT_SUCCESS; ++j)
							ret = bint_smult2(b);
					}
				);

				ft_sprintf(assert_name, "set c to 10^%u", i);
				BINT_ASSERT(assert_name, ret == BINT_SUCCESS,
					ret = bintset_pow10(c, i, 0));

				ft_sprintf(assert_name, "mult d by 10 (%d times)", step);
				BINT_ASSERT(
					assert_name,
					ret == BINT_SUCCESS,
					{
						for (int j = 0; j < step && ret == BINT_SUCCESS; ++j)
							ret = bint_smult10(d);
					}
				);

				BINT_ASSERT("a == b", !ret, ret = bintcmp(a, b));
				BINT_ASSERT("c == d", !ret, ret = bintcmp(c, d));
				BINT_ASSERT("a != d", !!ret, ret = bintcmp(a, d));
				BINT_ASSERT("b != c", !!ret, ret = bintcmp(b, c));
			}
		);
	}

	BINT_TEST(
		"sub pow10 test setup",
		"bintcpy",
		{
			BINT_ASSERT("a = c", ret == BINT_SUCCESS, ret = bintcpy(a, c));
			BINT_ASSERT("b = c", ret == BINT_SUCCESS, ret = bintcpy(b, c));
			BINT_ASSERT("a == c", !ret, ret = bintcmp(a, c));
			BINT_ASSERT("b == c", !ret, ret = bintcmp(b, c));
			BINT_ASSERT("d == c", !ret, ret = bintcmp(d, c));
		}
	);

	i -= step;
	ft_sprintf(test_name, "(x * 10) - (x * 9) == x (where x == 10^%u)", i);
	BINT_TEST(
		test_name,
		"bint_sub, bint_mult_u32, bint_smult10",
		{
			BINT_ASSERT("a *= 10", ret == BINT_SUCCESS,
				ret = bint_smult10(a));
			BINT_ASSERT("b = c * 9", ret == BINT_SUCCESS,
				ret = bint_mult_u32(b, c, 9));
			BINT_ASSERT("c = a - b", ret == BINT_SUCCESS,
				ret = bint_sub(c, a, b));
			BINT_ASSERT("c == d", !ret, ret = bintcmp(c, d));
		}
	);

	BINT_TEST(
		"sub negtive pow10/pow2 test setup",
		"bintcpy",
		{
			BINT_ASSERT("a = c", ret == BINT_SUCCESS, ret = bintcpy(a, c));
			BINT_ASSERT("b = c", ret == BINT_SUCCESS, ret = bintcpy(b, c));
			BINT_ASSERT("a == c", !ret, ret = bintcmp(a, c));
			BINT_ASSERT("b == c", !ret, ret = bintcmp(b, c));
			BINT_ASSERT("d == c", !ret, ret = bintcmp(d, c));
		}
	);

	ft_sprintf(test_name, "-x - x == -(2 * x) (where x == 10^%u)", i);
	BINT_TEST(
		test_name,
		"bint_sub, SET_BINT_SIGN, bint_smult2",
		{
			BINT_ASSERT("a = -a", !bintcmp_abs(a, b) && BINT_SIGN(a),
				SET_BINT_SIGN(a, 1));
			BINT_ASSERT("c = a - b", ret == BINT_SUCCESS,
				ret = bint_sub(c, a, b));
			BINT_ASSERT("c == -(2 * d)", !bintcmp_abs(c, d) && BINT_SIGN(c),
				bint_smult2(d));
		}
	);

	BINT_TEST(
		"x - (2 * x) == x",
		"bint_sub",
		{
			BINT_ASSERT("c = b - d", ret == BINT_SUCCESS,
				ret = bint_sub(c, b, d));
			BINT_ASSERT("c == -b", !ret && BINT_SIGN(c),
				ret = bintcmp_abs(c, b));
		}
	);

	bintset_u64(a, UINT64_MAX);
	bintset_u64(b, UINT64_MAX);
	bintset_u64(c, UINT64_MAX);
	BINT_TEST(
		"x + x == 2 * x (where x == UINT64_MAX)",
		"bint_add, bint_smult2",
		{
			bint_add(d, a, b);
			bint_smult2(a);
			BINT_ASSERT("d = a + b = 2 * a", !ret, ret = bintcmp(a, d));
		}
	);

	bintset_u64(a, UINT64_MAX);
	bintset_u64(b, UINT64_MAX);
	bintset_u64(c, UINT64_MAX);
	BINT_TEST(
		"x - x == 0 (where x == UINT64_MAX)",
		"bint_sub",
		{
			bint_sub(d, a, b);
			bintclr(a);
			BINT_ASSERT("d = a - b = 0", !ret && !BINT_LEN(d),
				ret = bintcmp(a, d));
		}
	);

	bintset_u64(a, UINT64_MAX);
	bintset_u64(b, UINT64_MAX - 1);
	bintset_u64(c, 1);
	BINT_TEST(
		"x - (x-1) == 1 (where x == UINT64_MAX)",
		"bint_sub",
		{
			bint_sub(d, a, b);
			BINT_ASSERT("d == a - b == 1", !ret, ret = bintcmp(d, c));
		}
	);

	bintset_u64(a, UINT64_MAX);
	bintset_u64(b, UINT64_MAX - 1);
	bintset_u64(c, 1);
	SET_BINT_SIGN(c, 1);
	BINT_TEST(
		"(x-1) - x == -1 (where x == UINT64_MAX)",
		"bint_sub",
		{
			bint_sub(d, b, a);
			BINT_ASSERT("d == b - a == -1", !ret, ret = bintcmp(d, c));
		}
	);

	bintset_u64(a, UINT64_MAX);
	bint_smult2(a);
	bintset_u64(b, UINT64_MAX);
	bintset_u64(c, UINT64_MAX);
	BINT_TEST(
		"(2 * x) - x == x (where x == UINT64_MAX)",
		"bint_sub",
		{
			bint_sub(d, a, b);
			BINT_ASSERT("d == a - b == c", !ret, ret = bintcmp(d, c));
		}
	);

	bintset_u64(a, UINT64_MAX);
	bint_smult2(a);
	bint_smult2(a);
	bintset_u64(b, UINT64_MAX);
	bint_smult2(b);
	bintset_u64(c, UINT64_MAX);
	bint_smult2(c);
	BINT_TEST(
		"(2 * x) - x == x (where x == 2 * UINT64_MAX)",
		"bint_sub",
		{
			bint_sub(d, a, b);
			BINT_ASSERT("d == a - b == c", !ret, ret = bintcmp(d, c));
		}
	);

	bintset_pow10(a, 128, 0);
	bintset_pow10(b, 256, 0);
	bintset_pow10(c, 384, 0);
	BINT_TEST(
		"10^x * 10^y == 10^(x+y) (where x == 128 && y == 256)",
		"bint_mult",
		{
			bint_mult(d, a, b);
			BINT_ASSERT("d == a * b == c", !ret, ret = bintcmp(d, c));
		}
	);

	#define MODTC 6
	#define MODTL 4
	int64_t modtable[MODTL][MODTC] = {
		{ 117, 17, 6, 15, 6, 15 },
		{ -117, 17, -7, 2, -6, -15 },
		{ -117, -17, 6, -15, 6, -15 },
		{ 117, -17, -7, -2, -6, 15 },
	};
	uint32_t e[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t f[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t g[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	uint32_t h[BINT_SIZE_DEF] = BINT_DEFAULT(0);
	for (int i = 0; i < MODTL; ++i)
	{
		bintset_i64(a, modtable[i][0]);
		bintset_i64(b, modtable[i][1]);
		bintset_i64(c, modtable[i][2]);
		bintset_i64(d, modtable[i][3]);
		bintset_i64(e, modtable[i][4]);
		bintset_i64(f, modtable[i][5]);
		ft_sprintf(test_name, "divmod vs divide => a/b (a = %ld, b = %ld)",
			modtable[i][0], modtable[i][1]);
		BINT_TEST(
			test_name,
			"bint_divmod, bint_divide",
			{
				bint_divmod(g, h, a, b);
				ft_sprintf(assert_name, "g == a / b == %ld / %ld ==  %ld",
					modtable[i][0], modtable[i][1], modtable[i][2]);
				BINT_ASSERT(assert_name, !ret, ret = bintcmp(g, c));
				ft_sprintf(assert_name, "h == a %% b == %ld / %ld ==  %ld",
					modtable[i][0], modtable[i][1], modtable[i][3]);
				BINT_ASSERT(assert_name, !ret, ret = bintcmp(h, d));

				bint_divide(g, h, a, b);
				ft_sprintf(assert_name, "g == a / b == %ld / %ld ==  %ld",
					modtable[i][0], modtable[i][1], modtable[i][4]);
				BINT_ASSERT(assert_name, !ret, ret = bintcmp(g, e));
				ft_sprintf(assert_name, "h == a %% b == %ld / %ld ==  %ld",
					modtable[i][0], modtable[i][1], modtable[i][5]);
				BINT_ASSERT(assert_name, !ret, ret = bintcmp(h, f));
			}
		);
	}

	BINT_TEST(
		"set mask to BINT_MAX (with bitlen == BINT_MAX_LOG2)",
		"bintset_mask",
		{
			bintclr(a);
			bintset_mask(a, BINT_MAX_LOG2, 0);
			BINT_ASSERT("a == BINT_MAX", !ret, ret = bintcmp(a, BINT_MAX));
		}
	);

	int bitlen = ((BINT_SIZE(a) - 1) * 32) + 1;
	BINT_TEST(
		"try to set a mask with bitlen == ((BINT_SIZE(a) - 1) * 32) + 1",
		"bintset_mask",
		{
			bintclr(a);
			BINT_ASSERT("bitlen == BINT_MAX_LOG2 + 1",
				!ret, ret = bitlen != BINT_MAX_LOG2 + 1);
			BINT_ASSERT(
				"bintset_mask cleanly fails with a too big bitlen value",
				ret == BINT_FAILURE,
				ret = bintset_mask(a, bitlen, 0)
			);
		}
	);

	#define XPOWS_LEN	7
	int xpows[XPOWS_LEN] = { 0, 1, 2, 3, 4005, BINT_MAX_LOG2 / 2, BINT_MAX_LOG2 - 1 };
	for (int i = 0, xpow = 0; i < XPOWS_LEN; ++i)
	{
		xpow = xpows[i];
		ft_sprintf(test_name,
			"bint_setmask(a, x, 0) == (2^x)-1 (where x == %d)", xpow);
		BINT_TEST(
			test_name,
			"bintset_mask",
			{
				bintclr(a);
				bintset_mask(a, xpow, 0);
				bintset_pow2(b, xpow, 0);
				bint_sub_u64(b, b, 1);
				BINT_ASSERT("a == b", !ret, ret = bintcmp(a, b));
			}
		);
	}

	bintclr(a);
	BINT_TEST(
		"generate positive bint between 0 and BINT_MAX",
		"bint_rand, bint_divide",
		{
			BINT_ASSERT(
				"bint_rand succeeds",
				ret == BINT_SUCCESS,
				ret = bint_rand(a, BINT_ZERO, BINT_MAX, 0)
			);
			BINT_ASSERT(
				"a >= BINT_ZERO",
				ret >= 0,
				ret = bintcmp(a, BINT_ZERO)
			);
			BINT_ASSERT(
				"a <= BINT_MAX",
				ret <= 0,
				ret = bintcmp(a, BINT_MAX)
			);
			BINT_ASSERT("a is positive", !ret, ret = BINT_SIGN(a));
		}
	);

	bintclr(b);
	BINT_TEST(
		"generate bint between BINT_MAX and BINT_MAX",
		"bint_rand, bint_divide",
		{
			BINT_ASSERT(
				"bint_rand succeeds",
				ret == BINT_SUCCESS,
				ret = bint_rand(b, BINT_MAX, BINT_MAX, 0)
			);
			BINT_ASSERT("b == BINT_MAX", !ret, ret = bintcmp(b, BINT_MAX));
			BINT_ASSERT("b is positive", !ret, ret = BINT_SIGN(b));
		}
	);

	bintclr(c);
	bintclr(d);
	bintset_mask(d, BINT_MAX_LOG2 / 2, 0);
	ft_sprintf(test_name,
		"generate positive number between 0 and (2^%d)-1",
		BINT_MAX_LOG2 / 2);
	BINT_TEST(
		test_name,
		"bint_rand, bint_divide",
		{
			BINT_ASSERT(
				"bint_rand succeeds",
				ret == BINT_SUCCESS,
				ret = bint_rand(c, BINT_ZERO, d, 0)
			);
			BINT_ASSERT(
				"c >= BINT_ZERO",
				ret >= 0,
				ret = bintcmp(c, BINT_ZERO)
			);
			BINT_ASSERT(
				"c <= d",
				ret <= 0,
				ret = bintcmp(c, d)
			);
			BINT_ASSERT("c is positive", !ret, ret = BINT_SIGN(c));
		}
	);

	bintclr(a);
	bintclr(b);
	bintclr(c);
	bintclr(d);
	BINT_TEST(
		"modexp basic big test (from rosetta code)",
		"bint_modexp",
		{
			BINT_ASSERT(
				"set a to defined value",
				ret == BINT_SUCCESS,
				ret = decimal_to_bint(a, "2988348162058574136915891421498819466320163312926952423791023078876139")
			);
			BINT_ASSERT(
				"set b to defined value (exponent)",
				ret == BINT_SUCCESS,
				ret = decimal_to_bint(b, "2351399303373464486466122544523690094744975233415544072992656881240319")
			);
			BINT_ASSERT(
				"set e to defined value (result)",
				ret == BINT_SUCCESS,
				ret = decimal_to_bint(e, "1527229998585248450016808958343740453059")
			);
			bintset_pow10(c, 40, 0);
			BINT_ASSERT(
				"bint_modexp(d, a, b, c) succeeds",
				ret == BINT_SUCCESS,
				ret = bint_modexp(d, a, b, c)
			);
			BINT_ASSERT("d == e", !ret, ret = bintcmp(d, e));
		}
	);

	//for (int i = 0; modexp_tests[i][0] != NULL; ++i)
	//works well up to 150 if -O3 is set
	for (int i = 0; i < 125 && modexp_tests[i][0] != NULL; ++i)
	{
		bintclr(a);
		bintclr(b);
		bintclr(c);
		bintclr(d);
		bintclr(e);
		ft_sprintf(test_name, "modexp generated test nb %d (%s bits)",
			i + 1, modexp_tests[i][4]);
		BINT_TEST(
			test_name,
			"bint_modexp",
			{
				BINT_ASSERT(
					"set a to defined value",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(a, modexp_tests[i][0])
				);
				BINT_ASSERT(
					"set b to defined value (exponent)",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(b, modexp_tests[i][1])
				);
				BINT_ASSERT(
					"set c to defined value (modulo)",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(c, modexp_tests[i][2])
				);
				BINT_ASSERT(
					"set e to defined value (result)",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(e, modexp_tests[i][3])
				);
				BINT_ASSERT(
					"bint_modexp(d, a, b, c) succeeds",
					ret == BINT_SUCCESS,
					ret = bint_modexp(d, a, b, c)
				);
				BINT_ASSERT("d == e", !ret, ret = bintcmp(d, e));
			}
		);
	}

	for (int i = 0; modinv_tests[i][0] != NULL; ++i)
	{
		bintclr(a);
		bintclr(b);
		bintclr(c);
		bintclr(d);
		bintclr(e);
		ft_sprintf(test_name, "modinv generated test nb %d (%s bits)",
			i + 1, modinv_tests[i][3]);
		BINT_TEST(
			test_name,
			"bint_modinv",
			{
				BINT_ASSERT(
					"set a to defined value",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(a, modinv_tests[i][0])
				);
				BINT_ASSERT(
					"set b to defined value (modulo)",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(b, modinv_tests[i][1])
				);
				BINT_ASSERT(
					"set c to defined value (result)",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(c, modinv_tests[i][2])
				);
				BINT_ASSERT(
					"bint_modinv(d, e, a, b) succeeds",
					ret == BINT_SUCCESS,
					ret = bint_modinv(d, e, a, b)
				);
				BINT_ASSERT("d == c", !ret, ret = bintcmp(d, c));
				BINT_ASSERT("e == 1", !ret, ret = bintcmp(e, BINT_ONE));
			}
		);
	}

	//for (int i = 0; modmul_tests[i][0] != NULL; ++i)
	for (int i = 0; i < 125 && modmul_tests[i][0] != NULL; ++i)
	{
		bintclr(a);
		bintclr(b);
		bintclr(c);
		bintclr(d);
		bintclr(e);
		ft_sprintf(test_name, "modmul generated test nb %d (%s bits)",
			i + 1, modmul_tests[i][4]);
		BINT_TEST(
			test_name,
			"bint_modexp",
			{
				BINT_ASSERT(
					"set a to defined value",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(a, modmul_tests[i][0])
				);
				BINT_ASSERT(
					"set b to defined value",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(b, modmul_tests[i][1])
				);
				BINT_ASSERT(
					"set c to defined value (modulo)",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(c, modmul_tests[i][2])
				);
				BINT_ASSERT(
					"set e to defined value (result)",
					ret == BINT_SUCCESS,
					ret = decimal_to_bint(e, modmul_tests[i][3])
				);
				BINT_ASSERT(
					"bint_modmul(d, a, b, c) succeeds",
					ret == BINT_SUCCESS,
					ret = bint_modmul(d, a, b, c)
				);
				BINT_ASSERT("d == e", !ret, ret = bintcmp(d, e));
			}
		);
	}
}

#define BINTF_MAX_ARGS 4

/*
** FIRST: return (V for void, I for int)
** SECOND AND AFTER: parameters
** B for t_bint
** U32 for uint32_t
** U64 for uint64_t
** I64 for int64_t
**
** NONE is just for the null element
*/
enum				e_ftype {
	V_B_U32,
	V_B,
	I_B_B,
	I_B_U64,
	I_B_I64,
	I_B_U32,
	I_B_B_B,
	I_B_B_U32,
	I_B,
	I_B_U32_U32,
	I_B_B_U64,
	I_B_B_B_B,
	I_B_B_B_U64,
	I_B_U64_U64_U64,
	V,
	NONE
};

/*
** NOPE: nothing (end of arguments)
** VR: void return
** IR: int return
** BA: t_bint argument
** U32A: uint32_t argument
** U64A: uint64_t argument
** I64A: int64_t argument
*/
enum				e_argtype { NOPE, VR, IR, BA, U32A, U64A, I64A };

const int			g_ftypes_protos[][BINTF_MAX_ARGS + 2] = {
	{VR,	BA,		U32A,	NOPE,	NOPE,	NOPE},
	{VR,	BA,		NOPE,	NOPE,	NOPE,	NOPE},
	{IR,	BA,		BA,		NOPE,	NOPE,	NOPE},
	{IR,	BA,		U64A,	NOPE,	NOPE,	NOPE},
	{IR,	BA,		I64A,	NOPE,	NOPE,	NOPE},
	{IR,	BA,		U32A,	NOPE,	NOPE,	NOPE},
	{IR,	BA,		BA,		BA,		NOPE,	NOPE},
	{IR,	BA,		BA,		U32A,	NOPE,	NOPE},
	{IR,	BA,		NOPE,	NOPE,	NOPE,	NOPE},
	{IR,	BA,		U32A,	U32A,	NOPE,	NOPE},
	{IR,	BA,		BA,		U64A,	NOPE,	NOPE},
	{IR,	BA,		BA,		BA,		BA,		NOPE},
	{IR,	BA,		BA,		BA,		U64A,	NOPE},
	{IR,	BA,		U64A,	U64A,	U64A,	NOPE},
	{VR,	NOPE,	NOPE,	NOPE,	NOPE,	NOPE},
	{NOPE,	NOPE,	NOPE,	NOPE,	NOPE,	NOPE},
};

typedef struct		s_bintcmd
{
	const char		*name;
	int				name_len;
	enum e_ftype	ftype;
	void			*f;
}					t_bintcmd;

static void	bc_help(void);
static void	bc_env(void);
static void	bc_exit(void);
static void	bc_log2(const t_bint n);
static void	bc_is_odd(const t_bint n);
static void	bc_is_even(const t_bint n);
static int	bc_is_prime(const t_bint n, uint64_t k);

#define DEFINE_BINTCMD(name, ftype, f) { name, (sizeof(name) - 1), ftype, f}
const t_bintcmd		g_bint_commands[] = {
	DEFINE_BINTCMD( "clean",		V_B,				bintclean			),
	DEFINE_BINTCMD( "clr",			V_B,				bintclr				),
	DEFINE_BINTCMD( "cpy",			I_B_B,				bintcpy				),
	DEFINE_BINTCMD( "set_u64",		I_B_U64,			bintset_u64			),
	DEFINE_BINTCMD( "set_i64",		I_B_I64,			bintset_i64			),
	DEFINE_BINTCMD( "set_pow2",		I_B_U32_U32,		bintset_pow2		),
	DEFINE_BINTCMD( "set_pow10",	I_B_U32_U32,		bintset_pow10		),
	DEFINE_BINTCMD( "set_mask",		I_B_U32_U32,		bintset_mask		),
	DEFINE_BINTCMD( "cmp",			I_B_B,				bintcmp				),
	DEFINE_BINTCMD( "cmp_abs",		I_B_B,				bintcmp_abs			),
	DEFINE_BINTCMD( "add",			I_B_B_B,			bint_add			),
	DEFINE_BINTCMD( "add_abs",		I_B_B_B,			bint_add_abs		),
	DEFINE_BINTCMD( "add_u64",		I_B_B_U64,			bint_add_u64		),
	DEFINE_BINTCMD( "sub",			I_B_B_B,			bint_sub			),
	DEFINE_BINTCMD( "sub_abs",		I_B_B_B,			bint_sub_abs		),
	DEFINE_BINTCMD( "sub_u64",		I_B_B_U64,			bint_sub_u64		),
	DEFINE_BINTCMD( "mult",			I_B_B_B,			bint_mult			),
	DEFINE_BINTCMD( "mult_u32",		I_B_B_U32,			bint_mult_u32		),
	DEFINE_BINTCMD( "mult2",		I_B_B,				bint_mult2			),
	DEFINE_BINTCMD( "multpow10",	I_B_B_U32,			bint_multpow10		),
	DEFINE_BINTCMD( "smult2",		I_B,				bint_smult2			),
	DEFINE_BINTCMD( "smult10",		I_B,				bint_smult10		),
	DEFINE_BINTCMD( "shiftleft",	I_B_U32,			bint_shiftleft		),
	DEFINE_BINTCMD( "shiftright",	I_B_U32,			bint_shiftright		),
	DEFINE_BINTCMD( "divide",		I_B_B_B_B,			bint_divide			),
	DEFINE_BINTCMD( "divmod",		I_B_B_B_B,			bint_divmod			),
	DEFINE_BINTCMD( "log2",			V_B,				bc_log2				),
	DEFINE_BINTCMD( "is_odd",		V_B,				bc_is_odd			),
	DEFINE_BINTCMD( "is_even",		V_B,				bc_is_even			),
	DEFINE_BINTCMD( "rand",			I_B_B_B_U64,		bint_rand			),
	DEFINE_BINTCMD( "modexp",		I_B_B_B_B,			bint_modexp			),
	DEFINE_BINTCMD( "modinv",		I_B_B_B_B,			bint_modinv			),
	DEFINE_BINTCMD( "modmul",		I_B_B_B_B,			bint_modmul			),
	DEFINE_BINTCMD( "find_prime",	I_B_U64_U64_U64,	bint_find_prime		),
	DEFINE_BINTCMD( "is_prime",		I_B_U64,			bc_is_prime			),
	DEFINE_BINTCMD( "shift_zeroes",	I_B,				bint_shift_zeroes	),
	DEFINE_BINTCMD( "print",		I_B_U32_U32,		bint_print			),
	DEFINE_BINTCMD( "help",			V,					bc_help				),
	DEFINE_BINTCMD( "env",			V,					bc_env				),
	DEFINE_BINTCMD( "exit",			V,					bc_exit				),
	DEFINE_BINTCMD( "quit",			V,					bc_exit				),
	DEFINE_BINTCMD( NULL,			NONE,				NULL				),
};

static void	bc_help(void)
{
	int			ftype;
	const char	*argtypes[4] = { "t_bint", "uint32_t", "uint64_t", "int64_t" };

	for (int i = 0; g_bint_commands[i].name; ++i)
	{
		ftype = g_bint_commands[i].ftype;
		ft_printf("%1$-16s --  %2$s %1$s(", g_bint_commands[i].name,
			g_ftypes_protos[ftype][0] == VR ? "void" : "int" );
		for (int j = 1; g_ftypes_protos[ftype][j] != NOPE; ++j)
			ft_printf("%s%s", j > 1 ? ", " : "",
				argtypes[g_ftypes_protos[ftype][j] - 3]);
		ft_printf(")\n");
	}
}

static int	get_var_bint(t_bint *res, const char *str, int init);

static void	bc_env(void)
{
	t_bint	var;
	int		first;
	char	name[2] = { 0 };

	first = 1;
	var = NULL;
	for (int c = 'a'; c <= 'z'; ++c)
	{
		name[0] = c;
		get_var_bint(&var, name, 0);
		if (var && *var)
		{
			ft_printf("%c%c:\n", first ? '\0' : '\n', c - 32);
			bint_print(var, 16, 1);
			first = 0;
		}
	}
}

static void	bc_exit(void)
{
	ft_exit(EXIT_SUCCESS, NULL);
}

static void	bc_log2(const t_bint n)
{
	uint32_t	log2;

	log2 = bintlog2(n);
	ft_printf("%u\n", log2);
}

static void	bc_is_odd(const t_bint n)
{
	int	is_odd;

	is_odd = bint_is_odd(n);
	ft_printf("%s\n", is_odd ? "true" : "false");
}

static void	bc_is_even(const t_bint n)
{
	int	is_even;

	is_even = bint_is_even(n);
	ft_printf("%s\n", is_even ? "true" : "false");
}

static int	bc_is_prime(const t_bint n, uint64_t k)
{
	int			ret;
	uint64_t	is_prime;

	k = !k ? 12 : k;
	if ((ret = bint_is_prime(n, k, &is_prime)) == BINT_SUCCESS)
		ft_printf("%s\n", !is_prime ? "true" : "false");
	return (ret);
}

int	v_b_u32(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	void	(*f)(t_bint, uint32_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	f(args[0], u32args[1]);
	return (BINT_SUCCESS);
}

int	v_b(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	void	(*f)(t_bint) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	f(args[0]);
	return (BINT_SUCCESS);
}

int	i_b_b(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		ret;
	int		(*f)(t_bint, t_bint) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	if (f == bintcmp || f == bintcmp_abs)
	{
		ret = f(args[0], args[1]);
		ft_printf("cmp: %d\n", ret);	
		return (BINT_SUCCESS);
	}
	return (f(args[0], args[1]));
}

int	i_b_u64(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, uint64_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], u64args[1]));
}

int	i_b_i64(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, int64_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], i64args[1]));
}

int	i_b_u32(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, uint32_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], u32args[1]));
}

int	i_b_b_b(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, t_bint, t_bint) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], args[1], args[2]));
}

int	i_b_b_u32(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, t_bint, uint32_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], args[1], u32args[2]));
}

int	i_b(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0]));
}

int	i_b_u32_u32(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, uint32_t, uint32_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], u32args[1], u32args[2]));
}

int	i_b_b_u64(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, t_bint, uint64_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], args[1], u64args[2]));
}

int	i_b_b_b_b(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, t_bint, t_bint, t_bint) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)u64args;
	(void)i64args;
	return (f(args[0], args[1], args[2], args[3]));
}

int	i_b_b_b_u64(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, t_bint, t_bint, uint64_t) = g_bint_commands[cmdi].f;

	(void)u32args;
	(void)i64args;
	return (f(args[0], args[1], args[2], u64args[3]));
}

int	i_b_u64_u64_u64(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	int		(*f)(t_bint, uint64_t, uint64_t, uint64_t) =
		g_bint_commands[cmdi].f;

	(void)u32args;
	(void)i64args;
	return (f(args[0], u64args[1], u64args[2], u64args[3]));
}

int	v(int cmdi, t_bint args[BINTF_MAX_ARGS],
	uint32_t u32args[BINTF_MAX_ARGS],
	uint64_t u64args[BINTF_MAX_ARGS],
	int64_t i64args[BINTF_MAX_ARGS])
{
	void	(*f)(void) = g_bint_commands[cmdi].f;

	(void)args;
	(void)u32args;
	(void)u64args;
	(void)i64args;
	f();
	return (BINT_SUCCESS);
}

static int	bint_to_u32(uint32_t *res, t_bint n)
{
	if (BINT_SIGN(n) || BINT_LEN(n) > 1)
		return (BINT_FAILURE);
	*res = BINT_LEN(n) ? n[1] : 0;
	return (BINT_SUCCESS);
}

static int	bint_to_i64(int64_t *res, t_bint n)
{
	uint64_t	abs;

	if (BINT_LEN(n) > 2)
		return (BINT_FAILURE);
	abs = BINT_LEN(n) ? (uint64_t)n[1] : 0;
	if (BINT_LEN(n) == 2)
		abs += (uint64_t)n[2] << 32;

	if (!BINT_SIGN(n) && abs > (uint64_t)INT64_MAX)
		return (BINT_FAILURE);
	else if (BINT_SIGN(n) && abs > (((uint64_t)INT64_MAX) + 1))
		return (BINT_FAILURE);
	*res = abs;
	if (BINT_SIGN(n))
		*res *= -1;
	return (BINT_SUCCESS);
}

static int	exec_cmd(t_bint args[BINTF_MAX_ARGS], int cmdi)
{
	uint32_t	u32args[BINTF_MAX_ARGS] = { 0 };
	uint64_t	u64args[BINTF_MAX_ARGS] = { 0 };
	int64_t		i64args[BINTF_MAX_ARGS] = { 0 };
	int			ftype;
	int			ret;

	ret = BINT_SUCCESS;
	ftype = g_bint_commands[cmdi].ftype;
	for (int i = 1; i <= BINTF_MAX_ARGS; ++i)
	{
		switch (g_ftypes_protos[ftype][i])
		{
			case U32A: ret = bint_to_u32(u32args + i - 1, args[i-1]);
				break;
			case U64A: ret = bint_to_u64(u64args + i - 1, args[i-1]);
				break;
			case I64A: ret = bint_to_i64(i64args + i - 1, args[i-1]);
				break;
			default:
				break;
		}
	}
	switch (ftype)
	{
		case V_B_U32: ret = v_b_u32(cmdi, args, u32args, u64args, i64args);
			break;
		case V_B: ret = v_b(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_B: ret = i_b_b(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_U64: ret = i_b_u64(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_I64: ret = i_b_i64(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_U32: ret = i_b_u32(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_B_B: ret = i_b_b_b(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_B_U32: ret = i_b_b_u32(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_B_B_B: ret = i_b_b_b_b(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_B_B_U64: ret = i_b_b_b_u64(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B: ret = i_b(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_U32_U32: ret = i_b_u32_u32(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_B_U64: ret = i_b_b_u64(cmdi, args, u32args, u64args, i64args);
			break;
		case I_B_U64_U64_U64: ret = i_b_u64_u64_u64(cmdi, args, u32args, u64args, i64args);
			break;
		case V: ret = v(cmdi, args, u32args, u64args, i64args);
			break;
		default: ret = BINT_FAILURE;
			break;
	}
	return (ret);
}

static int	skip_whites(char **line)
{
	char	*l;
	int		skipped;

	l = *line;
	while (*l && ft_isspace(*l))
		++l;
	skipped = l - *line;
	*line = l;
	return (skipped);
}

static int		read_command_name(char **line)
{
	int	len;
	int	cmdi;

	if (!ft_strncmp(*line, "bint", 4))
	{
		*line += 4;
		*line = **line == '_' ? *line + 1 : *line;
	}
	for (cmdi = 0; g_bint_commands[cmdi].name; ++cmdi)
	{
		len = g_bint_commands[cmdi].name_len;
		if (!ft_strncmp(*line, g_bint_commands[cmdi].name, len)
			&& ((*line)[len] == '(' || !(*line)[len]
			|| ft_isspace((*line)[len])))
		{
			*line += len;
			skip_whites(line);
			break ;
		}
	}
	return (cmdi);
}

int				decimal_to_bint(t_bint res, const char *str)
{
	uint32_t	n;
	const char	*p;
	int			sign;

	if ((sign = *str == '-'))
		++str;
	for (p = str; *p && ft_isdigit(*p); ++p);
	if (*p || p == str)
		return (BINT_FAILURE);
	bintclr(res);
	for (p = str; *p && bint_smult10(res) == BINT_SUCCESS; ++p)
	{
		n = (uint32_t)(*p - '0');
		if (bint_add_u64(res, res, n) == BINT_FAILURE)
			return (BINT_FAILURE);
	}
	SET_BINT_SIGN(res, sign);
	return (*p ? BINT_FAILURE : BINT_SUCCESS);
}

#define HEXL 8 //max length in hexadecimal of an unsigned int of 32bits

static int		hextouint32(t_bint res, char hex[HEXL], int i)
{
	uint32_t	n;
	char		*d;
	const char	digits[] = "0123456789abcdef";

	for (int j = 0; j < HEXL; ++j)
		hex[j] = ft_tolower(hex[j]);
	n = 0;
	for (int k = 0; k < HEXL; ++k)
	{
		n <<= 4;
		if (!(d = ft_strchr(digits, hex[k])))
			return (BINT_FAILURE);	
		n |= (uint32_t)(d - digits);
	}
	res[i] = n;
	if (!n && (uint32_t)i == BINT_LEN(res))
		SET_BINT_LEN(res, (uint32_t)i - 1);
	if (!BINT_LEN(res))
		SET_BINT_SIGN(res, 0);
	return (BINT_SUCCESS);
}

int				hex_to_bint(t_bint res, const char *str)
{
	const char	*p;
	uint32_t	len;
	uint32_t	blen;
	char		hex[HEXL];

	bintclr(res);
	SET_BINT_SIGN(res, *str == '-');
	str = *str == '-' ? str + 1 : str;
	if (str[0] != '0' || !(str[1] == 'x' || str[1] == 'X'))
		return (BINT_FAILURE);
	str += 2;
	for (p = str; *p && (ft_isdigit(*p) || ft_strchr("abcdefABCDEF", *p)); ++p);
	if (*p || p == str)
		return (BINT_FAILURE);
	len = ft_strlen(str);
	if ((blen = len / HEXL + !!(len % HEXL)) >= BINT_SIZE(res))
		return (BINT_FAILURE);
	SET_BINT_LEN(res, blen);
	len = len % HEXL ? len % HEXL : HEXL;
	ft_memset((void *)hex, '0', HEXL - len);
	ft_strncpy(hex + HEXL - len, str, len);
	for (str += len; hextouint32(res, hex, blen--) == BINT_SUCCESS && blen > 0;
		str += HEXL)
		ft_strncpy(hex, str, HEXL);
	return (BINT_SUCCESS);
}

/*
** The exponent is limited to a size 2 because the maximum size of bint is
** (2^15)-1 = 32767, which means the absolute value can at most 1048544 bits
** long, so the exponent can't be superior to 1048543. A single uint32_t
** number is more than enough to hold this value hence the size of exp.
*/
int				pow2_to_bint(t_bint res, const char *str)
{
	int			sign;
	uint32_t	exp[2] = BINT_DEFAULT(2);

	sign = *str == '-';
	str = *str == '-' ? str + 1 : str;
	if (ft_strncmp(str, "2^", 2) || !str[2])
		return (BINT_FAILURE);
	str += 2;
	if (decimal_to_bint(exp, str) == BINT_FAILURE || BINT_SIGN(exp))
		return (BINT_FAILURE);
	return (bintset_pow2(res, exp[1], sign));
}

/*
** The same holds for the exponent of the pow10, since the resulting value
** would be even bigger than with a power of 2 for the same exponent.
*/
int				pow10_to_bint(t_bint res, const char *str)
{
	int			sign;
	uint32_t	exp[2] = BINT_DEFAULT(2);

	sign = *str == '-';
	str = *str == '-' ? str + 1 : str;
	if (ft_strncmp(str, "10^", 3) || !str[3])
		return (BINT_FAILURE);
	str += 3;
	if (decimal_to_bint(exp, str) == BINT_FAILURE || BINT_SIGN(exp))
		return (BINT_FAILURE);
	return (bintset_pow10(res, exp[1], sign));
}

typedef struct		s_bintconst
{
	const char		*name;
	const t_bint	b;
}					t_bintconst;

#define BINT_CONSTS_LEN	4

t_bintconst	bintconsts[BINT_CONSTS_LEN + 1] = {
	{ "BINT_ZERO", BINT_ZERO },
	{ "BINT_ONE", BINT_ONE },
	{ "BINT_MAX", BINT_MAX },
	{ "BINT_MIN", BINT_MIN },
	{ NULL, NULL },
};

static int		get_const_bint(t_bint *res, const char *str)
{
	static uint32_t	constb[BINT_SIZE_DEF] = BINT_DEFAULT(0);

	for (t_bintconst *p = bintconsts; p->name; ++p)
	{
		if (!ft_strcmp(str, p->name))
		{
			if (bintcpy(constb, p->b) == BINT_FAILURE)
				return (BINT_FAILURE);
			*res = constb;
			return (BINT_SUCCESS);
		}
	}
	return (BINT_FAILURE);
}

#define BINT_VARS_LEN	26

/*
** Right now variable names are only one letter (so 26 variables)
** And there are 3 constants.
*/
static int		get_var_bint(t_bint *res, const char *str, int init)
{
	int				c;
	static uint32_t	vars[BINT_SIZE_DEF * BINT_VARS_LEN] = { 0 };

	if (ft_strlen(str) > 1)
		return (get_const_bint(res, str));
	else if (!ft_isalpha(*str))
		return (BINT_FAILURE);
	c = ft_tolower(*str) - 'a';
	*res = vars + (BINT_SIZE_DEF * c);
	if (init && !**res)
		**res = BINT_SIZE_DEF << 16;
	return (BINT_SUCCESS);
}

static char		*get_arg(char **line, int expect_com)
{
	char	*p;
	char	*arg;

	p = *line;
	arg = NULL;
	while (*p && !ft_isspace(*p) && !(expect_com && *p == ',')
		&& !(expect_com && *p == ')'))
		++p;
	if (p > *line)
		arg = ft_strndup(*line, (size_t)(p - (*line)));
	*line = p;
	return (arg);
}

static int		read_args(t_bint *args, char **line, int expect_com)
{
	int		ret;
	char	*arg;

	for (int i = 0; i < BINTF_MAX_ARGS && **line && **line != ')'; ++i)
	{
		if (i > 0 && expect_com && **line != ',')
			return (-1);
		else if (i > 0 && expect_com)
		{
			*line = *line + 1;
			skip_whites(line);
		}
		if (!(arg = get_arg(line, expect_com)))
			return (-1);
		ret = decimal_to_bint(args[i], arg) == BINT_FAILURE 
			&& hex_to_bint(args[i], arg) == BINT_FAILURE
			&& pow2_to_bint(args[i], arg) == BINT_FAILURE
			&& pow10_to_bint(args[i], arg) == BINT_FAILURE
			&& get_var_bint(args + i, arg, 1) == BINT_FAILURE;
		ft_memdel((void **)&arg);
		if (ret)
			return (-1);
		skip_whites(line);
	}
	return (0);
}

static int		parse_input(t_bint *args, char *line)
{
	int	cmdi;
	int	expect_par;

	skip_whites(&line);
	cmdi = read_command_name(&line);
	if (!g_bint_commands[cmdi].name)
		return (cmdi);
	if ((expect_par = *line == '('))
		++line;
	skip_whites(&line);
	if (read_args(args, &line, expect_par) || (expect_par && *line != ')')
		|| (!expect_par && *line == ')'))
		return (-1);
	else if (expect_par)
		++line;
	skip_whites(&line);
	return (*line ? -1 : cmdi);
}

static int		read_input(t_bint *args)
{
	int		ret;
	char	*line;

	line = NULL;
	if ((ret = get_next_line(0, &line)) < 0)
		ft_exit(EXIT_FAILURE, "%s: get_next_line error", __func__);
	else if (!ret)
		ft_exit(EXIT_SUCCESS, NULL);
	ret = parse_input(args, line);
	ft_memdel((void **)&line);
	return (ret);
}

static void	bintbc(const char *exec)
{
	int			ret;
	int			cmdi;
	t_bint		args[BINTF_MAX_ARGS];
	uint32_t	static_args[BINTF_MAX_ARGS][BINT_SIZE_DEF] = {
		[ 0 ... BINTF_MAX_ARGS - 1] = BINT_DEFAULT(0)
	};

	ret = -1;
	while (1)
	{
		for (int i = 0; i < BINTF_MAX_ARGS; ++i)
		{
			args[i] = (t_bint)&static_args[i];
			bintclr(args[i]);
		}
		ft_printf("[%c]%s> ", ret == -1 ? '\0'
			: ret == BINT_SUCCESS ? 'S' : 'F', exec);
		cmdi = read_input(args);
		if (cmdi < 0)
			ft_dprintf(2, "%s: invalid command\n", exec);
		else if (g_bint_commands[cmdi].name == NULL)
			ft_dprintf(2, "%s: unknown command\n", exec);
		else
			ret = exec_cmd(args, cmdi);
	}
}

static void	bc_cleanup(void)
{
	ft_rand_source(NULL, 0, NULL, 1);
}

int		main(int argc, char **argv)
{
	show = argc > 1;
	setlocale(LC_ALL, "C");
	ft_rand_source(NULL, 0, "/dev/urandom", 0);
	ft_atexit(bc_cleanup);
	if (show && !ft_strcmp(argv[1], "bc"))
		bintbc(argv[0]);
	else
		test_mandatory(argc, argv);
	ft_exit(EXIT_SUCCESS, NULL);
	return (0);
}
