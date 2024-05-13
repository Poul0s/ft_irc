CC	=	c++

CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -MD -g

SRC		=	$(wildcard srcs/class/*.cpp)				\
			$(wildcard srcs/*.cpp)						\

OBJ		=	$(addprefix obj/, $(SRC:.cpp=.o))

OBJ_DIR	=	$(sort $(dir $(OBJ)))

HEADER	=	-Iclass

NAME	=	ft_irc

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: %.cpp
	$(CC) $(CFLAGS) $(HEADER) $^ -o $@ -c

$(OBJ_DIR):
	@mkdir -p $@

clean:
	rm -rf obj/

fclean: clean
	rm -rf $(NAME)

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re

