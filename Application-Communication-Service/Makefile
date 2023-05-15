CC := g++
CFLAGS := -Wall -Wextra -pedantic -std=c++11
LDFLAGS := $(shell pkg-config --libs dbus-1)
CPPFLAGS := $(shell pkg-config --cflags dbus-1)

SRCS := application_communication_service.cpp main.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := application_communication

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)