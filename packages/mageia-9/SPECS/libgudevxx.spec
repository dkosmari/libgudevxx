%define libname    %mklibname gudevxx
%define devname    %mklibname gudevxx -d
%define staticname %mklibname gudevxx -d -s

Name:           libgudevxx
Version:        0.3
Release:        %mkrel 1
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

%package -n     %{libname}
Summary:        A C++ wrapper for libgudev.
Group:          System/Libraries

%description -n %{libname}
This is the library needed to run programs linked with %{libname}.


###########
## devel ##
###########

%package -n     %{devname}
Summary:        Development package for %{libname}.
Group:          Development/C++
Provides:       %{devname} = %{version}
Requires:       %{libname} = %{version}-%{release}

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

