NAME	=	uls

CFLAGS	=	-std=c11 -Wall -Wextra -Werror -Wpedantic

SRCD	=	src
INCD	=	inc
OBJD	=	obj

LMXDIR	=	libmx
LMXA	:=	$(LMXDIR)/libmx.a
LIBMXINC	:=	$(LMXDIR)/inc

INC		=	uls.h
INCS	=	$(addprefix $(INCD)/, $(INC))

SRC		= 	main.c uls.c mx_printerr.c print_names.c

SRCS	=	$(addprefix $(SRCD)/, $(SRC))
OBJS	=	$(addprefix $(OBJD)/, $(SRC:%.c=%.o))

all: install

install: $(LMXA) $(NAME)

$(NAME): $(OBJS)
	clang $(CFLAGS) $(OBJS) $(LMXA) -o $@

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	clang $(CFLAGS) -c $< -o $@ -I$(INCD) -I$(LIBMXINC)

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

$(LMXA):
	@make -sC $(LMXDIR)

clean:
	@make -sC $(LMXDIR) $@
	@rm -rf $(OBJD)

uninstall: clean
	@make -sC $(LMXDIR) $@
	@rm -rf $(NAME)

reinstall: uninstall install
