
#include "DenseImageMorpher.h"
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
#include "LucasKanadeOpenCV.h"
#endif
#include "PyramidalProesmans.h"
#include "SparseImageMorpher.h"
#include "SparseVectorField.h"
#include "VectorFieldIllustrator.h"
#include "version.h"

#include <boost/program_options.hpp>
#include "CImg_config.h"
#include <CImg.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace boost::program_options;
using namespace cimg_library;
using namespace std;

static void saveMorphImage(const CImg< unsigned char > &M, int i,
                           const string &outFilePrefix)
{
  ostringstream ostr;
  ostr<<outFilePrefix + "-morph-"<<setfill('0')<<setw(2)<<i<<".png";
  M.save_png(ostr.str().c_str());
}

static void saveMotionImages(const CImg< unsigned char > &motionImage1,
                             const CImg< unsigned char > &motionImage2,
                             const string &outFilePrefix)
{
  motionImage1.save_png((outFilePrefix + "-motion-1.png").c_str());
  motionImage2.save_png((outFilePrefix + "-motion-2.png").c_str());
}

int main(int argc, char **argv)
{
  options_description generalArgs("General options");
  generalArgs.add_options()
    ("help", "print usage")
    ("version", "print version number");
  
  options_description reqArgs("Required arguments");
  reqArgs.add_options()
    ("image1", value< std::string >(), "first source image")
    ("image2", value< std::string >(), "second source image")
    ("numtimesteps", value< int >(), "number of intermediate images")
    ("algorithm", value< std::string >(), "motion extraction algorithm to use (opencv, proesmans)")
    ("outprefix", value< std::string >(), "output file prefix");
  
  options_description allArgs("Usage: morph <required arguments>");
  allArgs.add(generalArgs).add(reqArgs);
  
  try {
    variables_map vm;
    store(parse_command_line(argc, argv, allArgs), vm);
    notify(vm);
    
    if(vm.size() == 0 || vm.count("help"))
    {
      std::cout<<allArgs<<std::endl;
      return EXIT_SUCCESS;
    }
    else if(vm.count("version"))
      std::cout<<OPTFLOW_VERSION_INFO<<std::endl;
    else if(!vm.count("image1") || !vm.count("image2") || !vm.count("numtimesteps") || 
            !vm.count("algorithm") || !vm.count("outprefix"))
    {
      std::cout<<"One or more required arguments missing."<<std::endl;
      std::cout<<reqArgs<<std::endl;
    }
    
    std::string image1FileName = vm["image1"].as< std::string >();
    std::string image2FileName = vm["image2"].as< std::string >();
    int numTimeSteps           = vm["numtimesteps"].as< int >();
    std::string algorithmName  = vm["algorithm"].as< std::string >();
    std::string outFilePrefix  = vm["outprefix"].as< std::string >();
    
    CImg< unsigned char > I1(image1FileName.c_str());
    CImg< unsigned char > I2(image2FileName.c_str());
    
    CImg< unsigned char > I1_smoothed, I2_smoothed;
  
    I1 = I1.get_channel(0);
    I1_smoothed = I1.get_blur(3.0, 3.0, 3.0);
    I2 = I2.get_channel(0);
    I2_smoothed = I2.get_blur(3.0, 3.0, 3.0);
    
    const int W = I1.dimx();
    const int H = I1.dimy();
    
    CImg< unsigned char > motionImage1(W, H, 1, 3);
    motionImage1.fill(0);
    CImg< unsigned char > motionImage2(W, H, 1, 3);
    motionImage2.fill(0);
    
#if defined (WITH_OPENCV) && defined(WITH_CGAL)
    if(algorithmName == "opencv")
    {
      SparseVectorField V1, V2;
      CImg< unsigned char > M;
      LucasKanadeOpenCV motionExtractor;
      SparseImageMorpher morpher;
      ostringstream ostr;
    
      motionExtractor.compute(I1_smoothed, I2_smoothed, V1);
      V1.triangulate();
      motionExtractor.compute(I2_smoothed, I1_smoothed, V2);
      V2.triangulate();
    
      VectorFieldIllustrator::renderSparseVectorField(V1, motionImage1);
      VectorFieldIllustrator::renderSparseVectorField(V2, motionImage2);
      saveMotionImages(motionImage1, motionImage2, outFilePrefix);
    
      double dt = 1.0 / (numTimeSteps - 1);
      int i = 1;
      for(double t = 0; t <= 1.0; t += dt)
      {
        morpher.morph(I1, I2, V1, V2, t, M);
        saveMorphImage(M, i, outFilePrefix);
      
        i++;
      }
    }
    else
#endif
    if(algorithmName == "proesmans")
    {
      CImg< double > V1, V2;
      CImg< unsigned char > M;
      PyramidalProesmans motionExtractor;
      DenseImageMorpher morpher;
      ostringstream ostr;
    
      motionExtractor.compute(I1_smoothed, I2_smoothed, V1, V2);
    
      VectorFieldIllustrator::renderDenseVectorField(V1, motionImage1, 15);
      VectorFieldIllustrator::renderDenseVectorField(V2, motionImage2, 15);
      saveMotionImages(motionImage1, motionImage2, outFilePrefix);
    
      double dt = 1.0 / (numTimeSteps - 1);
      int i = 1;
      for(double t = 0; t <= 1.0; t += dt)
      {
        morpher.morph(I1, I2, V2, V1, t, M);
        saveMorphImage(M, i, outFilePrefix);
      
        i++;
      }
    }
    else
    {
      std::cout<<"Invalid algorithm name."<<std::endl;
      return EXIT_SUCCESS;
    }
  }
  catch(CImgIOException &e1) {
    std::cout<<"Invalid source image(s)."<<std::endl;
  }
  catch(std::exception &e2) {
    std::cout<<allArgs<<std::endl;
  }
  
  return EXIT_SUCCESS;
}
