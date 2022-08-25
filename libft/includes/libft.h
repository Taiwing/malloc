/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 01:54:59 by yforeau           #+#    #+#             */
/*   Updated: 2022/03/23 05:39:01 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIBFT_H
# define FT_LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <sys/time.h>
# include "ft_errno.h"
# include "ft_getopt.h"
# include "ft_printf.h"
# include "bint.h"
# include "network.h"
# ifndef NO_COLLEC
#  include "ft_collector.h"
# else
#  include <dirent.h>
#  include "ft_exit.h"
void					*ft_secmalloc(size_t size);

# endif
# ifdef	THREAD_SAFE
#  include "thread_safe.h"
# endif

typedef unsigned char	t_uchar;
typedef __int128_t		int128_t;
typedef __uint128_t		uint128_t;

typedef struct			s_list
{
	void				*content;
	size_t				content_size;
	struct s_list		*next;
}						t_list;

int						get_next_line(const int fd, char **line);

void					*ft_memset(void *s, int c, size_t n);
void					ft_bzero(void *s, size_t n);
void					*ft_memcpy(void *dest, const void *src, size_t n);
void					*ft_memccpy(void *dest, const void *src,
									int c, size_t n);
void					*ft_memmove(void *dest, const void *src, size_t n);
void					*ft_memchr(const void *s, int c, size_t n);
void					*ft_memswap(void *dest, size_t n);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
void					*ft_memdup(const void *src, size_t size);
size_t					ft_strlen(const char *s);
char					*ft_strdup(const char *s);
char					*ft_strcpy(char *dest, const char *src);
char					*ft_strncpy(char *dest, const char *src, size_t n);
char					*ft_strcat(char *dest, const char *src);
char					*ft_strncat(char *dest, const char *src, size_t n);
size_t					ft_strlcat(char *dest, const char *src, size_t size);
char					*ft_strchr(const char *s, int c);
char					*ft_strrchr(const char *s, int c);
char					*ft_strstr(const char *haystack, const char *needle);
char					*ft_strnstr(const char *s1, const char *s2, size_t n);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_ignore_case_strcmp(const char *s1, const char *s2);
int						ft_ignore_case_strncmp(const char *s1, const char *s2,
							size_t n);
int						ft_atoi(const char *nptr);
int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
int						ft_isascii(int c);
int						ft_isprint(int c);
int						ft_isspace(int c);
int						ft_toupper(int c);
int						ft_tolower(int c);

void					*ft_memalloc(size_t size);
void					ft_memdel(void **ap);
char					*ft_strnew(size_t size);
void					ft_strdel(char **as);
void					ft_strclr(char *s);
void					ft_striter(char *s, void (*f)(char *));
void					ft_striteri(char *s, void (*f)(unsigned int, char *));
char					*ft_strmap(char const *s, char (*f)(char));
char					*ft_strmapi(char const *s,
									char (*f)(unsigned int, char));
int						ft_strequ(char const *s1, char const *s2);
int						ft_strnequ(char const *s1, char const *s2, size_t size);
char					*ft_strsub(char const *s,
									unsigned int start, size_t len);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s);
char					**ft_strsplit(char const *s, char c);
char					*ft_itoa(int n);
int						ft_putchar(char c);
int						ft_putstr(char const *s);
int						ft_putendl(char const *s);
int						ft_putnbr(int n);
int						ft_putchar_fd(char c, int fd);
int						ft_putstr_fd(char const *s, int fd);
int						ft_putendl_fd(char const *s, int fd);
int						ft_putnbr_fd(int n, int fd);

t_list					*ft_lstnew(void const *content, size_t content_size);
void					ft_lstdelone(t_list **alst,
									void (*del)(void *, size_t));
void					ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void					ft_lstadd(t_list **alst, t_list *new);
void					ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list					*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

t_list					*ft_lst_at(t_list *lst, unsigned int nbr);
void					ft_lst_discard(t_list *prev, t_list **alst);
t_list					*ft_lst_find(t_list *lst,
									void *content_ref, int (*cmp)());
void					ft_lst_foreach(t_list *lst, void (*f)(void *));
void					ft_lst_foreach_if(t_list *lst, void (*f)(void *),
										void *content_ref, int (*cmp)());
t_list					*ft_lst_last(t_list *lst);
void					ft_lst_merge(t_list **lst1, t_list *lst2);
void					ft_lst_move_back(t_list **lst, t_list *elem);
void					ft_lst_move_front(t_list **lst, t_list *elem);
t_list					*ft_lst_push_back(t_list **lst, void *content,
										size_t content_size);
t_list					*ft_lst_push_front(t_list **lst, void *content,
										size_t content_size);
t_list					*ft_lst_push_params(int ac, char **av);
void					ft_lst_remove_if(t_list **lst, void *content_ref,
										int (*cmp)());
void					ft_lst_reverse(t_list **lst);
int						ft_lst_size(t_list *lst);
void					ft_lst_sort(t_list **lst, int (*cmp)());
void					ft_lst_quicksort(t_list *lst, int len,
										int (*cmp)(void *, void *));
void					ft_lst_move_to_back(t_list *prev, t_list **alst1,
											t_list **alst2);
void					ft_lst_move_to_front(t_list *prev, t_list **alst1,
											t_list **alst2);
void					ft_lst_sorted_insert(t_list **lst, t_list *new,
											int (*cmp)());
t_list					*ft_lst_pop(t_list **lst, unsigned int pop);
int						ft_lst_find_index(t_list *lst,
									void *content_ref, int (*cmp)());
t_list					*ft_lst_pop_if(t_list **lst,
									void *content_ref, int (*cmp)());

char					*ft_stradd(char **base, char *str, size_t l);
char					*ft_strcut(char **s, size_t start, size_t l);
char					*ft_strrm(char **str, size_t start, size_t n);
char					*ft_strndup(const char *s, size_t n);
char					**ft_split_whitespaces(char *str);
void					ft_print_words_tables(char **tab);
void					ft_print_words_tables_fd(char **tab, int fd);
int						ft_atoi_forward(char **nptr);
void					ft_swap_p(uintptr_t *p1, uintptr_t *p2);
int						ft_intlen(intmax_t n);
int						ft_secatoi(int *dest, int min, int max,
									const char *nptr);
unsigned int			ft_rand(void);
uint64_t				*ft_rand_uint64(uint64_t *n,
									uint64_t min, uint64_t max);
int						ft_wtlen(char **wt);
char					**ft_wtdup(char **wt);
void					ft_wtfree(char **wt);
void					*ft_rand_source(void *dst, size_t size,
									const char *src, int flush);

char					*ft_exec_name(const char *path);

double					ft_sqrt(double y);

int						ft_timeval_is_zero(struct timeval *time);
int						ft_timeval_add(struct timeval *dest,
	const struct timeval *left, const struct timeval *right);
int						ft_timeval_sub(struct timeval *dest,
	const struct timeval *left, const struct timeval *right);
int						ft_timeval_abs(struct timeval *dest,
	const struct timeval *src);
int						ft_timeval_div(struct timeval *dest,
	const struct timeval *src, int div);
int						ft_timeval_mul(struct timeval *dest,
	const struct timeval *src, int mul);
int						ft_timeval_cmp(struct timeval *a, struct timeval *b);
int						ft_timeval_is_expired(struct timeval *date,
	struct timeval *expiry);


t_list					*ft_net_listiface(void);
void					ft_net_listiface_free(t_list **iflist);
int						ft_netconf_init(t_list *user_iflist);

# define FT_LS_UNKNOWN	0x00
# define FT_LS_FIFO		0x01
# define FT_LS_CHR		0x02
# define FT_LS_DIR		0x04
# define FT_LS_BLK		0x08
# define FT_LS_REG		0x10
# define FT_LS_LNK		0x20
# define FT_LS_SOCK		0x40
# define FT_LS_WHT		0x80
# define FT_LS_ALL		0xFF

t_list					*ft_ls_files(char *path, int type);

#endif
