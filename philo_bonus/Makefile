CC 			= 	cc
NAME		=	philo_bonus
FLAGS		= 	-Wall -Wextra -Werror
HEADER		=	philo_bonus.h
RM			=	rm -rf
MD			=	mkdir -p

OBJDIR		= 	obj_bonus
SRCS		= 	helper2_bonus.c \
				helper_bonus.c \
				parser_bonus.c \
				philo_bonus.c \
				simulation2_bonus.c \
				simulation_bonus.c \
				clean_bonus.c

OBJS		=	$(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)
bonus: all

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@printf "$(NAME) compiled\n"

$(OBJDIR)/%.o : %.c $(HEADER)
	@$(MD) $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
