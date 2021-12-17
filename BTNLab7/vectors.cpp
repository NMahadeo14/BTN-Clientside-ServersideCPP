#include <iostream>
#include <string>
#include "vectors.h"

//constructor for an empty Vector
Vector::Vector() {
	this->name = "";
	this->next = nullptr;
	this->size = 0;
	this->elements = nullptr;
}

//constructor for a named Vector
Vector::Vector(std::string name) {
	this->name = name;
	this->next = nullptr;
	std::cout << "Setting vector: " << name << std::endl;
	std::cout << "Enter vector size: ";
	std::cin >> this->size;
	this->elements = new int[this->size];
	for (int i = 0; i < this->size; i++) {
		std::cout << "Enter element " << i << " : ";
		std::cin >> this->elements[i];
	}
}

//destructor for Vector, note that it frees up *elements
Vector::~Vector() {
	std::cout << "Deleted vector: " << this->name << std::endl;
	delete[] this->elements;
}

//constructor for List
List::List() {
	head = nullptr;
	current = nullptr;
}

void List::add_vector(std::string name) {
	// write your code here
	Vector* copyVector = new Vector(name);

	if (head == nullptr)
	{
		head = copyVector;
		current = copyVector;
	}
	else
	{
		current->next = copyVector;
		current = copyVector;
	}
	
}

bool List::print_vector(std::string name) {
	// write your code here
	
	if (name == head->name)
	{
		std::cout << "Printing " << name << std::endl;

		for (int i = 0; i < head->size; i++)
		{
			std::cout << head->elements[i] << ",";
		}

		std::cout << std::endl;
		return true;
	}

	if (name == head->next->name)
	{
		std::cout << "Printing " << name << std::endl;

		for (int i = 0; i < head->next->size; i++)
		{
			std::cout << head->next->elements[i] << ",";
		}

		std::cout << std::endl;
		return true;
	}

	if (name == head->next->next->name)
	{
		std::cout << "Printing " << name << std::endl;

		for (int i = 0; i < head->next->next->size; i++)
		{
			std::cout << head->next->next->elements[i] << ",";
		}

		std::cout << std::endl;
		return true;
	}

	return false;

}

//removes an item from the List
bool List::remove_vector(std::string name) {
	// write your code here

	if (name == head->name)
	{
		Vector* copy = head->next;
		delete head;
		head = copy;
		current = copy;
		return true;
	}

	if (name == head->next->name)
	{
		Vector* copy = head->next->next;
		delete head->next;
		head->next = copy;
		current->next = copy;
		return true;
	}

	return false;
}



bool List::append_vector(std::string name) {
	// write your code here

	int newSize;

	std::cout << "Enter new Vector size: ";
	std::cin >> newSize;

	int* newElements = new int[newSize];

	for (int i = 0; i < newSize; i++)
	{
		newElements[i] = '\0';
	}

	for (int i = 0; i < head->size; i++)
	{
		newElements[i] = head->elements[i];
	}

	if (head->name == name)
	{
		head->size = newSize;
		head->elements = new int[newSize];

		for (int i = 0; i < head->size; i++) {
			
			if (newElements[i] == '\0')
			{
				std::cout << "Enter element " << i << " : ";
				std::cin >> newElements[i];
			}	
		}

		for (int i = 0; i < newSize; i++)
		{
			head->elements[i] = newElements[i];
		}

		delete[] newElements;

		return true;
	}

	return false;

}