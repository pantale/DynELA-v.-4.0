/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlKernel_List_h__
#define __dnlKernel_List_h__

#include <cstring>
#include <iostream>
#include <Errors.h>
#include <Defines.h>

#define DEFAULT_stack_size 10 // Default stack size of the List
#define DEFAULT_stack_inc 10  // Default stack increment of the List

template <class Type>
class ListIndex;

/*
@LABEL:List::List
@SHORT:Management of objects as List.
This class is used to store all type of object and manipulate them as a list (for example: list of Nodes, Elements, Boundary conditions,...).
This List is a dynamic one, the initialization is performed with a default stack size defined by DEFAULT\_stack\_size, as soon as there is no more space left to store a new object,
the List size is increased with respect to the DEFAULT\_stack\_inc value.
@END
*/
template <class Type>
class List
{
    friend class ListIndex<Type>;

private:
    long _size;     // Current size of the list (number of objects refered by the List)
    long _stack;    // Current stack size of the List (Space available for objects storage)
    long _stackInc; // Current stack increment for the List
    long _current;  // The current index of the current object in the List
    Type *_ptr;     // A pointer to the current object in the List
#ifdef VERIF_bounds
    bool locked = false; // Verification of the bounds of the list
#endif

public:
    List(const long = DEFAULT_stack_size);
    virtual ~List();

    // Interface methods excluded from SWIG
#ifndef SWIG
#endif

    // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

    bool contains(const Type) const;
    bool operator!=(const List<Type> &) const;
    bool operator==(const List<Type> &) const;
    List<Type> operator<<(const Type);
    long &stackIncrement();
    long index(const Type) const;
    long objectSize();
    long size() const;
    long stack() const;
    Type &operator()(const long);
    Type current();
    Type currentDown();
    Type currentUp();
    Type first();
    Type initLoop();
    Type last();
    Type next();
    Type operator()(const long) const;
    Type previous();
    Type search(long (*)(const Type, const long), const long) const;
    virtual void add(const Type);
    virtual void flush();
    virtual void insert(const Type, long);
    void close();
    void del(long, long);
    void del(long);
    void delAfter(long);
    void delBefore(long);
    void endLoop();
    void inverse();
    void print(std::ostream &) const;
    void redim(const long);
    void sort(bool (*)(const Type, const Type));
};

/*
@LABEL:ListIndex::ListIndex
@SHORT:Management of objects as ListIndex.
This class is used to store all type of object and manipulate them as a listIndex (for example: listIndex of Nodes, Elements, Boundary conditions,...).
This ListIndex is a dynamic one, the initialization is performed with a default stack size defined by DEFAULT\_stack\_size, as soon as there is no more space left to store a new object,
the ListIndex size is increased with respect to the DEFAULT\_stack\_inc value.
@END
*/
template <class Type>
class ListIndex : public List<Type>
{
    bool _sorted;    // Bool flag defining that the current ListIndex is _sorted (All elements are in _sorted with increasing Id)
    bool _compacted; // Bool flag defining that the current ListIndex is _compacted (No hole in the list of objects)

public:
    ListIndex(const long = DEFAULT_stack_size);
    ~ListIndex();

    // Interface methods excluded from SWIG
#ifndef SWIG
#endif

    // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

    bool compacted() const;
    bool sorted() const;
    long IAppN(const long) const;
    Type AppN(const long) const;
    void add(const Type);
    void compact();
    void del(const Type);
    void del(const Type, const Type);
    void del(long);
    void del(long, long);
    void delAfter(const Type);
    void delAfter(long);
    void delBefore(const Type);
    void delBefore(long);
    void flush();
    void forceSort();
    void insert(const Type, long);
    void sort();
    void sort(bool (*)(const Type, const Type));
};

