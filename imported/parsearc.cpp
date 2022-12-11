#include "parsearc.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>


#define TRUE  1
#define FALSE 0
#define CSVSEPARATOR ';'

#define MAGICOFFSET        54
#define HEADEROFFSET       90
#define DATEOFFSET        354
#define NCHANNELOFFSET    362
#define SAMPLEOFFSET      382
#define HEIGHTOFFSET      410
#define WEIGHTOFFSET      414
#define TELEPHONEOFFSET   418
#define PATIENTIDOFFSET   458
#define DIAGNOSISOFFSET   490
#define MEDICATIONOFFSET  592
#define ACCESSIONOFFSET   694
#define INOUTPEIDOFFSET   762
#define PATIENTAREAOFFSET 896
#define CHANNELOFFSET    2146
#define DEPARTMENTOFFSET 2310
#define RECORDEROFFSET   2654
#define VERSION1OFFSET   2702
#define VERSION2OFFSET   2766
#define LOWPASSOFFSET    2946
#define DFTFILTEROFFSET  2978
#define NAMEOFFSET       2987
#define PHYSICIANOFFSET  3050
#define TECHNICIANOFFSET 3114
#define PROCEDUREOFFSET  3178
#define HISTORYOFFSET    3242
#define ADDRESSOFFSET    3306
#define RAWFILEOFFSET    4026
#define HEADERLEN        4286


/*
   Parse an ARC (archive) file from the EDAN SE-20212 Holter recorder
   Details of the two components of the header (patient.hea) and ECG data (ecgraw.dat) can be found here
      http://paulbourke.net/dataformats/edf/
   Outputs: CSV file of individual channels and all channels combined
   Versions 1.0, 10 June
   Versions 1.1, 14 June - Added LogFile() to write metadata
                         - Single channel save an option, see -s
*/

int verbose = FALSE;
int debug = FALSE;
int savesingle = FALSE;





int nchannels = 12;
int nrecords = 0;





void CleanPrint(int,int);
void ReadString(FILE *,char *,int *,int);
void SkipTo(FILE *,int *,int);
void LogFile(FILE *,char *,char *,int,char);
char *TrimWhitespaces(char *str);

namespace {
    int frequency;
}

void SaveDataIntoCsv(std::vector<CHANNEL>&& channels, const std::string& outputFilePath)
{
    FILE *fall, *fout[nchannels];
    char fname[64];
    // Save data in CSV format
    // Open one file for each channel, and one for all combined
    if (savesingle) {
        for (int i=0; i < nchannels; i++) {
            sprintf(fname, "channel_%02d.csv",i+1);
            fout[i] = fopen(fname,"w");
            fprintf(fout[i],"Time%c%s\n",CSVSEPARATOR,channels[i].name);
        }
    }


    fall = fopen(outputFilePath.c_str(),"w");
    fprintf(fall,"Time%c",CSVSEPARATOR);
    for (int i = 0; i < nchannels; i++) {
        fprintf(fall,"%s",channels[i].name);
        if (i == nchannels-1)
            fprintf(fall,"\n");
        else
            fprintf(fall,"%c",CSVSEPARATOR);
    }
    for (int i = 0; i < nrecords; i++) {
        for (int j = 0; j < nchannels; j++) {
            if (savesingle) {
                fprintf(fout[j],"%lf%c%d\n",i/(double)frequency,CSVSEPARATOR,channels[j].v[i]);
            }
            if (j == 0)
                fprintf(fall,"%lf%c",i/(double)frequency,CSVSEPARATOR);
            fprintf(fall,"%d",channels[j].v[i]);
            if (j == nchannels-1) {
                fprintf(fall,"\n");
            } else {
                fprintf(fall,"%c",CSVSEPARATOR);
            }
        }
    }

    if (savesingle) {
        for (int j = 0; j < nchannels; j++)
            fclose(fout[j]);
    }
    fclose(fall);
}

