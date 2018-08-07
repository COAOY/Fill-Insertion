# Define required macros here
CFLAGS = -std=c++0x -c -O3
OBJS = main.o Parser.o Blank.o FileOut.o Density.o
CPPS = main.cpp Parser.cpp Blank.cpp FileOut.cpp Density.cpp
LIBS =
TARGET = main
CC = g++
NAME = cada018
NAME2 = cada018
CONFIG = circuit1.conf
ALL = main exe clean 

ALL: $(ALL)

main: $(OBJS)
	@ $(CC) -o $(NAME)  $(OBJS)
main.o:  $(CPPS)
	@ $(CC) $(CFLAGS) $(CPPS)
exe:
	@ ./$(NAME2) $(CONFIG) 
clean:
	@ rm *.o
	@ rm $(NAME2)

