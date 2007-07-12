%define name    squeak-vm
%define version 3.9
%define minor   11
%define release %{minor}olpc2
%define source  Squeak-%{version}-%{minor}
%define prefix  /usr
%define exclude FileCopyPlugin SqueakFFIPrims B3DAcceleratorPlugin PseudoTTYPlugin UnixOSProcessPlugin XDisplayControlPlugin

Name:		%{name}
Summary:	The Squeak virtual machine
Version:	%{version}
Release:	%{release}
Vendor:		Viewpoints Research
URL:		http://squeakvm.org/unix
License:	MIT
BuildRoot:      %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
Source:		%{source}.src.tar.gz
Group:		Development/Languages
Prefix:		%{prefix}
BuildRequires:	gawk
BuildRequires:	libX11-devel
BuildRequires:	libvorbis-devel
BuildRequires:	libtheora-devel
BuildRequires:	speex-devel
BuildRequires:	dbus-devel
BuildRequires:	alsa-lib-devel
#AutoReqProv:	no
#Requires:	libc.so.6
#Requires:	libm.so.6
#Requires:	libdl.so.2
#Requires:	libutil.so.1
#Requires:	libnsl.so.1
#Requires:	/lib/ld-linux.so.2
#Requires:	libogg.so.0
#Requires:	libvorbis.so.0
#Requires:	libspeex.so.1
#Requires:	libdbus-1.so.3
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
../platforms/unix/config/configure --without-gl --prefix=%{prefix}
make ROOT=%{buildroot}

%install
cd bld
[ -n "%{buildroot}" -a "%{buildroot}" != "/" ] && rm -rf %{buildroot}/*
make install ROOT=%{buildroot} docdir=%{prefix}/share/doc/squeak
for plugin in %{exclude} ; do
  rm -f %{buildroot}%{prefix}/lib/squeak/%{version}-%{minor}/$plugin
done


%clean
%files
%{prefix}/bin/squeak
%{prefix}/lib/squeak
%{prefix}/share/man/man1/squeak.1.gz
%{prefix}/share/man/man1/inisqueak.1.gz
%{prefix}/share/doc/squeak

%changelog
* Mon Jul 02 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-11olpc2: SVN r. 1735
  - clipboard/dns fixes (tak)
  - update spec to satisfy rpmlint
* Tue Jun 26 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-11olpc1: SVN r. 1728
  - IPv6 support (ian)
  - updated DBus plugin (impara, bf)
  - better drag-and-drop support, extended clipboard support (tak)
* Fri Jun 22 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc7: SVN r.1723
  - move build env to Fedora 7
  - require libdbus-1.so.3
  - configure without OpenGL (rather than hiding GL libs)
* Thu May 17 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc6: SVN r.1687
  - add ClipboardExtendedPlugin (tak)
  - add DBusPlugin (impara, bf)
  - support keypad keys (bf)
* Thu Apr 19 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc5: SVN r.1658
  - enabled Kedama plugin
* Wed Apr 04 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc4: SVN r.1652 (moved to OLPC branch)
  - added ogg plugin (tak)
* Fri Mar 23 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc3: SVN r.1613 + olpc.patch
  - big cursor support (bf)
  - faster camera input (dgd)
* Wed Nov 09 2006 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-10olpc1: SVN r.1591
  - work around ALSA SIGIO issue (bf)
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
