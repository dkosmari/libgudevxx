# include/gudevxx/Makefile.am


AM_CPPFLAGS = $(GUDEV_CFLAGS)


gudevxxdir = $(includedir)/gudevxx


nobase_gudevxx_HEADERS = \
	Client.hpp \
	detail/GObjectBase.hpp \
	Device.hpp \
	Enumerator.hpp



.PHONY: company

company: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 echo | sort -u > compile_flags.txt
