CC=g++
CFLAGS=-O0 -g `pkg-config --cflags opencv4`
LDFLAGS=`pkg-config --libs opencv4`
SRC= testHistogram.cpp
OBJ= testHistogram.o
TARGET= testHistogram

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)


# for ffmpeg: `ffmpeg -f image2 -framerate 30 -i frame_%d.pgm -c:v mpeg4 -q:v 5 output.mp4`
# -f image2 → Treat input as a sequence of images.
# -framerate 30 → Set frame rate (adjust as needed).
# -i frame_%d.pgm → Input image sequence (frame_0.pgm, frame_1.pgm, etc.).
# -c:v mpeg4 → Use MPEG-4 codec.
# -q:v 5 → Set video quality (lower is better, 1 is best, 31 is worst).
# output.mp4 → Final MPEG-4 video file.