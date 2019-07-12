#!/bin/bash
set -e -x


function setup_build {
apt -y update
apt -y install sudo

sudo apt -y install autoconf-archive \
	curl \
	libcmocka0 \
	libcmocka-dev \
	iproute2 \
	build-essential \
	git \
	pkg-config \
	python3-pip \
	gcc \
	g++ \
	m4 \
	libtool \
	automake \
	libgcrypt20-dev \
	libssl-dev \
	uthash-dev \
	autoconf \
	doxygen
}


setup_build

sleep 5

pip3 --version
pip3 install -U pip
pip --version

sleep 5


tssdir="$(mktemp -d)"
cd "$tssdir"
git clone https://github.com/tpm2-software/tpm2-tss.git
cd tpm2-tss
mkdir tmp
cd tmp
curl http://reflection.oss.ou.edu/gnu/autoconf-archive/autoconf-archive-2019.01.06.tar.xz -o autoconf-archive.tar.xz
tar -Jxvf autoconf-archive.tar.xz
cp -ar autoconf-archive-2019.01.06/m4/* ../m4/
cd ..
./bootstrap
./configure
make -j$(nproc)

# TODO
# - need to fix up part below here
# - still need to bundle in the lib

# Compile wheels
#for PYBIN in /opt/python/*/bin; do
#    "${PYBIN}/pip" wheel /io/ -w wheelhouse/
#3done
pip wheel /io/ -w wheelhouse

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" --plat $PLAT -w /io/wheelhouse/
done

# Install packages and test
for PYBIN in /opt/python/*/bin/; do
    "${PYBIN}/pip" install pytpm2 --no-index -f /io/wheelhouse
    (cd "$HOME"; "${PYBIN}/nosetests" pytpm2)
done
