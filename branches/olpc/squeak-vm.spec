%define name    squeak-vm
%define version 3.10
%define minor   3
%define release %{minor}olpc11
%define source  Squeak-%{version}-%{minor}
%define prefix  /usr
%define exclude FileCopyPlugin SqueakFFIPrims B3DAcceleratorPlugin PseudoTTYPlugin UnixOSProcessPlugin XDisplayControlPlugin

Name:		%{name}
Summary:	The Squeak virtual machine
Version:	%{version}
Release:	%{release}
URL:		http://squeakvm.org/unix
License:	MIT
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
Source:		%{source}.src.tar.gz
Group:		Development/Languages
Prefix:		%{prefix}
BuildRequires:	gawk
BuildRequires:	libX11-devel
BuildRequires:	libXt-devel
BuildRequires:	libvorbis-devel
BuildRequires:	libtheora-devel
BuildRequires:	speex-devel
BuildRequires:	dbus-devel
BuildRequires:	alsa-lib-devel
BuildRequires:	pango-devel
BuildRequires:	gstreamer-devel

%description
Squeak is a full-featured implementation of the Smalltalk programming
language and environment based on (and largely compatible with) the original
Smalltalk-80 system.

This package contains just the Squeak virtual machine.  You will have
to install it before you install Etoys.

%prep
%setup -n %{source}

%build
mkdir bld
cd bld
CPPFLAGS=-DSUGAR ../platforms/unix/config/configure --without-gl --prefix=%{prefix}
make ROOT=%{buildroot} %{?_smp_mflags}

%install
cd bld
[ -n "%{buildroot}" -a "%{buildroot}" != "/" ] && rm -rf %{buildroot}/*
make install ROOT=%{buildroot} docdir=%{prefix}/share/doc/squeak
for plugin in %{exclude} ; do
  rm -f %{buildroot}%{prefix}/lib/squeak/%{version}-%{minor}/$plugin
done

%clean
[ -n "%{buildroot}" -a "%{buildroot}" != "/" ] && rm -rf %{buildroot}/*

%files
%{prefix}/bin/squeak
%{prefix}/lib/squeak/
%{prefix}/share/man/man1/squeak.1.gz
%{prefix}/share/man/man1/inisqueak.1.gz
%{prefix}/share/doc/squeak/

%changelog
* Mon Sep 29 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc11; SVN r. 1921
  - fix pgUp/pgDown and view-source key mappings for ucs4 (yo,bf)
* Fri Aug 29 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc10; SVN r. 1914
  - make /dev/std(in|out|err) use stdin, stdout, stderr (bf)
* Wed Aug 27 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc9; SVN r. 1909
  - fix timezone offset in LocalePlugin to respect TZ variable (bf)
* Tue Aug 26 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc8; SVN r. 1907
  - fix sign of timezone offset in LocalePlugin (bf)
  - add SQUEAK_FAKEBIGCURSOR workaround for #8008 (bf)
* Mon Jul 28 2008 Takashi Yamamiya <takashi@vpri.org>
  - 3.10-3olpc7; SVN r. 1904
  - Fix DBusPlugin openContainer (trac #7624)
* Mon Jul 21 2008 Takashi Yamamiya <takashi@vpri.org>
  - 3.10-3olpc6; SVN r. 1901
  - fix line ending when pasting (tak)
* Mon Jul 07 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc5; SVN r. 1899
  - re-enable new (IPv6) socket primitives (bf)
  - RomePlugin pango fixes (yo)
* Mon Jun 23 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc4; SVN r. 1895
  - RomePlugin pango fixes (yo)
  - X11 drag-and-drop fixes (tak)
* Wed Jun 03 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc3; SVN r.1889
  - prefer 24 and 16 bpp over 32 to fix alpha issue in compositing desktops (bf)
* Mon May 19 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc2; SVN r.1879
  - re-add big cursor support (bf)
* Wed May 14 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.10-3olpc1; SVN r. 1878
  - updated DBus plugin (incompatible w/ previous versions)
  - add plugins: GStreamer, ImmX11, Aio
  - fixed international keyboard input
  - merged with trunk
* Fri Jan 04 2008 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-12olpc5; SVN r. 1811
  - remove Mpeg3Plugin
  - fix KedamaPlugin2 (exports where not listed) 
  - fix show-source key
  - fix SEGFAULT in OggPlugin
  - add RomePlugin w/ Pango support
  - fix drag-n-drop
* Mon Dec 03 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-12olpc4: SVN r. 1795
  - fix dbus plugin not zero-terminating some strings (bf)
* Wed Oct 31 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-12olpc3: SVN r. 1787
  - fixed errors about state checking in drag and drop (tak)
  - make cursor keys work with utf32 char code (yo)
  - add SetSwitch, GetSwitch, and SetDevice mixer functions (yo)
* Tue Oct 23 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-12olpc2: SVN r. 1779
  - use CPPFLAGS not CFLAGS for -DSUGAR
  - handle view-source key (bf)
* Mon Oct 15 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-12olpc1: SVN r. 1776
  - merged with trunk (bf)
  - signed oop fixes (dave)
  - unicode key events (ian)
  - fix dbus plugin (bf)
* Thu Aug 30 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-11olpc5: SVN r. 1761
  - set _NET_WM_PID to our process id (danw)
  - generate WindowEventClose on delete window message (ian)
* Thu Jul 17 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-11olpc4: SVN r. 1747
  - added LocalePlugin (ian)
  - FileCopyPlugin is external now
  - follow symlinks in npsqueakregister (bf)
  - explicitely link against libXt (bf)
* Thu Jul 12 2007 Bert Freudenberg <bert@freudenbergs.de>
  - 3.9-11olpc3: SVN r. 1740
  - fix spec file clean, add _smp_mflags as requested by fedora (bf)
  - XdndFinished is sent properly in drag-in with multiple types (tak)
  - Show cursor forced when drag-out (tak)
  - Fixed a bug about type index for clipboard (tak)
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
