TARGET = runtime

OBJS = JPSPlus.o

CXX = g++
CXXFLAGS = -std=c++17 -DLOCAL -Wall -Wextra -Wreorder -Ofast -O3 -flto -march=native -s

DFLAGS = -g -fsanitize=address -fsanitize=undefined
RFLAGS = -DNDEBUG

all: $(TARGET)

release: CXXFLAGS += $(RFLAGS)
release: $(TARGET)

debug: CXXFLAGS += $(DFLAGS)
debug: $(TARGET)

$(TARGET): $(OBJS) main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o
distclean: clean
	rm -f $(TARGET)
