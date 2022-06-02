# TSN
wireless router

# OpenWRT(lede) package add Makefile of tsn_app

Template:
```makefile
include $(TOPDIR)/rules.mk

# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=tsn_app
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
SOURCE_DIR:= xxxx

include $(INCLUDE_DIR)/package.mk

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/tsn_app
	SECTION:=tsn
	CATEGORY:=TSN
	DEPENDS := +libstdcpp +libpcap +libnet-1.2.x
	TITLE:=TSN APPLICATION
endef

# Package description; a more verbose description on what our package does
define Package/tsn_app/description
  A simple "TSN Application" -application.
endef

# Package preparation instructions; create the build directory and copy the source code. 
# The last command is necessary to ensure our preparation instructions remain compatible with the patching system.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	rsync -av --progress $(SOURCE_DIR)/* $(PKG_BUILD_DIR) --exclude $(SOURCE_DIR)/bin --exclude *.d --exclude *.a --exclude *.o --exclude *.so
	mkdir $(PKG_BUILD_DIR)/bin
	$(Build/Patch)
endef

# Package build instructions; invoke the target-specific compiler to first compile the source file, and then to link the file into the final executable
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)  CC="$(TARGET_CC)" CXX="$(TARGET_CXX)" AR="$(TARGET_AR)"
	$(MAKE) test -C $(PKG_BUILD_DIR)  CC="$(TARGET_CC)" CXX="$(TARGET_CXX)" AR="$(TARGET_AR)"
endef

# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/tsn_app/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/tsn_app $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,tsn_app))
```
