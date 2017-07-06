OPENCV_LIB = "/opt/opencv/lib"
CXXFLAGS = $(shell PKG_CONFIG_PATH=${OPENCV_LIB}/pkgconfig pkg-config --cflags opencv)
LDFLAGS  = $(shell PKG_CONFIG_PATH=${OPENCV_LIB}/pkgconfig pkg-config --libs-only-L opencv)
LDLIBS   = -lstdc++
LDLIBS   += $(shell PKG_CONFIG_PATH=${OPENCV_LIB}/pkgconfig pkg-config --libs-only-l opencv)

sources = chrono.cpp

.PHONY: clean

default: chrono

chrono: chrono.o

clean:
	rm *.o *.d chrono

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(sources:.cpp=.d)
