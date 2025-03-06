# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 `pkg-config --cflags --libs opencv4`

# Source files
BRIGHTEN_SRC = brighten.cpp
CAPTURE_SRC = capture.cpp
DIFFCAPTURE_SRC = diffcapture.cpp
SHARPEN_SRC = sharpen-psf.c
FACEDETECT_SRC = face_detect.cpp
CUSTOMCAMERA_SRC = custom_camera.cpp

# Output binaries
BRIGHTEN_BIN = brighten
CAPTURE_BIN = capture
DIFFCAPTURE_BIN = diffcapture
SHARPEN_BIN = sharpen-psf
FACEDETECT_BIN = face_detect
CUSTOMCAMERA_BIN = custom_camera

# Build targets
all: $(BRIGHTEN_BIN) $(CAPTURE_BIN) $(DIFFCAPTURE_BIN) $(SHARPEN_BIN) $(FACEDETECT_BIN) $(CUSTOMCAMERA_BIN)

$(BRIGHTEN_BIN): $(BRIGHTEN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CAPTURE_BIN): $(CAPTURE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DIFFCAPTURE_BIN): $(DIFFCAPTURE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SHARPEN_BIN): $(SHARPEN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(FACEDETECT_BIN): $(FACEDETECT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CUSTOMCAMERA_BIN): $(CUSTOMCAMERA_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean target
clean:
	rm -f $(BRIGHTEN_BIN) $(CAPTURE_BIN) $(DIFFCAPTURE_BIN) $(SHARPEN_BIN) $(FACEDETECT_BIN) $(CUSTOMCAMERA_BIN)

# Phony targets
.PHONY: all clean
