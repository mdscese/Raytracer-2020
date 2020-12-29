CC = gcc
CFLAGS = -g
SDLFLAGS = -lSDL2main -lSDL2 -lGL

game: raytracer.c render.o scene.o vector.o
	$(CC) $(CFLAGS) -o ../$@ $^ $(SDLFLAGS) -lm

render.o: ./lib/render.c
	$(CC) $(CFLAGS) -c $^

scene.o: ./lib/scene.c
	$(CC) $(CFLAGS) -c $^

vector.o: ./lib/vector.c
	$(CC) $(CFLAGS) -c $^

.PHONY:clean
clean:
	rm game render.o scene.o vector.o
