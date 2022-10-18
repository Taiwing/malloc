############################## ENVIRONMENT #####################################

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

############################## COMPILE VAR #####################################

CC			=	gcc
HDIR		=	includes
SRCDIR		=	src
SUB1D		=	libft
HFLAGS		=	-I $(HDIR) -I $(SUB1D)/$(HDIR)
LIBNAME		=	libft_malloc_$(HOSTTYPE).so
NAME		=	libft_malloc.so

ifdef THREAD_SAFE
#	CFLAGS		=	-fpic -Wall -Wextra -Werror -DTHREAD_SAFE
	CFLAGS		=	-fpic -Wall -Wextra -Werror -DTHREAD_SAFE -g
	LIBS		=	$(SUB1D)/libft.a -lpthread
else
#	CFLAGS		=	-fpic -Wall -Wextra -Werror
	CFLAGS		=	-fpic -Wall -Wextra -Werror -g
	LIBS		=	$(SUB1D)/libft.a
endif

############################## SOURCES #########################################

SRCC			=	malloc.c\
					block.c\
					debug.c\
					zone.c\

ODIR			=	obj
OBJ				=	$(patsubst %.c,%.o,$(SRCC))

vpath			%.o	$(ODIR)
vpath			%.h	$(HDIR)
vpath			%.h	$(SUB1D)/$(HDIR)
vpath			%.c	$(SRCDIR)

############################## BUILD ###########################################

all: $(NAME)

$(NAME): $(SUB1D)/libft.a $(ODIR) $(OBJ)
	gcc -shared -fpic -o $(LIBNAME) $(patsubst %.o,$(ODIR)/%.o,$(OBJ)) $(LIBS)
	ln -sf $(LIBNAME) $@

$(SUB1D)/libft.a:
	make -C $(SUB1D)

malloc.o: malloc.h libft.h
block.o: malloc.h libft.h
debug.o: malloc.h libft.h
zone.o: malloc.h libft.h
%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) -c $(CFLAGS) $< $(HFLAGS) -o $(ODIR)/$@

$(ODIR):
	mkdir -p $@

############################## CLEANUP #########################################

clean:
	rm -rf $(ODIR)
	make -C $(SUB1D) fclean

fclean: clean
	rm -f $(LIBNAME)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
