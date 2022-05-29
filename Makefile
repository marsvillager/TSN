# Inc.mk
DIR_INC	= ./include
DIR_TEST= ./test
DIR_BIN	= ./bin
DIR_LIB	= ./lib

DIR_SRC	= ./src
DIR_SRC_CORE = $(DIR_SRC)/core
DIR_SRC_CORE_PORT = $(DIR_SRC_CORE)/port
DIR_SRC_CORE_QUEUE = $(DIR_SRC_CORE)/queue
DIR_SRC_CORE_FORWARD = $(DIR_SRC_CORE)/forward

DIR_SRC_NETWORKING = $(DIR_SRC)/networking
DIR_SRC_NETWORKING_FRAME = $(DIR_SRC_NETWORKING)/frame
DIR_SRC_NETWORKING_INTERFACE = $(DIR_SRC_NETWORKING)/interface

DIR_SRC_UTILS = $(DIR_SRC)/utils
DIR_SRC_UTILS_REFLECTOR = $(DIR_SRC_UTILS)/reflector
DIR_SRC_UTILS_CONFIG = $(DIR_SRC_UTILS)/config
DIR_SRC_UTILS_LOG = $(DIR_SRC_UTILS)/log

DIR_SRC_TIMER = $(DIR_SRC)/timer

#获取当前正在执行的Makefile的绝对路径
MKFILE_PATH := patsubst$(%/, %, dir $($(abspath $(lastword $(MAKEFILE_LIST)))))

SRCS  	+= $(wildcard ${DIR_SRC_CORE}/*.cc)
SRCS	+= $(wildcard $(DIR_SRC_CORE_PORT)/*.cc)
SRCS	+= $(wildcard ${DIR_SRC_CORE_QUEUE}/*.cc)
SRCS	+= $(wildcard $(DIR_SRC_CORE_FORWARD)/*cc)

SRCS  	+= $(wildcard ${DIR_SRC_NETWORKING}/*.cc)
SRCS	+= $(wildcard $(DIR_SRC_NETWORKING_FRAME)/*.cc)
SRCS	+= $(wildcard $(DIR_SRC_NETWORKING_INTERFACE)/*.cc)

SRCS  	+= $(wildcard ${DIR_SRC_UTILS}/*.cc)
SRCS  	+= $(wildcard ${DIR_SRC_UTILS_REFLECTOR}/*.cc)
SRCS	+= $(wildcard ${DIR_SRC_UTILS_CONFIG}/*.cc)
SRCS	+= $(wildcard ${DIR_SRC_UTILS_LOG}/*.cc)

SRCS	+= $(wildcard ${DIR_SRC_TIMER}/*.cc)

MAIN	:= ./src/main.cc
MAIN	+= $(SRCS)	

OBJS    := $(patsubst %.cc,${DIR_BIN}/%.o,$(notdir $(MAIN)))

ifeq ($(CC), FALSE)
CC	:= gcc
endif
ifeq ($(CXX), FALSE)
CXX := g++
endif
DEBUG 	:= y
RM		:= rm -rf
make	:= make
LIBS    := -lpcap -lnet -lpthread -lrt
LIBS	+= ./lib/tinyxml2/libtinyxml2.a
LDFLAGS :=
DEFINES :=
INCLUDES:= -I$(DIR_INC)
CFLAGS  := -Wall $(DEFINES) $(INCLUDES) -MD
ifeq ($(DEBUG), y)
CFLAGS	+= -g
else
CFLAGS	+= -O3
endif
CXXFLAGS:= $(CFLAGS) -std=c++11

PROGRAM	:= tsn_app
TARGET  := ${DIR_BIN}/${PROGRAM}

.PHONY 	: all clean cleanall rebuild test

all: $(TARGET)

test:
	mkdir -p $(DIR_BIN)/test
	(cd $(DIR_TEST) && $(MAKE)) || exit 1;

cleantest:
	(cd $(DIR_TEST) && $(MAKE) clean) || exit 1;

cleanalltest:
	(cd $(DIR_TEST) && $(MAKE) cleanall) || exit 1;

# tsn_app
${TARGET}:${OBJS}
	for dir in $(DIR_LIB);do $(MAKE) -C $$dir all || exit 1; done
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)

# src
${DIR_BIN}/%.o:$(DIR_SRC)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 


${DIR_BIN}/%.o:$(DIR_SRC_CORE)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 

${DIR_BIN}/%.o:$(DIR_SRC_CORE_PORT)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 

${DIR_BIN}/%.o:$(DIR_SRC_CORE_QUEUE)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 

${DIR_BIN}/%.o:$(DIR_SRC_CORE_FORWARD)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 


${DIR_BIN}/%.o:$(DIR_SRC_NETWORKING)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 

${DIR_BIN}/%.o:$(DIR_SRC_NETWORKING_FRAME)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 

${DIR_BIN}/%.o:$(DIR_SRC_NETWORKING_INTERFACE)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 


${DIR_BIN}/%.o:$(DIR_SRC_UTILS)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o:$(DIR_SRC_UTILS_REFLECTOR)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@ 

${DIR_BIN}/%.o:$(DIR_SRC_UTILS_CONFIG)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o:$(DIR_SRC_UTILS_LOG)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@



${DIR_BIN}/%.o:$(DIR_SRC_TIMER)/%.cc
	$(CXX) $(CFLAGS) -c $< -o $@  

rebuild: cleanall

clean:
	rm -rf *.so *.d *.o
	find ${DIR_BIN} . -name '*.o' -exec $(RM) '{}' \;
	find ${DIR_BIN} . -name '*.d' -exec $(RM) '{}' \;
	for dir in $(DIR_LIB);do $(MAKE) -C $$dir clean || exit 1; done

cleanall: clean
	$(RM) ${DIR_BIN}/*

-include ${DIR_BIN}:$(OBJS:.o=.d)