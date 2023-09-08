# If necessary we can use $(info VARIABLE:$(VARIABLE)) to debug.
# We can also use make -d.

NAME		:=	pong
SRCS		:=	main.c pong.c
HEAD		:=	pong.h

C			:=	cc
CFLAGS		:=	-Wall -Wextra -Werror -O3

EXT			:=	.c

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
	$(C) $(CFLAGS) -I$(INCDIR) -I$(SDL_INC) -c $< -o $@

all: $(NAME)

$(OBJECTS): $(HEADERS) Makefile

$(NAME): $(OBJECTS)
	$(C) -L$(SDL_LIB) -lSDL2 $(OBJECTS) -o $(NAME)

clean:
	$(REMOVE) $(OBJECTS)

fclean: clean
	$(REMOVE) $(NAME)

re: fclean
	$(MAKE) all

.PHONY:	all clean fclean re
