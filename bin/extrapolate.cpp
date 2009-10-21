
/*
 * This program is for testing different 
 * image extrapolation algorithms.
 */

#include "DenseVectorFieldIO.h"
#include "ImageExtrapolatorDriver.h"
#include "InverseDenseImageExtrapolator.h"
#include "SparseImageExtrapolator.h"
#include "SparseVectorFieldIO.h"

#include <argtable2.h>
#include "CImg_config.h"
#include <CImg.h>
#include <cstdlib>

using namespace cimg_library;

struct ArgData
{
  CImg< unsigned char > I0;
  CImg< double > *Vd;
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
  SparseVectorField *Vs;
#endif
  InverseDenseImageExtrapolator *denseExtrapolator;
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
  SparseImageExtrapolator *sparseExtrapolator;
#endif
  int numSteps;
  string outFileNamePrefix;
};

static bool parseArgs(int argc, char **argv, ArgData &result)
{
  int num_errors;
  bool success = false;
  
  struct arg_file *imagefilename       = arg_file1(NULL, NULL, "<image>",         "image to extrapolate");
  struct arg_file *motionfieldfilename = arg_file1(NULL, NULL, "<motionfield>",   "motion field file name");
#ifdef WITH_CGAL
  struct arg_rem *motionfieldrem1      = arg_rem(NULL,         "in PDVM or PSVM format");
#else
  struct arg_rem *motionfieldrem1      = arg_rem(NULL,         "in PDVM format");
#endif
  struct arg_rem *motionfieldrem2      = arg_rem(NULL,         "for dense motion fields (I2->I1)");
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
  struct arg_rem *motionfieldrem3      = arg_rem(NULL,         "for sparse motion fields (I1->I2)");
#endif
  //struct arg_str *imageextrapolator   = arg_str1(NULL, NULL,   "extrapolator",  "the name of the extrapolation algorithm");
  struct arg_int *numsteps             = arg_int1(NULL, NULL,  "<numtimesteps>",  "number of images to produce");
  struct arg_str *outfileprefix        = arg_str1(NULL, NULL,  "<outfileprefix>", "prefix for output file names");
  struct arg_end *end                  = arg_end(20);
  
  void *argtable[] = 
  {
    imagefilename,
    motionfieldfilename,
    motionfieldrem1,
    motionfieldrem2,
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
    motionfieldrem3,
#endif
    //imageextrapolator,
    numsteps,
    outfileprefix,
    end
  };
  
  if(arg_nullcheck(argtable) != 0)
  {
    printf("insufficient memory\n");
    success = false;
    goto exit;
  }
  
  result.Vd = NULL;
  result.denseExtrapolator = NULL;
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
  result.Vs = NULL;
  result.sparseExtrapolator = NULL;
#endif
  
  num_errors = arg_parse(argc, argv, argtable);
  
  if(num_errors == 0)
  {
    result.I0                  = CImg< unsigned char >(imagefilename->filename[0]);
    string motionFieldFileName = motionfieldfilename->filename[0];
    string extension           = motionFieldFileName.substr(motionFieldFileName.size() - 4, 4);
    if(extension == "pdvm")
    {
      result.Vd = new CImg< double >();
      DenseVectorFieldIO::readVectorField(motionFieldFileName, *result.Vd);
      result.denseExtrapolator = new InverseDenseImageExtrapolator();
    }
#ifdef WITH_CGAL
    else if(extension == "psvm")
    {
      result.Vs = new SparseVectorField();
      SparseVectorFieldIO::readVectorField(motionFieldFileName, *result.Vs);
      result.Vs->triangulate();
      result.sparseExtrapolator = new SparseImageExtrapolator();
    }
#endif
    else
    {
      printf("invalid motion field file format\n");
      success = false;
      goto exit;
    }
    result.numSteps          = numsteps->ival[0];
    result.outFileNamePrefix = outfileprefix->sval[0];
    
    success = true;
  }
  else
  {
    printf("Usage: extrapolate");
    arg_print_syntax(stdout, argtable, "\n");
    printf("This program demonstrates the use of extrapolation algorithms implemented in the OptFlow library.\n");
    arg_print_glossary(stdout, argtable, "  %-30s %s\n");
    
    success = false;
  }
  
  exit:
  if(success == false)
  {
    delete result.Vd;
    delete result.denseExtrapolator;
#ifdef WITH_CGAL
    delete result.Vs;
    delete result.sparseExtrapolator;
#endif
  }
  
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
  return success;
}

int main(int argc, char **argv)
{
  ArgData argData;
  CImg< unsigned char > Ie;
#ifdef WITH_CGAL
  SparseVectorField V;
#endif
  
  if(!parseArgs(argc, argv, argData))
    return EXIT_SUCCESS;
  
#ifdef WITH_CGAL
  if(argData.Vs != NULL)
    ImageExtrapolatorDriver::runSparseImageExtrapolator(*argData.sparseExtrapolator,
                                                        argData.I0,
                                                        *argData.Vs,
                                                        argData.numSteps,
                                                        argData.outFileNamePrefix);
  else
#endif
    ImageExtrapolatorDriver::runDenseImageExtrapolator(*argData.denseExtrapolator,
                                                       argData.I0,
                                                       *argData.Vd,
                                                       argData.numSteps,
                                                       argData.outFileNamePrefix);
  
  return EXIT_SUCCESS;
}
