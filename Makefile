TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c -Wall -Wvla -Werror -O -s -std=c11
ASAN_FLAGS = -fsanitize=address,leak
SRC        = vm_riskxvii.c isa.c heap.c
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $<

run:
	./$(TARGET) $(program)

test:
	echo "Add tests here!"


clean:
	rm -f *.o *.obj $(TARGET)

# These are for testing etc.
# CFLAGS     = -c -Wall -Wvla -Werror -O0 -g -std=c11
# ASAN_FLAGS = -fsanitize=address -fsanitize=leak