NAME = philo
FILES = philo_check_fin.c philo_init.c philo_start_dining.c philo_utils.c philosophers.c
SRC= $(FILES)
OBJ = $(SRC=.c=.o)
FLAGS = -Wall -Werror -Wextra
CC = gcc

$(NAME):
	$(CC) $(FLAGS) -o $(NAME) $(SRC) $(LIB) $(LISTLIB)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
