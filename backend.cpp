#include <bits/stdc++.h>
// #include <iostream>
#include <fstream>
//comment
#include <cstdlib> // for std::atoi
using namespace std;
#include <chrono>
#include <unordered_map>



//This shall contain rotation logic as well...so AVL is incorporated into same code

typedef struct RBNode
        {
            int key;
            string value; //dont forget this is actually for an LRU cache
            int time_stamp;
            RBNode* left;
            RBNode* right;
            
            RBNode* parent; //new pointer for us
            char colour; //'r' or 'b'
            

            RBNode(int k, string v, int ts)
            {
                key = k;
                value = v;
                left = NULL; right = NULL;
                parent = NULL;
                colour = 'r';
                time_stamp = ts;
                
            }
        }RBNode;


class RBTree
{
    private:



        void leftRotate(RBNode * Imb_node)  //Imb_node = Imbalanced node
        {
            if (!Imb_node || !Imb_node->right) return;

            RBNode * child = Imb_node->right;
            RBNode * temp = child->left;
            RBNode * source = Imb_node->parent;

            //now lets rotate
            child->left = Imb_node;
            Imb_node->right = temp; // or temp
            if(temp!=NULL)temp->parent = Imb_node;
            Imb_node->parent = child;
            child->parent = source;

            //relink to source now
            if(source==NULL) 
            {
                //CHANGE MADE HERE
                root = child;
                return;
            }
            else
            {
                if(source->left == Imb_node)//Imb_Node WAS left child
                {
                    source->left = child;
                }
                else
                {
                    source->right = child;
                }
            }

            
        }
        //not designed to handle the colors right away
        //this is simply a rotator

        void rightRotate(RBNode * Imb_node)
        {
            if (!Imb_node || !Imb_node->left) return;
            RBNode * child = Imb_node->left;
            RBNode * temp = child->right;
            RBNode * source = Imb_node->parent;
            

            //now lets rotate
            child->right = Imb_node;
            Imb_node->left = temp;
            if(temp != NULL)temp->parent = Imb_node;
            Imb_node->parent = child;
            child->parent = source;

            //relink to source now
            if(source==NULL) 
            {
                root = child;
                return;
            }
            else
            {
                if(source->right == Imb_node)//Imb_Node WAS right child
                {
                    source->right = child;
                }
                else
                {
                    source->left = child;
                }
            }
        }
        //not designed to handle the colors right away
        //this is simply a rotator

        RBNode * find_sibling(RBNode * child)
        {
            RBNode* parent = child->parent;
            if(parent->left == child ) return parent->right;
            else return parent->left;
        }

        
        //btw parent always exists...uncle and grandparent may not
        void Balance(RBNode * child, RBNode* parent)
        {
            if(parent == NULL) return; // view uncle-'red' case
            if(parent->colour == 'b') return; //yay no problem here

            //now else case: parent->colour == 'r'
            //hence grandparent MUST be black for sure
            RBNode * grandparent = parent->parent;
            // if you reach here...grandparent MUST exist for sure

            RBNode * uncle = find_sibling(parent);

            if(uncle==NULL  || uncle->colour == 'b') //black uncle..in RB trees we consider NULL as black also
            {
                //rotation then recolouring
                //assuming grandparent exists..if not then still okay
                //as it will just be a NULL return point
                bool left_child = (parent->left == child)? true:false;


                if(grandparent->left == parent) //parent at left
                {
                    if(left_child)
                    {
                        rightRotate(grandparent);
                        grandparent->colour = 'r';
                        parent->colour = 'b';
                    }
                    else
                    {
                        leftRotate(parent);
                        rightRotate(grandparent);
                        grandparent->colour = 'r';
                        child->colour = 'b';//now child is in place of parent

                    }
                }

                else //parent at right
                {
                    if(left_child)
                    {
                        rightRotate(parent);
                        leftRotate(grandparent);
                        grandparent->colour = 'r';
                        child->colour = 'b';
                    }
                    else
                    {
                        leftRotate(grandparent);
                        grandparent->colour = 'r';
                        parent->colour = 'b';
                    }
                }

            }
            else if(uncle->colour == 'r')
            {
                parent->colour = 'b';
                uncle->colour = 'b';
                 //since uncle exists..grandparent must exist too
                 if(grandparent -> parent == NULL) //grandparent is the root
                 {
                        grandparent->colour = 'b';
                 }
                 else
                 {
                        grandparent->colour = 'r';
                        Balance(grandparent,grandparent->parent);
                 }
                
            }

            return;
        }

