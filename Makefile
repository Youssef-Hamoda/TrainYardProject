# ============================================
# Makefile for YardMaster
# ============================================
#
# Your task:
#   - Define variables (CC, CFLAGS, etc.)
#   - Add compilation rules for:
#         yardmaster.o
#         train.o
#         cargo.o
#   - Add rule to link final executable "yardmaster"
#   - Add a "clean" rule to delete .o files and executable
#
# Requirements:
#   • Use gcc
#   • Use -Wall -Wextra -std=c11
#   • Ensure correct header dependencies
#   • No automatic variables given — students must write them
#
# Example structure (but incomplete):
#     CC = gcc
#     CFLAGS = ...
#
#     yardmaster: yardmaster.o train.o cargo.o
#         ...
#
#     clean:
#         ...
#
# ============================================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Executable name
TARGET = yardmaster

# Object files
OBJS = yardmaster.o train.o cargo.o

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile yardmaster.o
yardmaster.o: yardmaster.c train.h cargo.h
	$(CC) $(CFLAGS) -c yardmaster.c

# Compile train.o
train.o: train.c train.h cargo.h
	$(CC) $(CFLAGS) -c train.c

# Compile cargo.o
cargo.o: cargo.c cargo.h train.h
	$(CC) $(CFLAGS) -c cargo.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
