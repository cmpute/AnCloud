#include "vtkSelectionOperationFilter.h"

#include "vtkCellData.h"
#include "vtkDataSet.h"
#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkSignedCharArray.h"
#include "vtkUnstructuredGrid.h"

vtkStandardNewMacro(vtkSelectionOperationFilter);
vtkSelectionOperationFilter::vtkSelectionOperationFilter()
{
	this->PreserveTopology = 0;
	this->Operation = Operations::MERGE;
}
vtkSelectionOperationFilter::~vtkSelectionOperationFilter() {}

void vtkSelectionOperationFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os,indent);
}

int vtkSelectionOperationFilter::RequestDataObject(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    if (!inInfo || !outInfo)
    {
        return 0;
    }
	vtkDataSet *first = vtkDataSet::GetData(inputVector[0], 0);
	const char *dataType = first->GetClassName();

    vtkDataSet *output = vtkDataSet::GetData(outInfo);
    if(!output || (PreserveTopology && !output->IsA(dataType)) ||
        (!PreserveTopology && !output->IsA("vtkUnstructuredGrid")))
    {
        vtkDataSet* newOutput = NULL;
        if (!PreserveTopology)
        {
            // The mesh will be modified.
            newOutput = vtkUnstructuredGrid::New();
			vtkDebugMacro(<< "output dataset " << newOutput);
        }
        else
        {
            // The mesh will not be modified.
            newOutput = first->NewInstance();
			vtkDebugMacro(<< "output dataset " << newOutput);
        }
		
        outInfo->Set(vtkDataObject::DATA_OBJECT(), newOutput);
        newOutput->Delete();
    }
}

