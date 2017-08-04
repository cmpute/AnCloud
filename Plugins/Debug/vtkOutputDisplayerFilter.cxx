#include "vtkOutputDisplayerFilter.h"

#include <vtkObjectFactory.h>
#include <vtkInformationVector.h>
#include <vtkInformation.h>

#include <sstream>
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
	qDebug() << "===========[RequestInformation]==============\n";
	DisplayInformation(request, inputVector, outputVector);
    return 1;
}

int vtkOutputDisplayerFilter::RequestUpdateExtent(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
	qDebug() << "===========[RequestInformation]==============\n";
	DisplayInformation(request, inputVector, outputVector);
    return 1;
}

int vtkOutputDisplayerFilter::RequestData(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
	qDebug() << "===========[RequestData]==============\n";
	DisplayInformation(request, inputVector, outputVector);
	vtkDataObject* input = vtkDataObject::GetData(inputVector[0], 0);
	vtkDataObject* output = vtkDataObject::GetData(outputVector,0);
    
	if(!output)
		output = input->NewInstance();
    output->ShallowCopy(input);
    return 1;
}

void vtkOutputDisplayerFilter::DisplayInformation(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
	std::stringstream ss;
	qDebug() << "---------- Request information object ------------\n";
	request->Print(ss);

	int numPorts = this->GetNumberOfInputPorts();
	for (int p = 0; p < numPorts; p++)
	{
		qDebug() << "---------- Output port " << p << " ------------\n";
		int numInputs = inputVector[p]->GetNumberOfInformationObjects();
		for (int i = 0; i < numInputs; i++)
		{
			qDebug() << "---------- Output information object " << i << " ------------\n";
			vtkInformation* inInfo = inputVector[p]->GetInformationObject(i);
			ss.str("");
			inInfo->Print(ss);
			inInfo->Get(vtkDataObject::DATA_OBJECT())->Print(ss);
			qDebug() << std::string(ss.str()).c_str();
		}
	}
}

int vtkOutputDisplayerFilter::FillInputPortInformation(
    int, vtkInformation *info)
{
    //info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataObject");
    info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
    qDebug() << "===========[FillInputPortInformation]==============\n";
    return 1;
}

void vtkOutputDisplayerFilter::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}
