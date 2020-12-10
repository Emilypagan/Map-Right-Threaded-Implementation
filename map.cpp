/** CS515 Assignment 9
 * File: map.cpp
 * Name: Mily Pagan
 * Section: 02
 * Date: 11/12/20
 * Collaboration Declaration: piazza */

// A generic Map implemented with right-threaded BST
// BST is not balanced

#include <map> // helper container for thread copying

//////////////////////////////////////////////////////////////////////////
//insert
template<typename KEY, typename T>
bool Map<KEY, T>::insert(Elem *& root, const KEY &key, const T &data, Elem *lastLeft) {
    if (root == _root || root == 0 ) {
        root = new Elem;
        root->key = key;
        root->data = data;
        root->left = 0;
        root->right = lastLeft;
        root->rightThread = true;
        return true;
    }
    if (key == root->key) {
        return false;
    }
    if (key < root->key) {
        return insert(root->left, key, data, root);
    }
    if (!root->rightThread){
        return insert(root->right, key, data, lastLeft);
    } else {
        root->rightThread = false;
        root->right = new Elem;
        root->right->key = key;
        root->right->data = data;
        root->right->left = 0;
        root->right->right = lastLeft;
        root->right->rightThread = true;
        return true;
    }
}
//////////////////////////////////////////////////////////////////////////
// output the structure of tree. The tree is output as "lying down"
// in which _root is the LEFT most Elem.
template <typename KEY, typename T>
void Map<KEY, T>::printTree(ostream& out, int level, Elem *p) const{
    int i;
    if (p) {
        if (p->right && !p->rightThread)
            printTree(out, level+1,p->right);
        for(i=0;i<level;i++) {
            out << "\t";
        }
        out << p->key << " " << p->data << '\n';
        printTree(out, level+1,p->left);
    }
}
//////////////////////////////////////////////////////////////////////////
//deallocate
template<typename KEY, typename T>
void Map<KEY, T>::destructCode(Elem *& p) {
    if(p->left != NULL){
        destructCode( p->left );
    }
    if(p->right != NULL && (!p->rightThread)){
        destructCode( p->right );
    }
    delete p;
}
//////////////////////////////////////////////////////////////////////////
// common copy code for deep copy a tree without copying threads
template <typename KEY, typename T>
void  Map<KEY,T>::copyCode(Elem* &newRoot, Elem* origRoot){
    if (origRoot == 0)
        newRoot = 0;
    else{
        newRoot = new Elem;
        newRoot->key = origRoot->key;
        newRoot->data = origRoot->data;
        newRoot->rightThread = origRoot->rightThread;
        copyCode(newRoot->left, origRoot->left);
        if (!origRoot->rightThread)
            copyCode(newRoot->right, origRoot->right);
    }
}
//////////////////////////////////////////////////////////////////////////
// construct a key-element map to rethread the new tree
// the map contains all nodes key values and their corresonding elem node address
// for furture lookup in setting up threads
template <typename KEY, typename T>
void Map<KEY, T>::addToMap(Elem* root, map<KEY, Elem*> &keyElemMap){
    if (root) {
        keyElemMap[root->key] = root;
        addToMap(root->left, keyElemMap);
        if (!root->rightThread)
            addToMap(root->right, keyElemMap);
    }
}
//////////////////////////////////////////////////////////////////////////
// common copy code for thread copying
template <typename KEY, typename T>
void Map<KEY, T>::copyThread(Elem* &newRoot, Elem* origRoot){
    map<KEY, Elem*> newKeyElemMap;
    map<KEY, Elem*> origKeyElemMap;
    addToMap(newRoot->left, newKeyElemMap);
    addToMap(origRoot->left, origKeyElemMap);

    typename std::map<KEY, Elem*>::reverse_iterator it = origKeyElemMap.rbegin();
    newKeyElemMap[it->first] -> rightThread = true;
    newKeyElemMap[it->first] -> right = newRoot;

    it++;
    while(it != origKeyElemMap.rend()){
        if (it->second->rightThread){
            newKeyElemMap[it->first] -> rightThread = true;
            newKeyElemMap[it->first] -> right = newKeyElemMap[ origKeyElemMap[it->first]->right->key ];
        }
        it++;
    }
}
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::begin() const {  // return the left most (smallest) tree node
    // fill in here
    Elem *n = _root;
    if(_size > 0) {
        while ( n->left != NULL ){
            n = n->left;
        }
    }
    Iterator it(n);
    return it;
}
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::end() const {
    // fill in here
    Iterator it(_root);
    return it;
}
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename T>
Map<KEY, T>::Map(){
    _root = new Elem;
    _root->left = _root;
    _root->right = 0;
    _root->rightThread = false;
    _size = 0;
}
//////////////////////////////////////////////////////////////////////////
// copy constructor
template <typename KEY, typename T>
Map<KEY, T>::Map(const Map<KEY,T> &v){
    if (v._root == v._root->left){
        _root = new Elem;
        _root->left = _root;
        _root->right = 0;
        _size = 0;
    } else {
        _root = new Elem;
        _root->left = _root;
        _root->right = 0;
        copyCode(_root->left, v._root->left);
        copyThread(_root, v._root);
        _size = v._size;
    }
}
//////////////////////////////////////////////////////////////////////////
//destruct
template<typename KEY, typename T>
Map<KEY, T>::~Map() {
    if(_size == 0)
        delete _root;
    else{
        destructCode( _root->left );
        delete _root;
    }
}
//////////////////////////////////////////////////////////////////////////
//assignment operator
template<typename KEY, typename T>
Map<KEY, T>& Map<KEY, T>::operator=(const Map &rhs) {
    if(this == &rhs) {
        return *this;
    }
    if(_size > 0){
        destructCode(_root);
    } else {
        delete _root;
    }

    _root = new Elem;
    _root->left = _root;
    _root->right = 0;
    copyCode( _root ->left, rhs._root->left );
    copyThread(_root, rhs._root);
    _size = rhs._size;
    return *this;
}
//////////////////////////////////////////////////////////////////////////
//insert
template<typename KEY, typename T>
bool Map<KEY, T>::insert(KEY k, T val) {
    if ( insert(_root->left, k, val, _root )){
        _size++;
        return true;
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////
//erase
template<typename KEY, typename T>
bool Map<KEY, T>::erase(KEY k) {

    Iterator i = find(k);
    Iterator j(_root);
    if(i == j) {
        return false;
    }
    Elem* previous = _root;
    Elem* nodeToDelete = _root->left;
    bool isRight = 0;
    //finding the desired key and getting its parent
    while(nodeToDelete->key != k){
        if(k < nodeToDelete->key){
            previous = nodeToDelete;
            nodeToDelete = nodeToDelete->left;
            isRight = 0;
        } else {
            previous = nodeToDelete;
            nodeToDelete = nodeToDelete->right;
            isRight = 1;
        }
    }

    //if node to delete is a leaf node
    if(nodeToDelete->left == 0 && nodeToDelete->rightThread) {
        if(isRight) {
            previous->right = nodeToDelete->right;
            previous->rightThread = 1;
            delete nodeToDelete;
            _size--;
            return true;
        } else {
            previous->left = 0;
            delete nodeToDelete;
            _size--;
            return true;
        }
    } else if(((nodeToDelete->left == 0) && !(nodeToDelete->rightThread)) || ((nodeToDelete->left != 0)  && (nodeToDelete->rightThread))) {
        if(!nodeToDelete->rightThread){
            if (isRight) {
                previous->right = nodeToDelete->right;
            } else {
                previous->left = nodeToDelete->right;
            }

            //changed right to left
            delete nodeToDelete;
            _size--;
            return true;
        } else {
            if(isRight) {
                previous->right = nodeToDelete->left;
            } else {
                previous->left = nodeToDelete->left;
            }

            if (nodeToDelete->left->rightThread) {
                nodeToDelete->left->right = nodeToDelete->right;
            } else {
                Elem *temp = nodeToDelete->left->right;
                while(!temp->rightThread)
                    temp = temp->right;
                temp->right = nodeToDelete->right;
            }

            delete nodeToDelete;
            _size--;
            return true;
        }
    } else if ((nodeToDelete->left != 0) && !(nodeToDelete->rightThread)) {
        Elem* m = nodeToDelete->right;
        Elem* f = nodeToDelete;
        KEY tempKey;
        T tempVal;
        if(m->left == 0) {
            tempKey = m->key;
            tempVal = m->data;
            m->key = nodeToDelete->key;
            m->data = nodeToDelete->data;
            nodeToDelete->key = tempKey;
            nodeToDelete->data = tempVal;
            //previous points to same location
            if (m->rightThread) {
                nodeToDelete->right = m->right;
                nodeToDelete->rightThread = 1;
            } else { //or else if (f==nodetodelete) -
                nodeToDelete->right = m->right;
            }


            //f->right = m->right;
            //f->rightThread = 1;
            delete m;
            _size--;
            return true;
        }
        while( m->left != 0 ) {
            f = m ;
            m = m->left;
        }
        tempKey = m->key;
        tempVal = m->data;
        m->key = nodeToDelete->key;
        m->data = nodeToDelete->data;
        nodeToDelete->key = tempKey;
        nodeToDelete->data = tempVal;
        if (m->rightThread) {
            f->left = m->left;
        } else {
            f->left = m->right;
        }

        delete m;
        _size--;
        return true;
    }
}
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename T>
int Map<KEY, T>::size() const{
    return _size;
}
//////////////////////////////////////////////////////////////////////////
//find
template<typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::find(KEY k) const {
    Elem* p =_root;
    Elem* r =_root;
    if( _size != 0){
        p = p->left;
    } else {
        Iterator emp(r);
        return emp;
    }
    while( true ){
        if( p->key == k ) {
            break;
        }else{
            if( k < p->key ) {
                if( p->left == NULL ){
                    Iterator emp(r);
                    return emp;
                }
                p = p->left;
            } else {
                if( p->right == NULL || ( p->rightThread ) ){
                    Iterator emp(r);
                    return emp;
                }
                p = p->right;
            }
        }
    }
    Iterator it(p);
    return it;
}
//////////////////////////////////////////////////////////////////////////
//[] operator
template<typename KEY, typename T>
T& Map<KEY, T>::operator[](KEY k) {
    T temp = T();
    Elem* e;
    Iterator it = find(k);
    if(it != end()){
        e = &*it;
        return e->data;
    } else {
        insert(k,temp);
        it = find(k);
        e = &*it;
        return e->data;
    }

}
//////////////////////////////////////////////////////////////////////////
// outputs information in tree in inorder traversal order
template <typename KEY, typename T>
ostream& Map<KEY, T>::dump(ostream& out) const{
    if (_root == _root->left) {// tree empty
        return out;
    }
    printTree(out, 0, _root->left);
    return out;
}
//////////////////////////////////////////////////////////////////////////
// outputs using overloaded << operator
template<typename KEY, typename T>
ostream& operator<< (ostream& out, const Map<KEY, T>& v){
    v.dump(out);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////
//Iterator(){}
//////////////////////////////////////////////////////////////////////////
//explicit Iterator(Elem *cur):_cur(cur) {}
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename T>
typename Map<KEY, T>::Elem& Map<KEY, T>::Iterator::operator*(){
    // fill in here
    return *_cur;
}
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename T>
typename Map<KEY, T>::Elem* Map<KEY, T>::Iterator::operator->(){
    // fill in here
    return _cur;
}
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename T>
typename Map<KEY, T>::Iterator Map<KEY, T>::Iterator::operator++(int val){
    // fill in here
    if( _cur->rightThread ){
        _cur = _cur->right;
    }
    else{
        if( _cur->right != NULL ){
            _cur = _cur->right; // point to the next right
        }
        while( _cur->left != NULL ){
            _cur = _cur->left; // point to the next left
        }
    }
    return *this;
}
//////////////////////////////////////////////////////////////////////////
//== operator
template <typename KEY, typename T>
bool Map<KEY, T>::Iterator::operator==(Iterator it) {
    if( _cur == it._cur ){
        return true;
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////
// the != operator
template <typename KEY, typename T>
bool Map<KEY, T>::Iterator::operator!=(Iterator it) {
    if (_cur != it._cur)
    {
        return true;
    }
    return false;
}