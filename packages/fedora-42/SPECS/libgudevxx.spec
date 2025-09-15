%define libname    libgudevxx
%define devname    libgudevxx-devel
%define staticname libgudevxx-static-devel

Name:           libgudevxx
Version:        0.3
Release:        1%{?dist}
Summary:        A C++20 wrapper for libgudev.
Group:          System/Libraries
License:        GPLv3
Source0:        https://github.com/dkosmari/libgudevxx/releases/download/v%{version}/%{name}-%{version}.tar.gz

%description
%{name} is a C++20 wrapper for libgudev.

BuildRequires:  automake
BuildRequires:  gcc-c++
BuildRequires:  libtool
BuildRequires:  pkgconfig(gudev-1.0)


#############
## library ##
#############


###########
## devel ##
###########

%package -n     %{devname}
Summary:        Development package for %{libname}.
Group:          Development/C++
Provides:       %{devname} = %{version}
Requires:       %{libname} = %{version}-%{release}
Requires:       pkgconfig(gudev-1.0)

%description -n %{devname}
This package provides libraries and headers files for %{name}
development.


##################
## static-devel ##
##################

%package -n     %{staticname}
Summary:        Development package for %{libname}, with static linking.
Group:          Development/C++
Provides:       %{staticname} = %{version}
Requires:       pkgconfig(gudev-1.0)

%description -n %{staticname}
This package provides static libraries for %{name} development.


###########
## build ##
###########

%prep
%autosetup

%build
%configure --disable-examples
%make_build

%install
%make_install


#####################
## package library ##
#####################

%files -n %{libname}
%license COPYING
%doc AUTHORS
%{_libdir}/*.so.*


###################
## package devel ##
###################

%files -n %{devname}
%doc README.md
%{_includedir}/gudevxx/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc


##########################
## package static-devel ##
##########################

%files -n %{staticname}
%doc README.md
%{_includedir}/gudevxx/*
%{_libdir}/*.a
%{_libdir}/pkgconfig/*.pc

