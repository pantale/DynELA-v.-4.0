/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*
  \file List.h
  Declaration and definition of the Finite Elements Lists.

  This file declares and defines the Finite Elements Lists, which are the core of the DynELA Finite Element code as all structures are usually embers of such lists.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_List_h__
#define __dnlKernel_List_h__

#include <cstring>
#include <iostream>
#include <Errors.h>

#define DEFAULT_stack_size 10 // Default stack size of the List
#define DEFAULT_stack_inc 10  // Default stack increment of the List

template <class Type>
class ListIndex;

/*
  \class List
  Management of objects as list

  This class is used to store all type of object and manupulate them as a list (for example: list of Nodes, Elements, Boundary conditions,...) \n
  This List is a dynamic one, the initialization is performed with a default stack size defined by \ref DEFAULT_stack_size, as soon as there is no more space left to store a new object,
  the List size ins increased with respect to the \ref DEFAULT_stack_inc value.
  \ingroup dnlKernel
  \code
  void test()
  {
  List <object*> listOfObjects;
  object* obj1 = new object;
  listOfObjects << object;
  }
  \endcode
*/
template <class Type>
class List
{
    friend class ListIndex <Type>;

private:
    long sz;       // Current size of the list (number of objects refered by the List)
    long s_size;   // Current stack size of the List (Space available for objects storage)
    long s_inc;    // Current stack increment for the List
    long pcurrent; // The current index of the current object in the List
    Type *ptr;     // A pointer to the current object in the List

public:
    List(const long stack = DEFAULT_stack_size);
    virtual ~List();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

    bool contains(const Type objet) const;
    bool operator!=(const List <Type> &objet) const;
    bool operator==(const List <Type> &objet) const;
    List <Type> operator<<(const Type objet);
    long &stackIncrement();
    long getIndex(const Type objet) const;
    long getSize() const;
    long objectSize();
    long stackSize() const;
    Type &operator()(const long i);
    Type current();
    Type currentDown();
    Type currentUp();
    Type dichotomySearch(long (*funct)(const Type objet1, const long i), const long i) const;
    Type first();
    Type last();
    Type next();
    Type operator()(const long i) const;
    Type previous();
    virtual void add(const Type objet);
    virtual void flush();
    virtual void insert(const Type objet, long index);
    void close();
    void del(long index);
    void del(long start, long stop);
    void delAfter(long index);
    void delBefore(long index);
    void getInverse();
    void print(std::ostream &os) const;
    void redim(const long taille);
    void sort(bool (*funct)(const Type objet1, const Type objet2));
};

/*
  \class ListIndex
  Management of objects as list with indexes

  This class is used to store all type of object and manupulate them as a list (for example: list of Nodes, Elements, Boundary conditions,...) \n
  This ListIndex is a dynamic one, the initialization is performed with a default stack size defined by \ref DEFAULT_stack_size, as soon as there is no more space left to store a new object,
  the ListIndex size ins increased with respect to the \ref DEFAULT_stack_inc value.
  \warning In order to be managed by the ListIndex, the objects MUST have a \ref _listIndex number used to reference all objects.
  \ingroup dnlKernel
  \code
  void test()
  {
  ListIndex <object*> listOfObjects;
  object* obj1 = new object;
  listOfObjects << object;
  }
  \endcode
*/
template <class Type>
class ListIndex : public List <Type>
{
    bool sorted;    // Bool flag defining that the current ListIndex is sorted (All elements are in sorted with increasing Id)
    bool compacted; // Bool flag defining that the current ListIndex is compacted (No hole in the list of objects)

public:
    ListIndex(const long stack = DEFAULT_stack_size);
    ~ListIndex();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

