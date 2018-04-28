#include ../config.mk

TARGET  = ImageCapture

OBJDIR	= ../out/obj/$(TARGET)_obj
$(shell test -d $(OBJDIR) || mkdir -p $(OBJDIR))

SRC	:= $(wildcard *.cpp)
OBJS	:= $(patsubst %.cpp, $(OBJDIR)/%.o,$(SRC))

#CXXFLAGS += -I../include/commprotocol
#CXXFLAGS += -I../include
#CXXFLAGS += -I../Base
#CXXFLAGS += -I../include/tinyxml
#CXXFLAGS += -D__APP_VERSION__=\"${RELEASE_VERSION}\"

LDFLAGS += -lpthread -lrt
#LDFLAGS += -L../lib -lcommprotocol -lBase -ltinyxml -llog4cplus
#LDFLAGS += -L../lib/zlib/lib -lz

all:$(TARGET)

$(TARGET):$(OBJS) $(OBJS_BASE)
	$(CXX) -std=c++11 $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o:%.cpp
	$(CXX) -std=c++11 $(CXXFLAGS) -c -o $@ $<

#install:
#	cp $(TARGET) ../$(REL_APP_BIN_DIR)
#	cp AppConfig.xml ../$(REL_APP_BIN_DIR)
#	cp *.prop ../$(REL_APP_BIN_DIR)
clean:
	-rm $(OBJDIR)/*.o $(TARGET)
