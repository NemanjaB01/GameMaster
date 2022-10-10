NAME=spacecollector
NCURSES = $(shell ncursesw5-config --cflags --libs)

all: $(NAME)

$(NAME): $(NAME).c
		gcc -Wall -Werror -std=gnu11  -o $(NAME) $(NAME).c helpers.c -pthread $(NCURSES)

run:
	./$(NAME)
		
clean:
	rm -rf $(NAME) $(NAME)
