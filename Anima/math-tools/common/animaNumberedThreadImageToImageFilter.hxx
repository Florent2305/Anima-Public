#pragma once
#include "animaNumberedThreadImageToImageFilter.h"

namespace anima
{

template <typename TInputImage, typename TOutputImage>
void
NumberedThreadImageToImageFilter <TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
    m_ThreadIdsVector.clear();
    Superclass::BeforeThreadedGenerateData();

    m_NumberOfProcessedPoints = 0;
    m_NumberOfPointsToProcess = this->GetOutput(0)->GetRequestedRegion().GetNumberOfPixels();
}

template <typename TInputImage, typename TOutputImage>
void
NumberedThreadImageToImageFilter <TInputImage, TOutputImage>
::GenerateData()
{
    // Copy pasted from itk Image source to de-activate progress handling by ITK

    // Call a method that can be overriden by a subclass to allocate
    // memory for the filter's outputs
    this->AllocateOutputs();

    // Call a method that can be overridden by a subclass to perform
    // some calculations prior to splitting the main computations into
    // separate threads
    this->BeforeThreadedGenerateData();

    if (!this->GetDynamicMultiThreading())
    {
        this->ClassicMultiThread(this->ThreaderCallback);
    }
    else
    {
        this->GetMultiThreader()->SetNumberOfWorkUnits(this->GetNumberOfWorkUnits());
        this->GetMultiThreader()->template ParallelizeImageRegion<OutputImageDimension>(
                    this->GetOutput()->GetRequestedRegion(),
                    [this](const OutputImageRegionType & outputRegionForThread)
        { this->DynamicThreadedGenerateData(outputRegionForThread); }, ITK_NULLPTR);
    }

    // Call a method that can be overridden by a subclass to perform
    // some calculations after all the threads have completed
    this->AfterThreadedGenerateData();
}

template <typename TInputImage, typename TOutputImage>
void
NumberedThreadImageToImageFilter <TInputImage, TOutputImage>
::IncrementNumberOfProcessedPoints()
{
    m_LockProcessedPoints.lock();

    ++m_NumberOfProcessedPoints;

    double ratio = static_cast <double> (m_NumberOfProcessedPoints) / m_NumberOfPointsToProcess;
    this->UpdateProgress(ratio);

    m_LockProcessedPoints.unlock();
}

template <typename TInputImage, typename TOutputImage>
unsigned int
NumberedThreadImageToImageFilter <TInputImage, TOutputImage>
::GetSafeThreadId()
{
    m_LockThreadIdNumber.lock();

    unsigned int threadId = 0;
    bool presentInIdsVector = true;
    while (presentInIdsVector)
    {
        presentInIdsVector = false;
        for (unsigned int i = 0;i < m_ThreadIdsVector.size();++i)
        {
            if (m_ThreadIdsVector[i] == threadId)
            {
                presentInIdsVector = true;
                break;
            }
        }

        if (presentInIdsVector)
            threadId++;
    }

    m_ThreadIdsVector.push_back(threadId);

    m_LockThreadIdNumber.unlock();

    return threadId;
}

template <typename TInputImage, typename TOutputImage>
void
NumberedThreadImageToImageFilter <TInputImage, TOutputImage>
::SafeReleaseThreadId(unsigned int threadId)
{
    m_LockThreadIdNumber.lock();

    unsigned int indexThreadId = 0;
    for (unsigned int i = 0;i < m_ThreadIdsVector.size();++i)
    {
        if (threadId == m_ThreadIdsVector[i])
        {
            indexThreadId = i;
            break;
        }
    }

    m_ThreadIdsVector.erase(m_ThreadIdsVector.begin() + indexThreadId);

    m_LockThreadIdNumber.unlock();
}

} // end namespace anima