        bool balanced = false;
        //height() and balance_check() is not required thanks to RB tree properties
        void insert_helper(RBNode * current, RBNode * new_node)
        {
            //STEP 1: normal BST insertion, except the parent thing
            //here I am ordering this on the basis of timestamps

            if(new_node->time_stamp <= current->time_stamp)
            {
                if(current->left == NULL)
                {
                    current->left = new_node;
                    new_node->parent = current;
                }
                else
                {
                    current->left->parent = current;
                    //hence as we go along, the parents are being established also 
                    insert_helper(current->left,new_node);
                }
                
                //now current point becomes parent
            }
            else //if(new_node->time_stamp > current->time_stamp)
            {
                if(current->right == NULL)
                {
                    current->right = new_node;
                    new_node->parent = current;
                }
                else
                {
                    current->right->parent = current;//extra precaution
                    //hence as we go along, the parents are being established also 
                    insert_helper(current->right,new_node);
                }
            }

            //but wait wait...we are NOT done yet
            //finally, new node inserted at bottom and current is its parent

            //STEP 2: Validate RB Tree properties

            if(new_node->parent == current  && balanced == false) //so validation occurs only once
            //balancing only required ONCE in insertion case, but NOT the same for deletion
            {
                Balance(new_node,current);
                balanced = true;
                //Now just CLIMB to the top from here and whatever is 
                //the topmost thing..just make it the root

                while(current->parent != NULL) current = current->parent;
                root = current; //SUPER SUPER IMPORTANT step

                //because the tree is rotating around all the time...so root keeps changing
                 return;
            }
        }
        //this function always returns the links to root

        bool both_black_children(RBNode * node)
        {
            bool ans = true;

            if(node->left !=NULL)
            {
                if(node->left->colour == 'r')
                {
                    ans = false;
                }
            }
            if(node->right !=NULL)
            {
                if(node->right->colour == 'r')
                {
                    ans = false;
                }
            }

            return ans;
        }



        void delete_fixup(RBNode * d_black, RBNode * parent)
        {
            if(parent == NULL) //dbalck is root
            {
                root = d_black;
                return;
                //the black height of tree just went up by 1
                // as we converted double black to single without actually adding a black node
            }


            RBNode * sibling = (parent->left == d_black)? parent->right: parent->left;

            bool left_child = (parent->left == d_black)? true : false;
            //talking about dblack in left_child

            //Q: what if sibling is NULL?
            if(sibling == NULL)
            {
                delete_fixup(parent, parent->parent);
                return;
                //if sibling is null, then double black is pushed up to parent node
            }

            if(sibling->colour == 'r')
            {
                if(left_child)
                {
                    parent->colour = 'r';
                    sibling->colour = 'b';
                    leftRotate(parent);
                    

                    //now we will have a NEW sibling..whose colour is ALWAYS BLACK
                    //as it was child of a red node
                    //Hence, be go back to case of sibling->colour being black

                    if(parent == root)
                    {
                        root = parent->parent;
                    }

                    delete_fixup(d_black, parent);
                }
                else
                {
                    parent->colour = 'r';
                    sibling->colour = 'b';
                    rightRotate(parent);
                    

                    //root manipulation
                    if(parent == root)
                    {
                        root = parent->parent;
                        //should be 99 in my ex.
                    }

                    //now the new sibling is guaranteed to be red
                    delete_fixup(d_black, parent);
                }
            }
            else //sibling is black
            {
                
                if( both_black_children(sibling) )
                {
                    //both kids black
                    if(parent->colour == 'r')
                    {
                        parent->colour = 'b';
                        //red + black = double black
                        sibling->colour = 'r';//extra colouring...always try to do so
                        //wherever RED is possible,,it should be added to make tree more balanced
                        return;
                    }
                    else
                    {
                        sibling->colour = 'r';
                        delete_fixup(parent, parent->parent);
                    }


                }
                else//at least one of child sibling is red
                //check this else Logic out one more time during final testing
                {
                    bool left_red;
                    if(sibling->left != NULL)
                    {
                        if(sibling->left->colour == 'r') left_red = true;
                        else left_red = false;
                    }
                    else //if left is null i.e. black, then right must be red
                    {
                        left_red = false;
                    }

                    //don't assume that parent is black only
                    if(left_child) //dblack is left child
                    {
                        if(left_red)
                        {
                            sibling->left->colour = 'b';
                            //colour then rotate..as rotation CAN change concepts 
                            //of left and right
                            rightRotate(sibling);
                            leftRotate(parent);
                            
                        }
                        else
                        {
                                sibling->right->colour = 'b';
                                sibling->colour = parent->colour;
                                leftRotate(parent);
                                
                                
                        }
                        parent->colour = 'b';
                    }
                    else //dblack is right child
                    {
                        if(left_red)
                        {
                                sibling->left->colour = 'b';
                                sibling->colour = parent->colour;
                                rightRotate(parent);
                                
                                
                        }
                        else
                        {
                                sibling->right->colour = 'b';
                                leftRotate(sibling);
                                rightRotate(parent);
                                
                        }
                        parent->colour = 'b';
                    }
                    //root manipulation
                    if(parent == root)
                    {
                    root = parent->parent;
                    }            

                }
            }
    }


