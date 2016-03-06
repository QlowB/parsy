CXX         := g++
WARNFLAGS   := -Wno-deprecated-register
CXXFLAGS    := $(WARNFLAGS) # -std=c++11
LNFLAGS     :=
YACC        := bison
LEX         := flex

OBJECTS     := $(sort $(patsubst %.cpp, %.o ,$(wildcard *.cpp)) parser.o lexer.o)
EXECUTABLE  := parsy

debug: CXXFLAGS += -g
debug: all

fast: CXXFLAGS += -flto -O3 -fomit-frame-pointer
fast: LNFLAGS += -flto -O3
fast: all

release: CXXFLAGS += -O3 -fomit-frame-pointer
release: LNFLAGS += -O3
release: all

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LNFLAGS)

%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)

parser.cpp: parser.y
	$(YACC) --defines=parser.h -o $@ $<

lexer.cpp: lexer.l parser.cpp
	$(LEX) -o $@ $<

.PHONY: clean
clean:
	rm $(OBJECTS) parser.h parser.cpp tokens.cpp

