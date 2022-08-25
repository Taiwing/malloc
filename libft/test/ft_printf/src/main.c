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

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_RESET   "\x1b[0m"

#define	CONVERSIONS	"dDibBoOuUxXeEfFgGcCsSpPt%"
#define	FLAGS		"#0-+ "
#define	CAST		"hljzL"
#define	SPACES		"                                                         "

#define FT_PRINTF		0x0001
#define FT_DPRINTF		0x0002
#define FT_SPRINTF		0x0004
#define FT_SNPRINTF		0x0008
#define FT_ASPRINTF		0x0010
#define FT_ZPRINTF		0x0020
#define FT_DZPRINTF		0x0040
#define FT_SZPRINTF		0x0080
#define FT_SNZPRINTF	0x0100
#define FT_ALL			0x01ff

#define IS_BUFFERED(version) (version & (FT_SPRINTF|FT_SNPRINTF|FT_ASPRINTF|FT_SZPRINTF|FT_SNZPRINTF))

#define	BUF_DEFSIZE	16384

int		ret_orig;
int		ret_mine[FT_SNZPRINTF + 1];
char	*abuf_mine;
char	buf_orig[BUF_DEFSIZE];
char	buf_mine[BUF_DEFSIZE];
int		margin;
int		show;

char	*fstrings[FT_SNZPRINTF + 1] = {
	[FT_PRINTF] = "ft_printf",
	[FT_DPRINTF] = "ft_dprintf",
	[FT_SPRINTF] = "ft_sprintf",
	[FT_SNPRINTF] = "ft_snprintf",
	[FT_ASPRINTF] = "ft_asprintf",
	[FT_ZPRINTF] = "ft_zprintf",
	[FT_DZPRINTF] = "ft_dzprintf",
	[FT_SZPRINTF] = "ft_szprintf",
	[FT_SNZPRINTF] = "ft_snzprintf",
};

#define	PRINTF_TEST(other_printfs, format, arg) {\
	ret_orig = sprintf(buf_orig, format, arg);\
	other_printfs = !other_printfs ? FT_SPRINTF : other_printfs;\
	for (uint16_t index = 1; index < other_printfs + 1; index *= 2)\
	{\
		if (!(other_printfs & index))\
			continue ;\
		margin = ft_printf("TEST: [%s] [%s]", fstrings[index], format);\
		if (!IS_BUFFERED(index))\
			margin = ft_printf("\noutput_mine:") - 1;\
		ft_printf("%.*s", 50 - margin, SPACES);\
		abuf_mine = NULL;\
		switch (index)\
		{\
			case FT_PRINTF:\
			ret_mine[index] = ft_printf(format, arg); break;\
			case FT_DPRINTF:\
			ret_mine[index] = ft_dprintf(1, format, arg); break;\
			case FT_SPRINTF:\
			ret_mine[index] = ft_sprintf(buf_mine, format, arg); break;\
			case FT_SNPRINTF:\
			ret_mine[index] = ft_snprintf(buf_mine, BUF_DEFSIZE, format, arg); break;\
			case FT_ASPRINTF:\
			ret_mine[index] = ft_asprintf(&abuf_mine, format, arg); break;\
			case FT_ZPRINTF:\
			ret_mine[index] = ft_zprintf(format, arg); break;\
			case FT_DZPRINTF:\
			ret_mine[index] = ft_dzprintf(1, format, arg); break;\
			case FT_SZPRINTF:\
			ret_mine[index] = ft_szprintf(buf_mine, format, arg); break;\
			case FT_SNZPRINTF:\
			ret_mine[index] = ft_snzprintf(buf_mine, BUF_DEFSIZE, format, arg); break;\
		}\
		if (!IS_BUFFERED(index))\
			ft_printf("\n%.*s", 50, SPACES);\
		if (abuf_mine && ret_mine[index] >= 0)\
			ft_memcpy(buf_mine, abuf_mine,\
				ret_mine[index] < BUF_DEFSIZE ? ret_mine[index] : BUF_DEFSIZE);\
		if (ret_orig == ret_mine[index]\
				&& (ret_orig == -1 || !IS_BUFFERED(index)\
					|| !strncmp(buf_orig, buf_mine, ret_orig)))\
		{\
			ft_printf(C_GREEN"OK\n"C_RESET);\
			if (show && ret_orig != -1 && IS_BUFFERED(index))\
				ft_printf("%.*s\n", ret_orig, buf_orig);\
			else if (show)\
				ft_printf("ERROR\n");\
		}\
		else\
		{\
			ft_printf(C_RED "FAILED\n" C_RESET);\
			ft_printf("ret_orig = %d\n", ret_orig);\
			ft_printf("ret_mine = %d\n", ret_mine[index]);\
			ft_printf("ORIG>\t");\
			if (ret_orig != -1)\
			{\
				ft_printf("%.*s", ret_orig, buf_orig);\
				ft_printf("\n");\
			}\
			else\
				ft_printf("ERROR\n");\
			ft_printf("MINE>\t");\
			if (ret_mine[index] != -1 && IS_BUFFERED(index))\
			{\
				ft_printf("%.*s", ret_mine[index], buf_mine);\
				ft_printf("\n");\
			}\
			else\
				ft_printf("ERROR\n");\
		}\
		ft_memdel((void **)&abuf_mine);\
	}\
}