    bool isCompacted() const;
    bool isSorted() const;
    long IAppN(const long i) const;
    Type AppN(const long i) const;
    void add(const Type objet);
    void compact();
    void del(const Type object);
    void del(const Type start, const Type stop);
    void del(long index);
    void del(long start, long stop);
    void delAfter(const Type object);
    void delAfter(long index);
    void delBefore(const Type object);
    void delBefore(long index);
    void flush();
    void forceSort();
    void insert(const Type objet, long index);
    void sort();
    void sort(bool (*funct)(const Type objet1, const Type objet2));
};

/*
  Default constructor of the List class

  This constructor allocates the default memory for an instance of the List class. \n
  If the size of the list is not specified, the default size is taken into account, which is defined by the value of \ref DEFAULT_stack_size.
  - stack defines the initial size of the List
*/
//-----------------------------------------------------------------------------
template <class Type>
List <Type>::List(const long stack)
//-----------------------------------------------------------------------------
{
    // Definition of the default constants
    s_size = stack;
    s_inc = DEFAULT_stack_inc;
    sz = 0;
    pcurrent = 0;

    // Memory allocation for the List
    ptr = new Type[s_size];

#ifdef VERIF_alloc
    if (ptr == NULL)
        fatalError("List <Type>::List",
                   "Memory allocation error\n"
                   "May be out of memory on this system\n");
#endif
}

/*
  Resize the storage space of a List

  This method is used to increase or decrease the size of a list. \n
  If the proposed new size is smaller than the minimum size needed to store the current elements of the list, an error is generated.
  This method should generally not be called by the user (unless the user has full mastery of the trick ;-0 ).
  This method is heavily used internally by the other methods of the class.
  In the case where the user does not have enough mastery for this kind of operation, it is better to let the class manage its own memory allocations. \n
  A possible use of this method is in the pre-allocation memory, when we know in advance the number of objects that will be stored in the list.
  The size of the list is then adjusted to this value which avoids CPU time consuming dynamic size adjustment operations.
  Of course, dynamic allocation mechanisms exist and you can exceed this value.
  \code
  void test()
  {
  List <object*> listOfObjects;
  listOfObjects.redim(1280); // 1280 objets to store
  for (i=0;i<1280);i++)
  {
  object* obj1=new object;
  listOfObjects << obj1; // storage without reallocation
  }
  object* obj2=new object;
  listOfObjects << obj2; // and now a 1281th element
  }
  \endcode
  - newSize defines the new size of the List
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::redim(const long newSize)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (newSize < sz)
        fatalError("template <class Type> void List <Type>::redim(const long)\n",
                   "new getSize < actual getSize means to truncate the List\n"
                   "This is not allowed with redim method\n"
                   "Must use a 'del' method");
#endif

    // set the newSize
    s_size = newSize;

    // Dynamic allocation of the storage space
    Type *ptr2 = new Type[s_size];

#ifdef VERIF_alloc
    if (ptr2 == NULL)
        fatalError("List <Type>::redim",
                   "memory allocation error\nMay be out of memory on this system\n");
#endif

    // Copy the memory
    memcpy(ptr2, ptr, sz * sizeof(Type));

    // Delete of the old pointer
    delete[] ptr;

    // Re-affectation of the pointer
    ptr = ptr2;
}

/*
  This method closes the List, i.e. resize the stack size to the number of elements to save memory.

  This method is used to adjust the size of the list according to the number of real objects in the list.
  This method allows to recover memory space, mainly for small lists.
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::close()
//-----------------------------------------------------------------------------
{
    // on fait un redim a la size_ reelle de la pile
    redim(sz);
}

/*
  This is the accessor to the elements of the list.

  This method is used to access items on the list.
  This access is both read and write.
  This method returns element [i] of the list.
  The baseline is 0 (first element of index 0) as usual in C and C++.
  - index defines the ith element of the list.
  Return : the ith element of the List.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type &List <Type>::operator()(const long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((index < 0) || (index >= sz))
    {
        std::cerr << "Fatal Error in template <class Type> Type& List <Type>::operator ()\n";
        std::cerr << "long " << index << " out of allowd range {0-" << sz - 1 << "}\n";
        exit(-1);
    }
#endif

    return ptr[pcurrent = index];
}

/*
  This is the accessor to the elements of the list.

  This method is used to access items on the list.
  This access is read only.
  This method returns element [i] of the list.
  The baseline is 0 (first element of index 0) as usual in C and C++.
  - index defines the ith element of the list.
  Return : the ith element of the List.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::operator()(const long index)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((index < 0) || (index >= sz))
    {
        std::cerr << "Fatal Error in template <class Type> Type& List <Type>::operator ()\n";
        std::cerr << "long " << index << " out of allowd range {0-" << sz - 1 << "}\n";
        exit(-1);
    }
#endif

    return ptr[index];
}

/*
  Next element in the list
  This method uses an internal list lookup mechanism to return the next element in the list.
  To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
  Return : next item in the list or NULL if it does not exist.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::next()
//-----------------------------------------------------------------------------
{
    if (pcurrent >= sz - 1)
    {
        return NULL;
    }
    return ptr[++pcurrent];
}

/*
  Next element in the list

  This method uses an internal list lookup mechanism to return the element following the previous call in the list.
  To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
  Return : next item in the list or NULL if it does not exist.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::currentUp()
//-----------------------------------------------------------------------------
{
    if (pcurrent >= sz)
    {
        return NULL;
    }

    return ptr[pcurrent++];
}

/*
  Previous element in the list

  This method uses an internal list lookup mechanism to return the element preceding the previous call in the list.
  To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
  Return : previous item in the list or NULL if it does not exist.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::currentDown()
//-----------------------------------------------------------------------------
{
    if (pcurrent < 0)
    {
        return NULL;
    }

    return ptr[pcurrent--];
}

/*
  The first element in the list

  This method returns the first item in the list.
  Return : first item in the list or NULL if it does not exist.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::first()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent = 0];
}

/*
  The last element in the list

  This method returns the last item in the list.
  Return : last item in the list or NULL if it does not exist.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::last()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent = sz - 1];
}

/*
  Previous element in the list

  This method uses an internal list lookup mechanism to return the element preceding the previous call in the list.
  To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
  Return : previous item in the list or NULL if it does not exist.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::previous()
//-----------------------------------------------------------------------------
{
    if (pcurrent == 0)
    {
        return NULL;
    }
    return ptr[--pcurrent];
}

/*
  current element in the list

  This method uses an internal list lookup mechanism to return the element preceding the current call in the list.
  To use this method, it is necessary to define the list boundaries, and to have the start referenced by the first(), last() or accessors() methods.
  Return : current item in the list or NULL if it does not exist.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::current()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent];
}

/*
  current size of the list

  Return : the size of the List.
*/
//-----------------------------------------------------------------------------
template <class Type>
long List <Type>::getSize() const
//-----------------------------------------------------------------------------
{
    return sz;
}

