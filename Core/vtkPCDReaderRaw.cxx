/*
 * Software License Agreement (BSD License)
 *
 *  Jacob Zhong
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2011, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "vtkPCDReaderRaw.h"

#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <algorithm>
#include <functional>
#include <locale>
#include <string>
#include <stdlib.h>
// #include <pcl/io/boost.h>
// #include <pcl/common/io.h>
// #include <pcl/io/pcd_io.h>
// #include <pcl/io/lzf.h>
// #include <pcl/console/time.h>
#include "ThirdParty/PCL/io/lzf.h"

#include <cctype>
#include <cstring>
#include <cerrno>

#ifdef _WIN32
# include <io.h>
# include <windows.h>
# define pcl_open                    _open
# define pcl_close(fd)               _close(fd)
# define pcl_lseek(fd,offset,origin) _lseek(fd,offset,origin)
#else
# include <sys/mman.h>
# define pcl_open                    open
# define pcl_close(fd)               close(fd)
# define pcl_lseek(fd,offset,origin) lseek(fd,offset,origin)
#endif

#include <QDebug> // Debug utils
#define VTK_AOS_DATA_ARRAY_TEMPLATE_INSTANTIATING
#include <vtkAbstractArray.h>
#include <vtkAOSDataArrayTemplate.h>
#include <vtkDataArrayTemplate.h>
#include <vtkSetGet.h>
#include <vtkPointData.h>

inline void trim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

inline void split(const std::string &str, std::vector<std::string> &ret, const char* delimit = "\t\r ")
{
	std::string substring;
	std::string::size_type start = 0, index;
	ret.clear();

	do
	{
		index = str.find_first_of(delimit, start);
		if (index != std::string::npos)
		{
			substring = str.substr(start, index - start);
			if(substring.size() > 0)
				ret.push_back(substring);
			start = str.find_first_not_of(delimit, index);
			if (start == std::string::npos) return;
		}
	} while (index != std::string::npos);

	//the last token
	substring = str.substr(start);
	ret.push_back(substring);
}

inline int getFieldType(const int size, char type)
{
	type = std::toupper(type, std::locale::classic());
	switch (size)
	{
	case 1:
		if (type == 'I')
			return (VTK_TYPE_INT8);
		if (type == 'U')
			return (VTK_TYPE_UINT8);

	case 2:
		if (type == 'I')
			return (VTK_TYPE_INT16);
		if (type == 'U')
			return (VTK_TYPE_UINT16);

	case 4:
		if (type == 'I')
			return (VTK_TYPE_INT32);
		if (type == 'U')
			return (VTK_TYPE_UINT32);
		if (type == 'F')
			return (VTK_TYPE_FLOAT32);

	case 8:
		return (VTK_TYPE_FLOAT64);

	default:
		return (-1);
	}
}

template<class Type> Type copyStringValue(std::string &str)
{
	Type value;
	if (str.compare("nan") == 0)
		value = std::numeric_limits<Type>::quiet_NaN();
	else
	{
		std::istringstream is(str);
		is.imbue(std::locale::classic());
		if (!(is >> value))
			value = static_cast<Type> (atof(str.c_str()));
	}
	return value;
}

//Traits from PCL and VTK
template<int> struct asType {};
#define INSTANTIATE_TYPE(etype, dtype)                            \
	template<> struct asType<etype> { typedef dtype type; };      \
	VTK_AOS_DATA_ARRAY_TEMPLATE_INSTANTIATE(dtype);
INSTANTIATE_TYPE(VTK_TYPE_INT8, int8_t)
INSTANTIATE_TYPE(VTK_TYPE_UINT8, uint8_t)
INSTANTIATE_TYPE(VTK_TYPE_INT16, int16_t)
INSTANTIATE_TYPE(VTK_TYPE_UINT16, uint16_t)
INSTANTIATE_TYPE(VTK_TYPE_INT32, int32_t)
INSTANTIATE_TYPE(VTK_TYPE_UINT32, uint32_t)
INSTANTIATE_TYPE(VTK_TYPE_FLOAT32, float)
INSTANTIATE_TYPE(VTK_TYPE_FLOAT64, double)

//////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int vtkPCDReaderRaw::readHeader (const char *file_name, int &data_type, std::vector<PointField> &fields,
                             unsigned int &data_idx, unsigned int &point_step, const int offset)
{
    // Default values
    data_idx = 0;
    data_type = 0;

    unsigned int nr_points = 0;
    std::ifstream fs;
    std::string line;

    int specified_channel_count = 0;

	if (file_name == "") // Not assigned
	{
		qDebug() << "Null filename to open";
		return (-1);
	}

    // Open file in binary mode to avoid problem of 
    // std::getline() corrupting the result of ifstream::tellg()
    fs.open (file_name, std::ios::binary);
	if (!fs.is_open() || fs.fail())
	{
		qCritical() << "Could not open file" << file_name;
		return (-1); // Fail to open
	}

    // Seek at the given offset
    fs.seekg (offset, std::ios::beg);

    // field_types represents the type of data in a field (e.g., F = float, U = unsigned)
    std::vector<char> field_types;
    std::vector<std::string> st;
	bool size_parsed = false;

    // Read the header and fill it in with wonderful values
    try
    {
        while (!fs.eof ())
        {
			getline (fs, line);
			// Ignore empty lines
			if (line == "")
				continue;
			// Ignore comments
			if (line.substr(0, 1) == "#")
				continue;

			// Tokenize the line
			trim (line);
			split (line, st);

			std::stringstream sstream (line);
			sstream.imbue (std::locale::classic ());

			std::string line_type;
			sstream >> line_type;


			// Version numbers are not needed for now, but we are checking to see if they're there
			if (line_type.substr (0, 7) == "VERSION")
				continue;

			// Get the field indices (check for COLUMNS too for backwards compatibility)
			if ( (line_type.substr (0, 6) == "FIELDS") || (line_type.substr (0, 7) == "COLUMNS") )
			{
				specified_channel_count = static_cast<int> (st.size () - 1);

				// Allocate enough memory to accommodate all fields
				fields.resize (specified_channel_count);
				for (int i = 0; i < specified_channel_count; ++i)
				{
					std::string col_type = st.at (i + 1);
					fields[i].name = col_type;
				}

				// Default the sizes and the types of each field to float32 to avoid crashes while using older PCD files
				for (int i = 0; i < specified_channel_count; ++i)
				{
					fields[i].datatype = VTK_TYPE_FLOAT32;
					fields[i].count    = 1;
				}
				continue;
			}

			// Get the field sizes
			if (line_type.substr (0, 4) == "SIZE")
			{
				specified_channel_count = static_cast<int> (st.size () - 1);

				// Allocate enough memory to accommodate all fields
				if (specified_channel_count != static_cast<int> (fields.size ()))
				throw "The number of elements in <SIZE> differs than the number of elements in <FIELDS>!";

				point_step = 0;
				for (int i = 0; i < specified_channel_count; ++i)
				{
					int col_type ;
					sstream >> col_type;
					fields[i].size = col_type;
					point_step += col_type;
				}
				size_parsed = true;
				continue;
			}

			// Get the field types
			if (line_type.substr (0, 4) == "TYPE")
			{
				if (!size_parsed)
				throw "TYPE of FIELDS specified before SIZE in header!";

				specified_channel_count = static_cast<int> (st.size () - 1);

				// Allocate enough memory to accommodate all fields
				if (specified_channel_count != static_cast<int> (fields.size ()))
				throw "The number of elements in <TYPE> differs than the number of elements in <FIELDS>!";

				// Resize to accommodate the number of values
				field_types.resize (specified_channel_count);

				for (int i = 0; i < specified_channel_count; ++i)
				{
					field_types[i] = st.at (i + 1).c_str ()[0];
					fields[i].datatype = static_cast<uint8_t> (getFieldType (fields[i].size, field_types[i]));
				}
				continue;
			}

			// Get the field counts
			if (line_type.substr (0, 5) == "COUNT")
			{
				if (!size_parsed || field_types.empty ())
					throw "COUNT of FIELDS specified before SIZE or TYPE in header!";

				specified_channel_count = static_cast<int> (st.size () - 1);

				// Allocate enough memory to accommodate all fields
				if (specified_channel_count != static_cast<int> (fields.size ()))
					throw "The number of elements in <COUNT> differs than the number of elements in <FIELDS>!";

				point_step = 0;
				for (int i = 0; i < specified_channel_count; ++i)
				{
					int col_count;
					sstream >> col_count;
					fields[i].count = col_count;
					point_step += col_count * fields[i].size;
				}
				continue;
			}

			// Get the width of the data (organized point cloud dataset)
			if (line_type.substr (0, 5) == "WIDTH")
				continue;

			// Get the height of the data (organized point cloud dataset)
			if (line_type.substr (0, 6) == "HEIGHT")
				continue;

			// Get the acquisition viewpoint
			if (line_type.substr (0, 9) == "VIEWPOINT")
				continue;

			// Get the number of points
			if (line_type.substr (0, 6) == "POINTS")
			{
				sstream >> nr_points;
				continue;
			}

			// Read the header + comments line by line until we get to <DATA>
			if (line_type.substr (0, 4) == "DATA")
			{
				data_idx = static_cast<int> (fs.tellg ());
				if (st.at (1).substr (0, 17) == "binary_compressed")
					data_type = 2;
				else if (st.at (1).substr (0, 6) == "binary")
					data_type = 1;
				break;
			}
			break;
        }
    }
    catch (const char *exception)
    {
        qCritical() << exception;
        fs.close ();
        return (-1);
    }

    // Exit early: if no points have been given, there's no sense to read or check anything anymore
    if (nr_points == 0)
    {
		qCritical() << "No points to read\n";
        fs.close ();
        return (-1);
    }

    // Close file
    fs.close ();

    return nr_points;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define acTemplateMacroCase(dtype, call)     \
  case dtype: { typedef asType<dtype>::type itype; call; }; break
#define acTemplateMacro(call)											\
	acTemplateMacroCase(VTK_TYPE_FLOAT32, call);	\
	acTemplateMacroCase(VTK_TYPE_FLOAT64, call);	\
	acTemplateMacroCase(VTK_TYPE_INT32, call);		\
	acTemplateMacroCase(VTK_TYPE_UINT32, call);	\
	acTemplateMacroCase(VTK_TYPE_INT16, call);		\
	acTemplateMacroCase(VTK_TYPE_UINT16, call);	\
	acTemplateMacroCase(VTK_TYPE_INT8, call);		\
	acTemplateMacroCase(VTK_TYPE_UINT8, call)		\

vtkSmartPointer<vtkPointCloudType> vtkPCDReaderRaw::readFile(const char *file_name, const int offset)
{
	int data_type;
	unsigned int data_idx;
	int nr_points;
	std::vector<PointField> fields;
	std::vector<vtkAbstractArray*> data;
	unsigned int point_step;
	int cloud_data_size;

	nr_points = readHeader(file_name, data_type, fields, data_idx, point_step, offset);

	if (nr_points < 0)
		return NULL;
	cloud_data_size = nr_points * point_step;

	// reserve space for data and process valid fields
	for (int i = 0; i < fields.size(); ++i)
	{
		if (fields[i].name == "_")
			continue;

		switch (fields[i].datatype)
		{
		// MACRO Start
#		define SetupField(type)																	\
			vtkAOSDataArrayTemplate<type> *darray = vtkAOSDataArrayTemplate<type>::New();		\
			darray->SetName(fields[i].name.c_str());											\
			if (fields[i].count > 1)															\
			{																					\
				darray->SetNumberOfComponents(fields[i].count);									\
				darray->SetNumberOfTuples(nr_points);											\
			}																					\
			else																				\
				darray->SetNumberOfValues(nr_points);											\
			data.push_back(darray)
		// MACRO End
		acTemplateMacro(SetupField(itype));

		default:
			qWarning() << "Incorrect field data type specified" << fields[i].datatype;
			break;
		}
	}

	unsigned int idx = 0; // point index

	// --[ ASCII mode ]--
	if (data_type == 0)
	{
		// Re-open the file (readHeader closes it)
		std::ifstream fs;
		fs.open(file_name);
		if (!fs.is_open() || fs.fail())
		{
			qCritical() << "Could not open file" << file_name;
			return NULL;
		}

		fs.seekg(data_idx);

		std::string line;
		std::vector<std::string> st;

		// Read the rest of the file
		try
		{
			while (idx < nr_points && !fs.eof())
			{
				getline(fs, line);
				// Ignore empty lines
				if (line == "")
					continue;

				// Tokenize the line
				trim(line);
				split(line, st);

				if (idx >= nr_points)
				{
					qWarning() << "input file" << file_name << "has more points (" << idx << ") than advertised (" << nr_points << ")!";
					break;
				}

				size_t toff = 0;
				// Copy data
				for (int d = 0; d < fields.size(); ++d)
				{
					// Ignore invalid padded dimensions that are inherited from binary data
					if (fields[d].name != "_")
					{
						switch (fields[d].datatype)
						{
						// MACRO start
#						define InsertField(type) 																	\
							type *tupleData = new type[fields[d].count];											\
							for (unsigned int c = 0; c < fields[d].count; ++c)										\
								tupleData[c] = copyStringValue<type>(st.at(toff + c));								\
							if (fields[d].count == 1)																\
								data[d]->SetVariantValue(idx, *tupleData);											\
							else																					\
								static_cast<vtkAOSDataArrayTemplate<type>*>(data[d])->SetTypedTuple(idx, tupleData);\
							delete tupleData
						// MACRO end
						acTemplateMacro(InsertField(itype));
						}
					}
					toff += fields[d].count; // jump over this many elements in the string token
				}
				idx++;
			}
		}
		catch (const char *exception)
		{
			qCritical() << exception;
			fs.close();
			return NULL;
		}

		// Close file
		fs.close();
	}
	else
	// --[ Binary mode only ]--
	// We must re-open the file and read with mmap () for binary
	{
		// Open for reading
		int fd = pcl_open(file_name, O_RDONLY);
		if (fd == -1)
		{
			qCritical() << "Could not open file" << file_name;
			return NULL;
		}

		// Seek at the given offset
		off_t result = pcl_lseek(fd, offset, SEEK_SET);
		if (result < 0)
		{
			pcl_close(fd);
			qCritical() << "lseek errno: " << errno << " strerror: " << strerror(errno);
			qCritical() << "Error during lseek ()";
			return NULL;
		}

		size_t data_size = data_idx + cloud_data_size; // cloud.data.size();
		// Prepare the map
#ifdef _WIN32
		// As we don't know the real size of data (compressed or not), 
		// we set dwMaximumSizeHigh = dwMaximumSizeLow = 0 so as to map the whole file
		HANDLE fm = CreateFileMapping((HANDLE)_get_osfhandle(fd), NULL, PAGE_READONLY, 0, 0, NULL);
		// As we don't know the real size of data (compressed or not), 
		// we set dwNumberOfBytesToMap = 0 so as to map the whole file
		char *map = static_cast<char*>(MapViewOfFile(fm, FILE_MAP_READ, 0, 0, 0));
		if (map == NULL)
		{
			CloseHandle(fm);
			pcl_close(fd);
			qCritical() << "Error mapping view of file, " << file_name;
			return NULL;
		}
#else
		char *map = static_cast<char*> (mmap(0, data_size, PROT_READ, MAP_SHARED, fd, 0));
		if (map == reinterpret_cast<char*> (-1))    // MAP_FAILED
		{
			pcl_close(fd);
			qCritical() << "Error preparing mmap for binary PCD file, " << file_name.c_str();
			return NULL;
		}
#endif

		// ---[ Binary compressed mode only ]--
		if (data_type == 2)
		{
			// Uncompress the data first
			unsigned int compressed_size, uncompressed_size;
			memcpy(&compressed_size, &map[data_idx + 0], sizeof(unsigned int));
			memcpy(&uncompressed_size, &map[data_idx + 4], sizeof(unsigned int));
			qDebug() << "Read a binary compressed file with " << compressed_size
				<< " bytes compressed and " << uncompressed_size << " original.\n";
			// For all those weird situations where the compressed data is actually LARGER than the uncompressed one
			// (we really ought to check this in the compressor and copy the original data in those cases)
			if (data_size < compressed_size || uncompressed_size < compressed_size)
			{
				qDebug() << "Allocated data size (" << data_size << ") or uncompressed size ("
					<< uncompressed_size << ") smaller than compressed size (" << compressed_size << "). Need to remap.\n";
#ifdef _WIN32
				UnmapViewOfFile(map);
				data_size = compressed_size + data_idx + 8;
				map = static_cast<char*>(MapViewOfFile(fm, FILE_MAP_READ, 0, 0, data_size));
#else
				munmap(map, data_size);
				data_size = compressed_size + data_idx + 8;
				map = static_cast<char*> (mmap(0, data_size, PROT_READ, MAP_SHARED, fd, 0));
#endif
			}

			if (uncompressed_size != cloud_data_size)
			{
				qWarning() << "The estimated cloud.data size (" << cloud_data_size <<
					") is different than the saved uncompressed value (" << uncompressed_size << ")! Data corruption?";
			}

			char *buf = static_cast<char*> (malloc(data_size));
			// The size of the uncompressed data better be the same as what we stored in the header
			unsigned int tmp_size = pcl::lzfDecompress(&map[data_idx + 8], compressed_size, buf, static_cast<unsigned int> (data_size));
			if (tmp_size != uncompressed_size)
			{
				free(buf);
				pcl_close(fd);
				qCritical() << "Size of decompressed lzf data (" << tmp_size
					<< ") does not match value stored in PCD header (" << uncompressed_size << "). Errno: " << errno;
				return NULL;
			}

			size_t bufptr = 0;
			for (int d = 0; d < fields.size(); d++)
			{
				if (fields[d].name != "_")
				{
					switch (fields[d].datatype)
					{
					// MACRO START
#					define InsertField_BC(type)																			\
						type *fieldData = reinterpret_cast<type*>(buf + bufptr);										\
						type *tupleData = new type[fields[d].count];													\
						vtkAOSDataArrayTemplate<type>* tdata = static_cast<vtkAOSDataArrayTemplate<type>*>(data[d]);	\
						if (fields[d].count == 1)																		\
						{																								\
							for (unsigned int c = 0; c < nr_points; c++)												\
								tdata->SetVariantValue(c, fieldData[c]);												\
						}																								\
						else																							\
						{																								\
							for (unsigned int c = 0; c < nr_points; c++)												\
							{																							\
								for (unsigned int t = 0; t < fields[d].count; ++t)										\
									tupleData[t] = fieldData[c * fields[d].count + t];									\
								tdata->SetTypedTuple(c, tupleData);														\
							}																							\
						}																								\
						delete tupleData
					// MACRO END
					acTemplateMacro(InsertField_BC(itype));
					}
				}
				bufptr += fields[d].count * fields[d].size * nr_points;
			}

			free(buf);
		}
		else
		{
			// Set the pointer to each 'xyz' of xyzxyzxyz...
			std::vector<char*> pters(nr_points);
			char *buf = &map[0] + data_idx;
			size_t toff = 0;
			for (size_t i = 0; i < pters.size(); ++i)
			{
				pters[i] = &buf[toff];
				toff += point_step;
			}
			// Copy data
			toff = 0;
			for (int d = 0; d < fields.size(); d++)
			{
				if (fields[d].name != "_")
				{
					switch (fields[d].datatype)
					{
					// MACRO START
#					define InsertField_B(type)																			\
						type *tupleData = new type[fields[d].count];													\
						vtkAOSDataArrayTemplate<type>* tdata = static_cast<vtkAOSDataArrayTemplate<type>*>(data[d]);	\
						if (fields[d].count == 1)																		\
							for (unsigned int c = 0; c < nr_points; c++)												\
							{																							\
								type *fieldData = reinterpret_cast<type*>(pters[c] + toff);								\
								tdata->SetVariantValue(c, *fieldData);													\
							}																							\
						else																							\
							for (unsigned int c = 0; c < nr_points; c++)												\
							{																							\
								type *fieldData = reinterpret_cast<type*>(pters[c] + toff);								\
								for (unsigned int t = 0; t < fields[d].count; ++t)										\
									tupleData[t] = fieldData[t];														\
								tdata->SetTypedTuple(c, tupleData);														\
							}																							\
						delete tupleData
					// MACRO END
					acTemplateMacro(InsertField_B(itype));
					}
				}
				toff += fields[d].count * fields[d].size;
			}
		}

		// Unmap the pages of memory
#ifdef _WIN32
		UnmapViewOfFile(map);
		CloseHandle(fm);
#else
		if (munmap(map, data_size) == -1)
		{
			pcl_close(fd);
			qCritical() << "Munmap failure";
			return NULL;
		}
#endif
	}

	// Add points data
	vtkSmartPointer<vtkPoints> points = vtkPoints::New();
	points->SetNumberOfPoints(nr_points);
	int coord_idx[3];
	// Init point set
	vtkSmartPointer<vtkPointCloudType> pointSet = vtkPointCloudType::New();
	vtkSmartPointer<vtkPointData> pointData = pointSet->GetPointData();

	// Add fields
	for (int i = 0; i < fields.size();i++)
	{
		if (fields[i].name == "x")
			coord_idx[0] = i;
		else if (fields[i].name == "y")
			coord_idx[1] = i;
		else if (fields[i].name == "z")
			coord_idx[2] = i;
		else
			pointData->AddArray(data[i]);
	}

	// Add points
	if (fields[0].datatype == fields[1].datatype && fields[1].datatype == fields[2].datatype)
	{
		points->SetDataType(fields[0].datatype);
		switch (fields[0].datatype)
		{
		//MACRO Start
#		define CAST_POINT(type)																						   \
			vtkAOSDataArrayTemplate<type> *coordx = static_cast<vtkAOSDataArrayTemplate<type>*>(data[coord_idx[0]]);   \
			vtkAOSDataArrayTemplate<type> *coordy = static_cast<vtkAOSDataArrayTemplate<type>*>(data[coord_idx[1]]);   \
			vtkAOSDataArrayTemplate<type> *coordz = static_cast<vtkAOSDataArrayTemplate<type>*>(data[coord_idx[2]]);   \
			for (int i = 0; i < nr_points; i++)																		   \
				points->SetPoint(i, coordx->GetValue(i), coordy->GetValue(i), coordz->GetValue(i))
		//MACRO End
		acTemplateMacro(CAST_POINT(itype));
		}
	}
	else
	{
		qWarning() << "The type of x, y, z are not same, convert all to double!";
		vtkAbstractArray *coordx = data[coord_idx[0]];
		vtkAbstractArray *coordy = data[coord_idx[1]];
		vtkAbstractArray *coordz = data[coord_idx[2]];
		for (int i = 0; i < nr_points; i++)
			points->SetPoint(i, coordx->GetVariantValue(i).ToDouble(),
								coordy->GetVariantValue(i).ToDouble(),
								coordz->GetVariantValue(i).ToDouble());
	}
	pointSet->SetPoints(points);

	//Add point vertices
	vtkSmartPointer<vtkIdTypeArray> cells = vtkIdTypeArray::New();
	cells->SetNumberOfValues(nr_points * 2);
	vtkIdType* ids = cells->GetPointer(0);
	for (vtkIdType i = 0; i < nr_points; ++i)
	{
		ids[i * 2] = 1;
		ids[i * 2 + 1] = i;
	}

	vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
	cellArray->SetCells(nr_points, cells.GetPointer());
	pointSet->SetVerts(cellArray);

	return pointSet;
}
