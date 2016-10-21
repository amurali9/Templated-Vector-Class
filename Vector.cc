// Implementation of the templated Vector class
// ECE 6122 Lab3
// Abhinav Murali

#include <iostream>
#include "Vector.h"
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
	elements = NULL;
	count = 0;
	reserved = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
	count = rhs.count;
	reserved = rhs.reserved;
	elements = (T*)malloc(count*sizeof(T));
	for (size_t i = 0; i < count; ++i)
	{
		new (&elements[i]) T(rhs[i]);
	}
	
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
	for (size_t i = 0; i < count; ++i)
    {
    	elements[i].~T();
    } 
    free(elements);
	count = rhs.count;
	reserved = rhs.reserved;
	
	elements = (T*)malloc(sizeof(T) * rhs.count);
	for (size_t i = 0; i < count; ++i)
	{
		new (&elements[i]) T(rhs[i]);
	}

}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ 
	// Initialize with reserved memory but do not add elements yet
	elements = (T*)malloc(sizeof(T) * nReserved);	
	count 	 = 0;
	reserved = nReserved;
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ 
	// Initialize with "n" copies of "t"
	count 	 = n;
	reserved = 0;
	
	elements = (T*)malloc(count*sizeof(T));
	for (size_t i = 0; i < count; ++i)
	{
		new (&elements[i]) T(t);
	}
	
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{	
	//Reserve memory for n elements and copy the elements into the new array
	T* new_elements = (T*)malloc(n*sizeof(T));
	
	for (size_t i = 0; i < count; ++i)
	{
		new (&new_elements[i]) T(elements[i]);
		elements[i].~T();
	}
	
	// Free the memory from old elements and set the pointer to the new elements array
	free(elements);
	elements = new_elements;
	reserved = n;
	
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
	//Call the destructor to clear the memory only if elements is non-empty.
	if(elements != NULL){
		for (size_t i = 0; i < count; ++i)
    	 {
    	   elements[i].~T();
   		 }    
    
     free(elements);
     elements = NULL;
     count = 0;
     reserved = 0;		
   }

}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
	//Call reserve to allocate memory for count+1 elements and then copy the new element at the back. Otherwise just add it to the end
	if (count == reserved)
	{
		Reserve(count+1);
	}
	new (&elements[count]) T(rhs);
	count += 1;
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
	// We have extra space in memory. 
	if (count < reserved)
	{
		for (size_t i = count; i > 0; --i)
		{
			elements[i] = elements[i-1];
		}
		new (&elements[0]) T(rhs);
		count++;
	}
	else	// Count == reserved. Assign new memory and copy the elements 
	{
		T* new_elements = (T*)malloc(sizeof(T) * (count+1));
		for (size_t i = count; i > 0; --i)
		{
			new (&new_elements[i]) T(elements[i-1]);
			elements[i-1].~T();
		}
		free(elements);
		elements = new_elements;
		new (&elements[0]) T(rhs);
		count += 1;
		reserved = count;
	}	
}

template <typename T>
void Vector<T>::Pop_Back()
{ 
	// Remove last element if elements is non-empty
	if(elements != NULL){
	  	count--;
    	elements[count].~T();		
	}

}

template <typename T>
void Vector<T>::Pop_Front()
{ 
	// Copy the elements from index 1:end. Overwrite them into indices 0:end-1
	for (size_t i = 0; i < count-1; ++i)
	{
		elements[i].~T();
		new (&elements[i]) T(elements[i+1]);
	}

	//Remove last element. Count is set within Pop_Back
	Pop_Back();
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
	return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
	return elements[count-1];
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{
	return this->elements[i];
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{
	return this->elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
	return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
	return count == 0;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
	for (size_t i = 0; i < count; ++i)
    {
    	elements[i].~T();
    }  
	count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  return VectorIterator<T>(elements + count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
    size_t index;
    
    //Get the index of the element to be removed
    for (size_t i = 0; i < count; ++i)
    {
    	if (&elements[i] == it.current)
    	{
    		index = i;
    		break;
    	}
    }
    
    //Copy the elements with index > pos and move it up
    for (size_t i = index; i < count-1; ++i)
    {
    	elements[i].~T();
    	new (&elements[i]) T(elements[i+1]);
    }
    
    //Reduce the count and delete the last element pos
    count--;
    elements[count].~T();
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
	size_t index;
    
    //Find the index of the position to be inserted
    for (size_t i = 0; i < count; ++i)
    {
    	if (&elements[i] == it.current)
    	{
    		index = i;
    		break;
    	}
    }
    
    //We have space in memory. Slide the array 'down' to make way for the element at the index position
	if (count < reserved)
	{
		for (size_t i = count; i > index; --i)
		{
			elements[i] = elements[i-1];
		}
		new (&elements[index]) T(rhs);
		count++;
	}
	else // Count == reserved. Assign new array with memory = count+1 and copy the elements carefully
	{
		T* new_elements = (T*)malloc(sizeof(T) * (count+1));
		for (size_t i = 0; i < index; ++i)
		{
			new (&new_elements[i]) T(elements[i]);
			elements[i].~T();
		}

		for (size_t i = count; i > index; --i)
		{
			new (&new_elements[i]) T(elements[i-1]);
			elements[i-1].~T();
		}
		free(elements);
		elements = new_elements;
		new (&elements[index]) T(rhs);
		count += 1;
		reserved = count;
	}	
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
	current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	++current;
	return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	VectorIterator<T> tmp(*this); 
	++current; 
	return tmp;
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	return current != rhs.current;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	return current == rhs.current;
}