/*
  current stack size of the list

  Return : the stack size of the List.
*/
//-----------------------------------------------------------------------------
template <class Type>
long List <Type>::stackSize() const
//-----------------------------------------------------------------------------
{
    return s_size;
}

/*
  current stack increment of the list

  Return : the stack increment of the List.
*/
//-----------------------------------------------------------------------------
template <class Type>
long &List <Type>::stackIncrement()
//-----------------------------------------------------------------------------
{
    return s_inc;
}

/*
  Empties the List.

  This method empties the contents of the stack and returns its real size to zero and its stack size to DEFAULT_stack_size.
  The stack is as good as new !!! (it's a rechargeable battery ;-] )
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::flush()
//-----------------------------------------------------------------------------
{
    s_size = DEFAULT_stack_size;
    sz = 0;
    delete[] ptr;
    pcurrent = 0;

    ptr = new Type[s_size];

#ifdef VERIF_alloc
    if (ptr == NULL)
        fatalError("template <class Type> void List <Type>::flush()",
                   "fonction flush de template <class Type> List <Type>\n");
#endif
}

/*
  Add an element to the List.

  This method adds an object to the list. The object is added to the end of the list, and the list size is automatically incremented if necessary.
  - objet item to add to the end of the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
List <Type> List <Type>::operator<<(const Type object)
//-----------------------------------------------------------------------------
{
    add(object);
    return *this;
}

/*
  Insert an element to the List.

  This method inserts an object to the list. The object isiInserted at a given index in the list, and the list size is automatically incremented if necessary.
  - objet item to insert in the list.
  - index defines the index of the insersion.
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::insert(const Type objet, long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (index > sz)
        fatalError("List <Type>::insert(long index)",
                   "index indice (%d) out of bounds (%d)\n", index, sz);
#endif

    // add the last object at the end
    add(ptr[sz - 1]);

    // move from the end to the insertion point
    if (sz - 2 >= index)
    {
        memmove(ptr + index + 1, ptr + index, (sz - index - 2) * sizeof(Type));
    }

    // insert the object
    ptr[index] = objet;
}

/*
  Insert an element to the List.

  This method inserts an object to the list. The object isiInserted at a given index in the list, and the list size is automatically incremented if necessary.
  - objet item to insert in the list.
  - index defines the index of the insersion.
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::insert(const Type objet, long index)
//-----------------------------------------------------------------------------
{
    List <Type>::insert(objet, index);
    sorted = false;
    compacted = false;
}

/*
  Add an element to the List.

  This method adds an object to the list. The object is added to the end of the list, and the list size is automatically incremented if necessary.
  - objet item to add to the end of the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // Test for memory reallocation
    if (sz >= s_size)
    {
        redim(s_size + s_inc);
    }

    // Store the current index
    pcurrent = sz;

    // Add the object
    ptr[sz++] = object;
}

/*
  This method reverses the order of the elements in the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::getInverse()
//-----------------------------------------------------------------------------
{
    Type v;
    for (long i = 0; i < sz / 2; i++)
    {
        v = ptr[i];
        ptr[i] = ptr[sz - i - 1];
        ptr[sz - i - 1] = v;
    }
}

/*
  Removes a set of elements from the List.

  This method removes a set of items from the list.
  This method is used to remove an entire segment from the list, by defining the start and end indexes of the segment in the list.
  If the start and stop parameters are equal, only one element is deleted.
  - start first element to suppress
  - stop last element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::del(long start, long stop)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (start > stop)
        fatalError("List <Type>::del(long start,long stop)",
                   "start indice %d > stop indice %d\n", start, stop);
    if (stop >= sz)
        fatalError("List <Type>::del(long start,long stop)",
                   "stop indice (%d) out of bounds (%d)\n", stop, sz);
#endif

    // recouvrement
    if (sz - stop - 1 > 0)
    {
        memmove(ptr + start, ptr + (stop + 1), (sz - stop - 1) * sizeof(Type));
    }

    // recalcul de la size_
    sz -= (stop - start + 1);
}

/*
  Removes an element from the List.

  This method removes an items from the list.
  This method is used to remove an item from the list, by defining the index of the element in the list.
  - index index of the element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::del(long index)
//-----------------------------------------------------------------------------
{
    List <Type>::del(index, index);
}

/*
  Removes a set of elements from the List.

  This method removes all items in the list between the beginning of the list and the value given as an argument to this method.
  This method is equivalent to del(0, index-1).
  - index defines the first element to keep in the List
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::delBefore(long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (index - 1 >= sz)
        fatalError("List <Type>::delBefore(long index)",
                   "index indice (%d) out of bounds (%d)\n", index, sz);
#endif

    List <Type>::del(0, index - 1);
}

/*
  Removes a set of elements from the List.

  This method deletes all items in the list between the value given as an argument to this method and the end of the list.
  This method is equivalent to del(index+1, last()).
  - index defines the last element to keep in the List
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::delAfter(long index)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (index + 1 >= sz)
        fatalError("List <Type>::delAfter(long index)",
                   "index indice (%d) out of bounds (%d)\n", index, sz);
#endif

    List <Type>::del(index + 1, sz - 1);
}

/*
  Comparison of two lists

  This method is used to compare two lists with each other. It tests for equality.
  - liste second list to compare to.
  Return : true if both lists are the same
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List <Type>::operator==(const List <Type> &liste) const
//-----------------------------------------------------------------------------
{
    // la comparaison porte sur la size_
    if (sz != liste.sz)
    {
        return (false);
    }

    // et sur le contenu
    for (long i = 0; i < sz; i++)
        if (ptr[i] != liste.ptr[i])
        {
            return (false);
        }

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
bool List <Type>::operator!=(const List <Type> &liste) const
//-----------------------------------------------------------------------------
{
    return !(*this == liste);
}

/*
  Memory size of a list

  This method return the memory size a list, i.e. the memory size of the stack and of the List itself.
  - liste second list to compare to.
  Return : memory size of the list in bytes
*/
//-----------------------------------------------------------------------------
template <class Type>
long List <Type>::objectSize()
//-----------------------------------------------------------------------------
{
    return (sizeof(*this) + s_size * sizeof(ptr));
}

