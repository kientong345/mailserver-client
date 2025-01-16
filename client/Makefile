all:
	g++ client_ctrl.cpp client_transport.cpp client_mailbox.cpp client_cli.cpp client_ultility.cpp -o client -lncurses -ljsoncpp

clean:
	rm ./client

graphic:
	g++ client_graphic.cpp test_graphic.cpp -o graphic -lSDL2 -lSDL2_image -lSDL2_ttf

clean_graphic:
	rm ./graphic