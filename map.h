// A generic Map implemented with right-threaded BST
// BST is not balanced

#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <map> 
using namespace std;

template <typename KEY, typename T>
class Map{
private:
	struct Elem {
		KEY key;
		T data;
		Elem *left;
		Elem *right;
		bool rightThread;  // normal right child link or a thread link
	};
	Elem *_root;  // a dummy root sentinel 
	int _size;

	// helper method for inserting record into tree.
	bool insert(Elem *& root, const KEY &key, const T &data, Elem *lastLeft);

	// helper method for print tree
	void printTree(ostream& out, int level, Elem *p) const; //done

	// common code for deallocation
	void destructCode(Elem *& p);

	// common code for copy tree; not required. 
	void copyCode(Elem* &newRoot, Elem* origRoot); //done
	
	// common code for deep copying threaded tree
	void addToMap(Elem* root, map<KEY, Elem*> &keyElemMap); //done
	void copyThread(Elem* &newRoot, Elem* origRoot); //done

public:
	// a simple Iterator, traverse the collection in one direction
	class Iterator{
	public:
		Iterator(){}
		explicit Iterator(Elem *cur):_cur(cur) {}
		Elem& operator*(); //done
		Elem* operator->(); //done
		Iterator operator++(int); //done
		bool operator==(Iterator it);
		bool operator!=(Iterator it);
	private:
		Elem* _cur;
	};
	Iterator begin() const; //done
	Iterator end() const; //done

	// constructs empty Map
	Map(); //done

	// copy constructor
	Map(const Map &rhs); //done

	// destructor
	~Map();

	// assignment operator
	Map& operator=(const Map &rhs);

	// insert an element into the Map; return true if successful
	bool insert(KEY, T);

	// remove an element from the Map; return true if successful
	bool erase(KEY);

	// return size of the Map
	int size() const; //done

	// return an iterator pointing to the end if the element is not found,
	// otherwise, return an iterator to the element
	Iterator find(KEY) const;

	// overloaded subscript operator
	T& operator[](KEY);

	// output the underlying BST
	ostream& dump(ostream& out) const; //done
};

template<typename KEY, typename T>
ostream& operator<< (ostream&, const Map<KEY, T>&); //done

#include "map.cpp"  // must include source file for template compilation
#endif
