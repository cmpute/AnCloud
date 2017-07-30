#ifndef vtkPCDReader_h
#define vtkPCDReader_h

#include <vtkAbstractPolyDataReader.h>
#include <vtkSetGet.h> // Macro

class VTK_EXPORT vtkPCDReader : public vtkAbstractPolyDataReader
{
public:
    static vtkPCDReader *New();
    vtkTypeMacro(vtkPCDReader, vtkAbstractPolyDataReader);
    void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

protected:
    vtkPCDReader();
    ~vtkPCDReader() VTK_OVERRIDE;

    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) VTK_OVERRIDE;

private:
	vtkPCDReader(const vtkPCDReader&) VTK_DELETE_FUNCTION;
    void operator=(const vtkPCDReader&) VTK_DELETE_FUNCTION;
};

#endif // vtkPCDReader_h
