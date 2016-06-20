/*
 * list_model.cpp
 *
 *  Created on: 20 de jun de 2016
 *      Author: carlosfaruolo
 */

#include "list_model.hpp"

using std::vector;
using std::string;

using WidgetsExtra::DefaultUIListModel;


DefaultUIListModel::DefaultUIListModel()
: data()
{}

DefaultUIListModel::DefaultUIListModel(const std::vector<std::string>& data)
: data(data)
{}

string DefaultUIListModel::getStringfiedElementAt(unsigned index)
{
	return data.at(index);
}

unsigned DefaultUIListModel::size()
{
	return data.size();
}
