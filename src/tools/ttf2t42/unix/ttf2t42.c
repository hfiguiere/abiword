/* Copyright (c) 1997-1998 by Juliusz Chroboczek */

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "proto.h"

static void endianness_test(void);
static void usage(char*);

int verbosity=0;

int 
main(int argc, char **argv)
{
  int fd = 0;
  int i;
  FILE *out = NULL;
  /*FILE *afm=NULL;*/
  struct OffsetTable ot;
  struct HeadTable *ht;
  struct PostTable *pt;
  struct TableDirectoryEntry *td;
  /*void *loca=NULL;*/
  /*struct HheaTable *hhea=NULL;*/
  /*struct Box *bbox=NULL;*/
  /*longHorMetric *hmtx=NULL;*/
  char **strings=NULL;
  struct GlyphName *gnt=NULL;
  /*struct KernEntry0 **ke;*/
  /*int *nke;*/
  int nglyphs,postType/*,nkern*/;
  off_t headOff=0, maxpOff=0, postOff=0, nameOff=0, 
    locaOff=0, glyfOff=0, hheaOff=0, hmtxOff=0, kernOff=0;

  endianness_test();

  for(i = 1; i < argc; i++)
  {
	if(!strcmp(argv[i], "-t"))
	{
		if((fd=open(argv[++i],O_RDONLY))<0)
			syserror("Error opening input file");
		continue;
	}

	if(!strcmp(argv[i], "-p"))
	{
		if((out=fopen(argv[++i],"w"))==NULL)
			syserror("Error opening output file");
		continue;
	}

	if(!strncmp(argv[i], "-v",2))
	{
		char * p = argv[i] + 1;
		while(*p++ == 'v')
			verbosity++;
		continue;
	}

	if(!strncmp(argv[i], "-q",2))
	{
		char * p = argv[i] + 1;
		while(*p++ == 'q')
			verbosity--;
		continue;
	}

	usage(argv[0]);
	exit(2);

  };

  if(!out || !fd)
  {
	usage(argv[0]);
	exit(2);
  }

  td=readDirectory(fd,&ot);
  if(verbosity>=2)
    fprintf(stderr,"True type version %d.%u\n",
            ot.version.mantissa,ot.version.fraction);

  for(i=0;i<ot.numTables;i++) {
    if(verbosity>=2)
      fprintf(stderr,"Found `%c%c%c%c' table\n",
              (char)(td[i].tag>>24),
              (char)(td[i].tag>>16)&255,
              (char)(td[i].tag>>8)&255,
              (char)td[i].tag&255);
    switch(td[i].tag) {
    case MAKE_ULONG('m','a','x','p'):
      maxpOff=td[i].offset;
      break;
    case MAKE_ULONG('h','e','a','d'):
      headOff=td[i].offset;
      break;
    case MAKE_ULONG('p','o','s','t'):
      postOff=td[i].offset;
      break;
    case MAKE_ULONG('n','a','m','e'):
      nameOff=td[i].offset;
      break;
    case MAKE_ULONG('l','o','c','a'):
      locaOff=td[i].offset;
      break;
    case MAKE_ULONG('g','l','y','f'):
      glyfOff=td[i].offset;
      break;
    case MAKE_ULONG('h','h','e','a'):
      hheaOff=td[i].offset;
      break;
    case MAKE_ULONG('h','m','t','x'):
      hmtxOff=td[i].offset;
      break;
    case MAKE_ULONG('k','e','r','n'):
      kernOff=td[i].offset;
      break;
    }
  }
  if(maxpOff==0 || headOff==0 || postOff==0 || nameOff==0)
    error("Incomplete TTF file\n");

  if(verbosity>=1)
    fprintf(stderr,"Processing `maxp' table\n");
  surely_lseek(fd,maxpOff,SEEK_SET);
  nglyphs=readMaxpTable(fd);
  if(verbosity>=1)
    fprintf(stderr,"  %d glyphs\n",nglyphs);

  if(verbosity>=1)
    fprintf(stderr,"Processing `head' table\n");
  surely_lseek(fd,headOff,SEEK_SET);
  ht=mymalloc(sizeof(struct HeadTable));
  readHeadTable(fd,ht);

  if(verbosity>=1)
    fprintf(stderr,"Processing `post' table\n");
  surely_lseek(fd,postOff,SEEK_SET);
  pt=mymalloc(sizeof(struct PostTable));
  postType=readPostTable(fd,nglyphs,pt,&gnt);
  
  if(verbosity>=1)
    fprintf(stderr,"Processing `name' table\n");
  surely_lseek(fd,nameOff,SEEK_SET);
  strings=readNamingTable(fd);

/*  if(afm) {
    if(locaOff) {
      if(verbosity>=1)
        fprintf(stderr,"Processing `loca' table\n");
      surely_lseek(fd,locaOff,SEEK_SET);
      loca=readLocaTable(fd,nglyphs,ht->indexToLocFormat);
      if(glyfOff) {
        if(verbosity>=1)
          fprintf(stderr,"Processing `glyf' table\n");
        surely_lseek(fd,glyfOff,SEEK_SET);
        bbox=readGlyfTable(fd,nglyphs,ht->indexToLocFormat,loca);
      }
      if(hheaOff) {
        if(verbosity>=1)
          fprintf(stderr,"Processing `hhea' table\n");
        surely_lseek(fd,hheaOff,SEEK_SET);
        hhea=readHheaTable(fd);
        if(hmtxOff) {
          if(verbosity>=1)
            fprintf(stderr,"Processing `hmtx' table\n");
          surely_lseek(fd,hmtxOff,SEEK_SET);
          hmtx=readHmtxTable(fd,hhea->numberOfHMetrics);
        }
      }
    }
    if(kernOff) {
      if(verbosity>=1)
        fprintf(stderr,"Processing `kern' table\n");
      surely_lseek(fd,kernOff,SEEK_SET);
      nkern=readKernTable(fd,&nke,&ke);
    } else
      nkern=0;
  }  */
  
  if(verbosity>=1)
    fprintf(stderr,"Generating PS file\n");
  printPSFont(out,ht,strings,nglyphs,postType,pt,gnt,fd);
  fclose(out);

/*  if(afm) {
    if(verbosity>=1)
      fprintf(stderr,"Generating AFM file\n");
    printAFM(afm,ht,strings,nglyphs,postType,pt,gnt,
             bbox,hhea,hmtx,
             nkern,nke,ke);
    fclose(afm);
  }  */

  if(verbosity>=1)
    fprintf(stderr,"Done.\n");
  close(fd);
  return 0;
}


static void 
endianness_test()
{
  union {
    BYTE b[4];
    ULONG l;
  } x;
  ULONG v;

  x.b[0]=1; x.b[1]=2; x.b[2]=3; x.b[3]=4;

  v=UL(x.l);

  if(v!=(((((1<<8)+2)<<8)+3)<<8)+4) {
    fprintf(stderr,"Error: Code badly compiled for this architecture\n");
    fprintf(stderr,"Please set SMALLENDIAN and recompile\n");
    exit(2);
  }
}

static void
usage(char * name)
{
  fprintf(stderr,
          "Usage: %s [-q[q[q]]] [-v[v[v]]] -t infile.ttf -p outfile.pfa\n",
          name);
}
