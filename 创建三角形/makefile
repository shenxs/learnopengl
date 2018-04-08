OBJS = example.cpp

CC = g++ -std=c++11

COMPILER_FLAGS = -Wall -DUSE_GLEW

LINKER_FLAGS = -lGLEW -lGL -lGLU -lglut -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

OBJ_NAME = example.out

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

run: $(OBJ_NAME)
	./$(OBJ_NAME)

clean:
	rm *.out

