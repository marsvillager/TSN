SUBDIRS := gtest tinyxml2
TARGET_LIBS := gtest/gtest.a tinyxml2/tinyxml2.a
MAKE = make

all: $(TARGET_LIBS)

$(TARGET_LIBS):
	for dir in $(SUBDIRS);do $(MAKE) -C $$dir all || exit 1; done #-C enter son dir to make

clean:
	@for dir in $(SUBDIRS); do make -C $$dir clean || exit 1; done

cleanall:
	@for dir in $(SUBDIRS); do make -C $$dir cleanall || exit 1; done


.PHONY: all clean