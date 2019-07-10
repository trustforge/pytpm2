from setuptools import setup, Extension

module = Extension( "pytpm2",
                    sources = ['src/get_random.c'],
                    libraries = ['tss2-esys', 'tss2-tcti-device', 'tss2-tcti-mssim', 'tss2-sys', 'tss2-mu' ],
                    library_dirs=['/usr/local/lib'])

setup(  name = "pytpm2",
        version = '0.1',
        description = 'pytpm2 makes TPM 2.0 devices accessible to python through the tpm2-tss library',
        author = 'Kevin Briggs',
        author_email = 'krb686@gmail.com',
        ext_modules=[module])
