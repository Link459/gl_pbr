CFLAGS = -Wall -Werror 
#needs lm for stb image
LDFLAGS = -lglfw -lcglm -lGL -lm
IFLAGS = -I ext/glad/include/ -I ext/
CMD = clang src/*.c  ext/glad/src/glad.c $(CFLAGS) $(IFLAGS)  $(LDFLAGS) -o gl_pbr
SRCS = $(shell find src -name '*.c')

build: 
	clang $(SRCS) ext/glad/src/glad.c  $(CFLAGS) $(IFLAGS) $(LDFLAGS) -O1 -o gl_pbr
release:
	clang $(SRCS) ext/glad/src/glad.c  $(CFLAGS) $(IFLAGS) $(LDFLAGS) -O2 -o gl_pbr
run: build
	./gl_pbr
debug:
	clang  $(SRCS) ext/glad/src/glad.c  $(CFLAGS) $(IFLAGS) $(LDFLAGS) -O2 -o gl_pbr -ggdb
	gdb gl_pbr
compile_commands: 	
	bear -- $(CMD)
clean:
	rm gl_pbr

.PHONY: build run compile_commands clean debug
