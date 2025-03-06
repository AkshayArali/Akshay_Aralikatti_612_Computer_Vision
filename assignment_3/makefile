

CC=g++
CFLAGS=-O0 -g `pkg-config --cflags opencv4`
LDFLAGS=`pkg-config --libs opencv4`

# Task 2 - houghlines
task_2_houghlines: task_2/houghlines.o
	$(CC) $(CFLAGS) -o task_2/houghlines task_2/houghlines.o $(LDFLAGS)

task_2/houghlines.o: task_2/houghlines.cpp
	$(CC) $(CFLAGS) -c task_2/houghlines.cpp -o task_2/houghlines.o

# Task 3 - houghCircles
task_3_houghCircles: task_3/houghCircles.o
	$(CC) $(CFLAGS) -o task_3/houghCircles task_3/houghCircles.o $(LDFLAGS)

task_3/houghCircles.o: task_3/houghCircles.cpp
	$(CC) $(CFLAGS) -c task_3/houghCircles.cpp -o task_3/houghCircles.o

# Task 4 - task4_capture
task_4_task4_capture: task_4/task4_capture.o
	$(CC) $(CFLAGS) -o task_4/task4_capture task_4/task4_capture.o $(LDFLAGS)

task_4/task4_capture.o: task_4/task4_capture.cpp
	$(CC) $(CFLAGS) -c task_4/task4_capture.cpp -o task_4/task4_capture.o

# Task 5 - task5_captureGray
task_5_task5_captureGray: task_5/task5_captureGray.o
	$(CC) $(CFLAGS) -o task_5/task5_captureGray task_5/task5_captureGray.o $(LDFLAGS)

task_5/task5_captureGray.o: task_5/task5_captureGray.cpp
	$(CC) $(CFLAGS) -c task_5/task5_captureGray.cpp -o task_5/task5_captureGray.o

# Task 6 - pointDetector
task_6_pointDetector: task_6/pointDetector.o
	$(CC) $(CFLAGS) -o task_6/pointDetector task_6/pointDetector.o $(LDFLAGS)

task_6/pointDetector.o: task_6/pointDetector.cpp
	$(CC) $(CFLAGS) -c task_6/pointDetector.cpp -o task_6/pointDetector.o

# All tasks to be built by default
all: task_2_houghlines task_3_houghCircles task_4_task4_capture task_5_task5_captureGray task_6_pointDetector

# Clean up build files
clean:
	rm -f task_*/*.o task_*/task_*

# Clean up build files
clean:
	rm -f task_*/*.o task_*/task_*



# for ffmpeg: `ffmpeg -f image2 -framerate 30 -i frame_%d.pgm -c:v mpeg4 -q:v 5 output.mp4`
# -f image2 → Treat input as a sequence of images.
# -framerate 30 → Set frame rate (adjust as needed).
# -i frame_%d.pgm → Input image sequence (frame_0.pgm, frame_1.pgm, etc.).
# -c:v mpeg4 → Use MPEG-4 codec.
# -q:v 5 → Set video quality (lower is better, 1 is best, 31 is worst).
# output.mp4 → Final MPEG-4 video file.