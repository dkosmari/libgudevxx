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
	utils.hpp


libgudevxx_la_LIBADD = $(GUDEV_LIBS)


.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> compile_flags.txt
