CC 			= 	cc
NAME		=	philo
FLAGS		=	-Wall -Wextra -Werror
HEADER		=	philo.h
RM			=	rm -rf
MD			=	mkdir -p

OBJDIR		= 	obj

SRCS		=	simulation.c \
				parsing.c \
				monitor.c \
				routine.c \
				helper2.c \
				helper.c \
			 	philo.c \

OBJS		=	$(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled$(DEF_COLOR)"

$(OBJDIR)/%.o : %.c $(HEADER)
	@$(MD) $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
