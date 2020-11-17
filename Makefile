BIN_DIR = ./bin
SRC_DIR = ./src

videoswitcher: $(BIN_DIR)/main.o $(BIN_DIR)/switch_system.o $(BIN_DIR)/signal_handler.o
	gcc $(BIN_DIR)/main.o $(BIN_DIR)/switch_system.o $(BIN_DIR)/signal_handler.o -o videoswitcher `pkg-config --libs --cflags gstreamer-1.0`


$(BIN_DIR)/main.o: src/main.c
	gcc $(SRC_DIR)/main.c -c -o $(BIN_DIR)/main.o `pkg-config --libs --cflags gstreamer-1.0`

$(BIN_DIR)/switch_system.o: src/switch_system.c src/switch_system.h
	gcc $(SRC_DIR)/switch_system.c -c -o $(BIN_DIR)/switch_system.o `pkg-config --libs --cflags gstreamer-1.0`

$(BIN_DIR)/signal_handler.o: src/signal_handler.c src/signal_handler.h
	gcc $(SRC_DIR)/signal_handler.c -c -o $(BIN_DIR)/signal_handler.o `pkg-config --libs --cflags gstreamer-1.0`


clean:
	rm ./bin/* > /dev/null
	rm videoswitcher > /dev/null
