NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -g

FILES = main.c parsing.c error.c thread.c thread_utils.c \
		dongle.c dongle_utils.c heap.c utils.c scheduler.c

OBJ = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean bonus re