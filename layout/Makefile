# Define required macros here
CFLAGS = -std=c++0x -c -O3
OBJS = main.o
CPPS = main.cpp
LIBS =
TARGET = main
CC = g++
NAME = hw
NAME2 = hw
ALL = main exe clean 

ALL: $(ALL)

main: $(OBJS)
	@ $(CC) -o $(NAME)  $(OBJS)
main.o:  $(CPPS)
	@ $(CC) $(CFLAGS) $(CPPS)
exe:
	@ ./$(NAME2) circuit1.conf
clean:
	@ rm *.o
	@ rm $(NAME2)