void	test_mandatory(int printfs)
{
	PRINTF_TEST(printfs, "%s", "poney");
	PRINTF_TEST(printfs, "%s" "4", "poney");
	PRINTF_TEST(printfs, "%s", "poney truite");
	PRINTF_TEST(printfs, "%4s", "poney truite");
	PRINTF_TEST(printfs, "%22s", "poney truite");
	PRINTF_TEST(printfs, "%.s", "poney truite");
	PRINTF_TEST(printfs, "%.4s", "poney truite");
	PRINTF_TEST(printfs, "%.20s", "poney truite");
	PRINTF_TEST(printfs, "%20.0s", NULL);
	PRINTF_TEST(printfs, "%-12s*", NULL);
	PRINTF_TEST(printfs, "%20.0s*", "toy story");
	PRINTF_TEST(printfs, "{%05.s}", 0);
	PRINTF_TEST(printfs, "{%.0s}", 0);
	PRINTF_TEST(printfs, "%s", NULL);
	PRINTF_TEST(printfs, "%025s", "steak hache");
	PRINTF_TEST(printfs, "% 4.8hi", (short)-2345);
	PRINTF_TEST(printfs, "%d", 42);
	PRINTF_TEST(printfs, "%d", INT_MIN);
	PRINTF_TEST(printfs, "%d", INT_MAX);
	PRINTF_TEST(printfs, "%.8d", 25);
	PRINTF_TEST(printfs, "%.08d", 25);
	PRINTF_TEST(printfs, "%8d", -2525);
	PRINTF_TEST(printfs, "%.12d", 50);
	PRINTF_TEST(printfs, "%.8d", 585);
	PRINTF_TEST(printfs, "%-.8d", 585);
	PRINTF_TEST(printfs, "%-8d", 585);
	PRINTF_TEST(printfs, "%zhd", 4294967296);
	PRINTF_TEST(printfs, "% 03d", 0);
	PRINTF_TEST(printfs, "%o", 0);
	PRINTF_TEST(printfs, "%+o", 0);
	PRINTF_TEST(printfs, "%o", -1);
	PRINTF_TEST(printfs, "%x", 42);
	PRINTF_TEST(printfs, "%#x", 2147);
	PRINTF_TEST(printfs, "%x", -42);
	PRINTF_TEST(printfs, "%-15x*", 542);
	PRINTF_TEST(printfs, "%#.x", 1);
	PRINTF_TEST(printfs, "%X", INT_MAX);
	PRINTF_TEST(printfs, "%c", 'G');
	PRINTF_TEST(printfs, "%5c", 'G');
	PRINTF_TEST(printfs, "%c", NULL);
	PRINTF_TEST(printfs, "%5c", 0);
	PRINTF_TEST(printfs, "%1c", 0);
	PRINTF_TEST(printfs, "%.2c", NULL);
	PRINTF_TEST(printfs, "%.1c", NULL);
	PRINTF_TEST(printfs, "%.3c", NULL);
	PRINTF_TEST(printfs, "{%3c}", 0);
	PRINTF_TEST(printfs, "{%03c}", 0);
	PRINTF_TEST(printfs, "%c", 0);
	PRINTF_TEST(printfs, "{%05.c}", 0);
	PRINTF_TEST(printfs, "%p", 0);
	PRINTF_TEST(printfs, "{%05.%}", 0);
	PRINTF_TEST(printfs, "{%10R}", 42);
	PRINTF_TEST(printfs, "{%-10R}", 42);
	PRINTF_TEST(printfs, "{%05.Z}", 0);
	PRINTF_TEST(printfs, "%o", 42);
	PRINTF_TEST(printfs, "%f", 42.00);
	PRINTF_TEST(printfs, "{%f}", 1.42);
	PRINTF_TEST(printfs, "%f", FLT_MIN);
	PRINTF_TEST(printfs, "%f", FLT_MAX);
	PRINTF_TEST(printfs, "%f", DBL_MIN);
	PRINTF_TEST(printfs, "%f", DBL_MAX);
	PRINTF_TEST(printfs, "%Lf", LDBL_MIN);
}

