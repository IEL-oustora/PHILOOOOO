NAME = philo_bonus

Src = main.c parse_args.c process_management.c time_utils.c philosopher.c free.c
CFLAGS  = cc -Wall -Wextra -Werror -pthread -g #-fsanitize=thread 

all : $(NAME)

$(NAME) : $(Src)
	$(CFLAGS) $(Src) -o $(NAME)

clean : 
	rm -f $(NAME)

fclean :  clean

re : fclean all
