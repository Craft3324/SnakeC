PROG=main.exe
CC=gcc
CFLAGS=-g -Wall -Werror
CFLAGS2=-O3 -Wall -Werror
CPPFLAGS=-Iinclude
DEPS =
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/screens/*.c) $(wildcard $(SRC_DIR)/game/*.c)
OBJ_CODE =
OBJ = $(OBJ_CODE) $(SRC:.c=.o)

GTEST = . #gtest-1.7.0/include
#LIBGTEST = /mingw64/lib/libgtest_main.a /mingw64/lib/libgtest.a
LIBGTEST = C:\msys64\mingw64\lib\libgtest_main.a C:\msys64\mingw64\lib\libgtest.a
#TESTDIR = tests

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

run: $(PROG)
	./$(PROG)

clean:
	rm -f $(OBJ) $(PROG)

.PHONY: run clean
