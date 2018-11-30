/* $Id: horoscope.c,v 1.5 1999/07/16 01:48:11 edwardc Exp $ */

#include <time.h>
#include <stdio.h>
#include <math.h>
#include "bbs.h"
#define MAX_LINE        (15)

char    *Ctime(date)
time_t  *date;
{
        static char buf[80];

        strcpy(buf, (char *)ctime(date));
        buf[strlen(buf)-1] = '\0';
        return buf;
}

main(argc, argv)
  char *argv[];
{
  FILE *fp;
  int now;
  int max = 0,item;
  int i, j;
  int boy=0, girl=0;
  char buf[256];
  char    *blk[10] =
  {
      "  ", "  ", "  ", "  ", "  ",
      "  ", "  ", "  ", "  ", "  ",
  };
  struct userec aman;
  struct userec allman[ MAXUSERS ];
  int mhoro[12];
  int fhoro[12];
  int bound[12] = {21,19,21,21,21,22,23,23,23,24,23,22};
  int mani[12];
  int fani[12];

  sprintf(buf,"%s/.PASSWDS", BBSHOME);
  if ((fp = fopen(buf, "rb")) == NULL)
  {
    printf("Can't open record data file.\n");
    return 1;
  }
  for (i=0; ; i++) {
    if (fread(&allman[ i ], sizeof( aman ), 1, fp ) <= 0) break;
    if (allman[i].birthmonth == 0) continue;
    if (allman[i].gender=='M') {
       boy++;
       mani[allman[i].birthyear % 12]++;
    } else {
       girl++;
       fani[allman[i].birthyear % 12]++;
    }
    if (allman[i].birthday < bound[allman[i].birthmonth-1]) {
      if(allman[i].gender=='M') mhoro[allman[i].birthmonth-1]++; else fhoro[allman[i].birthmonth-1]++;
    } else {
      if(allman[i].birthmonth == 12) {
        if(allman[i].gender=='M') mhoro[0]++; else fhoro[0]++;
      } else {
        if(allman[i].gender=='M') mhoro[allman[i].birthmonth]++; else fhoro[allman[i].birthmonth]++;
      }
    }
  }
  fclose(fp);

  now=time(0);
  for (i = 0; i < 12; i++)
  {
    if (mhoro[i] > max)
      max = mhoro[i];
    if (fhoro[i] > max)
      max = fhoro[i];
  }

  item = max / MAX_LINE + 1;

  sprintf(buf,"%s/0Announce/bbslist/counthoro", BBSHOME);
  if ((fp = fopen(buf, "w")) == NULL)
  {
    printf("Can't open counthoro\n");
    return 1;
  }

  fprintf(fp,"\n[1;37m    ©³                                                                        ©·\n");
  for (i = MAX_LINE ; i >= 0; i--)
  {
    fprintf(fp, "[1;37m%4d  ",(i+1)*item);
    for (j = 0; j < 12; j++)
    {
      if ((item * (i) > mhoro[j]) && (item * (i-1) <= mhoro[j]) && mhoro[j])
        fprintf(fp, "[35m%-3d", (mhoro[j]));
      else if(mhoro[j]-item*i<item && item*i<mhoro[j])
              fprintf(fp,"[36m%s ", blk[((mhoro[j]-item * i)*10)/item]);
      else if(mhoro[j]-item * i>=item)
              fprintf(fp,"[36m%s ",blk[9]);
      else
           fprintf(fp,"   ");
      if ((item * (i) > fhoro[j]) && (item * (i-1) <= fhoro[j]) && fhoro[j])
        fprintf(fp, "[35m%-3d", (fhoro[j]));
      else if(fhoro[j]-item*i<item && item*i<fhoro[j])
              fprintf(fp,"[31m%s ", blk[((fhoro[j]-item * i)*10)/item]);
      else if(fhoro[j]-item * i>=item)
              fprintf(fp,"[31m%s ",blk[9]);
      else
           fprintf(fp,"   ");
    }
    fprintf(fp, "[1;37m  \n");
  }
  fprintf(fp,"   [37m0©»      Ä¿Ç°±¾Õ¾×¢²áÊ¹ÓÃÕßÐÇ×ùÍ³¼Æ          %s      ©¿\n"
       "    [1;33m  Ä¦ôÉ  Ë®Æ¿  Ë«Óã  ÄµÑò  ½ðÅ£  Ë«×Ó  ¾ÞÐ·  Ê¨×Ó  ´¦Å®  Ìì³Ó  ÌìÐ«  ÉäÊÖ\n\n"
       "            [32m1 [37m   [32m= [37m%-5d       [36m   [32m= [37mÄÐÉú (%d)    [31m   [32m= [37mÅ®Éú (%d)[m\n"
    ,Ctime(&now),item,boy,girl);
  fclose(fp);

  for (i = 0; i < 12; i++)
  {
    if (mani[i] > max)
      max = mani[i];
    if (fani[i] > max)
      max = fani[i];
  }

  item = max / MAX_LINE + 1;

  sprintf(buf,"%s/0Announce/bbslist/countani", BBSHOME);
  if ((fp = fopen(buf, "w")) == NULL)
  {
    printf("Can't open countani\n");
    return 1;
  }

  fprintf(fp,"\n[1;37m    ©³                                                                        ©·\n");
  for (i = MAX_LINE ; i >= 0; i--)
  {
    fprintf(fp, "[1;37m%4d  ",(i+1)*item);
    for (j = 0; j < 12; j++)
    {
      if ((item * (i) > mani[j]) && (item * (i-1) <= mani[j]) && mani[j])
        fprintf(fp, "[35m%-3d", (mani[j]));
      else if(mani[j]-item*i<item && item*i<mani[j])
              fprintf(fp,"[36m%s ", blk[((mani[j]-item * i)*10)/item]);
      else if(mani[j]-item * i>=item)
              fprintf(fp,"[36m%s ",blk[9]);
      else
           fprintf(fp,"   ");
      if ((item * (i) > fani[j]) && (item * (i-1) <= fani[j]) && fani[j])
        fprintf(fp, "[35m%-3d", (fani[j]));
      else if(fani[j]-item*i<item && item*i<fani[j])
              fprintf(fp,"[31m%s ", blk[((fani[j]-item * i)*10)/item]);
      else if(fani[j]-item * i>=item)
              fprintf(fp,"[31m%s ",blk[9]);
      else
           fprintf(fp,"   ");
    }
    fprintf(fp, "[1;37m  \n");
  }
  fprintf(fp,"   [37m0©»      Ä¿Ç°±¾Õ¾×¢²áÊ¹ÓÃÕßÉúÐ¤Í³¼Æ          %s      ©¿\n"
       "    [1;33m    Êó    Å£    »¢    ÍÃ    Áú    Éß    Âí    Ñò    ºï    ¼¦    ¹·    Öí\n\n"
       "            [32m1 [37m   [32m= [37m%-5d       [36m   [32m= [37mÄÐÉú (%d)    [31m   [32m= [37mÅ®Éú (%d)[m\n"
    ,Ctime(&now),item,boy,girl);
  fclose(fp);

}
