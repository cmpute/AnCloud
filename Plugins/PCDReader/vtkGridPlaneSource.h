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

#ifndef vtkGridPlaneSource_h
#define vtkGridPlaneSource_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkSmartPointer.h>

/* Generate plane square and arc grids */
class VTK_EXPORT vtkGridPlaneSource : public vtkPolyDataAlgorithm
{
public:
    static vtkGridPlaneSource *New();
    vtkTypeMacro(vtkGridPlaneSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

    vtkSetMacro(GridSize, int);
    vtkGetMacro(GridSize, int);

    vtkSetMacro(Scale, double);
    vtkGetMacro(Scale, double);

    vtkSetVector3Macro(Origin, double);
    vtkGetVector3Macro(Origin, double);

    vtkSetVector3Macro(Normal, double);
    vtkGetVector3Macro(Normal, double);

    static vtkSmartPointer<vtkPolyData> CreateGrid(int gridSize, double scale, double origin[3], double normal[3]);

    protected:
    vtkGridPlaneSource();
    ~vtkGridPlaneSource() VTK_OVERRIDE;

    int RequestData(vtkInformation *,
                    vtkInformationVector **,
                    vtkInformationVector *) VTK_OVERRIDE;

    int GridSize;
    double Scale;
    double Origin[3];
    double Normal[3];

private:
    vtkGridPlaneSource(const vtkGridPlaneSource&) VTK_DELETE_FUNCTION;
    void operator =(const vtkGridPlaneSource&) VTK_DELETE_FUNCTION;

};

#endif // vtkGridPlaneSource_h
