# If necessary we can use $(info VARIABLE:$(VARIABLE)) to debug.
# We can also use make -d.

NAME		:=	pong
SRCS		:=	collision.c draw.c init.c main.c update.c utils.c
HEAD		:=	pong.h

CC			:=	clang
CFLAGS		:=	-Wall -Wextra -g3 -fsanitize=address,undefined

EXT			:=	.c

REMOVE		:=	rm -f

SRCDIR		:=	src
OBJDIR		:=	obj
INCDIR		:=	inc

SDL_PATH		:=	/opt/homebrew/Cellar/sdl2/2.28.2/
SDL_INC			:=	$(SDL_PATH)include/SDL2/
SDL_TTF_PATH	:=	/opt/homebrew/Cellar/sdl2_ttf/2.20.2/
SDL_TTF_INC		:=	$(SDL_TTF_PATH)include/SDL2/
SDL_LIB			:=	$(SDL_PATH)lib/
SDL_IMAGE_LIB	:=	$(SDL_TTF_PATH)lib/

SOURCES		:=	$(addprefix $(SRCDIR)/, $(SRCS))
HEADERS		:=	$(addprefix $(INCDIR)/, $(HEAD))
OBJECTS		:=	$(patsubst $(SRCDIR)/%$(EXT), $(OBJDIR)/%.o, $(SOURCES))

$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(SDL_INC) -I$(SDL_TTF_INC) -c $< -o $@

all: $(NAME)

$(OBJECTS): $(HEADERS) Makefile

$(NAME): $(OBJECTS)
	$(CC) -L$(SDL_LIB) -L$(SDL_IMAGE_LIB) -lSDL2 -lSDL2_ttf -fsanitize=address,undefined $(OBJECTS) -o $(NAME)

clean:
	$(REMOVE) $(OBJECTS)

fclean: clean
	$(REMOVE) $(NAME)

re: fclean
	$(MAKE) all

.PHONY:	all clean fclean re
