#include <DGtal/base/Common.h>

#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/io/readers/ITKReader.h"
#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "DGtal/io/writers/GenericWriter.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "segmentdetector.h"
#include "imagedata.h"
#include "DGtal/io/boards/Board2D.h"


using namespace std;
using namespace DGtal;
using namespace Z3i;


namespace po = boost::program_options;


int main(int argc, char** argv)
{
  typedef DGtal::ImageContainerBySTLVector<DGtal::Z2i::Domain, unsigned int> Image2D;


  po::options_description general_opt("Allowed options are: ");
  general_opt.add_options()
    ("help,h", "display this message")
    ("input-file,i", po::value<std::string>(), "Input image file " )
    ("nameBGImage,n", po::value<std::string>(), "Name of bg image " )
    ("width, w",  po::value<unsigned int>(), "width of the segment " )
    ("stepXY, s",  po::value<unsigned int>(), "set the detection step defined on the x/y axis. (default 25) " )
    ("initLength, l",  po::value<unsigned int>(), "set the length of directional line (to start the recognition) (default 20) " )
    ("minSegSize, m",  po::value<unsigned int>(), "set the minimal of point of a segment in order to be considered  (default 10 ) " )
    ("acceptedLack,a",  po::value<unsigned int>(), "set the accepted lack in the segment recognition (i.e the number of consecutive outliers) (default 5) " ) 
    ("output-file,o", po::value<std::string>(), "set output basename (default output)" ) ;

  bool parseOK=true;
  po::variables_map vm;
  try{
    po::store(po::parse_command_line(argc, argv, general_opt), vm);
  }catch(const std::exception& ex){
    parseOK=false;
    trace.info()<< "Error checking program options: "<< ex.what()<< endl;
  }
  po::notify(vm);
  if( !parseOK || vm.count("help")||argc<=1)
    {
      std::cout << "Usage: " << argv[0] << " [input-file]\n"
		<< "Detect line according the liveSegment algorithm applied on all the image. "
		<< general_opt << "\n" 
                << "Typical use: \n"
                << " ";
      return 0;
    }
  
  if(! vm.count("input-file"))
    {
      trace.error() << " No input file was given" << endl;
      return 0;
    }
  string inputFilename;
  string outputFilename;
  string bgFilename;

  if( vm.count("input-file")){
    inputFilename = vm["input-file"].as<std::string>();
  }
  if( vm.count("output-file")){
    outputFilename = vm["output-file"].as<std::string>();
  }
  if( vm.count("nameBGImage")){
    bgFilename = vm["nameBGImage"].as<std::string>();
  }

  
  

  Image2D sourceImage =  DGtal::GenericReader<Image2D>::import(inputFilename);
  unsigned int width = sourceImage.domain().upperBound()[0];
  unsigned int height = sourceImage.domain().upperBound()[1];
  
  // Getting line parameter:
  unsigned int STEP_XY= 25; 
  unsigned int WIDTH_XY= 20; 
  unsigned int WIDTH_SEG= 5; 
  unsigned int MIN_SEG_SIZE = 10;
  unsigned int ACCEPTED_LACK = 5;
  if(vm.count("stepXY")){
    STEP_XY = vm["stepXY"].as<unsigned int>();
  }
  if(vm.count("initLength")){
    WIDTH_XY = vm["initLength"].as<unsigned int>();
  }
  if(vm.count("minSegSize")){
    MIN_SEG_SIZE = vm["minSegSize"].as<unsigned int>();
  }
  if(vm.count("width")){
    WIDTH_SEG = vm["width"].as<unsigned int>();
  }
  if(vm.count("acceptedLack")){
    ACCEPTED_LACK = vm["acceptedLack"].as<unsigned int>();
  }
  
  
  trace.info() << " " << width << " "  << height << std::endl;
  int  ** tabImage = new int * [height];
  unsigned int i =  0;
  for ( unsigned int i =0; i < height; i++)
    {
      tabImage[i] = new int [width];
      for ( unsigned int j =0; j < width; j++){
        tabImage[i][j] = sourceImage(Z2i::Point(j,i));
      }
    }
  SegmentDetector segDect;
  ImageData imageDat (width, height);
  imageDat.buildGradientMap (tabImage);
  segDect.setImageData (&imageDat);
  segDect.setSegmentMaxWidth(WIDTH_SEG);
  segDect.setAcceptedLacks(ACCEPTED_LACK);
  vector< vector<Pixel> >  vectAllSS;


  for(unsigned py=0 ; py < height- STEP_XY; py+=STEP_XY){ 
    for(unsigned px=0 ; px < width- STEP_XY; px+=STEP_XY){ 
      trace.info() << "Step x=" << px << "Step y=" << py << std::endl;  
      Pixel p1(px-WIDTH_XY, py-WIDTH_XY);
      Pixel p2(px+WIDTH_XY, py+WIDTH_XY);
      BlurredSegment *bs1 = segDect.detect (p1, p2);
      if(bs1 != NULL && bs1->getListePixel().size()>MIN_SEG_SIZE){
        vectAllSS.push_back( bs1->getPrintedPoints ());
      }
      Pixel p3(px-WIDTH_XY, py+WIDTH_XY);
      Pixel p4(px+WIDTH_XY, py-WIDTH_XY); 
      BlurredSegment *bs2 = segDect.detect (p3, p4);
      if(bs2 != NULL&& bs2->getListePixel().size()>MIN_SEG_SIZE){
        vectAllSS.push_back( bs2->getPrintedPoints ());
      }
    }
  }
        
  Board2D aBoard;  
  double scale = 1.0;
  aBoard.setUnit (0.05*scale, LibBoard::Board::UCentimeter);
  Z2i::Point ptInf = sourceImage.domain().lowerBound(); 
  Z2i::Point ptSup = sourceImage.domain().upperBound(); 
  unsigned int widthI = abs(ptSup[0]-ptInf[0]+1);
  unsigned int heightI = abs(ptSup[1]-ptInf[1]+1);    
  aBoard.drawImage(bgFilename, 0-0.5,heightI-0.5, widthI, heightI);



  trace.info() << "NB segments detected=" <<  vectAllSS.size() <<  std::endl; 
  for (vector<vector<Pixel> >::iterator itAll = vectAllSS.begin(); itAll != vectAllSS.end(); itAll++){
    for (vector<Pixel>::iterator it = (*itAll).begin(); it != (*itAll).end(); it++){
      aBoard<< CustomStyle( Z2i::Point().className(), 
                            new CustomColors(  DGtal::Color::Red,  DGtal::Color::Red ) );        
      aBoard << Z2i::Point((*it).x(), (*it).y());
    } 
  }

  aBoard.saveFIG("imageResult.fig");
   
    
  
}
