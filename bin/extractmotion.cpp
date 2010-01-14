
/*
 * This program is for testing different 
 * motion extraction algorithms.
 */

#include "DenseMotionExtractor.h"
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
#include "LucasKanadeOpenCV.h"
#endif
#include "MotionExtractorDriver.h"
#include "PyramidalLucasKanade.h"
#include "PyramidalProesmans.h"
#include "SparseImageExtrapolator.h"
#include "version.h"

//#include <argtable2.h>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace boost::program_options;
using namespace std;

/*struct ArgData
{
  string srcImgFileName1;
  string srcImgFileName2;
  DenseMotionExtractor *denseMotionExtractor;
#ifdef WITH_CGAL
  SparseMotionExtractor *sparseMotionExtractor;
#endif
  string outFileNamePrefix;
};

struct ArgList1
{
  struct arg_file *srcimg1;
  struct arg_file *srcimg2;
  struct arg_str  *motionextractor;
  struct arg_str  *outfileprefix;
  struct arg_int  *numpyramidlevels;
  struct arg_int  *windowradius;
  struct arg_int  *numiterations;
  struct arg_dbl  *tau;
  struct arg_dbl  *sigmap;
  struct arg_end  *end;
};

struct ArgList2
{
  struct arg_file *srcimg1;
  struct arg_file *srcimg2;
  struct arg_str  *motionextractor;
  struct arg_str  *outfileprefix;
  struct arg_int  *numpyramidlevels;
  struct arg_int  *numiterations;
  struct arg_dbl  *lambda;
  struct arg_int  *boundcond;
  struct arg_end  *end;
};

struct ArgList3
{
  struct arg_file *srcimg1;
  struct arg_file *srcimg2;
  struct arg_str  *motionextractor;
  struct arg_str  *outfileprefix;
  struct arg_int  *numpyramidlevels;
  struct arg_int  *windowsize;
  struct arg_int  *maxnumfeaturepoints;
  struct arg_dbl  *minfeaturepointdist;
  struct arg_dbl  *qualitylevel;
  struct arg_int  *stopcritmaxnumiter;
  struct arg_dbl  *stopcritepsilon;
  struct arg_end  *end;
};

static bool parseArgs(int argc, char **argv, ArgData &result)
{
  int num_errors;
  bool success = false;
  
  ArgList1 arglist1;
  ArgList2 arglist2;
  ArgList3 arglist3;
  
  arglist1.srcimg1          = arg_file1(NULL, NULL,           "<image1>",        "first source image");
  arglist1.srcimg2          = arg_file1(NULL, NULL,           "<image2>",        "second source image");
  arglist1.motionextractor  = arg_str1(NULL,  NULL,           "lucaskanade",     NULL);
  arglist1.outfileprefix    = arg_str1(NULL,  NULL,           "<outfileprefix>", "prefix for output file names");
  arglist1.numpyramidlevels = arg_int0(NULL,  "numlevels",    "<int>",           "number of image pyramid levels");
  arglist1.windowradius     = arg_int0(NULL,  "windowradius", "<int>",           "feature extraction window radius");
  arglist1.numiterations    = arg_int0(NULL,  "numiter",      "<int>",           "number of iterations");
  arglist1.tau              = arg_dbl0(NULL,  "tau",          "<double>",        "eigenvalue threshold");
  arglist1.sigmap           = arg_dbl0(NULL,  "sigmap",       "<double>",        "regularization parameter");
  arglist1.end              = arg_end(20);
  
  arglist2.srcimg1          = arg_file1(NULL, NULL,        "<image1>",        NULL);
  arglist2.srcimg2          = arg_file1(NULL, NULL,        "<image2>",        NULL);
  arglist2.motionextractor  = arg_str1(NULL,  NULL,        "proesmans",       NULL);
  arglist2.outfileprefix    = arg_str1(NULL,  NULL,        "<outfileprefix>", NULL);
  arglist2.numpyramidlevels = arg_int0(NULL,  "numlevels", "<int>",           NULL);
  arglist2.numiterations    = arg_int0(NULL,  "numiter",   "<int>",           "number of iterations");
  arglist2.lambda           = arg_dbl0(NULL,  "lambda",    "<double>",        "smoothness parameter");
  arglist2.boundcond        = arg_int0(NULL,  "boundcond", "<int>",           "1 = Neumann, 2 = Dirichlet");
  arglist2.end              = arg_end(20);
  
  arglist3.srcimg1             = arg_file1(NULL, NULL,           "<image1>",          NULL);
  arglist3.srcimg2             = arg_file1(NULL, NULL,           "<image2>",          NULL);
  arglist3.motionextractor     = arg_str1(NULL,  NULL,           "lucaskanadeopencv", NULL);
  arglist3.outfileprefix       = arg_str1(NULL,  NULL,           "<outfileprefix>",   NULL);
  arglist3.numpyramidlevels    = arg_int0(NULL,  "numlevels",    "<int>",             NULL);
  arglist3.windowsize          = arg_int0(NULL,  "windowsize",   "<int>",             "window size");
  arglist3.maxnumfeaturepoints = arg_int0(NULL,  "maxnumpoints", "<int>",             "maximum number of feature points");
  arglist3.minfeaturepointdist = arg_dbl0(NULL,  "minpointdist", "<double>",          "minimum feature point distance");
  arglist3.qualitylevel        = arg_dbl0(NULL,  "qualitylevel", "<double>",          "feature point quality threshold");
  arglist3.stopcritmaxnumiter  = arg_int0(NULL,  "maxnumiter",   "<int>",             "maximum number of iterations");
  arglist3.stopcritepsilon     = arg_dbl0(NULL,  "epsilon",      "<double>",          "stopping criterion threshold");
  arglist3.end                 = arg_end(20);
  
  void *argtable1[] = 
  {
    arglist1.srcimg1,
    arg_rem(NULL, "(8-bit grayscale image, width and height "),
    arg_rem(NULL, "are powers of two)"),
    arglist1.srcimg2,
    arg_rem(NULL, "(8-bit grayscale image, width and height "),
    arg_rem(NULL, "are powers of two)"),
    arglist1.motionextractor,
    arglist1.outfileprefix,
    arglist1.numpyramidlevels,
    arglist1.windowradius,
    arglist1.numiterations,
    arglist1.tau,
    arglist1.sigmap,
    arglist1.end
  };
  
  void *argtable2[] =
  {
    arglist2.srcimg1,
    arglist2.srcimg2,
    arglist2.motionextractor,
    arglist2.outfileprefix,
    arglist2.numpyramidlevels,
    arglist2.numiterations,
    arglist2.lambda,
    arglist2.boundcond,
    arglist2.end
  };
  
  void *argtable3[] =
  {
    arglist3.srcimg1,
    arglist3.srcimg2,
    arglist3.motionextractor,
    arglist3.outfileprefix,
    arglist3.numpyramidlevels,
    arglist3.windowsize,
    arglist3.maxnumfeaturepoints,
    arglist3.minfeaturepointdist,
    arglist3.qualitylevel,
    arglist3.stopcritmaxnumiter,
    arglist3.stopcritepsilon,
    arglist3.end
  };
  
  if(arg_nullcheck(argtable1) != 0 || 
     arg_nullcheck(argtable3) != 0 || 
     arg_nullcheck(argtable3) != 0)
  {
    printf("insufficient memory\n");
    success = false;
    goto exit;
  }
  
  result.denseMotionExtractor = NULL;
#ifdef WITH_CGAL
  result.sparseMotionExtractor = NULL;
#endif
  
  num_errors = arg_parse(argc, argv, argtable1);
  if(num_errors == 0 && 
     strcmp(arglist1.motionextractor->sval[0], "lucaskanade") == 0)
  {
    result.srcImgFileName1 = arglist1.srcimg1->filename[0];
    result.srcImgFileName2 = arglist1.srcimg2->filename[0];
    result.outFileNamePrefix = arglist1.outfileprefix->sval[0];
    //result.extrapolationMethod = arg_extrapolationmethod_1->sval[0];
    
    result.denseMotionExtractor = new PyramidalLucasKanade(
      arglist1.windowradius->count > 0     ? arglist1.windowradius->ival[0] : 16,
      arglist1.numiterations->count > 0    ? arglist1.numiterations->ival[0] : 5,
      arglist1.tau->count > 0              ? arglist1.tau->dval[0] : 0.0025,
      arglist1.sigmap->count > 0           ? arglist1.sigmap->dval[0] : 0.0,
      arglist1.numpyramidlevels->count > 0 ? arglist1.numpyramidlevels->ival[0] : 4,
      true);
    
    success = true;
    goto exit;
  }
  else
    num_errors = arg_parse(argc, argv, argtable2);
	
  if(num_errors == 0 && strcmp(arglist2.motionextractor->sval[0], "proesmans") == 0)
  {
    result.srcImgFileName1   = arglist2.srcimg1->filename[0];
    result.srcImgFileName2   = arglist2.srcimg2->filename[0];
    result.outFileNamePrefix = arglist2.outfileprefix->sval[0];
    //result.extrapolationMethod = arg_extrapolationmethod_2->sval[0];
    
    int boundCondCode;
    
    if(arglist2.boundcond->count > 0)
      boundCondCode = arglist2.boundcond->ival[0];
    else
      boundCondCode = 1;
    
    Proesmans::BoundaryConditions boundCond;
    if(boundCondCode == 1)
      boundCond = Proesmans::NEUMANN;
    else if(boundCondCode == 2)
      boundCond = Proesmans::DIRICHLET;
    else
    {
      printf("invalid boundary condition type\n");
      success = false;
      goto exit;
    }
    
    result.denseMotionExtractor = new PyramidalProesmans(
      arglist2.numiterations->count > 0    ? arglist2.numiterations->ival[0] : 200,
      arglist2.lambda->count > 0           ? arglist2.lambda->dval[0] : 100.0,
      arglist2.numpyramidlevels->count > 0 ? arglist2.numpyramidlevels->ival[0] : 4,
      boundCond);
    
    success = true;
    goto exit;
  }
  else
    num_errors = arg_parse(argc, argv, argtable3);
  
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
  if(num_errors == 0 && strcmp(arglist3.motionextractor->sval[0], "lucaskanadeopencv") == 0)
  {
    result.srcImgFileName1   = arglist3.srcimg1->filename[0];
    result.srcImgFileName2   = arglist3.srcimg2->filename[0];
    result.outFileNamePrefix = arglist3.outfileprefix->sval[0];
    
    result.sparseMotionExtractor = new LucasKanadeOpenCV(
      arglist3.numpyramidlevels->count > 0    ? arglist3.numpyramidlevels->ival[0] : 4,
      arglist3.windowsize->count > 0          ? arglist3.windowsize->ival[0] : 40,
      arglist3.maxnumfeaturepoints->count > 0 ? arglist3.maxnumfeaturepoints->ival[0] : 1000,
      arglist3.minfeaturepointdist->count > 0 ? arglist3.minfeaturepointdist->dval[0] : 10,
      arglist3.qualitylevel->count > 0        ? arglist3.qualitylevel->dval[0] : 0.025,
      arglist3.stopcritmaxnumiter->count > 0  ? arglist3.stopcritmaxnumiter->ival[0] : 10,
      arglist3.stopcritepsilon->count > 0     ? arglist3.stopcritepsilon->dval[0] : 0.001);
    
    success = true;
    goto exit;
  }
  else
  {
#endif
    printf("Usage: extractmotion");
    arg_print_syntax(stdout, argtable1, "\n");
    printf("       extractmotion");
    arg_print_syntax(stdout, argtable2, "\n");
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
    printf("       extractmotion");
    arg_print_syntax(stdout, argtable3, "\n");
#endif
    printf("This program demonstrates the use of motion extraction algorithms implemented in the OptFlow library.\n");
    arg_print_glossary(stdout, argtable1, "  %-30s %s\n");
    arg_print_glossary(stdout, argtable2, "  %-30s %s\n");
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
    arg_print_glossary(stdout, argtable3, "  %-30s %s\n");
#endif
    success = false;
    goto exit;
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
  }
#endif

  exit:
  /*if(result.extrapolationMethod == "triangulation")
    result.extrapolator = new SparseImageExtrapolator();
  else
  {
    printf("invalid extrapolation method\n");
    success = false;
  }*/
  /*if(success == false)
  {
    delete result.denseMotionExtractor;
#ifdef WITH_CGAL
    delete result.sparseMotionExtractor;
#endif
  }
  
  arg_freetable(argtable1, sizeof(argtable1) / sizeof(argtable1[0]));
  arg_freetable(argtable2, sizeof(argtable2) / sizeof(argtable2[0]));
  arg_freetable(argtable3, sizeof(argtable3) / sizeof(argtable3[0]));
	
  return success;
}

/*int main(int argc, char **argv)
{
  ArgData argData;
  
  if(!parseArgs(argc, argv, argData))
    return EXIT_SUCCESS;
	
  if(argData.denseMotionExtractor != NULL)
    MotionExtractorDriver::runDenseMotionExtractor(*argData.denseMotionExtractor,
                                                   argData.srcImgFileName1,
                                                   argData.srcImgFileName2,
                                                   argData.outFileNamePrefix);
#ifdef WITH_CGAL
  else if(argData.sparseMotionExtractor != NULL)
    MotionExtractorDriver::runSparseMotionExtractor(*argData.sparseMotionExtractor,
                                                    argData.srcImgFileName1,
                                                    argData.srcImgFileName2,
                                                    argData.outFileNamePrefix);
#endif
  
  return EXIT_SUCCESS;
}*/

