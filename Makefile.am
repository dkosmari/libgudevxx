# Makefile.am


ACLOCAL_AMFLAGS = -I m4


EXTRA_DIST = \
	bootstrap \
	libgudevxx.pc.in \
	libgudevxx.spec.in \
	README.md


SUBDIRS = \
	include/gudevxx \
	src \
	tests


pcfiledir = $(pkgconfigdir)
pcfile_DATA = libgudevxx.pc



.PHONY: rpm

TARBALL_NAME = $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz
RPM_TARBALL_NAME = $(HOME)/rpmbuild/SOURCES/$(TARBALL_NAME)

rpm: libgudevxx.spec dist-gzip
	[ -e $(RPM_TARBALL_NAME) ] || \
		ln -s $(PWD)/$(TARBALL_NAME) $(RPM_TARBALL_NAME)
	rpmbuild -ba $<
