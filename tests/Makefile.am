# libgudevxx/utils/Makefile.am

AM_CXXFLAGS = -Wall -Wextra

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(GUDEV_CFLAGS) \
	$(GLIBMM_CFLAGS)

LDADD = \
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

company: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 echo | sort -u > compile_flags.txt
