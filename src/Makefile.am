# src/Makefile.am


AM_CPPFLAGS = \
	$(GUDEV_CFLAGS)


lib_LTLIBRARIES = libgudevxx.la

gudevxxdir = $(includedir)/gudevxx

gudevxx_HEADERS = \
	client.hpp \
	device.hpp \
	enumerator.hpp \
	gobject_base.hpp \
	utils.hpp


libgudevxx_la_SOURCES = \
	client.cpp \
	device.cpp \
	enumerator.cpp \
	gobject_base.hpp \
	utils.hpp


libgudevxx_la_LIBADD = $(GUDEV_LIBS)



.PHONY: company

company: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 echo | sort -u > compile_flags.txt
