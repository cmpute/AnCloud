#include "vtkPCDReader.h"
#include "PCDReaderRaw.h"

#include <vtkPolyData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkSmartPointer.h>

#include <functional>

vtkStandardNewMacro(vtkPCDReader);

vtkPCDReader::vtkPCDReader()
{
    this->FileName = NULL;
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkPCDReader::~vtkPCDReader()
{
    delete [] this->FileName;
    this->FileName = NULL;
}

int vtkPCDReader::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **vtkNotUsed(inputVector),
    vtkInformationVector *outputVector)
{
    vtkDataSet *output = vtkDataSet::GetData(outputVector, 0);

    if (!this->GetFileName())
    {
        vtkErrorMacro("Filename is not set");
        return 0;
    }

    vtkSmartPointer<vtkPointCloudType> polyData = PCDReaderRaw::readFile(this->GetFileName(),
		std::bind1st(std::mem_fun(&vtkPCDReader::UpdateProgress), this));

    if (!polyData)
    {
        vtkErrorMacro("Failed to read pcd file: " << this->GetFileName());
        return 0;
    }

    output->ShallowCopy(polyData);
    return 1;
}

void vtkPCDReader::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "File Name: "
       << (this->FileName ? this->FileName : "(none)") << "\n";
}
