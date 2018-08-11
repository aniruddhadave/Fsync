##########################################################################################################################
#
#	FILENAME	:	header.h
#
#	DESCRIPTION	:	This file contains header files, macros and function prototypes used in the program.
#
#	REVISION HISTORY:
#
#	DATE			NAME			REFERENCE			REASON			
#	22/04/2014		Saket Sharad						Initial Creation
#
#	Copyright @ Aricent 2014
#
##########################################################################################################################
BIN = ../bin/
SRC = ../src/
OBJ = ../obj/
INCLUDE = ../include/header.h
HEADER = -I../include/
CC = gcc
CFLAGS = -c -g -Wall
VFLAGS = --tool=memcheck --leak-check=yes --show-reachable=yes
PORT = 36669
IP = 10.203.161.8

$(BIN)client: $(OBJ)client.o $(OBJ)functions.o $(OBJ)functions_menu.o $(OBJ)synclientfunction.o
	$(CC) $(OBJ)client.o $(OBJ)functions.o $(OBJ)functions_menu.o $(OBJ)synclientfunction.o -o client
	mv client $(BIN)

$(OBJ)client.o: $(SRC)client.c $(INCLUDE)
	$(CC) $(CFLAGS) $(SRC)client.c $(HEADER)
	mv client.o $(OBJ)

$(OBJ)functions.o: $(SRC)functions.c $(INCLUDE)
	$(CC) $(CFLAGS) $(SRC)functions.c $(HEADER)
	mv functions.o $(OBJ)

$(OBJ)functions_menu.o: $(SRC)functions_menu.c $(INCLUDE)
	$(CC) $(CFLAGS) $(SRC)functions_menu.c $(HEADER)
	mv functions_menu.o $(OBJ)

$(OBJ)synclientfunction.o: $(SRC)synclientfunction.c $(INCLUDE)
	$(CC) $(CFLAGS) $(SRC)synclientfunction.c $(HEADER)
	mv synclientfunction.o $(OBJ)

clean:
	rm $(OBJ)*.o
	rm $(BIN)client

run:
	$(BIN)client

valgrind: 
	valgrind $(VFLAGS) $(BIN)client
