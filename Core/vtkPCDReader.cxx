#include "vtkPCDReader.h"

#include "vtkPolyData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkSmartPointer.h"

#include "vtkPCDReaderRaw.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPCDReader);

//----------------------------------------------------------------------------
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
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    vtkDataSet *output = vtkDataSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    if (!this->GetFileName())
    {
        vtkErrorMacro("Filename is not set");
        return 0;
    }

    vtkSmartPointer<vtkPolyData> polyData = vtkPCDReaderRaw::readFile(this->GetFileName());

    if (!polyData)
    {
        vtkErrorMacro("Failed to read pcd file: " << this->GetFileName());
    }

    output->ShallowCopy(polyData);
    return 1;
}

//----------------------------------------------------------------------------
void vtkPCDReader::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os,indent);
    os << indent << "File Name: "
       << (this->FileName ? this->FileName : "(none)") << "\n";
}
