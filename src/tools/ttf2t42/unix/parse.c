/* Copyright (c) 1997-1998 by Juliusz Chroboczek */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include "types.h"
#include "proto.h"

struct TableDirectoryEntry 
*readDirectory(int fd, struct OffsetTable *ot)
{
  unsigned n;
  int i;

  struct TableDirectoryEntry *td;
  surely_read(fd,ot,sizeof(struct OffsetTable));
  FIX_OffsetTable(*ot);
  if(verbosity>=2)
    fprintf(stderr,"%d tables\n",ot->numTables);
  n=sizeof(struct TableDirectoryEntry)*ot->numTables;
  td=mymalloc(n);
  surely_read(fd,td,n);
  for(i=0; i<ot->numTables; i++)
    FIX_TableDirectoryEntry(td[i]);
  return td;
}

char **
readNamingTable(int fd)
{
  USHORT format;
  USHORT nrecords;
  off_t position;
  USHORT offset;
  int i,index,maxIndex;
  struct NameRecord *records;
  char *data;
  char **strings;

  position=surely_lseek(fd,0,SEEK_CUR);

  surely_read(fd,&format,sizeof(USHORT));
  FIX_UH(format);
  if(format!=0)
    error("Bad TTF file\n");
  surely_read(fd,&nrecords,sizeof(USHORT));
  FIX_UH(nrecords);
  surely_read(fd,&offset,sizeof(USHORT));
  FIX_UH(offset);
  records=mymalloc(nrecords*sizeof(struct NameRecord));
  surely_read(fd,records,nrecords*sizeof(struct NameRecord));

  for(i=0,maxIndex=-1;i<nrecords;i++) {
    FIX_NameRecord(records[i]);
    index=records[i].offset+records[i].length;
    maxIndex=maxIndex>index?maxIndex:index;
  }
  data=mymalloc(maxIndex);
  surely_lseek(fd,position+offset,SEEK_SET);
  surely_read(fd,data,maxIndex);

  strings=mymalloc(8*sizeof(char*));
  for(i=0;i<8;i++)
    strings[i]=NULL;

  for(i=0; i<nrecords; i++) {
    if(records[i].platformID==3 && /* Microsoft */
       records[i].encodingID==1 && /* UGL */
       records[i].languageID==0x0409 && /* US English */
       records[i].nameID<=7) {
      strings[records[i].nameID]=mymalloc(records[i].length/2+1);
      unistrncpy(strings[records[i].nameID],
                 data+records[i].offset,
                 records[i].length);
      if(verbosity>=2)
        fprintf(stderr,"%d: %s\n",records[i].nameID,
                strings[records[i].nameID]);
    }
  }
  free(records);
  free(data);
  return strings;
}

int 
readMaxpTable(int fd)
{
  struct {
    Fixed version;
    USHORT nglyphs;
  } data;
  surely_read(fd,&data,sizeof(data));
  FIX_Fixed(data.version);
  FIX_UH(data.nglyphs);
  if(verbosity>=2)
    fprintf(stderr,"  version %d.%u\n",
            data.version.mantissa,data.version.fraction);
  return data.nglyphs;
}

void 
readHeadTable(int fd, struct HeadTable *ht)
{
  surely_read(fd,ht,sizeof(struct HeadTable));
  FIX_HeadTable(*ht);
  if(verbosity>=2) {
    fprintf(stderr,"  version %d.%d\n",
            ht->version.mantissa,ht->version.fraction);
    fprintf(stderr,"  font revision %d.%d\n",
            ht->fontRevision.mantissa, ht->fontRevision.fraction);
  }
  if(ht->magicNumber!=0x5F0F3CF5)
    error("Bad magic number in TTF file");
  if(verbosity>=2)
    fprintf(stderr,"  %d units per Em\n",ht->unitsPerEm);
}
            
int 
readPostTable(int fd, int nglyphs,struct PostTable *pt, struct GlyphName **gt)
{
  USHORT nglyphspost;
  USHORT *glyphNameIndex;
  struct GlyphName *glyphNames;
  char **glyphNamesTemp;
  int i,maxIndex;
  CHAR c;

  surely_read(fd,pt,sizeof(struct PostTable));
  FIX_PostTable(*pt);
  if(verbosity>=2)
    fprintf(stderr,"  format type %d.%u\n",
            pt->formatType.mantissa,pt->formatType.fraction);

  switch(pt->formatType.mantissa) {
  case 1:
    return 1;                   /* MacGlyphEncoding */
  case 2:
    if(pt->formatType.fraction!=0)
      error("Unsupported `post' table format");
    surely_read(fd,&nglyphspost,sizeof(USHORT));
    FIX_UH(nglyphspost);
    if(nglyphspost!=nglyphs)
      error("Inconsistency between `maxp' and `nglyphs' tables!");
    if(verbosity>=2)
      fprintf(stderr,"  %d glyphs\n",nglyphs);
    glyphNameIndex=mymalloc(sizeof(USHORT)*nglyphs);
    surely_read(fd,glyphNameIndex,sizeof(USHORT)*nglyphs);
    glyphNames=mymalloc(sizeof(struct GlyphName)*nglyphs);
    for(i=0,maxIndex=-1;i<nglyphs;i++) {
      FIX_UH(glyphNameIndex[i]);
      if(glyphNameIndex[i]<258) {
        glyphNames[i].type=0;
        glyphNames[i].name.index=glyphNameIndex[i];
      } else {
        int index;
        glyphNames[i].type=1;
        index=glyphNameIndex[i]-258;
        glyphNames[i].name.index=index;
        maxIndex=maxIndex>index?maxIndex:index;
      }
    }
    free(glyphNameIndex);
    
    i=0; glyphNamesTemp=mymalloc(sizeof(char *)*(maxIndex+1));
    while(i<=maxIndex) {
      surely_read(fd,&c,1);
      glyphNamesTemp[i]=mymalloc(c+1);
      surely_read(fd,glyphNamesTemp[i],c);
      glyphNamesTemp[i][c]='\0';
      if(verbosity>=3)
        fprintf(stderr,"    %d: %s\n",i,glyphNamesTemp[i]);
      i++;
    }
    for(i=0;i<nglyphs;i++)
      if(glyphNames[i].type==1)
        glyphNames[i].name.name=glyphNamesTemp[glyphNames[i].name.index];
    free(glyphNamesTemp);
    *gt=glyphNames;
    return 2;
  case 3:
    return 3;                   /* no name table */
  default:
    return 0;
  }
  /*NOTREACHED*/
}

