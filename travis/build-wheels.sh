#!/bin/bash
set -e -x

# Install a system package required by our library
yum install tpm2-tss

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/dev-requirements.txt
    "${PYBIN}/pip" wheel /io/ -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" --plat $PLAT -w /io/wheelhouse/
done

# Install packages and test
for PYBIN in /opt/python/*/bin/; do
    "${PYBIN}/pip" install pytpm2 --no-index -f /io/wheelhouse
    (cd "$HOME"; "${PYBIN}/nosetests" pytpm2)
done