/*
  Prints the content of a list
  This method displays the items in the list. It is used for debugging purposes.
  \warning Objects managed by the list must have a method \b << for displaying their contents.
  - outputStream \c ostream flux for display
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::print(std::ostream &outputStream) const
//-----------------------------------------------------------------------------
{
    outputStream << "list " << sz << "/" << s_size << "={";
    for (long i = 0; i < sz; i++)
    {
        if (i != 0)
        {
            std::cout << ",";
        }
        outputStream << *ptr[i];
    }
    outputStream << "}";
}

/*
  Search an object in the List

  This method performs a simple search for an item in the list and returns an Index indicating the place of the object in the list.
  If the object is not found, it returns the value -1.
  - objet object to search for in the List
  Return : Index de l'objet dans la liste
*/
//-----------------------------------------------------------------------------
template <class Type>
long List <Type>::getIndex(const Type objet) const
//-----------------------------------------------------------------------------
{
    // recherche bourrin de base
    for (long i = 0; i < sz; i++)
    {
        if (ptr[i] == objet)
        {
            //	  pcurrent=i;
            return i;
        }
    }

    // bourrin pas trouve alors on retourne -1
    return -1;
}

/*
  Search if an object is in the List

  This method performs a simple search for an item in the list and returns a boolean according to the presence or or not of this object in the list.
  - objet object to search in the list
  Return : true if the requested object is in the list, false if not
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List <Type>::contains(const Type objet) const
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < sz; i++)
    {
        if (ptr[i] == objet)
        {
            return true;
        }
    }
    return false;
}

/*
  Default constructor of the ListIndex class

  This constructor allocates the default memory for an instance of the ListIndex class. \n
  If the size of the list is not specified, the default size is taken into account, which is defined by the value of \ref DEFAULT_stack_size.
  - stack defines the initial size of the ListIndex
*/
//-----------------------------------------------------------------------------
template <class Type>
ListIndex <Type>::ListIndex(const long stack) : List <Type>(stack)
//-----------------------------------------------------------------------------
{
    // Definition of the default constants
    sorted = true;
    compacted = true;
}

