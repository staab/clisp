SHELL := /bin/zsh

# Book: http://www.buildyourownlisp.com/chapter7_evaluation
# Reference: https://en.cppreference.com/w/c

all: repl

compile: src/**/*.c
	cc --std=c99 \
	-Wall \
	-I lib/mpc \
	src/main.c lib/mpc/mpc.c src/lval.c \
	-ledit -lm \
	-o build/main

repl: compile
	./build/main
