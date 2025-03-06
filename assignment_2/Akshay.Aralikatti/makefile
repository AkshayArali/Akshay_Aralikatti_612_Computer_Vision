

# Directories and Compiler
INCLUDE_DIRS = -I/usr/include/opencv4
LIB_DIRS = 
CC = gcc
CXX = g++

# Compilation Flags
CDEFS = 
CFLAGS = -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS = -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lrt `pkg-config --libs opencv4`

# Source and Object Files
HFILES = 
CFILES = simple_capture.c
CPPFILES = sobel.cpp canny.cpp capture_sobel_canny.cpp capture.cpp

SRCS = ${HFILES} ${CFILES} ${CPPFILES}
OBJS = ${CFILES:.c=.o} ${CPPFILES:.cpp=.o}

# Default Target
all: simple_capture capture sobel canny capture_sobel_canny

# Cleaning Object and Binary Files
clean:
	-rm -f *.o *.d
	-rm -f simple_capture capture sobel canny capture_sobel_canny

# Building Targets for C Files
simple_capture: simple_capture.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Building Targets for C++ Files
capture: capture.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

sobel: sobel.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

canny: canny.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

capture_sobel_canny: capture_sobel_canny.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

# Pattern Rules for Compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

# Dependency Management
depend:
	$(CXX) -MM $(CPPFILES) > .depend
	$(CC) -MM $(CFILES) >> .depend

-include .depend
