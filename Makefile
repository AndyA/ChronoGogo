OPENCV_LIB = "/opt/opencv/lib"
OPENCV_LIB_BREW = "/usr/local/opt/opencv3/lib"
CXXFLAGS = -Wall -g3
CXXFLAGS += $(shell PKG_CONFIG_PATH=${OPENCV_LIB}/pkgconfig:${OPENCV_LIB_BREW}/pkgconfig pkg-config --cflags opencv)
LDFLAGS  = $(shell PKG_CONFIG_PATH=${OPENCV_LIB}/pkgconfig:${OPENCV_LIB_BREW}/pkgconfig pkg-config --libs-only-L opencv)
LDLIBS   = -lstdc++ -lm
LDLIBS   += $(shell PKG_CONFIG_PATH=${OPENCV_LIB}/pkgconfig:${OPENCV_LIB_BREW}/pkgconfig pkg-config --libs-only-l opencv)

sources = chrono.cpp framestore.cpp timebend.cpp chronogogo.cpp

.PHONY: clean

default: chrono

chrono: $(sources:.cpp=.o)

clean:
	rm -f *.o *.d chrono

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(sources:.cpp=.d)
