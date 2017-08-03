#include "vtkOutputDisplayerFilter.h"

#include <vtkObjectFactory.h>
#include <QDebug>

vtkStandardNewMacro(vtkOutputDisplayerFilter);
 
vtkOutputDisplayerFilter::vtkOutputDisplayerFilter()
{
}

int vtkOutputDisplayerFilter::RequestInformation(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
	DisplayInformation(request, inputVector, outputVector);
    return 1;
}

int vtkOutputDisplayerFilter::RequestUpdateExtent(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
	DisplayInformation(request, inputVector, outputVector);
    return 1;
}

int vtkOutputDisplayerFilter::RequestData(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
	DisplayInformation(request, inputVector, outputVector);
	vtkDataObject* input = vtkDataObject::GetData(inputVector[0], 0);
	vtkDataObject* output = vtkDataObject::GetData(outputVector,0);
    
    output->ShallowCopy(input);
    return 1;
}

void vtkOutputDisplayerFilter::DisplayInformation(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
    qDebug() << request << inputVector << outputVector;
}

void vtkOutputDisplayerFilter::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}