/*
  Default destructor of the List class
*/
//-----------------------------------------------------------------------------
template <class Type>
List <Type>::~List()
//-----------------------------------------------------------------------------
{
}

/*
  Default destructor of the ListIndex class
*/
//-----------------------------------------------------------------------------
template <class Type>
ListIndex <Type>::~ListIndex()
//-----------------------------------------------------------------------------
{
}

/*
  Search for an element in the List

  This method is used to search for an item in the list using a dichotomous algorithm. This method returns the corresponding element in the list or the NULL value if the element is not in the list.
  \code
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
  \endcode
  \warning This method is only valid if the list is sorted on the search parameter.
  - funct function defining the comparison method to be used.
  - seekForValue particulate value of the element to be searched for in the list
  Return : the corresponding element in the list or the value NULL if the element is not in the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List <Type>::dichotomySearch(long (*funct)(const Type objet1, const long in), const long seekForValue) const
//-----------------------------------------------------------------------------
{
    long i;

    // if size_ is zeron returns NULL
    if (this->sz == 0)
    {
        return NULL;
    }

    // dichotomy
    long g, d, dx;
    g = 0;
    d = this->sz - 1;
    do
    {
        i = (g + d) / 2;
        dx = funct(this->ptr[i], seekForValue);
        if (dx == 0)
        {
            return this->ptr[i];
        }
        if (dx > 0)
        {
            d = i - 1; // the right
        }
        else
        {
            g = i + 1; // the left
        }
    } while (g <= d);

    return NULL;
}

/*
  Search for an element in the List

  This method is used to search for an item in the list. The search algorithm is based on the parameter \b _listIndex declared in the list objects.
  This method returns the corresponding element in the list or the NULL value if the element is not in the list.
  - seekForValue particulate value of the element to be searched for in the list
  Return : the corresponding element in the list or the value NULL if the element is not in the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
Type ListIndex <Type>::AppN(const long seekForValue) const
//-----------------------------------------------------------------------------
{
    long i;

    // if size_ is zeron returns NULL
    if (this->sz == 0)
    {
        return NULL;
    }

    // si elle est triee et compactee
    if (compacted == true)
        if ((seekForValue >= 0) && (seekForValue < this->sz))
        {
            return this->ptr[seekForValue];
        }

    // recherche selon le cas
    if (sorted == true)
    {
        // dichotomy
        long g, d;
        g = 0;
        d = this->sz - 1;
        do
        {
            i = (g + d) / 2;
            if (this->ptr[i]->_listIndex == seekForValue)
            {
                return this->ptr[i];
            }
            if (seekForValue < this->ptr[i]->_listIndex)
            {
                d = i - 1; // the right
            }
            else
            {
                g = i + 1; // the left
            }
        } while (g <= d);
    }
    else
    {
        // tri bete mais terriblement efficace falsen !!
        for (i = 0; i < this->sz; i++)
            if (this->ptr[i]->_listIndex == seekForValue)
            {
                return this->ptr[i];
            }
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
long ListIndex <Type>::IAppN(const long seekForValue) const
//-----------------------------------------------------------------------------
{
    long i;

    // if size_ is zeron returns NULL
    if (this->sz == 0)
    {
        return 0;
    }

    // si elle est triee et compactee
    if (compacted == true)
        if ((seekForValue >= 0) && (seekForValue < this->sz))
        {
            return (seekForValue);
        }

    // recherche selon le cas
    if (sorted == true)
    {
        // dichotomy
        long g, d;
        g = 0;
        d = this->sz - 1;
        do
        {
            i = (g + d) / 2;
            if (this->ptr[i]->_listIndex == seekForValue)
            {
                return (i);
            }
            if (seekForValue < this->ptr[i]->_listIndex)
            {
                d = i - 1; // the right
            }
            else
            {
                g = i + 1; // the left
            }
        } while (g <= d);
    }
    else
    {
        // tri bete
        for (i = 0; i < this->sz; i++)
            if (this->ptr[i]->_listIndex == seekForValue)
            {
                return (i);
            }
    }
    return 0;
}

/*
  Test if the list is sorted

  This method tests whether the list is currently sorted.
  Return : true if the list is sorted false if it is not.
*/
//-----------------------------------------------------------------------------
template <class Type>
bool ListIndex <Type>::isSorted() const
//-----------------------------------------------------------------------------
{
    return sorted;
}

