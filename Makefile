NAME	= philo
RM  	= rm -rf
CFLAGS	= -Wextra -Werror -Wall -pthread -fsanitize=thread -ltsan
SRC		= main.c pars_utils.c inits.c free_print_utils.c simulation.c exit_utils.c
OBJ		= $(SRC:.c=.o)
CC		= gcc

all:	$(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	$(RM) $(OBJ) a.out
fclean:	clean
	$(RM) $(NAME)
re:	fclean all

.PHONY: make all clean fclean