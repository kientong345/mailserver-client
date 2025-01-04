all:
	g++ mailclient_ctrl.cpp mailclient_main.cpp client_GUI.cpp -o mailclient

new:
	g++ client_ctrl.cpp client_transport.cpp client_mailbox.cpp client_graphic.cpp client_ultility.cpp -o client

clean:
	rm ./mailclient

clean_new:
	rm ./client

graphic:
	g++ client_graphic.cpp test_graphic.cpp -o graphic -lSDL2 -lSDL2_image -lSDL2_ttf

clean_graphic:
	rm ./graphic