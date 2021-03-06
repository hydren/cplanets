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
		FORMAT_TXT,  //Plain text

		FORMAT_CSV, //Comma separated values
//		FORMAT_XML,
//		FORMAT_ODS,
//		FORMAT_SER, //Java Serialized Object File
//		FORMAT_PROP, //.properties file
//		FORMAT_JSON,
//		FORMAT_YAML,
	};

	void save(const Universe2D& universe, const std::string& filename);
	void save(const Universe2D& universe, const std::string& filename, FileFormat format);
	void save(const Universe2D& universe, const std::string& filename, bool appendExtension);
	void save(const Universe2D& universe, const std::string& filename, FileFormat format, bool appendExtension);
	Universe2D* load(std::string filename);
	Universe2D* load(std::string filename, FileFormat format);

	void save_txt(const Universe2D& universe, const std::string& filename);
	Universe2D* load_txt(const std::string& filename);

	void save_csv(const Universe2D& universe, const std::string& filename);
	Universe2D* load_csv(const std::string& filename);
}



#endif /* PROGRAM_IO_HPP_ */
