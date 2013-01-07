#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "uthash.h"

#define MUR_PLUS0_ALIGNED(p) (((unsigned long)p & 0x3) == 0)
#define MUR_PLUS1_ALIGNED(p) (((unsigned long)p & 0x3) == 1)
#define MUR_PLUS2_ALIGNED(p) (((unsigned long)p & 0x3) == 2)
#define MUR_PLUS3_ALIGNED(p) (((unsigned long)p & 0x3) == 3)

#define yn(rc) (rc?"y":"n")
int main(int argc,char*argv[]) {
  unsigned rc;
  char *c = malloc(8);
  *(c+0) = 0x00;  unsigned *al = (unsigned*)(c+0);
  *(c+1) = 0x01;  unsigned *u1 = (unsigned*)(c+1);
  *(c+2) = 0x02;  unsigned *u2 = (unsigned*)(c+2);
  *(c+3) = 0x03;  unsigned *u3 = (unsigned*)(c+3);
  *(c+4) = 0x04;
  *(c+5) = 0x05;
  *(c+6) = 0x06;
  *(c+7) = 0x07;

  /* ---------------------------------------- */
  /* test whether alignment is detected properly */

  rc = MUR_PLUS0_ALIGNED(al); printf("al aligned (y): %s\n", yn(rc));
  rc = MUR_PLUS0_ALIGNED(u1); printf("u1 aligned (n): %s\n", yn(rc));
  rc = MUR_PLUS0_ALIGNED(u2); printf("u2 aligned (n): %s\n", yn(rc));
  rc = MUR_PLUS0_ALIGNED(u3); printf("u3 aligned (n): %s\n", yn(rc));
  printf("\n");

  rc = MUR_PLUS1_ALIGNED(al); printf("al plus1 (n): %s\n", yn(rc));
  rc = MUR_PLUS1_ALIGNED(u1); printf("u1 plus1 (y): %s\n", yn(rc));
  rc = MUR_PLUS1_ALIGNED(u2); printf("u2 plus1 (n): %s\n", yn(rc));
  rc = MUR_PLUS1_ALIGNED(u3); printf("u3 plus1 (n): %s\n", yn(rc));
  printf("\n");

  rc = MUR_PLUS2_ALIGNED(al); printf("al plus2 (n): %s\n", yn(rc));
  rc = MUR_PLUS2_ALIGNED(u1); printf("u1 plus2 (n): %s\n", yn(rc));
  rc = MUR_PLUS2_ALIGNED(u2); printf("u2 plus2 (y): %s\n", yn(rc));
  rc = MUR_PLUS2_ALIGNED(u3); printf("u3 plus2 (n): %s\n", yn(rc));
  printf("\n");

  rc = MUR_PLUS3_ALIGNED(al); printf("al plus3 (n): %s\n", yn(rc));
  rc = MUR_PLUS3_ALIGNED(u1); printf("u1 plus3 (n): %s\n", yn(rc));
  rc = MUR_PLUS3_ALIGNED(u2); printf("u2 plus3 (n): %s\n", yn(rc));
  rc = MUR_PLUS3_ALIGNED(u3); printf("u3 plus3 (y): %s\n", yn(rc));
  printf("\n");

  /* ---------------------------------------- */
  /* test careful reassembly of an unaligned integer */
#if 0 /* commented out since result is endian dependent */
  rc = MUR_GETBLOCK(al,0); printf("%x\n", rc);
  rc = MUR_GETBLOCK(u1,0); printf("%x\n", rc);
  rc = MUR_GETBLOCK(u2,0); printf("%x\n", rc);
  rc = MUR_GETBLOCK(u3,0); printf("%x\n", rc);
#endif

  return 0;
}
