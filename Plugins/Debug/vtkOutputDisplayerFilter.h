#ifndef vtkOutputDisplayerFilter_h
#define vtkOutputDisplayerFilter_h

#include <vtkDataObjectAlgorithm.h>

class VTK_EXPORT vtkOutputDisplayerFilter : public vtkDataObjectAlgorithm
{
public:
    vtkTypeMacro(vtkOutputDisplayerFilter, vtkDataObjectAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

    static vtkOutputDisplayerFilter *New();
 
protected:
    vtkOutputDisplayerFilter();
    ~vtkOutputDisplayerFilter() {}

    int RequestInformation(vtkInformation *,
        vtkInformationVector **,
        vtkInformationVector *) VTK_OVERRIDE;
    int RequestUpdateExtent(vtkInformation *,
        vtkInformationVector **,
        vtkInformationVector *) VTK_OVERRIDE;
    int RequestData(vtkInformation *,
        vtkInformationVector **,
        vtkInformationVector *) VTK_OVERRIDE;
             
    // int FillOutputPortInformation(int port, vtkInformation *info) VTK_OVERRIDE;
 
private:
	vtkOutputDisplayerFilter(const vtkOutputDisplayerFilter&) VTK_DELETE_FUNCTION;
    void operator=(const vtkOutputDisplayerFilter&) VTK_DELETE_FUNCTION;

    void DisplayInformation(vtkInformation *,
        vtkInformationVector **,
        vtkInformationVector *);
};

#endif // vtkOutputDisplayerFilter_h
