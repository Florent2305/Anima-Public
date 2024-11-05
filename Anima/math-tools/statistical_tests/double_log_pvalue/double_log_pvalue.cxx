#include <tclap/CmdLine.h>
#include <iostream>
#include <string>

#include <itkImage.h>
#include <itkImageRegionIterator.h>

#include <animaReadWriteFunctions.h>

int main(int argc, char **argv)
{
    TCLAP::CmdLine cmd("INRIA / IRISA - VisAGeS/Empenn Team", ' ',ANIMA_VERSION);

    TCLAP::ValueArg<std::string>  inArg  ("i", "inputFile",  "input image",  true,  "", "input image",  cmd);
    TCLAP::ValueArg<std::string>  outArg ("o", "outputFile", "output image", true,  "", "output image", cmd);
    TCLAP::ValueArg<std::string>  maskArg("m", "maskfile",   "mask file",    false, "", "output image", cmd);

    try
    {
        cmd.parse(argc,argv);
    }
    catch (TCLAP::ArgException& e)
    {
        std::cerr << "Error: " << e.error() << "for argument " << e.argId() << std::endl;
        return EXIT_FAILURE;
    }

    using ImageType = itk::Image <double,3>;

    ImageType::Pointer inputImage = anima::readImage <ImageType> (inArg.getValue());
    ImageType::Pointer resImage = ImageType::New();

    resImage->Initialize  ();
    resImage->SetRegions  (inputImage->GetLargestPossibleRegion());
    resImage->SetSpacing  (inputImage->GetSpacing());
    resImage->SetOrigin   (inputImage->GetOrigin());
    resImage->SetDirection(inputImage->GetDirection());

    resImage->Allocate();

    itk::ImageRegionConstIterator<ImageType> inItr(inputImage, resImage->GetLargestPossibleRegion());
    anima::writeImage <ImageType> (outArg.getValue(),resImage);
 

    return EXIT_SUCCESS;
}
