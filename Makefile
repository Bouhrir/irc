NAME = icserv
SRC = main.cpp parse/Parse.cpp 
OBJ = ${SRC:.cpp=.o}
CC = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g

.SILENT:
all : ${NAME}

${NAME} : ${OBJ}
	${CC} ${CXXFLAGS} ${OBJ} -o ${NAME}
	@echo "\033[1;7;32mSuccessfully compiled $(NAME)!\033[0m"

r: ${OBJ}
	${CC} ${CXXFLAGS} ${OBJ} -o ${NAME} 2> /dev/null
	@./PmergeMe 8 4 16 2 18 3 11 13 9 21 17 5 19 14 20 6 15 1 7 12 10

clean:
	rm -rf $(OBJ)
	@echo "\033[1;9;7;31mCleaning up...!\033[0m"

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : clean
