/* cmdline.h */

/* File autogenerated by gengetopt version 2.19.1  */

#ifndef CMDLINE_H
#define CMDLINE_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
#define CMDLINE_PARSER_PACKAGE PACKAGE
#endif

#ifndef CMDLINE_PARSER_VERSION
#define CMDLINE_PARSER_VERSION VERSION
#endif

struct gengetopt_args_info
{
  const char *help_help; /* Print help and exit help description.  */
  const char *version_help; /* Print version and exit help description.  */
  char * outfile_arg;	/* Output filename.  */
  char * outfile_orig;	/* Output filename original value given at command line.  */
  const char *outfile_help; /* Output filename help description.  */
  int num_samples_arg;	/* Number of samples.  */
  char * num_samples_orig;	/* Number of samples original value given at command line.  */
  const char *num_samples_help; /* Number of samples help description.  */
  int verbose_flag;	/* verbose (default=off).  */
  const char *verbose_help; /* verbose help description.  */
  float cutoff_arg;	/* e-value cutoff.  */
  char * cutoff_orig;	/* e-value cutoff original value given at command line.  */
  const char *cutoff_help; /* e-value cutoff help description.  */
  int gtf_flag;	/* GTF output (default=off).  */
  const char *gtf_help; /* GTF output help description.  */
  int fast_sampling_flag;	/* Fast sampling (default=off).  */
  const char *fast_sampling_help; /* Fast sampling help description.  */
  
  int help_given ;	/* Whether help was given.  */
  int version_given ;	/* Whether version was given.  */
  int outfile_given ;	/* Whether outfile was given.  */
  int num_samples_given ;	/* Whether num-samples was given.  */
  int verbose_given ;	/* Whether verbose was given.  */
  int cutoff_given ;	/* Whether cutoff was given.  */
  int gtf_given ;	/* Whether gtf was given.  */
  int fast_sampling_given ;	/* Whether fast-sampling was given.  */

  char **inputs ; /* unamed options */
  unsigned inputs_num ; /* unamed options number */
} ;

extern const char *gengetopt_args_info_purpose;
extern const char *gengetopt_args_info_usage;
extern const char *gengetopt_args_info_help[];

int cmdline_parser (int argc, char * const *argv,
  struct gengetopt_args_info *args_info);
int cmdline_parser2 (int argc, char * const *argv,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);
int cmdline_parser_file_save(const char *filename,
  struct gengetopt_args_info *args_info);

void cmdline_parser_print_help(void);
void cmdline_parser_print_version(void);

void cmdline_parser_init (struct gengetopt_args_info *args_info);
void cmdline_parser_free (struct gengetopt_args_info *args_info);

int cmdline_parser_required (struct gengetopt_args_info *args_info,
  const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CMDLINE_H */