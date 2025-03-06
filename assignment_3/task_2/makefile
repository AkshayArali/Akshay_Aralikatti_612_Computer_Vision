CC=g++
CFLAGS=-O0 -g `pkg-config --cflags opencv4`
LDFLAGS=`pkg-config --libs opencv4`
SRC= houghlines.cpp
OBJ= houghlines.o
TARGET= houghlines


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
