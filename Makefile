build:
	clang main.c -o sockets -g

format:
	clang-format main.c > main.c

