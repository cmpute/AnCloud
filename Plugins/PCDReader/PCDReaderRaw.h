// Edit from pcl/io/pcd_io
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
*   * Neither the name of the copyright holder(s) nor the names of its
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

#ifndef PCDReaderRaw_h
#define PCDReaderRaw_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include <functional>

typedef vtkPolyData vtkPointCloudType;

class PCDReaderRaw
{
public:
	struct PointField {
		uint8_t datatype;
		uint8_t count;
		uint8_t size;
		std::string name;
	};

	/**
	 * return : NULL if failed
	 */
    static vtkSmartPointer<vtkPointCloudType> readFile(const char *file_name, std::function<void(double)> progress, const int offset = 0);

private:
	/**
	 * file_name[in] : path fo file to read
	 * data_type[out] : storage type
	 * fields[out] : field list
	 * data_idx[out] : data offset in the file
	 * offset[in] : header offset in the file
	 * return : the number of points in the data
	 */
    static unsigned int readHeader (const char *file_name, int &data_type, std::vector<PointField> &fields,
					unsigned int &data_idx, unsigned int &point_step, const int offset = 0);
};

#endif // PCDReaderRaw_h