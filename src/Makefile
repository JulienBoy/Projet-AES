CC = gcc
CFLAGS = -Wall -O3 -Wextra -std=c11 -g 
CPPFLAGS = -I../include
LDFLAGS =
TARGET = main

 

all: $(TARGET)

$(TARGET): main.o parser.o cipher.o mode.o
	$(CC) $(CFLAGS)  $^ -o $@

main.o : main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

parser.o : parser.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

cipher.o : cipher.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

mode.o : mode.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

   

help:
	@echo "Targets available"
	@echo = all : generate the takuzu binary file from the source files
	@echo = clean : remove all temporary files + binary file generated by the compilation
	@echo = help : display the targets of the Makefile with a short description

.PHONY : all AES clean help