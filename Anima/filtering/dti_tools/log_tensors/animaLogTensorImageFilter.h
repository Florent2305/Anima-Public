#pragma once

#include <iostream>
#include <itkImageToImageFilter.h>
#include <itkVectorImage.h>

#include <animaBaseTensorTools.h>

namespace anima
{

template <class TScalarType, unsigned int NDimensions = 3>
class LogTensorImageFilter :
public itk::ImageToImageFilter< itk::VectorImage <TScalarType, NDimensions> , itk::VectorImage <TScalarType, NDimensions> >
{
public:
    /** Standard class typedefs. */
    typedef LogTensorImageFilter Self;
    typedef itk::VectorImage <TScalarType, NDimensions> TInputImage;
    typedef itk::VectorImage <TScalarType, NDimensions> TOutputImage;

    typedef itk::ImageToImageFilter< TInputImage, TOutputImage > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self)

    /** Run-time type information (and related methods) */
    itkTypeMacro(LogTensorImageFilter, ImageToImageFilter)

    typedef typename TOutputImage::PixelType OutputPixelType;
    typedef typename TInputImage::PixelType InputPixelType;
    typedef typename TInputImage::IndexType InputIndexType;
    typedef typename TInputImage::PointType InputPointType;

    /** Image typedef support */
    typedef typename TInputImage::Pointer InputImagePointer;
    typedef typename TOutputImage::Pointer OutputImagePointer;

    /** Superclass typedefs. */
    typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

    using LECalculatorType = anima::LogEuclideanTensorCalculator <double>;
    using LECalculatorPointer = typename LECalculatorType::Pointer;

    itkSetMacro(ScaleNonDiagonal, bool)

protected:
    LogTensorImageFilter()
    {
        m_ScaleNonDiagonal = true;

        m_TensorDimension = 3;
        m_VectorSize = m_TensorDimension * (m_TensorDimension + 1) / 2;
    }

    virtual ~LogTensorImageFilter() {}

    void GenerateOutputInformation() ITK_OVERRIDE;
    void BeforeThreadedGenerateData() ITK_OVERRIDE;
    void DynamicThreadedGenerateData(const OutputImageRegionType &outputRegionForThread) ITK_OVERRIDE;

private:
    ITK_DISALLOW_COPY_AND_ASSIGN(LogTensorImageFilter);

    bool isZero(const InputPixelType &tensVec)
    {
        bool testZero = true;

        for (unsigned int i = 0;i < m_VectorSize;++i)
        {
            if (tensVec[i] != 0)
            {
                testZero = false;
                break;
            }
        }

        return testZero;
    }

    bool m_ScaleNonDiagonal;
    unsigned int m_TensorDimension;
    unsigned int m_VectorSize;
};

} // end of namespace anima

#include "animaLogTensorImageFilter.hxx"
