
CXX = c++
CXX_FLAGS = -Wall -Werror -Wextra -std=c++98
INC_DIR = ./inc
SRC_DIR = ./src
OBJ_DIR = ./obj
INCLUDES = $(INC_DIR)/Listener.hpp $(INC_DIR)/Connection.hpp \
			$(INC_DIR)/Server.hpp $(INC_DIR)/Cluster.hpp $(INC_DIR)/util.hpp \
			$(INC_DIR)/request/Request.hpp \
			$(INC_DIR)/response/Response.hpp \
			$(INC_DIR)/Logger.hpp $(INC_DIR)/Config.hpp
SRCS = main.cpp Connection.cpp Listener.cpp Server.cpp util.cpp \
	   request/Request.cpp response/Response.cpp Logger.cpp Config.cpp \
	   Cluster.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
NAME = webserv

all: $(NAME)

re: fclean
	$(MAKE) all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -I$(INC_DIR) -c $(filter %.cpp, $<) -o $@
$(NAME): $(OBJS) $(INCLUDES)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJS) 
clean:
	rm -rf $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

.PHONY: all re clean fclean
