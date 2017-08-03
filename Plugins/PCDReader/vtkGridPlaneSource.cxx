// Copyright 2013 Velodyne Acoustics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "vtkGridPlaneSource.h"

#include "vtkNew.h"
#include "vtkSmartPointer.h"
#include "vtkPlaneSource.h"
#include "vtkArcSource.h"
#include "vtkExtractEdges.h"
#include "vtkAppendPolyData.h"
#include "vtkPolyData.h"
#include "vtkMath.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkGridPlaneSource);

vtkGridPlaneSource::vtkGridPlaneSource() : GridSize(10), Scale(1.0)
{
    this->Origin[0] = 0.0;
    this->Origin[1] = 0.0;
    this->Origin[2] = 0.0;

    this->Normal[0] = 0.0;
    this->Normal[1] = 0.0;
    this->Normal[2] = 1.0;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkGridPlaneSource::~vtkGridPlaneSource()
{
}

vtkSmartPointer<vtkPolyData> vtkGridPlaneSource::CreateGrid(int gridSize, double scale, double origin[3], double normal[3])
{
    vtkNew<vtkPlaneSource> plane;
    vtkNew<vtkExtractEdges> edges;
    vtkNew<vtkAppendPolyData> append;

    plane->SetOrigin(-gridSize*scale, -gridSize*scale, 0.0);
    plane->SetPoint1(gridSize*scale, -gridSize*scale, 0.0);
    plane->SetPoint2(-gridSize*scale, gridSize*scale, 0.0);
    plane->SetResolution(gridSize*2, gridSize*2);
    plane->SetCenter(origin);
    plane->SetNormal(normal);

    edges->SetInputConnection(plane->GetOutputPort());
    append->AddInputConnection(edges->GetOutputPort());

    double arcStartVector[3];
    vtkMath::Perpendiculars (normal, arcStartVector, NULL, 0);

    for (int i = 1; i <= gridSize; ++i)
    {
        double startPoint[3] = {arcStartVector[0] * i * scale,
            arcStartVector[1] * i * scale, arcStartVector[2] * i * scale};
        vtkNew<vtkArcSource> arc;
        arc->UseNormalAndAngleOn();
        arc->SetCenter(origin);
        arc->SetPolarVector(startPoint);
        arc->SetAngle(360);
        arc->SetNormal(normal);
        arc->SetResolution(360);
        append->AddInputConnection(arc->GetOutputPort());
    }

    append->Update();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(append->GetOutput());
    return polyData;
}

int vtkGridPlaneSource::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **vtkNotUsed(inputVector),
    vtkInformationVector *outputVector)
{
    vtkDataSet *output = vtkDataSet::GetData(outputVector, 0);

    if (this->GridSize < 1)
    {
        vtkErrorMacro("Specified grid size " << this->GridSize << " is out of range.  Must be >= 1.");
        return 0;
    }

    vtkSmartPointer<vtkPolyData> data = this->CreateGrid(this->GridSize, this->Scale, this->Origin, this->Normal);
    
    if (!data)
    {
        vtkErrorMacro("Failed to generate grids");
        return 0;
    }

    output->ShallowCopy(data);
    return 1;
}

void vtkGridPlaneSource::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "GridSize: " << this->GridSize << endl;
}