    RBNode * smallest(RBNode * node)
    {
        while(node && node->left)
        {
            node= node->left;
        }
        return node;
    }

    RBNode * replacement(RBNode * node)
    {
        //leaf
        if(node->left == NULL && node->right == NULL) return NULL;

        //one child
        if(node->left == NULL) return node->right;
        else if(node->right == NULL) return node->left;

        //else : only option left: both not null
        else return smallest(node->right); //inorder successor
    }

    void swap_data(RBNode * n1, RBNode * n2)
    {
        swap(n1->key, n2->key);
        swap(n1->value, n2->value);
        swap(n1->time_stamp, n2->time_stamp);
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:
    RBNode * LRU_node; //keep updating this node as we go deleting LRU elements
    RBNode * root; //make this private later..this is just for Inorder Traversal testing

    void printLinks(RBNode* node) 
    {
        // cout<<" at end of insertion root is "<<root->time_stamp<<endl;
        if (!node) return;
        cout << "Node " << node->time_stamp << ": "<<node->value<<" ";
        if (node->left) cout << "Left->" << node->left->time_stamp << " ";
        else cout << "Left->NULL ";
        if (node->right) cout << "Right->" << node->right->time_stamp << " ";
        else cout << "Right->NULL ";
        if (node->parent) cout << "Parent->" << node->parent->time_stamp << " ";
        else cout << "Parent->NULL ";
        cout << "Colour: " << node->colour << endl;
        printLinks(node->left);
        printLinks(node->right);
    }

    RBNode  * insert(int key, string val, int time_stamp)
    {
        RBNode * new_node = new RBNode(key, val, time_stamp);
        //note: default colour is red

        if(root == NULL)
        {
            root = new_node;
            LRU_node = root;
            new_node->colour = 'b'; // root is always black
        }
        else
        {
            balanced = false;//restore the balanced variable every time
            insert_helper(root,new_node);
            //old root, new_node
        }

        return new_node;
        //I will thus be returning the new_node's address so that it can be put into the 
        //hashmap also
        
        //hashmap shall have the key followed by node address
    }

    void Inorder(RBNode * node)
    {
        if(node == NULL) return;

        Inorder(node->left);
        cout<<node->key<<":"<<node->value<<" ("<<node->time_stamp <<") colour: "<<node->colour<<endl;    
        Inorder(node->right);
    }

    void delete_node(RBNode * v)
    {
        RBNode * rep = replacement(v);
        RBNode * parent = v->parent;
        bool left_child = true;
        bool fixup_done = false;

        if(v==LRU_node) //dont forget v is the leftmost node...so v->left = null always
        //so inorder successor will always be either parent or right child
        {
            if(v->right == NULL)
            {
                LRU_node = v->parent;
            }
            else 
            {
                LRU_node = v->right;
            }
        }

        if(parent!=NULL)
         left_child = (parent->left == v) ? true: false;

         //NEVER DO THIS ON BASIS OF TIMESTAMPS...BECAUSE WE HAVE SWAPPING OCCURING TOO 
        
        bool both_black = (rep == NULL || rep->colour == 'b') && v->colour == 'b' ? true: false;

        if(both_black) // hence double blacks are going to be created here
        {

            RBNode * d_black;

            if(rep == NULL) //hence v is leaf
            {
                //enetered here
                if(parent == NULL)//root
                {
                    root = NULL;
                    //empty tree now
                }
                else if(left_child)
                {
                    parent->left = NULL;
                }
                else
                {
                    parent->right = NULL;
                }
                delete(v);
                d_black = NULL;
                //so HOW will we locate it?-> parent of dblack is still the same as v's parent...
            }
            else if(rep==v->right || rep==v->left) // rep is a child of v..AND a leaf
            {
                if(parent == NULL) //v can even be the root..so it Must be black
                {
                    swap_data(v,rep);
                    delete_node(rep);
                    fixup_done = true;
                }
                else if(left_child) 
                {
                    parent->left = rep;
                    rep->parent = parent;
                    delete(v);
                }
                else
                {
                    parent->right = rep;
                    rep->parent = parent;
                    delete(v);
                }
                
                d_black = rep;

            }
            else //rep is inorder successor of v
            {
                swap_data(v,rep);
                delete_node(rep); //recur this to the rep node now
                fixup_done = true;
                //also, rep is always a leaf node in this case so the value it gets doesn't matter
                //now if rep is red...itll be direct deleted..but this is both black case
                // else it will get a delete_fixup workout 
            }

            //Now time for dblack fixing
            if(fixup_done == false)
            delete_fixup(d_black, parent);
            //parent is just in case dblack is NULL

        }
        else //on of them is red..easier case
        {
            //copy data from rep to v and delete one of them

            if(rep == NULL) //hence v is leaf..and a red one for sure
            {
                if(parent == NULL)
                {
                    root = rep;
                    rep->colour = 'b';
                }
                else if(left_child)
                {
                    parent->left = NULL;
                }
                else
                {
                    parent->right = NULL;
                }
                delete(v);
            }
            else if(rep==v->right || rep==v->left) // rep is a child of v..AND a leaf
            {
                if(parent == NULL) //v can even be the root..so it Must be black
                {
                    if(rep == v->right)
                    {
                        rep->left = v->left;
                        if(v->left != NULL)
                            v->left->parent = rep;
                    }
                    else
                    {
                        rep->right =v->right;
                        if(v->right != NULL)
                            v->right->parent = rep;
                    }

                    root = rep;
                    rep->colour = 'b';
                    rep->parent = NULL;
                }
                else if(left_child) 
                {
                    parent->left = rep;
                    rep->parent = parent;
                }
                else
                {
                    parent->right = rep;
                    rep->parent = parent;
                }
                rep->colour = 'b';
                delete(v);


            }
            else //rep is inorder successor of v
            {
                swap_data(v,rep);
                v->colour = 'b';
                delete_node(rep); //recur this to rep node now
            }
        }
    }


    RBTree()
    {
        root = NULL;
        LRU_node = NULL;
    }

};






class LRUCache{
    private: 
    int capacity;   //determines the size of cache
    int time;       //used for modification of timestamp
    unordered_map<int, RBNode*> map; //hashmap to store the nodes
    RBTree tree; 
    int hits; // Cache hit counter
    int misses; // Cache miss counter
    int evictions; // Eviction counter
    long long totalAccessTime; // Total access time for cache operations

