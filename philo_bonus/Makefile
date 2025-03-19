CC 			= 	cc
NAME		=	philo_bonus
FLAGS		= 	-Wall -Wextra -Werror -g -fsanitize=thread
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
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) -lpthread
	@echo "$(GREEN)$(NAME) compiled$(DEF_COLOR)"

$(OBJDIR)/%.o : %.c $(HEADER)
	@$(MD) $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJDIR)
	@echo "$(GRAY)$(NAME) object files removed$(DEF_COLOR)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(GRAY)$(NAME) removed$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re

# Colors
DEF_COLOR       :=      \033[0;39m
GRAY            :=      \033[0;90m
GREEN           :=      \033[0;92m
