#!/bin/sh

[ ! -d bld ] && mkdir bld
(cd bld && CPPFLAGS="-DSUGAR -D_FORTIFY_SOURCE=0" ../platforms/unix/config/configure --without-gl "$@")

cat > Makefile <<EOF
default:
	make -C bld

install:
	make -C bld install

check:
	@echo SKIPPED: No tests defined for Squeak VM
EOF
