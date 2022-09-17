NAME = philo
FILES = philo_check_fin.c philo_init.c philo_start_dining.c philo_utils.c philosophers.c
SRC= $(FILES)
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Werror -Wextra -pthread -g
CC = gcc

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
