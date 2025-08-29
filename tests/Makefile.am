# tests/Makefile.am

if BUILD_TESTS


AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	$(GUDEV_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	-I$(top_srcdir)/include


LDADD = \
	../libgudevxx.la \
	$(GLIBMM_LIBS)


noinst_PROGRAMS = \
	device-info \
	enumerator \
	listener


device_info_SOURCES = device-info.cpp

enumerator_SOURCES = enumerator.cpp

listener_SOURCES = listener.cpp


.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> compile_flags.txt


endif BUILD_TESTS