/*
@LABEL:List::List(long size)
@SHORT:Constructor of the List class.
@ARG:long & size & Initial size of the list (default value DEFAULT\_stack\_inc).
This constructor allocates the default memory for an instance of the List class.
If the size of the list is not specified, the default size is taken into account, which is defined by the value of DEFAULT\_stack\_size.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
List<Type>::List(const long size)
//-----------------------------------------------------------------------------
{
    // Definition of the default constants
    _stack = size;
    _stackInc = DEFAULT_stack_inc;
    _size = 0;
    _current = 0;

    // Memory allocation for the List
    _ptr = new Type[_stack];

#ifdef VERIF_alloc
    if (_ptr == NULL)
        fatalError("List <Type>::List",
                   "Memory allocation error\n"
                   "May be out of memory on this system\n");
#endif
}

//-----------------------------------------------------------------------------
template <class Type>
List<Type>::~List()
//-----------------------------------------------------------------------------
{
}

/*
@LABEL:List::redim(long size)
@SHORT:Resize the storage space of a List.
@ARG:long & size & New size of the list.
This method is used to increase or decrease the size of a list.
If the proposed new size is smaller than the minimum size needed to store the current elements of the list, an error is generated.\\
This method should generally not be called by the user (unless the user has mastered the trick).
This method is heavily used internally by the other methods of the class.
In case the user does not master this kind of operation sufficiently, it is better to let the class manage its own memory allocations.\\
A possible use of this method is memory pre-allocation, when the number of objects that will be stored in the list is known in advance.
The size of the list is then adjusted to this value, which avoids dynamic size adjustment operations that take CPU time.
Of course, dynamic allocation mechanisms exist and you can exceed this value.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::redim(const long size)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (size < _size)
        fatalError("template <class Type> void List <Type>::redim(const long)\n",
                   "new size < actual size means to truncate the List\n"
                   "This is not allowed with redim method\n"
                   "Must use a 'del' method");
#endif

    // set the size
    _stack = size;

    // Dynamic allocation of the storage space
    Type *ptr = new Type[_stack];

#ifdef VERIF_alloc
    if (ptr == NULL)
        fatalError("List <Type>::redim",
                   "memory allocation error\nMay be out of memory on this system\n");
#endif

    // Copy the memory
    memcpy(ptr, _ptr, _size * sizeof(Type));

    // Delete of the old pointer
    delete[] _ptr;

    // Re-affectation of the pointer
    _ptr = ptr;
}

/*
@LABEL:List::close()
@SHORT:This method closes the List, i.e. resize the stack size to the number of elements to save memory.
This method is used to adjust the size of the list according to the number of real objects in the list.
This method allows to recover memory space, mainly for small lists.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::close()
//-----------------------------------------------------------------------------
{
    // on fait un redim a la size_ reelle de la pile
    redim(_size);
}

/*
@LABEL:List::operator()(long index)
@SHORT:This is the accessor to the elements of the list.
@ARG:long & index & Index of the element to get in the list.
This method is used to access items on the list.
This access is both read and write.
This method returns element [i] of the list.
The baseline is 0 (first element of index 0) as usual in C and C++.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type &List<Type>::operator()(const long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((index < 0) || (index >= _size))
    {
        std::cerr << "Fatal Error in template <class Type> Type& List <Type>::operator ()\n";
        std::cerr << "long " << index << " out of allowd range {0-" << _size - 1 << "}\n";
        exit(-1);
    }
#endif

    return _ptr[index];
}

//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::operator()(const long index)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((index < 0) || (index >= _size))
    {
        std::cerr << "Fatal Error in template <class Type> Type& List <Type>::operator ()\n";
        std::cerr << "long " << index << " out of allowd range {0-" << _size - 1 << "}\n";
        exit(-1);
    }
#endif

    return _ptr[index];
}

/*
@LABEL:List::initLoop()
@SHORT:Initialization of a loop on the list.
This method has to be called before a loop using one of the iterator methods for the List.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::initLoop()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (locked == true)
    {
        std::cerr << "Fatal Error in template <class Type> Type List<Type>::initLoop()\n";
        std::cerr << "Already in a previous loop\n";
        exit(-1);
    }
    locked = true;
#endif

    if (_size == 0)
        return NULL;

    return _ptr[_current = 0];
}

/*
@LABEL:List::initLoop()
@SHORT:Finalization of a loop on the list.
This method has to be called after a loop using one of the iterator methods for the List.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::endLoop()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    locked = false;
#endif
}

/*
@LABEL:List::next()
@SHORT:Next element in the list.
@RETURN:Next item in the list or NULL if it does not exist.
This method uses an internal list lookup mechanism to return the next element in the list.
To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::next()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (locked == false)
    {
        std::cerr << "Fatal Error in template <class Type> Type List<Type>::next()\n";
        std::cerr << "Not in a loop\n";
        exit(-1);
    }
#endif

    if (_current >= _size - 1)
        return NULL;

    return _ptr[++_current];
}

/*
@LABEL:List::currentUp()
@SHORT:Next element in the list.
@RETURN:Next item in the list or NULL if it does not exist.
This method uses an internal list lookup mechanism to return the element following the previous call in the list.
To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::currentUp()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (locked == false)
    {
        std::cerr << "Fatal Error in template <class Type> Type List<Type>::currentUp()\n";
        std::cerr << "Not in a loop\n";
        exit(-1);
    }
#endif

    if (_current >= _size)
        return NULL;

    return _ptr[_current++];
}

/*
@LABEL:List::currentDown()
@SHORT:Previous element in the list.
@RETURN:Previous item in the list or NULL if it does not exist.
This method uses an internal list lookup mechanism to return the element preceding the previous call in the list.
To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::currentDown()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (locked == false)
    {
        std::cerr << "Fatal Error in template <class Type> Type List<Type>::currentDown()\n";
        std::cerr << "Not in a loop\n";
        exit(-1);
    }
#endif

    if (_current < 0)
        return NULL;

    return _ptr[_current--];
}

/*
@LABEL:List::first()
@SHORT:First element in the list.
@RETURN:First item in the list or NULL if it does not exist.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::first()
//-----------------------------------------------------------------------------
{
    if (_size == 0)
        return NULL;

    return _ptr[_current = 0];
}

/*
@LABEL:List::last()
@SHORT:Last element in the list.
@RETURN:Last item in the list or NULL if it does not exist.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::last()
//-----------------------------------------------------------------------------
{
    if (_size == 0)
        return NULL;

    return _ptr[_current = _size - 1];
}

/*
@LABEL:List::previous()
@SHORT:Previous element in the list.
@RETURN:Previous item in the list or NULL if it does not exist.
This method uses an internal list lookup mechanism to return the element preceding the previous call in the list.
To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::previous()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (locked == false)
    {
        std::cerr << "Fatal Error in template <class Type> Type List<Type>::previous()\n";
        std::cerr << "Not in a loop\n";
        exit(-1);
    }
#endif

    if (_current == 0)
        return NULL;

    return _ptr[--_current];
}

/*
@LABEL:List::current()
@SHORT:Current element in the list.
@RETURN:Current item in the list or NULL if it does not exist.
This method uses an internal list lookup mechanism to return the current element in the list.
To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::current()
//-----------------------------------------------------------------------------
{
    if (_size == 0)
        return NULL;

    return _ptr[_current];
}

/*
@LABEL:List::size()
@SHORT:Current size of the list.
@RETURN:long.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
long List<Type>::size() const
//-----------------------------------------------------------------------------
{
    return _size;
}

/*
@LABEL:List::stack()
@SHORT:Stack size of the list.
@RETURN:long.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
long List<Type>::stack() const
//-----------------------------------------------------------------------------
{
    return _stack;
}

/*
@LABEL:List::stackIncrement()
@SHORT:Stack increment size of the list.
@RETURN:long.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
long &List<Type>::stackIncrement()
//-----------------------------------------------------------------------------
{
    return _stackInc;
}

/*
@LABEL:List::flush()
@SHORT:Empties the list.
This method empties the contents of the stack and returns its real size to zero and its stack size to DEFAULT\_stack\_size.
The stack is as good as new !!! (it's a rechargeable battery !)
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::flush()
//-----------------------------------------------------------------------------
{
    _stack = DEFAULT_stack_size;
    _size = 0;
    delete[] _ptr;
    _current = 0;
    _ptr = new Type[_stack];

#ifdef VERIF_alloc
    if (_ptr == NULL)
        fatalError("template <class Type> void List <Type>::flush()",
                   "fonction flush de template <class Type> List <Type>\n");
#endif
}

/*
@LABEL:List::operator<<(Type object)
@SHORT:Add an object to the list.
@ARG:Type & object & Object to add to the list.
This method adds an object to the list. The object is added to the end of the list, and the list size is automatically incremented if necessary.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
List<Type> List<Type>::operator<<(const Type object)
//-----------------------------------------------------------------------------
{
    add(object);
    return *this;
}

/*
@LABEL:List::insert(Type object, long index)
@SHORT:Insert an object in the list.
@ARG:Type & object & Object to insert into the list.
@ARG:long & index & Defines the position of the insertion in the list..
This method inserts an object to the list. The object is inserted at a given index in the list, and the list size is automatically incremented if necessary.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::insert(const Type object, long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (index > _size)
        fatalError("List <Type>::insert(long index)",
                   "index indice (%d) out of bounds (%d)\n", index, _size);
#endif

    // add the last object at the end
    add(_ptr[_size - 1]);

    // move from the end to the insertion point
    if (_size - 2 >= index)
        memmove(_ptr + index + 1, _ptr + index, (_size - index - 2) * sizeof(Type));

    // insert the object
    _ptr[index] = object;
}

/*
@LABEL:List::add(Type object)
@SHORT:Add an object to the list.
@ARG:Type & object & Object to add to the list.
This method adds an object to the list. The object is added to the end of the list, and the list size is automatically incremented if necessary.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // Test for memory reallocation
    if (_size >= _stack)
        redim(_stack + _stackInc);

    // Store the current index
    _current = _size;

    // Add the object
    _ptr[_size++] = object;
}

/*
@LABEL:List::inverse()
@SHORT:Reverse the order of the list.
This method reverses the order of the elements in the list.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::inverse()
//-----------------------------------------------------------------------------
{
    Type v;
    for (long i = 0; i < _size / 2; i++)
    {
        v = _ptr[i];
        _ptr[i] = _ptr[_size - i - 1];
        _ptr[_size - i - 1] = v;
    }
}

/*
@LABEL:List::del(long start, long stop)
@SHORT:Removes a set of elements from the List.
@ARG:long & start & First element to suppress from the list.
@ARG:long & stop & Last element to suppress from the list.
This method removes a set of items from the list.
This method is used to remove an entire segment from the list, by defining the start and end indexes of the segment in the list.
If the start and stop parameters are equal, only one element is deleted.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::del(long start, long stop)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (start > stop)
        fatalError("List <Type>::del(long start,long stop)",
                   "start indice %d > stop indice %d\n", start, stop);
    if (stop >= _size)
        fatalError("List <Type>::del(long start,long stop)",
                   "stop indice (%d) out of bounds (%d)\n", stop, _size);
#endif

    // recouvrement
    if (_size - stop - 1 > 0)
        memmove(_ptr + start, _ptr + (stop + 1), (_size - stop - 1) * sizeof(Type));

    // recalcul de la size_
    _size -= (stop - start + 1);
}

/*
@LABEL:List::del(long index)
@SHORT:Removes an element from the List.
@ARG:long & index & Index of the element to suppress from the list.
This method removes an items from the list.
This method is used to remove an item from the list, by defining the index of the element in the list.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::del(long index)
//-----------------------------------------------------------------------------
{
    List<Type>::del(index, index);
}

/*
@LABEL:List::delBefore(long index)
@SHORT:Removes a set of elements from the List.
@ARG:long & index & Index of the last element to suppress from the list.
This method removes all items in the list between the beginning of the list and the value given as an argument to this method.
This method is equivalent to del(0, index-1).
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::delBefore(long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (index - 1 >= _size)
        fatalError("List <Type>::delBefore(long index)",
                   "index indice (%d) out of bounds (%d)\n", index, _size);
#endif

    List<Type>::del(0, index - 1);
}

/*
@LABEL:List::delAfter(long index)
@SHORT:Removes a set of elements from the List.
@ARG:long & index & Index of the first element to suppress from the list.
This method deletes all items in the list between the value given as an argument to this method and the end of the list.
This method is equivalent to del(index+1, last()).
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::delAfter(long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (index + 1 >= _size)
        fatalError("List <Type>::delAfter(long index)",
                   "index indice (%d) out of bounds (%d)\n", index, _size);
#endif

    List<Type>::del(index + 1, _size - 1);
}

/*
  Comparison of two lists

  This method is used to compare two lists with each other. It tests for equality.
  - liste second list to compare to.
  Return : true if both lists are the same
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List<Type>::operator==(const List<Type> &liste) const
//-----------------------------------------------------------------------------
{
    // la comparaison porte sur la size_
    if (_size != liste._size)
        return (false);

    // et sur le contenu
    for (long i = 0; i < _size; i++)
        if (_ptr[i] != liste._ptr[i])
            return (false);

    return (true);
}

/*
  Comparison of two lists

  This method is used to compare two lists with each other. It tests for non equality.
  - liste second list to compare to.
  Return : true if both lists are different
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List<Type>::operator!=(const List<Type> &liste) const
//-----------------------------------------------------------------------------
{
    return !(*this == liste);
}

/*
@LABEL:List::objectSize()
@SHORT:Memory size of a list.
This method return the memory size a list, i.e. the memory size of the stack and of the List itself.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
long List<Type>::objectSize()
//-----------------------------------------------------------------------------
{
    return (sizeof(*this) + _stack * sizeof(_ptr));
}

/*
@LABEL:List::print(ostream output)
@ARG:ostream & output & Output stream.
@SHORT:Prints the content of a list.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::print(std::ostream &outputStream) const
//-----------------------------------------------------------------------------
{
    outputStream << "list " << _size << "/" << _stack << "={";
    for (long i = 0; i < _size; i++)
    {
        if (i != 0)
            std::cout << ",";
        outputStream << *_ptr[i];
    }
    outputStream << "}";
}

/*
@LABEL:List::print(Type object)
@ARG:Type & object & Object to search.
@RETURN:Type
@SHORT:Search an object in the List.
This method performs a simple search for an item in the list and returns an Index indicating the place of the object in the list.
If the object is not found, it returns the value -1.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
long List<Type>::index(const Type object) const
//-----------------------------------------------------------------------------
{
    // recherche bourrin de base
    for (long i = 0; i < _size; i++)
        if (_ptr[i] == object)
            return i;

    // bourrin pas trouve alors on retourne -1
    return -1;
}

/*
@LABEL:List::contains(Type object)
@ARG:Type & object & Object to search.
@RETURN:bool
@SHORT:Search an object in the List.
This method performs a simple search for an item in the list and returns a boolean according to the presence or or not of this object in the list.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List<Type>::contains(const Type object) const
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < _size; i++)
        if (_ptr[i] == object)
            return true;

    // Not found
    return false;
}

/*
@LABEL:ListIndex::ListIndex(long size)
@SHORT:Constructor of the ListIndex class.
@ARG:long & size & Initial size of the list (default value DEFAULT\_stack\_inc).
This constructor allocates the default memory for an instance of the List class.
If the size of the list is not specified, the default size is taken into account, which is defined by the value of DEFAULT\_stack\_size.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
ListIndex<Type>::ListIndex(const long size) : List<Type>(size)
//-----------------------------------------------------------------------------
{
    // Definition of the default constants
    _sorted = true;
    _compacted = true;
}

//-----------------------------------------------------------------------------
template <class Type>
ListIndex<Type>::~ListIndex()
//-----------------------------------------------------------------------------
{
}

/*
@LABEL:ListIndex::search(long (*f)(Type, long), long i)
@SHORT:Sort the list using a comparison function.
@ARG:long & (*f) & Function defined in the objects to sort the elements.
@ARG:long & i & Index of the object
This method is used to search for an item in the list using a dichotomous algorithm. This method returns the corresponding element in the list or the NULL value if the element is not in the list.
The usage may seem complex, but it is defined in the example below.
\begin{CppListing}
 class truc
{
  public:
  long z; // a value
};
ListIndex <truc*> listeTrucs; // the list
long compare(truc* p1, long in) // the comparing function
{
  return (p1->z - in); // comparison
}
...
{
...
listeTrucs.sort(compare,10); // seeks for the value 10
}
\end{CppListing}
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::search(long (*funct)(const Type, const long), const long index) const
//-----------------------------------------------------------------------------
{
    long i;

    // if size_ is zero returns NULL
    if (this->_size == 0)
        return NULL;

    // dichotomy
    long g, d, dx;
    g = 0;
    d = this->_size - 1;
    do
    {
        i = (g + d) / 2;
        dx = funct(this->_ptr[i], index);
        if (dx == 0)
            return this->_ptr[i];

        if (dx > 0)
            d = i - 1; // the right

        else
            g = i + 1; // the left

    } while (g <= d);

    return NULL;
}

/*
  Search for an element in the List

  This method is used to search for an item in the list. The search algorithm is based on the parameter \b _listIndex declared in the list objects.
  This method returns the corresponding element in the list or the NULL value if the element is not in the list.
  - index particulate value of the element to be searched for in the list
  Return : the corresponding element in the list or the value NULL if the element is not in the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type ListIndex<Type>::AppN(const long index) const
//-----------------------------------------------------------------------------
{
    long i;

    // if size_ is zeron returns NULL
    if (this->_size == 0)
        return NULL;

    // si elle est triee et compactee
    if (_compacted == true)
        if ((index >= 0) && (index < this->_size))
            return this->_ptr[index];

    // recherche selon le cas
    if (_sorted == true)
    {
        // dichotomy
        long g, d;
        g = 0;
        d = this->_size - 1;
        do
        {
            i = (g + d) / 2;
            if (this->_ptr[i]->_listIndex == index)
                return this->_ptr[i];

            if (index < this->_ptr[i]->_listIndex)
                d = i - 1; // the right

            else
                g = i + 1; // the left

        } while (g <= d);
    }
    else
    {
        // tri bete mais terriblement efficace falsen !!
        for (i = 0; i < this->_size; i++)
            if (this->_ptr[i]->_listIndex == index)
                return this->_ptr[i];
    }
    return NULL;
}

/*
  Search for an element in the List

  This method is used to search for an item in the list. The search algorithm is based on the parameter \b _listIndex declared in the list objects.
  This method returns the index of the corresponding element in the list or the value 0 if the element is not in the list.
  \warning The return value 0 can be confused with the first value in the ??? list. BUG ???
  - seekForValue particulate value of the element to be searched for in the list
  Return : the corresponding element in the list or the value NULL if the element is not in the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
long ListIndex<Type>::IAppN(const long index) const
//-----------------------------------------------------------------------------
{
    long i;

    // if size_ is zeron returns NULL
    if (this->_size == 0)
        return 0;

    // si elle est triee et compactee
    if (_compacted == true)
        if ((index >= 0) && (index < this->_size))
            return (index);

    // recherche selon le cas
    if (_sorted == true)
    {
        // dichotomy
        long g, d;
        g = 0;
        d = this->_size - 1;
        do
        {
            i = (g + d) / 2;
            if (this->_ptr[i]->_listIndex == index)
                return (i);

            if (index < this->_ptr[i]->_listIndex)
                d = i - 1; // the right

            else
                g = i + 1; // the left

        } while (g <= d);
    }
    else
    {
        // tri bete
        for (i = 0; i < this->_size; i++)
            if (this->_ptr[i]->_listIndex == index)
                return (i);
    }
    return 0;
}

/*
@LABEL:ListIndex::sorted()
@SHORT:test if the list is sorted.
@RETURN:bool
A sorted list is a list where all object have an increasing internal number.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
bool ListIndex<Type>::sorted() const
//-----------------------------------------------------------------------------
{
    return _sorted;
}

/*
@LABEL:ListIndex::compacted()
@SHORT:test if the list is compacted.
@RETURN:bool
A sorted list is a list where all object have a continuous increasing internal number.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
bool ListIndex<Type>::compacted() const
//-----------------------------------------------------------------------------
{
    return _compacted;
}

/*
@LABEL:ListIndex::flush()
@SHORT:Empties the list.
This method empties the contents of the stack and returns its real size to zero and its stack size to DEFAULT\_stack\_size.
The stack is as good as new !!! (it's a rechargeable battery !)
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::flush()
//-----------------------------------------------------------------------------
{
    this->_stack = DEFAULT_stack_size;
    this->_size = 0;
    _sorted = true;
    _compacted = true;
    delete[] this->_ptr;
    this->_current = 0;
    this->_ptr = new Type[this->_stack];

#ifdef VERIF_alloc
    if (this->_ptr == NULL)
        fatalError("template <class Type> void ListIndex <Type>::flush()",
                   "fonction flush de template <class Type> ListIndex <Type>\n");
#endif
}

/*
@LABEL:ListIndex::add(Type object)
@SHORT:Add an object to the list.
@ARG:Type & object & Object to add to the list.
This method adds an object to the list. The object is added to the end of the list, and the list size is automatically incremented if necessary.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // Test for memory reallocation
    if (this->_size >= this->_stack)
        this->redim(this->_stack + this->_stackInc);

    // test de tri
    if (_sorted == true)
        if (this->_size != 0)
        {
            if (this->_ptr[this->_size - 1]->_listIndex > object->_listIndex)
            {
                _sorted = false;
                _compacted = false;
            }

            // test de _compacted
            if (object->_listIndex - this->_ptr[this->_size - 1]->_listIndex != 1)
            {
                _compacted = false;
            }
        }
        else
        {
            if (object->_listIndex != 0)
            {
                _compacted = false;
            }
        }

    // Store the current index
    this->_current = this->_size;

    // Add the object
    this->_ptr[this->_size++] = object;
}

/*
@LABEL:ListIndex::insert(Type object, long index)
@SHORT:Insert an object in the list.
@ARG:Type & object & Object to insert into the list.
@ARG:long & index & Defines the position of the insertion in the list..
This method inserts an object to the list. The object is inserted at a given index in the list, and the list size is automatically incremented if necessary.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::insert(const Type object, long index)
//-----------------------------------------------------------------------------
{
    List<Type>::insert(object, index);
    _sorted = false;
    _compacted = false;
}

/*
@LABEL:ListIndex::forceSort()
@SHORT:Sort the list.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::forceSort()
//-----------------------------------------------------------------------------
{
    _sorted = false;
    sort();
}

/*
@LABEL:ListIndex::sort()
@SHORT:Sort the list.
If the list is already sorted this does nothing.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::sort()
//-----------------------------------------------------------------------------
{
    if (_sorted == true)
    {
        return;
    }

    Type v;
    long i, j;
    long h = 1;
    while (h <= this->_size)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (long)(h / 3);
        for (i = h + 1; i <= this->_size; i++)
        {
            v = this->_ptr[i - 1];
            j = i;
            while (this->_ptr[j - h - 1]->_listIndex > v->_listIndex)
            {
                this->_ptr[j - 1] = this->_ptr[j - h - 1];
                j = j - h;
                if (j <= h)
                {
                    break;
                }
            }
            this->_ptr[j - 1] = v;
        }
    }

    // maintenant elle est triee
    _sorted = true;
}

/*
@LABEL:List::sort(bool (*f)(Type, Type))
@SHORT:Sort the list using a comparison function.
@ARG:bool & (*f) & Function defined in the objects to sort the elements.
This method sorts the elements of the stack using a user-defined comparison function.
This method is very powerful for sorting a list and very flexible in use.
The usage may seem complex, but it is defined in the example below.
\begin{CppListing}
class truc
{
  public:
  double z; // a value
};
List <truc*> listeTrucs; // the list
bool compare(truc* p1,truc* p2) // the comparing function
{
  return (p1->z < p2->z); // comparison
}
...
{
...
listeTrucs.sort(compare); // sorts the list using the comparison function
}
\end{CppListing}
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::sort(bool (*funct)(const Type, const Type))
//-----------------------------------------------------------------------------
{
    Type v;
    long i, j;
    long h = 1;
    while (h <= this->_size)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (long)(h / 3);
        for (i = h + 1; i <= this->_size; i++)
        {
            v = this->_ptr[i - 1];
            j = i;
            while (funct(this->_ptr[j - h - 1], v))
            {
                this->_ptr[j - 1] = this->_ptr[j - h - 1];
                j -= h;
                if (j <= h)
                {
                    break;
                }
            }
            this->_ptr[j - 1] = v;
        }
    }
}

/*
  Compacting the list
  This method compacts the list. The notion of compaction is related to the fact that the indices of the list are contiguous to each other in ascending order without any "hole".
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::compact()
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < this->_size; i++)
    {
        this->_ptr[i]->_listIndex = i;
    }

    // maintenant elle est triee et compactee
    _compacted = true;
    _sorted = true;
}

//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::del(const Type start, const Type stop)
//-----------------------------------------------------------------------------
{
    List<Type>::del(IAppN(start->_listIndex), IAppN(stop->_listIndex));
}

//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::del(const Type object)
//-----------------------------------------------------------------------------
{
    long i = IAppN(object->_listIndex);
    List<Type>::del(i, i);
}

//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::delBefore(const Type object)
//-----------------------------------------------------------------------------
{
    List<Type>::del(0, IAppN(object->_listIndex) - 1);
}

//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::del(long index)
//-----------------------------------------------------------------------------
{
    List<Type>::del(index);
}

/*
@LABEL:ListIndex::del(long start, long stop)
@SHORT:Removes a set of elements from the List.
@ARG:long & start & First element to suppress from the list.
@ARG:long & stop & Last element to suppress from the list.
This method removes a set of items from the list.
This method is used to remove an entire segment from the list, by defining the start and end indexes of the segment in the list.
If the start and stop parameters are equal, only one element is deleted.
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::del(long start, long stop)
//-----------------------------------------------------------------------------
{
    List<Type>::del(start, stop);
}

/*
@LABEL:ListIndex::delBefore(long index)
@SHORT:Removes a set of elements from the List.
@ARG:long & index & Index of the last element to suppress from the list.
This method removes all items in the list between the beginning of the list and the value given as an argument to this method.
This method is equivalent to del(0, index-1).
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::delBefore(long index)
//-----------------------------------------------------------------------------
{
    List<Type>::delBefore(index);
}

/*
@LABEL:List::delAfter(long index)
@SHORT:Removes a set of elements from the List.
@ARG:long & index & Index of the first element to suppress from the list.
This method deletes all items in the list between the value given as an argument to this method and the end of the list.
This method is equivalent to del(index+1, last()).
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::delAfter(long index)
//-----------------------------------------------------------------------------
{
    List<Type>::delAfter(index);
}

/*
  Removes a set of elements from the List.

  This method deletes all items in the list between the value given as an argument to this method and the end of the list.
  This method is equivalent to del(index+1, last()).
  - object element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::delAfter(const Type object)
//-----------------------------------------------------------------------------
{
    List<Type>::del(IAppN(object->_listIndex) + 1, this->_size - 1);
}

/*
@LABEL:ListIndex::sort(bool (*f)(Type, Type))
@SHORT:Sort the list using a comparison function.
@ARG:bool & *(f) & Function defined in the objects to sort the elements.
This method sorts the elements of the stack using a user-defined comparison function.
This method is very powerful for sorting a list and very flexible in use.
The usage may seem complex, but it is defined in the example below.
\begin{CppListing}
class truc
{
  public:
  double z; // a value
};
List <truc*> listeTrucs; // the list
bool compare(truc* p1,truc* p2) // the comparing function
{
  return (p1->z < p2->z); // comparison
}
...
{
...
listeTrucs.sort(compare); // sorts the list using the comparison function
}
\end{CppListing}
@END
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::sort(bool (*funct)(const Type, const Type))
//-----------------------------------------------------------------------------
{
    List<Type>::sort(*funct);
    _sorted = false;
}

#endif