void *
readLocaTable(int fd, int nglyphs, int format)
{
  int i;
  switch(format) {
  case 0:
    { 
      USHORT *offsets;
      offsets=mymalloc((nglyphs+1)*sizeof(USHORT));
      surely_read(fd,offsets,(nglyphs+1)*sizeof(USHORT));
      for(i=0; i<=nglyphs; i++)
        FIX_UH(offsets[i]);
      return offsets;
    }
    /*NOTREACHED*/
  case 1:
    {
      ULONG *offsets;
      offsets=mymalloc((nglyphs+1)*sizeof(ULONG));
      surely_read(fd,offsets,(nglyphs+1)*sizeof(ULONG));
      for(i=0; i<=nglyphs; i++)
        FIX_UL(offsets[i]);
      return offsets;
    }
    /*NOTREACHED*/
  default:
    error("Unknown `loca' table format");
    /*NOTREACHED*/
  }
  /*NOTREACHED*/
  return 0;
}

struct Box *
readGlyfTable(int fd, int nglyphs, int format, void *loca)
{
  int i;
  struct Box *bbox;
  off_t base,offset;

  base=surely_lseek(fd,0,SEEK_CUR);

  bbox=mymalloc(nglyphs*sizeof(struct Box));
  for(i=0;i<nglyphs;i++) {
    if(format==0)
      offset=2*((USHORT*)loca)[i];
    else
      offset=((ULONG*)loca)[i];
    surely_lseek(fd,base+offset+sizeof(SHORT),SEEK_SET);
    surely_read(fd,bbox+i,sizeof(struct Box));
    FIX_Box(bbox[i]);
  }
  return bbox;
}

longHorMetric *
readHmtxTable(int fd,int nummetrics)
{
  longHorMetric *metrics;
  int i;

  metrics=mymalloc(nummetrics*sizeof(longHorMetric));
  surely_read(fd,metrics,nummetrics*sizeof(longHorMetric));
  for(i=0; i<nummetrics; i++) {
    FIX_longHorMetric(metrics[i]);
  }
  return metrics;
}

struct HheaTable *
readHheaTable(int fd)
{
  struct HheaTable *hhea;
  hhea=mymalloc(sizeof(struct HheaTable));
  surely_read(fd,hhea,sizeof(struct HheaTable));
  FIX_HheaTable(*hhea);
  if(verbosity>=2)
    fprintf(stderr,"  version %d.%u\n",
            hhea->version.mantissa,hhea->version.fraction);
  if(hhea->metricDataFormat!=0)
    error("Unknown metric data format");
  return hhea;
}

int
readKernTable(int fd,int **nkep,struct KernEntry0 ***kep)
{
  struct KernTable kt;
  struct KernSubTableHeader ksth;
  struct KernSubTable0 kst;
  int i, j;
  int *nke;
  struct KernEntry0 **ke;

  surely_read(fd,&kt,sizeof(struct KernTable));
  FIX_KernTable(kt);
  if(verbosity>=2) {
    fprintf(stderr,"  version %d\n",
            kt.version);
    fprintf(stderr,"  %d subtables\n",
            kt.nTables);
  }
  nke=mymalloc(kt.nTables*sizeof(int));
  ke=mymalloc(kt.nTables*sizeof(struct KernEntry0 *));

  for(i=0; i<kt.nTables; i++) {
    surely_read(fd,&ksth,sizeof(struct KernSubTableHeader));
    FIX_KernSubTableHeader(ksth);
    if(verbosity>=2)
      fprintf(stderr,"  analyzing subtable %d, version %d... ",
              i,
              ksth.version);
    if((ksth.coverage&kernHorizontal) &&
       !(ksth.coverage&kernMinimum) &&
       !(ksth.coverage&kernCrossStream) &&
       (kernFormat(ksth.coverage)==0)) {
      surely_read(fd,&kst,sizeof(struct KernSubTable0));
      FIX_KernSubTable0(kst);
      if(verbosity>=2)
        fprintf(stderr,"reading %d entries.\n",kst.nPairs);
      nke[i]=kst.nPairs;
      ke[i]=mymalloc(kst.nPairs*sizeof(struct KernEntry0));
      surely_read(fd,ke[i],kst.nPairs*sizeof(struct KernEntry0));
      for(j=0; j<kst.nPairs; j++)
        FIX_KernEntry0(ke[i][j]);
    } else {
      if(verbosity>=2)
        fprintf(stderr,"skipping.\n");
      surely_lseek(fd,ksth.length-sizeof(struct KernSubTableHeader),SEEK_CUR);
    }
  }
  *nkep=nke;
  *kep=ke;
  return kt.nTables;
}
      



