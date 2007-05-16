%define name    squeak-vm
%define version 3.9
%define minor   10
%define release %{minor}olpc5
%define source  Squeak-%{version}-%{minor}
%define prefix  /usr
%define exclude FileCopyPlugin SqueakFFIPrims B3DAcceleratorPlugin PseudoTTYPlugin UnixOSProcessPlugin XDisplayControlPlugin
%define root    %{_tmppath}/Squeak-%{version}-%{release}-buildroot

Name:		%{name}
Summary:	The Squeak virtual machine
Version:	%{version}
Release:	%{release}
Vendor:		Viewpoints Research
URL:		http://squeakvm.org/unix
Packager:	Bert Freudenberg <bert@freudenbergs.de>
License:	MIT
BuildRoot:      %{root}
Source:		%{source}.src.tar.gz
Group:		Development/Languages
Prefix:		%{prefix}
AutoReqProv:	no
Requires:	libc.so.6
Requires:	libm.so.6
Requires:	libdl.so.2
Requires:	libutil.so.1
Requires:	libnsl.so.1
Requires:	/lib/ld-linux.so.2
Requires:	libogg.so.0
Requires:	libvorbis.so.0
Requires:	libspeex.so.1
Provides:	%{name}-%{version}

%description
Squeak is a full-featured implementation of the Smalltalk programming
language and environment based on (and largely compatible with) the original
Smalltalk-80 system.

This package contains just the Squeak virtual machine.  You will have
to install it before you install the Etoys activity.

%prep
%setup -n %{source}
%build
mkdir bld
cd bld
../platforms/unix/config/configure --prefix=%{prefix}
make ROOT=%{root}

%install
cd bld
[ -n "%{root}" -a "%{root}" != "/" ] && rm -rf %{root}/*
make install ROOT=%{root}
for plugin in %{exclude} ; do
  rm -f %{root}%{prefix}/lib/squeak/%{version}-%{minor}/$plugin
done

%clean
%files
%{prefix}/bin
%{prefix}/lib/squeak
%{prefix}/man
%{prefix}/doc

%changelog
* Thu Apr 19 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc5: SVN r.1658
  - enabled Kedama plugin
* Wed Apr 04 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc4: SVN r.1652 (moved to OLPC branch)
  - added ogg plugin
* Fri Mar 23 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc3: SVN r.1613 + olpc.patch
  - big cursor support
  - faster camera input
* Wed Nov 09 2006 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc1: SVN r.1591
  - work around ALSA SIGIO issue
  - added dgd's camera support (V4L plugin)
* Wed Nov 09 2006 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-9olpc4: SVN r.1571
  - compile without GL
* Wed Oct 18 2006 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-9olpc3: SVN r.1571
  - relicensed to MIT
* Tue Oct 17 2006 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-9olpc2: SVN r.1566
  - yet another ALSA fix
* Thu Oct 12 2006 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-9olpc1: SVN r.1563
  - fix ALSA prim failed
  - add warpblt optimization
  - add Kedama2 plugin
  - exclude unneeded plugins
* Wed Oct 11 2006 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-8olpc2: SVN r.1557 adds access to ALSA mixer
* Fri Oct 06 2006 Bert Freudenberg <bert@freudenbergs.de>
  - initial RPM for OLPC (3.9-8olpc1)
