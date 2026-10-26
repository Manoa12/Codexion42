SRCS_DIR := src
SRCS := main.c ft_writer.c ft_parse_args.c ft_time.c ft_coders.c ft_dongles.c \
		ft_simulation.c ft_monitor_routine.c ft_coder_routine.c \
		ft_thread.c ft_check_number.c ft_coder_utils.c ft_dongle_coder.c \
		ft_heap.c

SRCS := $(addprefix $(SRCS_DIR)/, $(SRCS))
NAME := codexion

HEADER_DIR := $(SRCS_DIR)
OBJS := $(SRCS:.c=.o)
CFLAGS := -Wall -Wextra -Werror -pthread
CC := @cc
RM := @rm -f

.PHONY: all
all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -I $(HEADER_DIR) -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
	@echo "Compiling Codexion ..."
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Codexion compiled successfully"

.PHONY: clean
clean:
	@echo "Cleaning workspace ..."
	$(RM) $(OBJS)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all