void	test_bonus(int ac, char **av, int printfs)
{
	ft_printf("implemented in original funcion:\n");
	PRINTF_TEST(printfs, "%D", 42);
	PRINTF_TEST(printfs, "%D", INT_MIN);
	PRINTF_TEST(printfs, "%D", INT_MAX);
	PRINTF_TEST(printfs, "%.8D", 25);
	PRINTF_TEST(printfs, "%.08D", 25);
	PRINTF_TEST(printfs, "%8D", -2525);
	PRINTF_TEST(printfs, "%.12D", 50);
	PRINTF_TEST(printfs, "%.8D", 585);
	PRINTF_TEST(printfs, "%-.8D", 585);
	PRINTF_TEST(printfs, "%-8D", 585);
	PRINTF_TEST(printfs, "%zhD", 4294967296);
	PRINTF_TEST(printfs, "% 03D", 0);
	PRINTF_TEST(printfs, "%U", 42);
	PRINTF_TEST(printfs, "%U", 4294967295);
	PRINTF_TEST(printfs, "%hhU", ULONG_MAX);
	PRINTF_TEST(printfs, "%U", 4294967296);
	PRINTF_TEST(printfs, "%zU", 4294967296);
	PRINTF_TEST(printfs, "%.2zU", 4294967296);
	PRINTF_TEST(printfs, "%.0U*", 0);
	PRINTF_TEST(printfs, "%0U*", 0);
	PRINTF_TEST(printfs, "%O", 42);
	PRINTF_TEST(printfs, "%O", 4294967295);
	PRINTF_TEST(printfs, "%hhO", ULONG_MAX);
	PRINTF_TEST(printfs, "%O", 4294967296);
	PRINTF_TEST(printfs, "%zO", 4294967296);
	PRINTF_TEST(printfs, "%.2zO", 4294967296);
	PRINTF_TEST(printfs, "%.0O*", 0);
	PRINTF_TEST(printfs, "%0O*", 0);
	PRINTF_TEST(printfs, "%S", L"chewing gum🤗 🤗 🤗 🤗\0");
	PRINTF_TEST(printfs, "%45S", L"chewing gum🤗 🤗 🤗 🤗\0");
	PRINTF_TEST(printfs, "%5S", L"🤗");
	PRINTF_TEST(printfs, "%5S", L"©");
	PRINTF_TEST(printfs, "%5S", L"ᚙ");
	PRINTF_TEST(printfs, "%S", L"123456🤗");
	PRINTF_TEST(printfs, "%S", L"🤗🤗🤗🤗");
	PRINTF_TEST(printfs, "%S", L"🤗🤗");
	PRINTF_TEST(printfs, "%2S", L"🤗🤗🤗🤗");
	PRINTF_TEST(printfs, "%S", L"米");
	PRINTF_TEST(printfs, "%15S", NULL);
	PRINTF_TEST(printfs, "%.3S", NULL);
	PRINTF_TEST(printfs, "%.10S", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.5S", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.11S", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.12S", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.12S", L"Valgrind ©🀙");
	PRINTF_TEST(printfs, "%030S", L"©©©");
	PRINTF_TEST(printfs, "{%05.S}", L"42 c est cool");
	PRINTF_TEST(printfs, "{%.22S}", L"🤗🤗🤗🤗🤗©©");
	PRINTF_TEST(printfs, "%ls", L"chewing gum🤗 🤗 🤗 🤗\0");
	PRINTF_TEST(printfs, "%45ls", L"chewing gum🤗 🤗 🤗 🤗\0");
	PRINTF_TEST(printfs, "%5ls", L"🤗");
	PRINTF_TEST(printfs, "%5ls", L"©");
	PRINTF_TEST(printfs, "%5ls", L"ᚙ");
	PRINTF_TEST(printfs, "%ls", L"123456🤗");
	PRINTF_TEST(printfs, "%ls", L"🤗🤗🤗🤗");
	PRINTF_TEST(printfs, "%ls", L"🤗🤗");
	PRINTF_TEST(printfs, "%2ls", L"🤗🤗🤗🤗");
	PRINTF_TEST(printfs, "%15ls", NULL);
	PRINTF_TEST(printfs, "%.3ls", NULL);
	PRINTF_TEST(printfs, "%.10ls", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.5ls", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.11ls", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.12ls", L"Valgrind ©©©©");
	PRINTF_TEST(printfs, "%.12ls", L"Valgrind ©🀙");
	PRINTF_TEST(printfs, "%030ls", L"©©©");
	PRINTF_TEST(printfs, "{%05.ls}", L"42 c est cool");
	PRINTF_TEST(printfs, "{%.22ls}", L"🤗🤗🤗🤗🤗©©");
	PRINTF_TEST(printfs, "%C", 'G');
	PRINTF_TEST(printfs, "%C", L'©');
	PRINTF_TEST(printfs, "%C", L'ߺ');
	PRINTF_TEST(printfs, "%C", L'ࠀ');
	PRINTF_TEST(printfs, "%C", L'🤗');
	PRINTF_TEST(printfs, "%C", L'©');
	PRINTF_TEST(printfs, "%C", L'©');
	PRINTF_TEST(printfs, "%C", L'¡');
	PRINTF_TEST(printfs, "%C", 173);
	PRINTF_TEST(printfs, "%C", 35);
	PRINTF_TEST(printfs, "%C", -5);
	PRINTF_TEST(printfs, "%C", 255);
	PRINTF_TEST(printfs, "%C", 256);
	PRINTF_TEST(printfs, "%C", 35);
	PRINTF_TEST(printfs, "%e", 1.000);
	PRINTF_TEST(printfs, "%e", 4242.42424242);
	PRINTF_TEST(printfs, "%e", 0.0004242);
	PRINTF_TEST(printfs, "%.3e", 0.0004242);
	PRINTF_TEST(printfs, "%10.2e", 0.0004242);
	PRINTF_TEST(printfs, "%#5.12e", 0.0004242);
	PRINTF_TEST(printfs, "%#5.12e", 0.0004242);
	PRINTF_TEST(printfs, "%e", FLT_MIN);
	PRINTF_TEST(printfs, "%e", FLT_MAX);
	PRINTF_TEST(printfs, "%e", DBL_MIN);
	PRINTF_TEST(printfs, "%e", DBL_MAX);
	PRINTF_TEST(printfs, "%Le", LDBL_MIN);
	PRINTF_TEST(printfs, "%Le", LDBL_MAX);
	PRINTF_TEST(printfs, "%g", 42.000);
	PRINTF_TEST(printfs, "%g", 42.420);
	PRINTF_TEST(printfs, "%g", FLT_MIN);
	PRINTF_TEST(printfs, "%g", FLT_MAX);
	PRINTF_TEST(printfs, "%g", DBL_MIN);
	PRINTF_TEST(printfs, "%g", DBL_MAX);
	PRINTF_TEST(printfs, "%Lg", LDBL_MIN);
	PRINTF_TEST(printfs, "%Lg", LDBL_MAX);

	ft_printf("\n\nnot implemented in original function:\n");
	ft_printf("\nt conversion:\n");
	ft_printf("\targv:\n");
	ft_printf("%#*t\t\t%s\n", ac, av);

	unsigned int		int_tab[10] = {
		0, 1, UINT_MAX, UINT_MAX, INT_MIN, INT_MIN, 6, 654676, 42, 9
	};
	ft_printf("\n\tunsigned int array (with 2 fields):\n");
	ft_printf("%#10.2t\t\tfirst field: %08x\tsecond field: %u\n", int_tab);

	ft_printf("b conversion:\n");
	ft_printf("\t\tunsigned int array:\n");
	ft_printf("\t\tdec: %#3t%-32u \n", int_tab + 5);
	ft_printf("\t\tbin: %#3t%.32b \n", int_tab + 5);

	int	fd = open("fdtest", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	ft_dprintf(fd, "test dprintf:\n");
	ft_dprintf(fd, "\n\tunsigned int array (with 2 fields):\n");
	ft_dprintf(fd, "%#10.2t\t\tfirst field: %08x\tsecond field: %u\n", int_tab);
	close(fd);
}

int		main(int argc, char **argv)
{
	int	other_printfs = 0;
	int	other_count = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (!ft_ignore_case_strcmp(argv[i], "ft_printf"))
			other_printfs |= FT_PRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_printf"))
			other_printfs |= FT_PRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_dprintf"))
			other_printfs |= FT_DPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_sprintf"))
			other_printfs |= FT_SPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_snprintf"))
			other_printfs |= FT_SNPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_asprintf"))
			other_printfs |= FT_ASPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_zprintf"))
			other_printfs |= FT_ZPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_dzprintf"))
			other_printfs |= FT_DZPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_szprintf"))
			other_printfs |= FT_SZPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_snzprintf"))
			other_printfs |= FT_SNZPRINTF;
		else if (!ft_ignore_case_strcmp(argv[i], "ft_all"))
			other_printfs |= FT_ALL;
		else
			++show;
	}
	setlocale(LC_ALL, "C");
	if (show && !strcmp(argv[1], "bonus"))
		test_bonus(argc, argv, other_printfs);
	else
		test_mandatory(other_printfs);
	return (0);
}
