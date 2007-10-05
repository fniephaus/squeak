#!/bin/sh

[ ! -d bld ] && mkdir bld
(cd bld && CFLAGS=-DSUGAR ../platforms/unix/config/configure --without-gl "$@")

cat > Makefile <<EOF
default:
	make -C bld

install:
	make -C bld install
EOF
