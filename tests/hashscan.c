/*
Copyright (c) 2005-2014, Troy D. Hanson    http://troydhanson.github.com/uthash/
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#ifdef __FreeBSD__
#include <sys/param.h>  /* MAXPATHLEN */
#include <vm/vm.h>      /* VM_PROT_* flags */
#endif

/* need this defined so offsetof can give us bloom offsets in UT_hash_table */
#define HASH_BLOOM 16
#include "uthash.h"

#ifdef __FreeBSD__
typedef struct {
  void *start;
  void *end;
} vma_t;
#else
typedef struct {
  off_t start;
  off_t end;
  char perms[4];   /* rwxp */
  char device[5];  /* fd:01 or 00:00 */
} vma_t;
#endif

const uint32_t sig = HASH_SIGNATURE;
int verbose=0;
int getkeys=0;

#define vv(...)  do {if (verbose>0) printf(__VA_ARGS__);} while(0)
#define vvv(...) do {if (verbose>1) printf(__VA_ARGS__);} while(0)

/* these id's are arbitrary, only meaningful within this file */
#define JEN 1
#define BER 2
#define SFH 3
#define SAX 4
#define FNV 5
#define OAT 6
#define MUR 7
#define NUM_HASH_FUNCS 8 /* includes id 0, the non-function */
char *hash_fcns[] = {"???","JEN","BER","SFH","SAX","FNV","OAT","MUR"};

/* given a peer key/len/hashv, reverse engineer its hash function */
int infer_hash_function(char *key, size_t keylen, uint32_t hashv) {
  uint32_t obkt, ohashv, num_bkts=0x01000000; /* anything ok */
  /* BER SAX FNV OAT JEN SFH */
  HASH_JEN(key,keylen,num_bkts,ohashv,obkt); if (ohashv == hashv) return JEN;
  HASH_BER(key,keylen,num_bkts,ohashv,obkt); if (ohashv == hashv) return BER;
  HASH_SFH(key,keylen,num_bkts,ohashv,obkt); if (ohashv == hashv) return SFH;
  HASH_SAX(key,keylen,num_bkts,ohashv,obkt); if (ohashv == hashv) return SAX;
  HASH_FNV(key,keylen,num_bkts,ohashv,obkt); if (ohashv == hashv) return FNV;
  HASH_OAT(key,keylen,num_bkts,ohashv,obkt); if (ohashv == hashv) return OAT;
  HASH_MUR(key,keylen,num_bkts,ohashv,obkt); if (ohashv == hashv) return MUR;
  obkt++; // this quiets an unused variable warning. yes, this is a ugly hack
  return 0;
}

/* read peer's memory from addr for len bytes, store into our dst */
#ifdef __FreeBSD__
int read_mem(void *dst, pid_t pid, void *start, size_t len) {
  struct ptrace_io_desc io_desc;
  int ret;

  io_desc.piod_op = PIOD_READ_D;
  io_desc.piod_offs = start;
  io_desc.piod_addr = dst;
  io_desc.piod_len = len;

  ret = ptrace(PT_IO, pid, (void *) &io_desc, 0);

  if (ret) {
    vv("read_mem: ptrace failed: %s\n", strerror(errno));
    return -1;
  } else if (io_desc.piod_len != len) {
    vv("read_mem: short read!\n");
    return -1;
  }

  return 0;
}
#else
int read_mem(void *dst, int fd, off_t start, size_t len) {
  int rc;
  size_t bytes_read=0;
  if (lseek(fd, start, SEEK_SET) == (off_t)-1) {
    fprintf(stderr, "lseek failed: %s\n", strerror(errno));
    return -1;
  }
  while ( len && ((rc=read(fd, (char*)dst+bytes_read, len)) > 0)) {
    len -= rc;
    bytes_read += rc;
  }
  if (rc==-1) vv("read_mem failed (%s)\n",strerror(errno));
  if ((len != 0 && rc >= 0)) vv("INTERNAL ERROR\n");
  return (rc == -1) ? -1 : 0;
}
#endif

