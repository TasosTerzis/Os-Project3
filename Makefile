# paths
INCLUDE = ./include
MODULES = ./modules

# compiler
CC = gcc

# Compile options. 
CFLAGS =  -Werror -g -I$(INCLUDE) 
LDFLAGS = -lpthread

# files .o
OBJS = fileserver.o  $(MODULES)/customer.o $(MODULES)/queue.o

# executable
EXEC = fileserver

# arguments
ARGS = 1 10 70 1

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC) 

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)