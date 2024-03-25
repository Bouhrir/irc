NAME = ircserv

SRC = server.cpp utils.cpp client.cpp channel.cpp bot.cpp
CXX = c++

CXXFLAGS = -std=c++98 -fsanitize=address -g

INCLUDES = head.hpp error.hpp server.hpp client.hpp bot.hpp

OBJ = ${SRC:.cpp=.o}

.SILENT:
all : ${NAME}

%.o:%.cpp ${INCLUDES}
	${CXX} ${CXXFLAGS} -c $< -o $@

${NAME} : ${OBJ} main.cpp
	${CXX} ${CXXFLAGS} ${OBJ} main.cpp -o ${NAME}
	@echo "\033[1;7;32mSuccessfully compiled $(NAME)!\033[0m"

# r: ${OBJ}
# 	${CC} ${CXXFLAGS} ${OBJ} -o ${NAME} 2> /dev/null
# 	./icserv 5550 123

clean:
	rm -rf $(OBJ)
	@echo "\033[1;9;7;31mCleaning up...!\033[0m"

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : clean