/*
  test if the list is compacted

  This method tests whether the list is currently compacted. The notion of compaction is related to the fact that the indices of the list are contiguous to each other in ascending order without any "hole".
  Return : true if the list is compacted false otherwise
*/
//-----------------------------------------------------------------------------
template <class Type>
bool ListIndex <Type>::isCompacted() const
//-----------------------------------------------------------------------------
{
    return compacted;
}

/*
  Empties the ListIndex.

  This method empties the contents of the stack and returns its real size to zero and its stack size to DEFAULT_stack_size.
  The stack is as good as new !!! (it's a rechargeable battery ;-] )
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::flush()
//-----------------------------------------------------------------------------
{
    this->s_size = DEFAULT_stack_size;
    this->sz = 0;
    sorted = true;
    compacted = true;
    delete[] this->ptr;
    this->pcurrent = 0;

    this->ptr = new Type[this->s_size];
#ifdef VERIF_alloc
    if (this->ptr == NULL)
        fatalError("template <class Type> void ListIndex <Type>::flush()",
                   "fonction flush de template <class Type> ListIndex <Type>\n");
#endif
}

/*
  Add an element to the ListIndex.

  This method adds an object to the listIndex. The object is added to the end of the listIndex, and the listIndex size is automatically incremented if necessary.
  - objet item to add to the end of the listIndex.
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // Test for memory reallocation
    if (this->sz >= this->s_size)
    {
        this->redim(this->s_size + this->s_inc);
    }

    // test de tri
    if (sorted == true)
    {
        if (this->sz != 0)
        {
            if (this->ptr[this->sz - 1]->_listIndex > object->_listIndex)
            {
                sorted = false;
                compacted = false;
            }

            // test de compacted
            if (object->_listIndex - this->ptr[this->sz - 1]->_listIndex != 1)
            {
                compacted = false;
            }
        }
        else
        {
            if (object->_listIndex != 0)
            {
                compacted = false;
            }
        }
    }
    // Store the current index
    this->pcurrent = this->sz;

    // Add the object
    this->ptr[this->sz++] = object;
}

/*
  Sort the list

  This method sorts the elements of the list according to the index \b _listIndex contained in each element of the list. This method forces the stack to be sorted, even if it seems to be already sorted.
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::forceSort()
//-----------------------------------------------------------------------------
{
    sorted = false;
    sort();
}

/*
  Sort the list

  This method sorts the elements of the list according to the index \b _listIndex contained in each element of the list.
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::sort()
//-----------------------------------------------------------------------------
{
    if (sorted == true)
    {
        return;
    }

    Type v;
    long i, j;
    long h = 1;
    while (h <= this->sz)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (long)(h / 3);
        for (i = h + 1; i <= this->sz; i++)
        {
            v = this->ptr[i - 1];
            j = i;
            while (this->ptr[j - h - 1]->_listIndex > v->_listIndex)
            {
                this->ptr[j - 1] = this->ptr[j - h - 1];
                j = j - h;
                if (j <= h)
                {
                    break;
                }
            }
            this->ptr[j - 1] = v;
        }
    }

    // maintenant elle est triee
    sorted = true;
}

/*
  Sorting the list from a comparison function
  This method sorts the elements of the stack using a user-defined comparison function. This method is very powerful for sorting a list and very flexible in use. The usage may seem complex, but it is defined in the example below.

  \code
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
  listeTrucs.sort(compare); // tri selon la fonction de comparaison
  }
  \endcode
  - funct function defining the comparison method to be used
*/
//-----------------------------------------------------------------------------
template <class Type>
void List <Type>::sort(bool (*funct)(const Type objet1, const Type objet2))
//-----------------------------------------------------------------------------
{
    Type v;
    long i, j;
    long h = 1;
    while (h <= this->sz)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (long)(h / 3);
        for (i = h + 1; i <= this->sz; i++)
        {
            v = this->ptr[i - 1];
            j = i;
            while (funct(this->ptr[j - h - 1], v))
            {
                this->ptr[j - 1] = this->ptr[j - h - 1];
                j -= h;
                if (j <= h)
                {
                    break;
                }
            }
            this->ptr[j - 1] = v;
        }
    }
}

