CC := g++
CXXFILES := $(shell find ./src -type f -name '*.cpp')
CXXFLAGS := -lblkid -lext2fs -std=gnu++17

.PHONY: all clean

all: tinyfsck

tinyfsck:
	$(CC) $(CXXFILES) -o $@ $(CXXFLAGS) -O0 -g

clean:
	rm ./tinyfsck