    //function to increase timestamp
    void increaseTimestamp(RBNode* node) 
    {
        node->time_stamp = ++time;
    }

     RBNode* leftmost() 
     {
        // Traverses the tree to find the leftmost (least recently ised with smallest timestamp) node
        RBNode* cur = tree.root;
        while (cur && cur->left != nullptr) {
            cur = cur->left;
        }
        return cur;
    }
    public:

    LRUCache(int cap) : capacity(cap), time(0) 
    {
        evictions =0;
        hits =0;
        misses =0;
        totalAccessTime =0;

    } //constructor for lru cache with capacity cap

        string get(int key) {

        auto start = chrono::high_resolution_clock::now();

        if (map.find(key) == map.end()) {
            misses++; // Record cache miss
            return "not found"; // Key not found
            
        }
        RBNode* node = map[key]; //find the node with key
        string tempo = node->value;

        //first delete value from tree
        tree.delete_node(node);

        //then reinsert with new timestamp
        RBNode* new_node = tree.insert(node->key, node->value, ++time);
        map[key] = new_node;
        
        hits++; // Record cache hit
        auto end = chrono::high_resolution_clock::now();
        totalAccessTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        
        return tempo; //return its value
    }

    void put(int key, string value) {

        auto start = chrono::high_resolution_clock::now();

        if (map.find(key) != map.end()) {
            // if Key already exists, update its value and timestamp
            RBNode* old_node = map[key]; //find node with key
            tree.delete_node(old_node);
            increaseTimestamp(old_node); //increase timestamp;
        }
        // if key does not exist already
         else 
         {
            if (map.size() == capacity) {
                // Cache is full, remove the least recently used (leftmost node in RBTree)
                if(tree.LRU_node)
                {
                    map.erase(tree.LRU_node->key);
                    tree.delete_node(tree.LRU_node);
                    evictions++;
                }
            }
            // Insert the new node in the cache and the RBTree
            
        }

        RBNode* newNode = tree.insert(key, value, ++time);
        map[key] = newNode;
        auto end = chrono::high_resolution_clock::now();
        totalAccessTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    

    double HitRate() const{                     //Measures how often the requested data is found in the cache 
                                                //without requiring a new insertion. This shows how effectively
                                                // the cache is meeting requests.
        return (double)hits / (hits + misses);
        //number of time we find key the data/total number of times we look for the data
    }

    long long AverageAccessTime() const{       //Measures the average time taken to retrieve a value from the cache.
                                               // This reflects the speed of cache operations.
        int totalOps = hits + misses;
        return totalOps > 0 ? (long long)totalAccessTime / totalOps : 0; 
        //total time/number of ops gives avg access time
    }

    int EvictionCount() const {                //Counts the number of evictions (when the cache removes the least 
                                               //recently used item to make space for new ones). A high eviction 
                                               //count can suggest that the cache size is too small for the workload. 
        return evictions;
    }

};





int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }

