# Đường dẫn thư viện (Đã khớp với máy của bạn)
CXX = g++
CPP_PATH = -I"D:\Libraries-VSCODE\SFML-3.0.2\include"
LIB_PATH = -L"D:\Libraries-VSCODE\SFML-3.0.2\lib"

# Các thư viện liên kết (Phải có sfml-main và mwindows để fix WinMain)
LIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lopengl32 -lsfml-main -mwindows

all: compile link

compile:
	$(CXX) -c main.cpp glad.c $(CPP_PATH)

link:
	$(CXX) main.o glad.o -o main $(LIB_PATH) $(LIBS)

clean:
	del /f main.exe *.o