/*
 * program_io.hpp
 *
 *  Created on: 3 de mai de 2016
 *      Author: carlosfaruolo
 */

#ifndef PROGRAM_IO_HPP_
#define PROGRAM_IO_HPP_

#include "physics/universe2d.hpp"

namespace ApplicationIO
{
	enum FileFormat {
		FORMAT_DEFAULT,

//		FORMAT_SER,

//		FORMAT_CSV,
//		FORMAT_XML,
//		FORMAT_ODS,

		FORMAT_TXT,

//		FORMAT_PROP,
//		FORMAT_JSON,
//		FORMAT_YAML,
	};

	void save(const Universe2D& universe, const std::string& filename);
	void save(const Universe2D& universe, const std::string& filename, FileFormat format);
	void save(const Universe2D& universe, const std::string& filename, bool appendExtension);
	void save(const Universe2D& universe, const std::string& filename, FileFormat format, bool appendExtension);
	Universe2D* load(std::string filename, FileFormat ff=FORMAT_DEFAULT);
}



#endif /* PROGRAM_IO_HPP_ */
