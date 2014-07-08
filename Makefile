LIBS = -lGL -lglut -lGLEW
OBJS = obj/Drawable.o obj/Exception.o obj/Bat.o obj/objloader.o obj/vboindexer.o obj/tga.o obj/Scene.o obj/Block.o obj/Ball.o
FLAGS = -g

all: inc/light.h inc/Drawable.h inc/Exception.h inc/Scene.h inc/Bat.h inc/Ball.h inc/Block.h game.cpp $(OBJS) 
	g++ -Wall $(OBJS) game.cpp $(LIBS) -o game $(FLAGS)

obj/Drawable.o: inc/Drawable.h inc/Exception.h src/Drawable.cpp inc/objloader.hpp inc/vboindexer.hpp inc/tga.h inc/textures.h inc/materials_samplers.h inc/light.h
	g++ -c -Wall src/Drawable.cpp -o obj/Drawable.o
	
obj/Bat.o: inc/Bat.h inc/Drawable.h src/Bat.cpp
	g++ -c -Wall src/Bat.cpp -o obj/Bat.o
	
obj/Ball.o: inc/Ball.h inc/Drawable.h src/Ball.cpp
	g++ -c -Wall src/Ball.cpp -o obj/Ball.o
	
obj/Exception.o: inc/Exception.h src/Exception.cpp
	g++ -c -Wall src/Exception.cpp -o obj/Exception.o

obj/objloader.o: inc/objloader.hpp src/objloader.cpp
	g++ -c -Wall src/objloader.cpp -o obj/objloader.o
	
obj/vboindexer.o: inc/vboindexer.hpp src/vboindexer.cpp
	g++ -c -Wall src/vboindexer.cpp -o obj/vboindexer.o

obj/tga.o: inc/tga.h src/tga.cpp
	g++ -c -Wall src/tga.cpp -o obj/tga.o

obj/Scene.o: inc/Bat.h inc/Block.h inc/Ball.h inc/light.h inc/Scene.h src/Scene.cpp
	g++ -c -Wall src/Scene.cpp -o obj/Scene.o

obj/Block.o: inc/Block.h inc/Drawable.h src/Block.cpp
	g++ -c -Wall src/Block.cpp -o obj/Block.o

clean:
	rm -r obj/*
