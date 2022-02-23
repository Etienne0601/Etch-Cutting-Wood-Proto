# EXE - objects we want to build
# OBJS - objects we want to compile (from C source)
# 
# We provide a phony target clean to rm all the .o files
# 
# Run make to run with warnings
# Run make clean to clean objects

CC = gcc
EXE = ecws
OBJS = ecws.o
CFLAGS = -Wall -Wextra -O0 -std=gnu11

.PHONY: clean

$(EXE) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean :
	rm $(EXE)
	rm -f $(OBJS)
