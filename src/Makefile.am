# src/Makefile.am


AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(GUDEV_CFLAGS)


lib_LTLIBRARIES = libgudevxx.la


libgudevxx_la_SOURCES = \
	Client.cpp \
	Device.cpp \
	Enumerator.cpp \
	GObjectBase.hpp \
	utils.hpp utils.hpp


libgudevxx_la_LIBADD = $(GUDEV_LIBS)



.PHONY: company

company: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 echo | sort -u > compile_flags.txt
