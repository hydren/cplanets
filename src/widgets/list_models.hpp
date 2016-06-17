/*
 * list_win_models.hpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_LIST_MODELS_HPP_
#define WIDGETS_LIST_MODELS_HPP_

#include "futil/futil.hpp"

#include <vector>
#include <string>

namespace WidgetsExtra
{
	template <typename StringCastableType>
	std::string stringfy_by_cast(StringCastableType value)
	{
		return (std::string) value;
	}

	template <typename StringCastableType>
	std::string stringfy_by_cast_ptr(StringCastableType* value)
	{
		return (std::string) *value;
	}

	template <typename StringableType, std::string(StringableType::*stringfyMethod)()>
	std::string stringfy_by_method(StringableType value)
	{
		return (value.*stringfyMethod)();
	}

	template <typename StringableType, std::string(StringableType::*stringfyMethod)() const>
	std::string stringfy_by_method_const(StringableType value)
	{
		return (value.*stringfyMethod)();
	}

	template <typename StringableType, std::string(StringableType::*stringfyMethod)()>
	std::string stringfy_by_method_ptr(StringableType* value)
	{
		return (value->*stringfyMethod)();
	}

	template <typename StringableType, std::string(StringableType::*stringfyMethod)() const>
	std::string stringfy_by_method_const_ptr(StringableType* value)
	{
		return (value->*stringfyMethod)();
	}

	/// A model for a list of elements to be displayed by a UI element like ListWin.
	struct UIListModel
	{
		virtual std::vector<std::string> getStringfiedList() abstract;
		virtual ~UIListModel() {};
	};

	/// Default implementation of UIListModel. The list's elements are standard strings.
	struct DefaultUIListModel extends UIListModel
	{
		DefaultUIListModel() : data() {}
		DefaultUIListModel(const std::vector<std::string>& data) : data(data) {}

		virtual std::vector<std::string> getStringfiedList() { return data; }

		protected:
		std::vector<std::string> data;
	};

	/// A more customizable implementation of UIListModel.
	/// StringableType should be a typename that is possible to be cast to a std::string or else a pointer to a function that peforms the operation needs to be specified.
	/// If provided, the function provided must receive a StringableType instance and return a std::string (most likely a string representation of the instance).
	/// This string returning operation is referred as a 'stringfy' operation, similiar to Java's toString() method.
	/// There are pre-baked functions defined in <todo insert location here>: stringfy_by_cast, stringfy_by_method, and others.
	template <typename StringableType>
	struct StringableTypeUIListModel extends UIListModel
	{
		StringableTypeUIListModel()
		: data(), stringfyFunction(stringfy_by_cast<StringableType>) {}

		StringableTypeUIListModel(const std::vector<StringableType>& data)
		: data(data), stringfyFunction(stringfy_by_cast<StringableType>) {}

		StringableTypeUIListModel(std::string (*stringfyFunction)(StringableType))
		: data(), stringfyFunction(stringfyFunction) {}

		StringableTypeUIListModel(const std::vector<StringableType>& data, std::string (*stringfyFunction)(StringableType))
		: data(data), stringfyFunction(stringfyFunction) {}

		virtual std::vector<std::string> getStringfiedList()
		{
			std::vector<std::string> lines;
			foreach(StringableType&, obj, typename std::vector<StringableType>, data)
			{
				lines.push_back(stringfyFunction(obj));
			}
			return lines;
		}

		protected:
		std::vector<StringableType> data;
		std::string (*stringfyFunction)(StringableType value);  //todo should stringfyFunction be a function that receives value by reference
	};

}



#endif /* WIDGETS_LIST_MODELS_HPP_ */
