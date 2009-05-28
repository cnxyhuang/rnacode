#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "rnaz_utils.h"
#include "treeML.h"
#include "cmdline.h"
#include "code.h"
#include "score.h"
#include "tree.h"
#include "treeSimulate.h"
#include "extreme_fit.h"
#include "postscript.h"
#include "misc.h"

void usage(void);
void help(void);
void version(void);

/* Global options */

double omega=-2.0;
double Omega=-4.0;
double Delta=-10.0;
double stopPenalty_0=-9999.0;
double stopPenalty_k=-8.0;

FILE *inputFile;
FILE *outputFile;
FILE *debugFile;
int sampleN=100;
char limit[10000]="";
double cutoff=1.0;
int outputFormat=0; /* 0: normal list; 1: GTF; 2:compact list (debugging) */
char debugFileName[1024]="";
char inputFileName[1024]="STDIN";
bgModel *models, *modelsRev;

/* For debugging purposes, set these variable to print out
   false-positive or false-negatives on test sets of noncoding or
   coding regions of known  annotation */

double printIfBelow=-1.0; 
double printIfAbove=-1.0;

void read_commandline(int argc, char *argv[]);

int main(int argc, char *argv[]){

  int i,j,k,L,N,hssCount;
  char *tmpSeq, *treeString;
  double kappa;
  TTree* tree;
  segmentStats *results;
  double parMu, parLambda;

  int (*readFunction)(FILE *clust,struct aln *alignedSeqs[]);

  struct aln *inputAln[MAX_NUM_NAMES];
  struct aln *inputAlnRev[MAX_NUM_NAMES];

  read_commandline(argc, argv);

  srand(time(NULL));

  ntMap['A']=ntMap['a']=0;
  ntMap['C']=ntMap['c']=1;
  ntMap['G']=ntMap['g']=2;
  ntMap['T']=ntMap['t']=3;
  ntMap['U']=ntMap['u']=3;

  switch(checkFormat(inputFile)){
  case CLUSTAL:
    readFunction=&read_clustal;
    break;
  case MAF:
    readFunction=&read_maf;
    break;
  case 0:
    nrerror("ERROR: Unknown alignment file format. Use Clustal W or MAF format.\n");
  }

  while (readFunction(inputFile, inputAln)!=0){
    /* Currently repeat masked regions are ignored and Ns are converted to gaps */
    /* Fix this */
    for (i=0; inputAln[i]!=NULL; i++){
      tmpSeq=inputAln[i]->seq;
      j=0;
      while (tmpSeq[j]){
        tmpSeq[j]=toupper(tmpSeq[j]);
        if (tmpSeq[j]=='N'){
          tmpSeq[j]='-';
        }
        j++;
      }
    }
    
    if (!strcmp(limit,"")==0){
      pruneAln(limit,(struct aln**)inputAln);
    }
    
    //printAlnMAF(stdout,(const struct aln**)inputAln,0); 
    
    //L=strlen(inputAln[0]->seq);
    //if (L<3){
    //continue;
    //}

    for (N=0; inputAln[N]!=NULL; N++);

    /* Currently minimum number of sequences is 3 because BIONJ seg-faults with 2 */
    /* Fix this that it works with two sequences*/

    if (N<=2){
      //fprintf(stderr,"There must be at least three sequences in the alignment.\n");
      continue;
      //exit(1);
    }

    if (treeML((const struct aln**)inputAln,&treeString,&kappa)==0){
      fprintf(stderr,"\nFailed to build ML tree.\n");
      continue; 
    }
    
    tree=string2tree(treeString);
    
    copyAln((struct aln**)inputAln,(struct aln**)inputAlnRev);
    revAln((struct aln**)inputAlnRev);
    
    models=getModels(tree,inputAln,kappa);
    modelsRev=getModels(tree,inputAlnRev,kappa);

    getExtremeValuePars(tree, (const struct aln**)inputAln, sampleN, &parMu, &parLambda);
    results=scoreAln((const struct aln**)inputAln, tree, kappa, parMu, parLambda);
    printResults(outputFile,outputFormat,results);

    freeResults(results);
    freeModels(models,N);
    freeModels(modelsRev,N);
         
  }
    
  //results=getHSS(models, (const struct aln**)inputAln, parMu, parLambda,cutoff);
  //hssCount=0;
  //while (results[hssCount].score>=0) hssCount++;
  //qsort((segmentStats*) results, hssCount,sizeof(segmentStats),compareScores);
  /*
    if (printIfAbove > -1.0){
    if (results[0].pvalue > printIfAbove){
    printAlnMAF(debugFile,(const struct aln**)inputAln,0);
    }
    }
    if (printIfBelow > -1.0){
      if (results[0].pvalue < printIfBelow){
        printAlnMAF(debugFile,(const struct aln**)inputAln,0);
      }
    }
    printResults(outputFile,outputFormat,results);
    if (args.gfx_given){
      colorAln("color.ps", (const struct aln**)inputAln, results[0]);
    }

  */

  
  if ((printIfAbove > -1.0) || (printIfBelow > -1.0)){
    fclose(debugFile);
  }
  fclose(inputFile);


  //freeResults(results);
  free(treeString);
  freeSeqgenTree(tree);
  freeAln((struct aln**)inputAln);
  exit(EXIT_SUCCESS);

}


