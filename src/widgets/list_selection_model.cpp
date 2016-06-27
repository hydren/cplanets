/*
 * list_selection_model.cpp
 *
 *  Created on: 27 de jun de 2016
 *      Author: carlosfaruolo
 */

#include "list_selection_model.hpp"

using WidgetsExtra::ListSelectionModel;
using std::vector;

void ListSelectionModel::select(unsigned index)
{
	selection.at(index) = true;
}

void ListSelectionModel::select(unsigned startIndex, unsigned endIndex)
{
	for(unsigned i = startIndex; i <= endIndex; i++)
		selection.at(i) = true;
}

void ListSelectionModel::unselect(unsigned index)
{
	selection.at(index) = false;
}

void ListSelectionModel::unselect(unsigned startIndex, unsigned endIndex)
{
	for(unsigned i = startIndex; i <= endIndex; i++)
		selection.at(i) = false;
}

void ListSelectionModel::toogle(unsigned index)
{
	selection.at(index).flip();
}

void ListSelectionModel::toogle(unsigned startIndex, unsigned endIndex)
{
	for(unsigned i = startIndex; i <= endIndex; i++)
		selection.at(i).flip();
}

bool ListSelectionModel::isSelected() const
{
	for(unsigned i = 0; i < selection.size(); i++)
		if(selection[i] == true)
			return true;
	return false;
}

bool ListSelectionModel::isSelected(unsigned index) const
{
	return selection.at(index);
}

unsigned ListSelectionModel::getSelected() const
{
	for(unsigned i = 0; i < selection.size(); i++)
		if(selection[i] == true)
			return i;
	return 0;
}

unsigned *ListSelectionModel::getSelected(unsigned & n) const
{
	vector<unsigned> selectedOnes = getSelectedIndexes();
	n = selectedOnes.size();
	unsigned* arr = new unsigned[n];
	for(unsigned i = 0; i < n; i++)
		arr[i] = selectedOnes[i];
	return arr;
}

vector<unsigned> ListSelectionModel::getSelectedIndexes() const
{
	vector<unsigned> selectedOnes;

	for(unsigned i = 0; i < selection.size(); i++)
		if(selection[i] == true)
			selectedOnes.push_back(i);

	return selectedOnes;
}

void ListSelectionModel::setSelected(unsigned index)
{
	clear();
	select(index);
}

void ListSelectionModel::setSelected(unsigned index, unsigned endIndex)
{
	clear();
	select(index, endIndex);
}

void ListSelectionModel::setSelected(const unsigned * indexes, unsigned n)
{
	clear();
	for(unsigned i = 0; i < n; i++)
		select(indexes[i]);
}

void ListSelectionModel::setSelectedIndexes(const std::vector<unsigned>& indexes)
{
	for(unsigned i = 0; i < indexes.size(); i++)
		select(indexes[i]);
}

void ListSelectionModel::clear()
{
	selection.assign(selection.size(), false);
}

void ListSelectionModel::shift(bool up, unsigned index)
{
	if(up)
		selection.erase(selection.begin() + index);
	else
		selection.insert(selection.begin() + index, false);
}

void ListSelectionModel::fit(unsigned size)
{
	while(size != selection.size())
		if(size > selection.size())
			selection.push_back(false);
		else
			selection.pop_back();
}
