all:
	g++ -o main main.cpp `pkg-config glfw3 --static --libs` -lGLU -lGLEW -lGL common/shader.cpp

clean:
	rm main
