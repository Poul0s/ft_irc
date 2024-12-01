CC	=	c++

CFLAGS	=	-Wall -Wextra -Werror -std=c++11 -MD -DGPT_TOKEN='"$(GPT_TOKEN)"' -g

SRC		=	$(wildcard srcs/class/*.cpp)				\
			$(wildcard srcs/commands/*.cpp)				\
			$(wildcard srcs/*.cpp)						\

OBJ		=	$(addprefix obj/, $(SRC:.cpp=.o))

OBJ_DIR	=	$(sort $(dir $(OBJ)))

HEADER	=	-Iclass -Iincludes

NAME	=	ircserv

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

