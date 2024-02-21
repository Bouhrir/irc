NAME = IRC

FLAGS = -std=c++98  #-Werror -Wall -Wextra 

SRC = main.cpp utils.cpp server.cpp client.cpp 

INCLUDES = head.hpp error.hpp server.hpp client.hpp

OBJ = $(SRC:.cpp=.o)

%.o:%.cpp
	c++ $(FLAGS) -c $< -o $@

$(NAME): $(OBJ) $(INCLUDES)
	c++ $(OBJ) -o $@

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re