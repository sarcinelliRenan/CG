all:
	make compile run clean 
compile:
	g++ -o trabalhocg main.cpp tinyxml2.cpp Shot.cpp Car.cpp GameObject.cpp std_draw.c lodepng.cpp -lGL -lglut -lGLU -lm
run:
	./trabalhocg
clean:
	rm trab


