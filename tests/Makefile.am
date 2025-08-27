# libgudevxx/utils/Makefile.am

AM_CXXFLAGS = -Wall -Wextra

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(GUDEV_CFLAGS) \
	$(GLIBMM_CFLAGS)

LIBS = \
	$(top_builddir)/src/libgudevxx.la \
	$(GLIBMM_LIBS)


noinst_PROGRAMS = \
	test-device-info \
	test-enumerator \
	test-listener


test_listener_SOURCES = test-listener.cpp


test_device_info_SOURCES = test-device-info.cpp


test_enumerator_SOURCES = test-enumerator.cpp


.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> compile_flags.txt
