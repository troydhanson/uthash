libut

The "libut" directory is an alternative way to use uthash, utarray, utlist, 
utstring and utvector in your programs. You can copy the libut directory
into your own source tree. Copy it this way to make sure you get the files
and not just links (-L dereferences symlinks in the source directory):

    cp -RL libut $DESTINATION

Then add CFLAGS to your application Makefile to pull in libut/include.

    CFLAGS=-I libut/include

You can use libut.h as a wrapper to pull in uthash.h, utarray.h, etc. 

    #include "libut.h"

utvector

The utvector is an alternative to utarray. It is a bit more efficient.
It's object code, not just a header. To use utvector, build libut.a:

    cd libut
    make

Link it into your program using:

    LDFLAGS=-Llibut -lut

Examples of using utvector are in libut/tests.
