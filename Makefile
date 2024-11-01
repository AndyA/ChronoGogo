CXXFLAGS = -Wall -O2 -std=c++11
LDLIBS   = -lstdc++ -lm
# PKG_CONFIG_PATH = "/opt/homebrew/lib/pkgconfig"

CXXFLAGS += $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags opencv4)
LDFLAGS  += $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs-only-L opencv4)
LDLIBS   += $(shell PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --libs-only-l opencv4)

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