int main(int argc, char **argv)
{
  DenseMotionExtractor *denseMotionExtractor = NULL;
#ifdef WITH_CGAL
  SparseMotionExtractor *sparseMotionExtractor = NULL;
#endif
  
  options_description generalArgs("General options");
  generalArgs.add_options()
    ("help", "print usage")
    ("options", value< std::string >(), "print options specific to the algorithm <arg>")
    ("version", "print version number");
  
  options_description reqArgs("Required arguments");
  reqArgs.add_options()
    ("image1", value< std::string >(), "first image")
    ("image2", value< std::string >(), "second image")
    ("algorithm", value< std::string >(), "motion detection algorithm (lucaskanade, opencv, proesmans)")
    ("outprefix", value< std::string >(), "output file prefix");
  
  // options specific to the Lucas-Kanade algorithm
  options_description lucasKanadeArgs("Options for the Lucas-Kanade algorithm");
  lucasKanadeArgs.add_options()
    ("numlevels",    value< int >(),   "number of pyramid levels (default = 4)")
    ("windowradius", value< int >(),   "feature matching window radius (default = 16)")
    ("numiter",      value< int >(),   "number of iterations (default = 5)")
    ("tau",          value< float >(), "eigenvalue threshold for feature matching (default = 0.0025)")
    ("sigmap",       value< float >(), "regularization parameter (default = 0)");
  
  // options specific to the Lucas-Kanade algorithm (OpenCV)
  options_description opencvArgs("Options for the Lucas-Kanade algorithm (OpenCV)");
  opencvArgs.add_options()
    ("numlevels",    value< int >(), "number of pyramid levels (default = 4)")
    ("windowsize",   value< int >(), "feature matching window size (default = 40)")
    ("maxnumpoints", value< int >(), "maximum number of feature points (default = 1000)")
    ("minpointdist", value< int >(), "minimum distance between feature points (default = 10)")
    ("qualitylevel", value< int >(), "feature point quality threshold (default = 0.025)")
    ("maxnumiter",   value< int >(), "maximum number of iterations (default = 10)")
    ("epsilon",      value< int >(), "stopping criterion threshold (default = 0.001)");
  
  // options specific to the Proesmans algorithm
  options_description proesmansArgs("Options for the Proesmans algorithm");
  proesmansArgs.add_options()
    ("numlevels", value< int >(),   "number of pyramid levels (default = 4)")
    ("numiter",   value< int >(),   "number of iterations (default = 200)")
    ("lambda",    value< float >(), "smoothness parameter (default = 100)")
    ("boundcond", value< int >(),   "boundary conditions (0 = Dirichlet, 1 = Neumann)  (default = 1)");
  
  std::string restrictions = "Restrictions:\n -the source images must be 8-bit grayscale images.\n -the dimensions of the source images must be powers of 2";
  
  options_description allArgs("");
  allArgs.add(generalArgs).add(reqArgs).add(lucasKanadeArgs).add(proesmansArgs).add(opencvArgs);
  options_description allVisibleArgs("Usage: extractmotion <required arguments> [algorithm-specific options]");
  allVisibleArgs.add(generalArgs).add(reqArgs);
  
  try {
    variables_map vm;
    store(parse_command_line(argc, argv, allArgs), vm);
    notify(vm);
    
    if(vm.size() == 0 || vm.count("help"))
    {
      std::cout<<allVisibleArgs<<std::endl;
      std::cout<<restrictions<<std::endl;
      return EXIT_SUCCESS;
    }
    else if(vm.count("version"))
      std::cout<<OPTFLOW_VERSION_INFO<<std::endl;
    else if(vm.count("options") && vm["options"].as< string >() == "lucaskanade")
    {
      std::cout<<lucasKanadeArgs<<std::endl;
      return EXIT_SUCCESS;
    }
    else if(vm.count("options") && vm["options"].as< string >() == "opencv")
    {
      std::cout<<opencvArgs<<std::endl;
      return EXIT_SUCCESS;
    }
    else if(vm.count("options") && vm["options"].as< string >() == "proesmans")
    {
      std::cout<<proesmansArgs<<std::endl;
      return EXIT_SUCCESS;
    }
    else if(vm.count("options"))
    {
      std::cout<<"Invalid algorithm name."<<std::endl;
      return EXIT_SUCCESS;
    }
    else if(!vm.count("image1") || !vm.count("image2") || !vm.count("algorithm"))
    {
      std::cout<<"One or more required arguments missing."<<std::endl;
      std::cout<<reqArgs<<std::endl;
    }
    
    if(vm["algorithm"].as< string >() == "lucaskanade")
    {
      denseMotionExtractor = new PyramidalLucasKanade(
        vm.count("windowradius") > 0 ? vm["windowradius"].as< int >() : 16,
        vm.count("numiter") > 0      ? vm["numiter"].as< int >() : 5,
        vm.count("tau") > 0          ? vm["tau"].as< float >() : 0.0025,
        vm.count("sigmap") > 0       ? vm["sigmap"].as< float >() : 0.0,
        vm.count("numlevels") > 0    ? vm["numlevels"].as< int >() : 4,
        true);
    }
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
    else if(vm["algorithm"].as< string >() == "opencv")
    {
      sparseMotionExtractor = new LucasKanadeOpenCV(
        vm.count("numlevels") > 0    ? vm["numlevels"].as< int >() : 4,
        vm.count("windowsize") > 0   ? vm["windowsize"].as< int >() : 40,
        vm.count("maxnumpoints") > 0 ? vm["maxnumpoints"].as< int >() : 1000,
        vm.count("minpointdist") > 0 ? vm["minpointdist"].as< float >() : 10,
        vm.count("qualitylevel") > 0 ? vm["qualitylevel"].as< float >() : 0.025,
        vm.count("maxnumiter") > 0   ? vm["maxnumiter"].as< int >() : 10,
        vm.count("epsilon") > 0      ? vm["epsilon"].as< float >() : 0.001);
    }
#endif
    else if(vm["algorithm"].as< string >() == "proesmans")
    {
      Proesmans::BoundaryConditions boundCond;
      
      int bc = 1;
      if(vm.count("boundcond") > 0)
        bc = vm["boundcond"].as< int >();
      if(bc == 0)
        boundCond = Proesmans::DIRICHLET;
      else
        boundCond = Proesmans::NEUMANN;
      
      denseMotionExtractor = new PyramidalProesmans(
        vm.count("numiter") > 0   ? vm["numiter"].as< int >() : 200,
        vm.count("lamda") > 0     ? vm["lambda"].as< float >() : 100.0,
        vm.count("numlevels") > 0 ? vm["numlevels"].as< int >() : 4,
        boundCond);
    }
    else
    {
      std::cout<<"Invalid algorithm name."<<std::endl;
      return EXIT_SUCCESS;
    }
  
    std::string srcImgFileName1 = vm["image1"].as< string >();
    std::string srcImgFileName2 = vm["image2"].as< string >();
    std::string outFilePrefix   = vm["outprefix"].as< string >();
    
    if(denseMotionExtractor != NULL)
    {
      MotionExtractorDriver::runDenseMotionExtractor(
        *denseMotionExtractor, srcImgFileName1, srcImgFileName2, outFilePrefix);
      delete denseMotionExtractor;
    }
#ifdef WITH_CGAL
    else if(sparseMotionExtractor != NULL)
    {
      MotionExtractorDriver::runSparseMotionExtractor(
        *sparseMotionExtractor, srcImgFileName1, srcImgFileName2, outFilePrefix);
      delete sparseMotionExtractor;
    }
#endif
  }
  catch(CImgIOException &e1) {
    std::cout<<"Invalid source image(s)."<<std::endl;
  }
  catch(std::exception &e2) {
    std::cout<<allVisibleArgs<<std::endl;
    std::cout<<restrictions<<std::endl;
  }
  
  return EXIT_SUCCESS;
}
