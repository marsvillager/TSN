GTEST_DIR = ./

CPPFLAGS += -isystem $(GTEST_DIR)/include

CXXFLAGS += -g -Wall -Wextra -pthread

GTEST_LIBS = libgtest.a libgtest_main.a

GTEST_HEADERS = ./include/gtest/*.h \
				./include/gtest/internal/*.h

GTEST_OBJS	= gtest-all.o gtest_main.o

all : $(GTEST_LIBS)

clean :
	rm -f *.o

cleanall :
	rm -f $(GTEST_LIBS)

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

$(GTEST_LIBS) : $(GTEST_OBJS)
	$(AR) $(ARFLAGS) $@ $^