/* later compensate for possible presence of bloom filter */
char *tbl_from_sig_addr(char *sig) {
  return (sig - offsetof(UT_hash_table,signature));
}

#define HS_BIT_TEST(v,i) (v[i/8] & (1U << (i%8)))
void found(int fd, char* peer_sig, pid_t pid) {
  UT_hash_table *tbl=NULL;
  UT_hash_bucket *bkts=NULL;
  UT_hash_handle hh;
  size_t i, bloom_len, bloom_bitlen,  bloom_on_bits=0,bloom_off_bits=0;
  char *peer_tbl, *peer_bloom_sig, *peer_bloom_nbits, *peer_bloombv_ptr,
       *peer_bloombv, *peer_bkts, *peer_key, *peer_hh, *key=NULL,
       *hash_fcn=NULL, sat[10];
  unsigned char *bloombv=NULL;
  static int fileno=0;
  char keyfile[50];
  unsigned char bloom_nbits=0;
  int keyfd=-1, mode=S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH,
      hash_fcn_hits[NUM_HASH_FUNCS], hash_fcn_winner;
  unsigned max_chain=0;
  uint32_t bloomsig;
  double bloom_sat=0;
  snprintf(sat,sizeof(sat),"         ");
  for(i=0; i < NUM_HASH_FUNCS; i++) hash_fcn_hits[i]=0;

  if (getkeys) {
    snprintf(keyfile, sizeof(keyfile), "/tmp/%u-%u.key", (unsigned)pid,fileno++);
    if ( (keyfd = open(keyfile, O_WRONLY|O_CREAT|O_TRUNC, mode)) == -1) {
      fprintf(stderr, "can't open %s: %s\n", keyfile, strerror(errno));
      exit(-1);
    }
  }

  vv("found signature at peer %p\n", peer_sig);
  peer_tbl = tbl_from_sig_addr(peer_sig);
  vvv("reading table at peer %p\n", peer_tbl);

  if ( (tbl = (UT_hash_table*)malloc(sizeof(UT_hash_table))) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(-1);
  }
#ifdef __FreeBSD__
  if (read_mem(tbl, pid, (void *)peer_tbl, sizeof(UT_hash_table)) != 0) {
#else
  if (read_mem(tbl, fd, (off_t)peer_tbl, sizeof(UT_hash_table)) != 0) {
#endif
    fprintf(stderr, "failed to read peer memory\n");
    goto done;
  }

  /* got the table. how about the buckets */
  peer_bkts = (char*)tbl->buckets;
  vvv("reading buckets at peer %p\n", peer_bkts);
  bkts = (UT_hash_bucket*)malloc(sizeof(UT_hash_bucket)*tbl->num_buckets);
  if (bkts == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(-1);
  }
#ifdef __FreeBSD__
  if (read_mem(bkts, pid, (void *)peer_bkts, sizeof(UT_hash_bucket)*tbl->num_buckets) != 0) {
#else
  if (read_mem(bkts, fd, (off_t)peer_bkts, sizeof(UT_hash_bucket)*tbl->num_buckets) != 0) {
#endif
    fprintf(stderr, "failed to read peer memory\n");
    goto done;
  }

  vvv("scanning %u peer buckets\n", tbl->num_buckets);
  for(i=0; i < tbl->num_buckets; i++) {
    vvv("bucket %u has %u items\n",  (unsigned)i, (unsigned)(bkts[i].count));
    if (bkts[i].count > max_chain) max_chain = bkts[i].count;
    if (bkts[i].expand_mult) vvv("  bucket %u has expand_mult %u\n",  (unsigned)i, (unsigned)(bkts[i].expand_mult));

    vvv("scanning bucket %u chain:\n",  (unsigned)i);
    peer_hh = (char*)bkts[i].hh_head;
    while(peer_hh) {
#ifdef __FreeBSD__
      if (read_mem(&hh, pid, (void *)peer_hh, sizeof(hh)) != 0) {
#else
      if (read_mem(&hh, fd, (off_t)peer_hh, sizeof(hh)) != 0) {
#endif
        fprintf(stderr, "failed to read peer memory\n");
        goto done;
      }
      if ((char*)hh.tbl != peer_tbl) goto done;
      peer_hh = (char*)hh.hh_next;
      peer_key = (char*)(hh.key);
      /* malloc space to read the key, and read it */
      if ( (key = (char*)malloc(sizeof(hh.keylen))) == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(-1);
      }
#ifdef __FreeBSD__
      if (read_mem(key, pid, (void*)peer_key, hh.keylen) != 0) {
#else
      if (read_mem(key, fd, (off_t)peer_key, hh.keylen) != 0) {
#endif
        fprintf(stderr, "failed to read peer memory\n");
        goto done;
      }
      hash_fcn_hits[infer_hash_function(key,hh.keylen,hh.hashv)]++;
      /* write the key if requested */
      if (getkeys) {
        write(keyfd, &hh.keylen, sizeof(unsigned));
        write(keyfd, key, hh.keylen);
      }
      free(key); key=NULL;
    }
  }

  /* does it have a bloom filter?  */
  peer_bloom_sig =   peer_tbl + offsetof(UT_hash_table, bloom_sig);
  peer_bloombv_ptr = peer_tbl + offsetof(UT_hash_table, bloom_bv);
  peer_bloom_nbits = peer_tbl + offsetof(UT_hash_table, bloom_nbits);
  vvv("looking for bloom signature at peer %p\n", peer_bloom_sig);
#ifdef __FreeBSD__
  if ((read_mem(&bloomsig, pid, (void *)peer_bloom_sig, sizeof(uint32_t)) == 0)  &&
      (bloomsig == HASH_BLOOM_SIGNATURE)) {
#else
  if ((read_mem(&bloomsig, fd, (off_t)peer_bloom_sig, sizeof(uint32_t)) == 0)  &&
      (bloomsig == HASH_BLOOM_SIGNATURE)) {
#endif
    vvv("bloom signature (%x) found\n",bloomsig);
    /* bloom found. get at bv, nbits */
#ifdef __FreeBSD__
    if (read_mem(&bloom_nbits, pid, (void *)peer_bloom_nbits, sizeof(char)) == 0) {
#else
    if (read_mem(&bloom_nbits, fd, (off_t)peer_bloom_nbits, sizeof(char)) == 0) {
#endif
       /* scan bloom filter, calculate saturation */
       bloom_bitlen = (1ULL << bloom_nbits);
       bloom_len = (bloom_bitlen / 8) + ((bloom_bitlen % 8) ? 1 : 0);
       vvv("bloom bitlen is %u, bloom_bytelen is %u\n", (unsigned)bloom_bitlen, (unsigned)bloom_len);
       if ( (bloombv = (unsigned char*)malloc(bloom_len)) == NULL) {
          fprintf(stderr, "out of memory\n");
          exit(-1);
       }
       /* read the address of the bitvector in the peer, then read the bv itself */
#ifdef __FreeBSD__
       if ((read_mem(&peer_bloombv, pid, (void *)peer_bloombv_ptr, sizeof(void*)) == 0) &&
          (read_mem(bloombv, pid, (void *)peer_bloombv, bloom_len) == 0)) {
#else
       if ((read_mem(&peer_bloombv, fd, (off_t)peer_bloombv_ptr, sizeof(void*)) == 0) &&
          (read_mem(bloombv, fd, (off_t)peer_bloombv, bloom_len) == 0)) {
#endif
          /* calculate saturation */
          vvv("read peer bloom bitvector from %p (%u bytes)\n", peer_bloombv, (unsigned)bloom_len);
          for(i=0; i < bloom_bitlen; i++) {
              if (HS_BIT_TEST(bloombv,(unsigned)i)) {
                /* vvv("bit %u set\n",(unsigned)i); */
                bloom_on_bits++;
              } else bloom_off_bits++;
          }
          vvv("there were %u on_bits among %u total bits\n", (unsigned)bloom_on_bits, (unsigned)bloom_bitlen);
          bloom_sat = bloom_on_bits * 100.0 / bloom_bitlen;
          snprintf(sat,sizeof(sat),"%2u %5.0f%%", bloom_nbits, bloom_sat);
       }
    }
  }

  /* choose apparent hash function */
  hash_fcn_winner=0;
  for(i=0;i<NUM_HASH_FUNCS;i++) {
    if (hash_fcn_hits[i] > hash_fcn_hits[hash_fcn_winner]) hash_fcn_winner=i;
  }
  hash_fcn = hash_fcns[hash_fcn_winner];

/*
Address            items    ideal  buckets mxch/<10 fl bloom/sat fcn keys saved to
------------------ -------- ----- -------- -------- -- --------- --- -------------
0x0123456789abcdef 10000000  98%  32000000 10  100% ok           BER /tmp/9110-0.key
0x0123456789abcdef 10000000 100%  32000000  9   90% NX 27/0.010% BER /tmp/9110-1.key
*/
  printf("Address            ideal    items  buckets mc fl bloom/sat fcn keys saved to\n");
  printf("------------------ ----- -------- -------- -- -- --------- --- -------------\n");
  printf("%-18p %4.0f%% %8u %8u %2u %s %s %s %s\n",
    (void*)peer_tbl,
    (tbl->num_items - tbl->nonideal_items) * 100.0 / tbl->num_items,
    tbl->num_items,
    tbl->num_buckets,
    max_chain,
    tbl->noexpand ? "NX" : "ok",
    sat,
    hash_fcn,
    (getkeys ? keyfile : ""));

#if 0
  printf("read peer tbl:\n");
  printf("num_buckets: %u\n", tbl->num_buckets);
  printf("num_items: %u\n", tbl->num_items);
  printf("nonideal_items: %u (%.2f%%)\n", tbl->nonideal_items,
    tbl->nonideal_items*100.0/tbl->num_items);
  printf("expand: %s\n", tbl->noexpand ? "inhibited": "normal");
  if (getkeys) printf("keys written to %s\n", keyfile);
#endif

 done:
  if (bkts) free(bkts);
  if (tbl) free(tbl);
  if (key) free(key);
  if (keyfd != -1) close(keyfd);
  if (bloombv) free(bloombv);
}


#ifdef __FreeBSD__
void sigscan(pid_t pid, void *start, void *end, uint32_t sig) {
  struct ptrace_io_desc io_desc;
  int page_size = getpagesize();
  char *buf;
  char *pos;

  /* make sure page_size is a multiple of the signature size, code below assumes this */
  assert(page_size % sizeof(sig) == 0);

  buf = malloc(page_size);

  if (buf == NULL) {
	fprintf(stderr, "malloc failed in sigscan()\n");
	return;
  }

  io_desc.piod_op = PIOD_READ_D;
  io_desc.piod_offs = start;
  io_desc.piod_addr = buf;
  io_desc.piod_len = page_size;

  /* read in one page after another and search sig */
  while(!ptrace(PT_IO, pid, (void *) &io_desc, 0)) {
    if (io_desc.piod_len != page_size) {
      fprintf(stderr, "PT_IO returned less than page size in sigscan()\n");
      return;
    }

    /* iterate over the the page using the signature size and look for the sig */
    for (pos = buf; pos < (buf + page_size); pos += sizeof(sig)) {
      if (*(uint32_t *) pos == sig) {
        found(pid, (char *) io_desc.piod_offs + (pos - buf), pid);
      }
    }

    /*
     * 'end' is inclusive (the address of the last valid byte), so if the current offset
     * plus a page is beyond 'end', we're already done. since all vm map entries consist
     * of entire pages and 'end' is inclusive, current offset plus one page should point
     * exactly one byte beyond 'end'. this is assert()ed below to be on the safe side.
     */
    if (io_desc.piod_offs + page_size > end) {
      assert(io_desc.piod_offs + page_size == (end + 1));
      break;
    }

    /* advance to the next page */
    io_desc.piod_offs += page_size;
  }
}
#else
void sigscan(int fd, off_t start, off_t end, uint32_t sig, pid_t pid) {
  int rlen;
  uint32_t u;
  off_t at=0;

  if (lseek(fd, start, SEEK_SET) == (off_t)-1) {
    fprintf(stderr, "lseek failed: %s\n", strerror(errno));
    return;
  }

  while ( (rlen = read(fd,&u,sizeof(u))) == sizeof(u)) {
     if (!memcmp(&u,&sig,sizeof(u))) found(fd, (char*)(start+at),pid);
     at += sizeof(u);
     if ((off_t)(at + sizeof(u)) > end-start) break;
  }

  if (rlen == -1) {
    //fprintf(stderr,"read failed: %s\n", strerror(errno));
    //exit(-1);
  }
}
#endif


#ifdef __FreeBSD__
int scan(pid_t pid) {
  vma_t *vmas=NULL, vma;
  unsigned i, num_vmas = 0;
  int ret;
  struct ptrace_vm_entry vm_entry;
  char path[MAXPATHLEN];

  vv("attaching to peer\n");
  if (ptrace(PT_ATTACH,pid,NULL,0) == -1) {
    fprintf(stderr,"failed to attach to %u: %s\n", (unsigned)pid, strerror(errno));
    exit(EXIT_FAILURE);
  }
  vv("waiting for peer to suspend temporarily\n");
  if (waitpid(pid,NULL,0) != pid) {
    fprintf(stderr,"failed to wait for pid %u: %s\n",(unsigned)pid, strerror(errno));
    goto die;
  }

  /* read memory map using ptrace */
  vv("listing peer virtual memory areas\n");
  vm_entry.pve_entry = 0;
  vm_entry.pve_path = path; /* not used but required to make vm_entry.pve_pathlen work */
  while(1) {
    /* set pve_pathlen every turn, it gets overwritten by ptrace */
    vm_entry.pve_pathlen = MAXPATHLEN;
    errno = 0;

    ret = ptrace(PT_VM_ENTRY, pid, (void *) &vm_entry, 0);

    if (ret) {
      if (errno == ENOENT) {
        /* we've reached the last entry */
	break;
      }
      fprintf(stderr, "fetching vm map entry failed: %s (%i)\n", strerror(errno), errno);
      goto die;
    }

    vvv("vmmap entry: start: %p, end: %p", (void *) vm_entry.pve_start, (void *) vm_entry.pve_end);

    /* skip unreadable or vnode-backed entries */
    if (!(vm_entry.pve_prot & VM_PROT_READ) || vm_entry.pve_pathlen > 0) {
      vvv(" -> skipped (not readable or vnode-backed)\n");
      vm_entry.pve_path[0] = 0;
      continue;
    }

    /* useful entry, add to list */
    vvv(" -> will be scanned\n");
    vma.start = (void *)vm_entry.pve_start;
    vma.end = (void *)vm_entry.pve_end;
    vmas = (vma_t *) realloc(vmas, (num_vmas + 1) * sizeof(vma_t));
    vmas[num_vmas++] = vma;
  }

  vv("peer has %u virtual memory areas\n", num_vmas);

  /* look for the hash signature */
  vv("scanning peer memory for hash table signatures\n");
  for(i=0;i<num_vmas;i++) {
    vma = vmas[i];
    sigscan(pid, vma.start, vma.end, sig);
  }

die:
  vv("detaching and resuming peer\n");
  if (ptrace(PT_DETACH, pid, NULL, 0) == -1) {
    fprintf(stderr,"failed to detach from %u: %s\n", (unsigned)pid, strerror(errno));
  }
  return 0;
}
# else
int scan(pid_t pid) {
  FILE *mapf;
  char mapfile[30], memfile[30], line[100];
  vma_t *vmas=NULL, vma;
  unsigned i, num_vmas = 0;
  int memfd;
  void *pstart, *pend, *unused;

  /* attach to the target process and wait for it to suspend */
  vv("attaching to peer\n");
  if (ptrace(PTRACE_ATTACH,pid,NULL,NULL) == -1) {
    fprintf(stderr,"failed to attach to %u: %s\n", (unsigned)pid, strerror(errno));
    exit(-1);
  }
  vv("waiting for peer to suspend temporarily\n");
  if (waitpid(pid,NULL,0) != pid) {
    fprintf(stderr,"failed to wait for pid %u: %s\n",(unsigned)pid, strerror(errno));
    goto die;
  }

  /* get ready to open its memory map. this gives us its valid memory areas */
  snprintf(mapfile,sizeof(mapfile),"/proc/%u/maps",(unsigned)pid);
  snprintf(memfile,sizeof(memfile),"/proc/%u/mem", (unsigned)pid);
  vv("opening peer memory map [%s]\n", mapfile);
  if ( (mapf = fopen(mapfile,"r")) == NULL) {
    fprintf(stderr,"failed to open %s: %s\n", mapfile, strerror(errno));
    goto die;
  }
  vv("listing peer virtual memory areas\n");
  while(fgets(line,sizeof(line),mapf)) {
    if (sscanf(line, "%p-%p %4c %p %5c", &pstart, &pend, vma.perms,
         &unused, vma.device) == 5) {
      vma.start = (off_t)pstart;
      vma.end = (off_t)pend;
      if (vma.perms[0] != 'r') continue;          /* only readable vma's */
      if (memcmp(vma.device,"fd",2)==0) continue; /* skip mapped files */
      vmas = (vma_t*)realloc(vmas, (num_vmas+1) * sizeof(vma_t));
      vmas[num_vmas++] = vma;
    }
  }
  vv("peer has %u virtual memory areas\n",num_vmas);
  fclose(mapf);

  /* ok, open up its memory and start looking around in there */
  vv("opening peer memory\n");
  if ( (memfd=open(memfile,O_RDONLY)) == -1) {
    fprintf(stderr,"failed to open %s: %s\n", memfile, strerror(errno));
    goto die;
  }
  /* look for the hash signature */
  vv("scanning peer memory for hash table signatures\n");
  for(i=0;i<num_vmas;i++) {
    vma = vmas[i];
    pstart = (void*)vma.start;
    pend = (void*)vma.end;
    /*fprintf(stderr,"scanning %p-%p %.4s %.5s\n", pstart, pend,
              vma.perms, vma.device);*/
    sigscan(memfd, vma.start, vma.end, sig, pid);
  }

  /* done. close memory and detach. this resumes the target process */
  close(memfd);

 die:
  vv("detaching and resuming peer\n");
  if (ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1) {
    fprintf(stderr,"failed to detach from %u: %s\n", (unsigned)pid, strerror(errno));
  }
  return 0;
}
#endif


void usage(const char *prog) {
  fprintf(stderr,"usage: %s [-v] [-k] <pid>\n", prog);
  exit(-1);
}

int main(int argc, char *argv[]) {
  pid_t pid;
  int opt;

  while ( (opt = getopt(argc, argv, "kv")) != -1) {
    switch (opt) {
      case 'v':
        verbose++;
        break;
      case 'k':
        getkeys++;
        break;
      default:
        usage(argv[0]);
        break;
    }
  }

  if (optind < argc) pid=atoi(argv[optind++]);
  else usage(argv[0]);

  return scan(pid);
}
