#ifndef vtkSelectionOperationFilter_h
#define vtkSelectionOperationFilter_h

#include <vtkDataObjectAlgorithm.h>

// Refer to vtkExtractSelectedIds, vtkExtractSelectionBase, vtkMultiBlockDataGroupFilter
class VTK_EXPORT vtkSelectionOperationFilter : public vtkDataObjectAlgorithm
{
public:
	enum Operations
	{
		MERGE,
		SUBTRACT,
		INTERSECTION,
		TOGGLE
	};

    vtkTypeMacro(vtkSelectionOperationFilter, vtkDataObjectAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

    static vtkSelectionOperationFilter *New();

    vtkSetMacro(PreserveTopology, int);
    vtkGetMacro(PreserveTopology, int);
    vtkBooleanMacro(PreserveTopology, int);

    vtkSetMacro(Operation, int);
    vtkGetMacro(Operation, int);

protected:
    vtkSelectionOperationFilter();
    ~vtkSelectionOperationFilter() VTK_OVERRIDE;

    // int RequestInformation(vtkInformation *,
    //                 vtkInformationVector **,
    //                 vtkInformationVector *) VTK_OVERRIDE;
    // int RequestUpdateExtent(vtkInformation *,
    //                 vtkInformationVector **,
    //                 vtkInformationVector *) VTK_OVERRIDE;
    int RequestDataObject(vtkInformation *,
                          vtkInformationVector **,
                          vtkInformationVector *) VTK_OVERRIDE;
    int RequestData(vtkInformation *,
                    vtkInformationVector **,
                    vtkInformationVector *) VTK_OVERRIDE;

    int FillInputPortInformation(int port, vtkInformation *info) VTK_OVERRIDE;

    int PreserveTopology;
    int Operation;

private:
    vtkSelectionOperationFilter(const vtkSelectionOperationFilter&) VTK_DELETE_FUNCTION;
    void operator=(const vtkSelectionOperationFilter&) VTK_DELETE_FUNCTION;
};

#endif // vtkSelectionOperationFilter_h
