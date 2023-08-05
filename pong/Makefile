# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbarberi <mbarberi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/23 21:40:52 by mbarberi          #+#    #+#              #
#    Updated: 2023/08/05 15:55:38 by mbarberi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# If necessary we can use $(info VARIABLE:$(VARIABLE)) to debug.
# We can also use make -d.

NAME		:=	pong
SRCS		:=	main.cpp Pong.class.cpp Paddle.class.cpp Ball.class.cpp
HEAD		:=	Pong.class.hpp Paddle.class.hpp Ball.class.hpp

CXX			:=	clang++
CXXFLAGS	:=	-Wall -Wextra

EXT			:=	.cpp

REMOVE		:=	rm -f

SRCDIR		:=	src
OBJDIR		:=	obj
INCDIR		:=	inc

SDL_PATH	:=	/opt/homebrew/Cellar/sdl2/2.28.2/
SDL_INC		:=	$(SDL_PATH)include/SDL2/
SDL_LIB		:=	$(SDL_PATH)lib/

SOURCES		:=	$(addprefix $(SRCDIR)/, $(SRCS))
HEADERS		:=	$(addprefix $(INCDIR)/, $(HEAD))
OBJECTS		:=	$(patsubst $(SRCDIR)/%$(EXT), $(OBJDIR)/%.o, $(SOURCES))

$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -I$(SDL_INC) -c $< -o $@

all: $(NAME)

$(OBJECTS): $(HEADERS) Makefile

$(NAME): $(OBJECTS)
	$(CXX) -L$(SDL_LIB) -lSDL2 $(OBJECTS) -o $(NAME)

clean:
	$(REMOVE) $(OBJECTS)

fclean: clean
	$(REMOVE) $(NAME)

re: fclean
	$(MAKE) all

.PHONY:	all clean fclean re
