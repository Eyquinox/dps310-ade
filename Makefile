# Quick and dirty Makefile: calling 'make' rebuilds and 'make buildrun' runs the program

CC      = gcc
CFLAGS  = -std=c11 -Wall -I.
LDFLAGS = -lpaho-mqtt3c
RM      = rm -f

TARGET  = thprobe

default: build

# build and run
buildrun: buildrun/$(TARGET)

# just build
build: $(TARGET)

# link all sources into the executable
$(TARGET): main.c dps310.c fakedps310.c mqttlink.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# run any target: make buildrun/<prog> will build if needed and run
buildrun/%: %
	./$(*F)

# remove object files
clean:
	$(RM) *.o

# remove everything
distclean: clean
	$(RM) $(TARGET)
