libut

The "libut" directory is an alternative way to use uthash, utarray, utlist, 
utstring and utvector in your programs. You can copy the libut directory
entirely into your own source tree, then use CFLAGS to add libut/include
to your include-header search path (-I). 

There are two upshots of this. One is that you can just #include "libut.h"
instead of uthash.h, utarray.h, etc. So it is a convenient wrapper. 

The other benefit of using the libut is that if you do a "make" in libut/
it builds libut/libut.a, which you can then link into your program 
using LDFLAGS -Llibut -lut. This makes the utvector available. 

This also links in the plumbing that supports having a utvector of utstrings.

