SOURCES = Main.cpp Spine.cpp
INCLUDES = Spine.hpp

Main: $(SOURCES) $(INCLUDES)
	g++ -o Main $(SOURCES) 