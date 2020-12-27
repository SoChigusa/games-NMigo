NMigo : main.o goban.o menu.o
	g++ -I/usr/local/include -L/usr/local/lib -lglfw -framework OpenGL `pkg-config --cflags --libs opencv` -o NMigo main.o goban.o menu.o

main.o : main.cpp class.h
	g++ -I/usr/local/include -L/usr/local/lib -lglfw -framework OpenGL `pkg-config --cflags --libs opencv` -c main.cpp

goban.o : goban.cpp class.h
	g++ -c goban.cpp

menu.o : menu.cpp class.h
	g++ -c menu.cpp

clean :
	rm main.o goban.o NMigo
