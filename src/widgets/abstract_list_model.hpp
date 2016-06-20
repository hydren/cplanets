/*
 * abstract_list_model.hpp
 *
 *  Created on: 20 de jun de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_ABSTRACT_LIST_MODEL_HPP_
#define WIDGETS_ABSTRACT_LIST_MODEL_HPP_

#include "futil/futil.hpp"

#include <string>

namespace WidgetsExtra
{
	/// A model for a list of elements to be displayed by a UI element like ListWin.
	struct UIListModel
	{
		virtual ~UIListModel() {};
		virtual std::string getStringfiedElementAt(unsigned index) abstract;
		virtual unsigned size() abstract;
	};
}

#endif /* WIDGETS_ABSTRACT_LIST_MODEL_HPP_ */