void usage(void){
  help();
}

void help(void){

  cmdline_parser_print_version ();

  printf("\nUsage: %s [OPTIONS]... [FILES]\n\n", CMDLINE_PARSER_PACKAGE);
  printf("%s\n","  -h, --help                       Help screen");
  printf("%s\n\n","  -V, --version                    Print version");
}

void version(void){
  //printf("RNAcode v Wed Dec 10 14:53:47 2008" PACKAGE_VERSION "\n");
  printf("RNAcode v Thu May 28 16:31:18 2009\n");
  exit(EXIT_SUCCESS);
}


void test(const struct aln *inputAln[]){

  int N;
  char *treeString;
  double kappa;
  TTree* tree;
  double**** Sk;
  double** S;
  bgModel* models;
  
  for (N=0; inputAln[N]!=NULL; N++);   

  if (treeML((const struct aln**)inputAln,&treeString,&kappa)==0){
    fprintf(stderr,"\nFailed to build ML tree.\n");
    exit(1);
  }

  tree=string2tree(treeString);
  models=getModels(tree,inputAln,kappa);


  Sk=getPairwiseScoreMatrix(models,(const struct aln**)inputAln);
  S=getMultipleScoreMatrix(Sk,models,(const struct aln**)inputAln);

  //backtrack(S, 1, (const struct aln **)inputAln);
    
}

void read_commandline(int argc, char *argv[]){

  struct gengetopt_args_info args;

  inputFile=stdin;
  outputFile=stdout;
  debugFile=stdout;

  if (cmdline_parser (argc, argv, &args) != 0){
    usage();
    exit(EXIT_FAILURE);
  }
  
  if (args.inputs_num>=1){
    inputFile = fopen(args.inputs[0], "r"); 
    if (inputFile == NULL){
      fprintf(stderr, "ERROR: Can't open input file %s\n", args.inputs[0]);
      exit(1);
    }
    strcpy(inputFileName,args.inputs[0]);
  }
  
  if (args.outfile_given){
    outputFile = fopen(args.outfile_arg, "w");
    if (outputFile == NULL){
      fprintf(stderr, "ERROR: Can't open output file %s\n", args.outfile_arg);
      exit(1);
    }
  }

  if (args.num_samples_given){
    sampleN=args.num_samples_arg;
  }

  if (args.gtf_given){
    outputFormat=1;
  }

  if (args.concise_given){
    outputFormat=2;
  }

  if (args.cutoff_given){
    cutoff=args.cutoff_arg;
  }

  if (args.debug_file_given){
    strcpy(debugFileName,args.debug_file_arg);
  }

  if (args.print_if_below_given){
    printIfBelow=args.print_if_below_arg; 
    debugFile = fopen(debugFileName, "w");
  }

  if (args.print_if_above_given){
    printIfAbove=args.print_if_above_arg; 
    debugFile = fopen(debugFileName, "w");
  }

  if (args.limit_given){
    strcpy(limit,args.limit_arg);
  }

  if (args.help_given){
    help();
    exit(EXIT_SUCCESS);
  }

  if (args.version_given){
    version();
    exit(EXIT_SUCCESS);
  }

  cmdline_parser_free(&args);

}
