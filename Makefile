linux:
	clang++ -o game src/game.cpp -I./3rd_party/linux/ -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17 -Wall

windows:
	clang++ -o game.exe src/game.cpp -I./3rd_party/linux/ -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lshell32 -static -std=c++17 -Wall

macos:
	clang++ -I./3rd_party/macos/ -I/opt/homebrew/include/ -L/opt/homebrew/lib/ -arch arm64 -mmacosx-version-min=10.15 -Wall -framework OpenGL -framework GLUT -framework Carbon -lpng -std=c++17 -o game src/game.cpp 