/*
  Compacting the list
  This method compacts the list. The notion of compaction is related to the fact that the indices of the list are contiguous to each other in ascending order without any "hole".
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::compact()
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < this->sz; i++)
    {
        this->ptr[i]->_listIndex = i;
    }

    // maintenant elle est triee et compactee
    compacted = true;
    sorted = true;
}

/*
  Removes a set of elements from the ListIndex.

  This method removes a set of items from the list.
  This method is used to remove an entire segment from the list, by defining the start and end indexes of the segment in the list.
  If the start and stop parameters are equal, only one element is deleted.
  - start first element to suppress
  - stop last element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::del(const Type start, const Type stop)
//-----------------------------------------------------------------------------
{
    List <Type>::del(IAppN(start->_listIndex), IAppN(stop->_listIndex));
}

/*
  Removes an element from the List.

  This method removes an items from the list.
  This method is used to remove an item from the list, by defining the index of the element in the list.
  - object element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::del(const Type object)
//-----------------------------------------------------------------------------
{
    long i = IAppN(object->_listIndex);
    List <Type>::del(i, i);
}

/*
  Removes a set of elements from the List.

  This method removes all items in the list between the beginning of the list and the value given as an argument to this method.
  This method is equivalent to del(0, index-1).
  - object element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::delBefore(const Type object)
//-----------------------------------------------------------------------------
{
    List <Type>::del(0, IAppN(object->_listIndex) - 1);
}

/*
  Removes an element from the List.

  This method removes an items from the list.
  This method is used to remove an item from the list, by defining the index of the element in the list.
  - index index of the element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::del(long index)
//-----------------------------------------------------------------------------
{
    List <Type>::del(index);
}
/*
  Removes a set of elements from the ListIndex.

  This method removes a set of items from the list.
  This method is used to remove an entire segment from the list, by defining the start and end indexes of the segment in the list.
  If the start and stop parameters are equal, only one element is deleted.
  - start first element to suppress
  - stop last element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::del(long start, long stop)
//-----------------------------------------------------------------------------
{
    List <Type>::del(start, stop);
}

/*
  Removes a set of elements from the List.

  This method removes all items in the list between the beginning of the list and the value given as an argument to this method.
  This method is equivalent to del(0, index-1).
  - index defines the first element to keep in the List
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::delBefore(long index)
//-----------------------------------------------------------------------------
{
    List <Type>::delBefore(index);
}

/*
  Removes a set of elements from the List.

  This method deletes all items in the list between the value given as an argument to this method and the end of the list.
  This method is equivalent to del(index+1, last()).
  - index defines the last element to keep in the List
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::delAfter(long index)
//-----------------------------------------------------------------------------
{
    List <Type>::delAfter(index);
}

/*
  Removes a set of elements from the List.

  This method deletes all items in the list between the value given as an argument to this method and the end of the list.
  This method is equivalent to del(index+1, last()).
  - object element to suppress
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::delAfter(const Type object)
//-----------------------------------------------------------------------------
{
    List <Type>::del(IAppN(object->_listIndex) + 1, this->sz - 1);
}

/*
  Sorting the list from a comparison function
  This method sorts the elements of the stack using a user-defined comparison function. This method is very powerful for sorting a list and very flexible in use. The usage may seem complex, but it is defined in the example below.

  \code
  class truc
  {
    public:
    double z; // a value
  };
  ListIndex <truc*> listeTrucs; // the list
  bool compare(truc* p1,truc* p2) // the comparing function
  {
    return (p1->z < p2->z); // comparison
  }
  ...
  {
  ...
  listeTrucs.sort(compare); // tri selon la fonction de comparaison
  }
  \endcode
  - funct function defining the comparison method to be used
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex <Type>::sort(bool (*funct)(const Type objet1, const Type objet2))
//-----------------------------------------------------------------------------
{
    List <Type>::sort(*funct);
    sorted = false;
}

#endif
