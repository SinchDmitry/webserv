NAME			= 	webserv
VPATH			=	src/
SRCS			=	main.cpp \
					Server.cpp \
					ListenSocket.cpp \
					LocationInfo.cpp \
					ConfigurationSingleton.cpp \


OBJS			= $(SRCS:.cpp=.o)
DEP				= $(SRCS:.cpp=.d)

CXX				= c++

RM				= rm -f
CXXFLAGS		= -MD -std=c++98 -I. -g
# -Wall -Wextra -Werror  


all:			$(NAME)

$(NAME):		$(OBJS)
				$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS) $(DEP)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

-include		$(DEP)

.PHONY:			all clean fclean re