std::vector<CHANNEL> ReadDataFromArc(const std::string& filename)
{
    const unsigned constNoOfChannels = nchannels;
    std::vector<CHANNEL> channels(constNoOfChannels, CHANNEL{});

    int ic,nc,nptr=0,nfooter=0;
    int hours,minutes,seconds;
    char s[1024];
    short int sc;
    unsigned short int usc;
    FILE *fin,*flog;
    time_t time1,time2;

    // Set up channel structure
//    channels = reinterpret_cast<CHANNEL*>(malloc(nchannels*sizeof(CHANNEL)));
    for (int i = 0; i < nchannels; i++) {
        for (int j = 0; j < 10; j++)
            channels[i].name[j] = '\0';
//        channels[i].v = NULL;
    }

    // Open log file
    flog = fopen("metadata.txt","w");

    // Open file
    if ((fin = fopen(filename.c_str(),"rb")) == NULL) {
        fprintf(stderr,"Failed to open file \"%s\"\n",filename.c_str());
        exit(-1);
    }

    // Skip over 4 binary bytes
    SkipTo(fin,&nptr,4);

    // Text
    ReadString(fin,s,&nptr,50);
    LogFile(flog,"Title:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to magic string
    SkipTo(fin,&nptr,MAGICOFFSET);
    ReadString(fin,s,&nptr,10);
    LogFile(flog,"Magic string:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to header file name
    SkipTo(fin,&nptr,HEADEROFFSET);
    ReadString(fin,s,&nptr,16);
    LogFile(flog,"Header filename:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Two dates
    SkipTo(fin,&nptr,DATEOFFSET);
    fread(&ic,4,1,fin);
    time1 = ic;
    nptr += 4;
    LogFile(flog,"Date 1:",ctime(&time1),0,'s');
    fread(&ic,4,1,fin);
    time2 = ic;
    nptr += 4;
    LogFile(flog,"Date 2:",ctime(&time2),0,'s');

    // Skip to and read number of channels
    SkipTo(fin,&nptr,NCHANNELOFFSET);
    usc = fgetc(fin);
    nchannels = usc;
    LogFile(flog,"Number of channels:"," ",usc,'i');
    nptr++;
    if (debug)
        fprintf(stderr,"--- Offset %d ---\n",nptr);
    sc = fgetc(fin); // Back to even byte boundary
    nptr++;

    // Skip to and read sample frequency
    SkipTo(fin,&nptr,SAMPLEOFFSET);
    fread(&usc,2,1,fin);
    frequency = usc;
    LogFile(flog,"Sample rate (Hz):"," ",usc,'i');
    nptr+=2;
    if (debug)
        fprintf(stderr,"--- Offset %d ---\n",nptr);

    // Skip to and read patient height
    SkipTo(fin,&nptr,HEIGHTOFFSET);
    fread(&sc,2,1,fin);
    if (sc < 0)
        sc = 0;
    LogFile(flog,"Height:"," ",sc,'i');
    nptr+=2;
    if (debug)
        fprintf(stderr,"--- Offset %d ---\n",nptr);

    // Skip to and read patient weight
    SkipTo(fin,&nptr,WEIGHTOFFSET);
    fread(&sc,2,1,fin);
    if (sc < 0)
        sc = 0;
    LogFile(flog,"Weight:"," ",sc,'i');
    nptr+=2;
    if (debug)
        fprintf(stderr,"--- Offset %d ---\n",nptr);

    // Skip to and read telephone number.
    // Software field length seems limited to 30 characters
    SkipTo(fin,&nptr,TELEPHONEOFFSET);
    ReadString(fin,s,&nptr,40);
    LogFile(flog,"Telephone number:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read patient ID
    // Software seems limited to 30 characters
    SkipTo(fin,&nptr,PATIENTIDOFFSET);
    ReadString(fin,s,&nptr,32);
    LogFile(flog,"Patient id:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read clinical diagnosis
    // Software seems limited to 61
    SkipTo(fin,&nptr,DIAGNOSISOFFSET);
    ReadString(fin,s,&nptr,102);
    LogFile(flog,"Diagnosis:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Read Medication
    // Software limited to 56(?)
    SkipTo(fin,&nptr,MEDICATIONOFFSET);
    ReadString(fin,s,&nptr,102);
    LogFile(flog,"Medication:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Read Accession number
    // Software seems limited to 65
    SkipTo(fin,&nptr,ACCESSIONOFFSET);
    ReadString(fin,s,&nptr,68);
    LogFile(flog,"Accession number:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Read In/Out/PE ID
    // Software seems limited to 31
    SkipTo(fin,&nptr,INOUTPEIDOFFSET);
    ReadString(fin,s,&nptr,134);
    LogFile(flog,"In/Out/PE ID:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Read Patient area
    // Software seems limited to 31
    SkipTo(fin,&nptr,PATIENTAREAOFFSET);
    ReadString(fin,s,&nptr,134);
    LogFile(flog,"Patient area:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read channel names
    SkipTo(fin,&nptr,CHANNELOFFSET);
    for (int j = 0; j < nchannels; j++) {
        ReadString(fin,s,&nptr,8);
        LogFile(flog,"Electrode name:",s,0,'s');
        strcpy(channels[j].name, TrimWhitespaces(s));
    }
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read department
    // Software seems limited to 31
    SkipTo(fin,&nptr,DEPARTMENTOFFSET);
    ReadString(fin,s,&nptr,134);
    LogFile(flog,"Department:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read recorder ID
    SkipTo(fin,&nptr,RECORDEROFFSET);
    ReadString(fin,s,&nptr,10);
    LogFile(flog,"Recorder id:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read first version number
    SkipTo(fin,&nptr,VERSION1OFFSET);
    ReadString(fin,s,&nptr,6);
    LogFile(flog,"Version 1:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read second version number
    SkipTo(fin,&nptr,VERSION2OFFSET);
    ReadString(fin,s,&nptr,6);
    LogFile(flog,"Version 2:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read low pass filter value
    SkipTo(fin,&nptr,LOWPASSOFFSET);
    fread(&usc,2,1,fin);
    nptr += 2;
    LogFile(flog,"Low pass filter:"," ",usc,'i');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read DFT filter value
    SkipTo(fin,&nptr,DFTFILTEROFFSET);
    ReadString(fin,s,&nptr,5);
    LogFile(flog,"DFT filter:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read name
    SkipTo(fin,&nptr,NAMEOFFSET);
    ReadString(fin,s,&nptr,64);
    LogFile(flog,"Name:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read physician name
    // Software seems limited to 61
    SkipTo(fin,&nptr,PHYSICIANOFFSET);
    ReadString(fin,s,&nptr,64);
    LogFile(flog,"Physician:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read tecnician name
    // Software seems limited to 61
    SkipTo(fin,&nptr,TECHNICIANOFFSET);
    ReadString(fin,s,&nptr,64);
    LogFile(flog,"Technician:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read procedure
    // Software seems limited to 61
    SkipTo(fin,&nptr,PROCEDUREOFFSET);
    ReadString(fin,s,&nptr,64);
    LogFile(flog,"Procedure:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read medical history
    // Software seems limited to 61
    SkipTo(fin,&nptr,HISTORYOFFSET);
    ReadString(fin,s,&nptr,64);
    LogFile(flog,"Medical history:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read address
    // Software seems limited to 87
    SkipTo(fin,&nptr,ADDRESSOFFSET);
    ReadString(fin,s,&nptr,88);
    LogFile(flog,"Address:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to and read raw file name
    SkipTo(fin,&nptr,RAWFILEOFFSET);
    ReadString(fin,s,&nptr,16);
    LogFile(flog,"Raw filename:",s,0,'s');
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);

    // Skip to data
    SkipTo(fin,&nptr,HEADERLEN);

    // Read ECG data
    // There seems to be stuff at the end
    // A value of 12 seems to be the indication of the end
    nc = 0;
    while (fread(&usc,2,1,fin) == 1) {
        if (usc < 20)
            break;
//        channels[nc].v = reinterpret_cast<short*>(realloc(channels[nc].v, (nrecords+1)*sizeof(short int)));
        channels[nc].v.push_back( (int)usc - 32768); // Unsigned to signed
        nc++;
        if (nc == nchannels) {
            nc = 0;
            nrecords++;
        }
    }
    LogFile(flog,"ECG samples:"," ",nrecords,'i');
    seconds = nrecords/frequency;
    if (seconds < 60) {
        sprintf(s,"%d seconds",seconds);
    } else if (seconds < 3600) {
        minutes = seconds / 60;
        seconds = seconds % 60;
        sprintf(s,"%d minutes, %d seconds",minutes,seconds);
    } else {
        hours = seconds / 3600;
        minutes = (seconds - hours*3600) / 60;
        seconds = (seconds - hours*3600) % 60;
        sprintf(s,"%d hours, %d minutes, %d seconds",hours,minutes,seconds);
    }
    LogFile(flog,"Duration:",s,nrecords/frequency,'s');

    // Read to end of file
    while ((ic = fgetc(fin)) != EOF) {
        CleanPrint(ic,FALSE);
        nfooter++;
    }
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",nptr);
    if (verbose)
        fprintf(stderr,"%d trailing bytes\n",nfooter);
    fclose(fin);
    fclose(flog);

    return channels;
}


void CleanPrint(int ic,int mode)
{
    if (!debug)
        return;

    if (ic < 32 || ic > 126) {
        if (ic == 0) {
            if (mode)
                fprintf(stderr,"     ");
            else
                fprintf(stderr,"[   ]");
        } else {
            fprintf(stderr,"[%03d]",ic);
        }
    } else {
        if (!mode)
            fprintf(stderr,"  %c  ",ic);
        else
            fprintf(stderr,"%c",ic);
    }
}

void ReadString(FILE *fptr,char *s,int *istart,int len)
{
    int i,ic;

    for (i=0;i<len;i++) {
        ic = fgetc(fptr);
        if (ic < 32 || ic > 126)
            ic = ' ';
        s[i] = ic;
        (*istart)++;
    }
    s[len] = '\0';
}

void SkipTo(FILE *fptr,int *istart,int istop)
{
    int n,i,ic,linecount=0;
    unsigned short sc = 0;

    sc = 0;
    ic = 0;

    // Byte at a time
    n = *istart;
    for (i=n;i<istop;i++) {
        ic = fgetc(fptr);
        CleanPrint(ic,FALSE);
        (*istart)++;

        linecount++;
        if (linecount == 16 && debug) {
            fprintf(stderr,"\n");
            linecount = 0;
        }
    }
    /*
   // short int at a time
   n = *istart;
   for (i=n;i<istop;i+=2) {
      fread(&sc,2,1,fptr);
      if (debug)
         fprintf(stderr,"%6d ",sc);
      (*istart)+=2;

      linecount++;
      if (linecount == 16 && debug) {
         fprintf(stderr,"\n");
         linecount = 0;
      }
   }
*/
    if (debug)
        fprintf(stderr,"\n--- Offset %d ---\n",*istart);
}

void LogFile(FILE *fptr,char *s1,char *s2,int a,char datatype)
{
    if (s2[strlen(s2)-1] == '\n')
        s2[strlen(s2)-1] = '\0';
    if (datatype == 's') {
        fprintf(fptr,"%s %s\n",s1,s2);
        if (verbose)
            fprintf(stderr,"%s %s\n",s1,s2);
    } else {
        fprintf(fptr,"%s %d\n",s1,a);
        if (verbose)
            fprintf(stderr,"%s %d\n",s1,a);
    }
}


char *TrimWhitespaces(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}


int parsearc(const std::string& filename, const std::string& outputFilePath) {
    std::vector<CHANNEL> channels = ReadDataFromArc(filename);
    SaveDataIntoCsv(std::move(channels), outputFilePath);



    return 0;
}
