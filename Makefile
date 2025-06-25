NAME = PIPEX
CC = cc
SRCS = pipex.c
OBJS = $(SRCS:.c=.o)
RM = rm -f
LIBFT = libft/libft.a
all: $(NAME)
$(NAME): $(OBJS) $(LIBFT)
	$(CC) -o $(NAME) $(OBJS) $(LIBFT)
$(LIBFT):
	make -C libft
clean:
	$(RM) $(OBJS)
	make -C libft clean
fclean: clean
	$(RM) $(NAME)
	make -C libft fclean
re: fclean all
.PHONY: all clean fclean re

