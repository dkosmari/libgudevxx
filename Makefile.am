# Makefile.am

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	bootstrap \
	libgudevxx.pc.in \
	libgudevxx.spec.in \
	README.md


SUBDIRS = . examples


gudevxxdir = $(includedir)/gudevxx

gudevxx_HEADERS = \
	include/gudevxx/basic_wrapper.hpp \
	include/gudevxx/Client.hpp \
	include/gudevxx/GObjectWrapper.hpp \
	include/gudevxx/Device.hpp \
	include/gudevxx/Enumerator.hpp


AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	$(GUDEV_CFLAGS) \
	-I$(srcdir)/include


lib_LTLIBRARIES = libgudevxx.la


libgudevxx_la_SOURCES = \
	src/Client.cpp \
	src/Device.cpp \
	src/Enumerator.cpp \
	src/utils.hpp


libgudevxx_la_LIBADD = $(GUDEV_LIBS)


pcfiledir = $(pkgconfigdir)
pcfile_DATA = libgudevxx.pc


.PHONY: rpm

RPM_TARBALL_NAME = $(HOME)/rpmbuild/SOURCES/$(TARBALL_NAME)

rpm: libgudevxx.spec dist-gzip
	[ -e $(RPM_TARBALL_NAME) ] || \
		ln -s $(PWD)/$(TARBALL_NAME) $(RPM_TARBALL_NAME)
	rpmbuild -ba $<


.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> compile_flags.txt