    // Get the capacity from the argument
    double capacity = atoi(argv[1]);  // Convert the input to a double

    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " <capacity>\n";
    //     return 1;
    // }

    // int capacity = std::atoi(argv[1]);

    // if (capacity <= 0) {
    //     std::cerr << "Capacity must be a positive integer\n";
    //     return 1;
    // }

    // int capacity;
    // cin>>capacity;

    LRUCache L(capacity);

    typedef struct Data100
    {
        string operation;
        int key;
        string value;
    } Data;

    ifstream file("0data.txt");
    if (!file) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    Data d;
    string line;

    // Process each line of the file
    while (getline(file, line)) 
    {
        istringstream iss(line);
        iss >> d.operation >> d.key;

        // If the operation is "put", read the rest of the line as the value
        if (d.operation == "put") {
            getline(iss >> ws, d.value);  // Read remaining line as value
        } else {
            d.value.clear(); // Clear any previous value for "get" operation
        }

        // Output the operation for debugging
        if (d.operation == "put") {
            // cout << ", Value: " << d.value;
        }

        // Perform the cache operation
        if (d.operation == "get") {
            L.get(d.key);
        } else if (d.operation == "put") {
            L.put(d.key, d.value);
        }
    }

    file.close();

    double hit_rate = L.HitRate(), total_access_time = L.AverageAccessTime();
    double eviction_count= L.EvictionCount();

    //final output is here
    cout<<hit_rate<<endl<<total_access_time<<endl<<eviction_count;

}