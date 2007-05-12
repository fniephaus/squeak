#!/bin/sh

[ ! -d bld ] && mkdir bld
(cd bld && ../platforms/unix/config/configure "$@")

cat > Makefile <<EOF
default:
	make -C bld

install:
	make -C bld install
EOF