int vtkSelectionOperationFilter::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
	vtkDataSet *first = vtkDataSet::GetData(inputVector[0], 0);
	vtkDataSet *output = vtkDataSet::GetData(outputVector, 0);
    output->ShallowCopy(first); // Remain other data the same

	// judge whether inputs are valid vtkDataSe
	int numInputs = inputVector[0]->GetNumberOfInformationObjects();
	const char *dataType = first->GetClassName();
	vtkIdType numPoints = first->GetNumberOfPoints(),
			  numCells = first->GetNumberOfCells();
	int numPointFields = first->GetPointData()->GetNumberOfArrays(),
		numCellFields = first->GetCellData()->GetNumberOfArrays();
	bool hasPointMask = first->GetPointData()->GetAbstractArray("vtkInsidedness"),
		 hasCellMask = first->GetCellData()->GetAbstractArray("vtkInsidedness");
	for (int idx = 0; idx < numInputs; ++idx)
	{
		vtkDataSet* input = vtkDataSet::GetData(inputVector[0], idx);
		if (!input)
			continue;
		if (!input || (input->GetClassName() != dataType) ||
			(numPoints != input->GetNumberOfPoints()) ||
			(numCells != input->GetNumberOfCells()) ||
			(numPointFields != input->GetPointData()->GetNumberOfArrays()) ||
			(numCellFields != input->GetCellData()->GetNumberOfArrays()))
		{
			vtkErrorMacro(<< "Input selection should have same origin data");
			return 0; // Invalid input
		}
		if ((hasPointMask && !input->GetPointData()->GetAbstractArray("vtkInsidedness")) ||
			(hasCellMask && !input->GetCellData()->GetAbstractArray("vtkInsidedness")))
		{
			vtkErrorMacro(<< "Input selection should have vtkInsidedness field, "
				<< "please turn on PreserveTopology of the selection");
			return 0; // Invalid input
		}
		// We don't do further check into the arrays, and the output use the first input
	}

	vtkIdType *pointMap; // Store inside point if not PreserveTopology

    if(hasPointMask)
    {
		// Copy insidedness
		vtkAbstractArray *pointInsidedInit = output->GetPointData()->GetAbstractArray("vtkInsidedness");
		vtkAbstractArray *pointInsided = vtkAbstractArray::CreateArray(pointInsidedInit->GetDataType());
		pointInsided->SetName("vtkInsidedness");
		pointInsided->SetNumberOfComponents(1);
		pointInsided->SetNumberOfTuples(numPoints);
		for (vtkIdType j = 0; j < numPoints; j++)
		{
			pointInsided->SetVariantValue(j, pointInsidedInit->GetVariantValue(j));
		}
		output->GetPointData()->AddArray(pointInsided);

		// Store input vtkInsidedness arrays
		vtkAbstractArray **input2 = new vtkAbstractArray*[numInputs];
		for (int i = 1; i < numInputs; i++)
		{
			input2[i] = vtkDataSet::GetData(inputVector[0], i)
				->GetPointData()->GetAbstractArray("vtkInsidedness");
		}

		// Perform operation
		double step = 1 / static_cast<double>(numPoints * (hasCellMask ? 2 : 1));
		if(PreserveTopology) step *= 0.8;
        switch(Operation)
        {
		case Operations::MERGE:
			for (vtkIdType j = 0; j < numPoints; j++)
			{
				vtkVariant val = pointInsided->GetVariantValue(j);
				for (int i = 1; i < numInputs; i++)
					val = (val > 0 || input2[i]->GetVariantValue(j) > 0) ? 1 : -1;
				pointInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
		case Operations::SUBTRACT:
			for (vtkIdType j = 0; j < numPoints; j++)
			{
				vtkVariant val = pointInsided->GetVariantValue(j);
				for (int i = 1; i < numInputs; i++)
					val = (val > 0 && input2[i]->GetVariantValue(j) < 0) ? 1 : -1;
				pointInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
		case Operations::INTERSECTION:
			for (vtkIdType j = 0; j < numPoints; j++)
			{
				vtkVariant val = pointInsided->GetVariantValue(j);
				for (int i = 1; i < numInputs; i++)
					val = (val > 0 && input2[i]->GetVariantValue(j) > 0) ? 1 : -1;
				pointInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
		case Operations::TOGGLE:
			for (vtkIdType j = 0; j < numPoints; j++)
			{
				signed char val = pointInsided->GetVariantValue(j).ToSignedChar();
				for (int i = 1; i < numInputs; i++)
					val *= -input2[i]->GetVariantValue(j).ToSignedChar();
				pointInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
        }
		delete[] input2;

		// Remove outlier points
		if (!PreserveTopology)
		{
			output->GetPointData()->RemoveArray("vtkInsidedness");
			pointMap = new vtkIdType[numPoints]; // maps old point ids into new
			vtkPoints *newPts = vtkPoints::New();

			vtkIdTypeArray* originalPtIds = vtkIdTypeArray::New();
			originalPtIds->SetNumberOfComponents(1);
			originalPtIds->SetName("vtkOriginalPointIds");

			vtkPointData* inPD = first->GetPointData();
			vtkPointData* outPD = output->GetPointData();
			outPD->SetCopyGlobalIds(1);
			outPD->CopyAllocate(inPD);

			for (vtkIdType i = 0; i < numPoints; i++)
			{
				if (pointInsided->GetVariantValue(i) > 0)
				{
					pointMap[i] = newPts->InsertNextPoint(first->GetPoint(i));
					outPD->CopyData(inPD, i, pointMap[i]);
					originalPtIds->InsertNextValue(i);
				}
				else
				{
					pointMap[i] = -1;
				}
			}

			outPD->AddArray(originalPtIds);
			originalPtIds->Delete();

			// outputDS must be either vtkPolyData or vtkUnstructuredGrid
			vtkPointSet::SafeDownCast(output)->SetPoints(newPts);
			newPts->Delete();
		}
    }

	if (hasCellMask)
	{
		// Copy insidedness
		vtkAbstractArray *cellInsidedInit = output->GetCellData()->GetAbstractArray("vtkInsidedness");
		vtkAbstractArray *cellInsided = vtkAbstractArray::CreateArray(cellInsidedInit->GetDataType());
		cellInsided->SetName("vtkInsidedness");
		cellInsided->SetNumberOfComponents(1);
		cellInsided->SetNumberOfTuples(numCells);
		for (vtkIdType j = 0; j < numCells; j++)
		{
			cellInsided->SetVariantValue(j, cellInsidedInit->GetVariantValue(j));
		}
		output->GetCellData()->AddArray(cellInsided);

		// Store input vtkInsidedness arrays
		vtkAbstractArray **input2 = new vtkAbstractArray*[numInputs];
		for (int i = 1; i < numInputs; i++)
		{
			input2[i] = vtkDataSet::GetData(inputVector[0], i)
				->GetCellData()->GetAbstractArray("vtkInsidedness");
		}

		// Perform operation
		double step = 1 / static_cast<double>(numPoints * (hasPointMask ? 2 : 1));
		if(PreserveTopology) step *= 0.8;
		switch (Operation)
		{
		case Operations::MERGE:
			for (vtkIdType j = 0; j < numCells; j++)
			{
				vtkVariant val = cellInsided->GetVariantValue(j);
				for (int i = 1; i < numInputs; i++)
					val = (val > 0 || input2[i]->GetVariantValue(j) > 0) ? 1 : -1;
				cellInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
		case Operations::SUBTRACT:
			for (vtkIdType j = 0; j < numCells; j++)
			{
				vtkVariant val = cellInsided->GetVariantValue(j);
				for (int i = 1; i < numInputs; i++)
					val = (val > 0 && input2[i]->GetVariantValue(j) < 0) ? 1 : -1;
				cellInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
		case Operations::INTERSECTION:
			for (vtkIdType j = 0; j < numCells; j++)
			{
				vtkVariant val = cellInsided->GetVariantValue(j);
				for (int i = 1; i < numInputs; i++)
					val = (val > 0 && input2[i]->GetVariantValue(j) > 0) ? 1 : -1;
				cellInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
		case Operations::TOGGLE:
			for (vtkIdType j = 0; j < numCells; j++)
			{
				signed char val = cellInsided->GetVariantValue(j).ToSignedChar();
				for (int i = 1; i < numInputs; i++)
					val *= -input2[i]->GetVariantValue(j).ToSignedChar();
				cellInsided->SetVariantValue(j, val);
				this->UpdateProgress(this->Progress + step);
			}
			break;
		}
		delete[] input2;

		// Remove outlier cells
		if (!PreserveTopology)
		{
			vtkCellData* inCD = first->GetCellData();
			vtkCellData* outCD = output->GetCellData();
			outCD->RemoveArray("vtkInsidedness");

			vtkIdTypeArray* originalIds = vtkIdTypeArray::New();
			originalIds->SetNumberOfComponents(1);
			originalIds->SetName("vtkOriginalCellIds");

			vtkIdType i, j, newId = 0;
			vtkIdList* ptIds = vtkIdList::New();
			for (i = 0; i < numCells; i++)
			{
				if (cellInsided->GetVariantValue(i) > 0)
				{
					// special handling for polyhedron cells
					if (vtkUnstructuredGrid::SafeDownCast(first) &&
						vtkUnstructuredGrid::SafeDownCast(output) &&
						first->GetCellType(i) == VTK_POLYHEDRON)
					{
						ptIds->Reset();
						vtkUnstructuredGrid::SafeDownCast(first)->GetFaceStream(i, ptIds);
						vtkUnstructuredGrid::ConvertFaceStreamPointIds(ptIds, pointMap);
					}
					else
					{
						first->GetCellPoints(i, ptIds);
						for (j = 0; j < ptIds->GetNumberOfIds(); j++)
						{
							ptIds->SetId(j, pointMap[ptIds->GetId(j)]);
						}
					}
					if (output->GetDataObjectType() == VTK_POLY_DATA)
					{
						vtkPolyData::SafeDownCast(output)->InsertNextCell(first->GetCellType(i), ptIds);
					}
					else
					{
						vtkUnstructuredGrid::SafeDownCast(output)->InsertNextCell(first->GetCellType(i), ptIds);
					}
					outCD->CopyData(inCD, i, newId++);
					originalIds->InsertNextValue(i);
				}
			}

			outCD->AddArray(originalIds);
			originalIds->Delete();
			ptIds->Delete();
		}
	}

	// Remove unnecessary space
	output->Squeeze();
	vtkDebugMacro(<< "Dataset squeezed.");

	if (output->GetNumberOfCells() == 0)
	{
		// Add default cell data
		numPoints = output->GetNumberOfPoints(); // squeezed number
		if (output->GetDataObjectType() == VTK_POLY_DATA)
		{
			vtkPolyData* outputPD = vtkPolyData::SafeDownCast(output);
			vtkCellArray *newVerts = vtkCellArray::New();
			newVerts->Allocate(newVerts->EstimateSize(numPoints,  1));
			for (vtkIdType i = 0; i < numPoints; ++i)
			{
				newVerts->InsertNextCell(1, &i);
			}
			outputPD->SetVerts(newVerts);
			newVerts->Delete();
		}
		else
		{
			vtkUnstructuredGrid* outputUG = vtkUnstructuredGrid::SafeDownCast(output);
			outputUG->Allocate(numPoints);
			for (vtkIdType i = 0; i < numPoints; ++i)
			{
				outputUG->InsertNextCell(VTK_VERTEX, 1, &i);
			}
		}
	}
	
	this->UpdateProgress(1.0);
    return 1;
}

int vtkSelectionOperationFilter::FillInputPortInformation(
    int vtkNotUsed(port), vtkInformation* info)
{
    // Support only dataSet currently
    info->Remove(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE());
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
    //info->Append(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkGraph");
    //info->Append(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkTable");
	info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
	return 1;
}