#include <vector>
#include <algorithm>

inline unsigned int getLeftIndex(unsigned int index) { return 2*index + 1; }
inline unsigned int getRightIndex(unsigned int index) { return 2*index + 2; }

template <typename type_t, typename function_t>
void buildHeap(std::vector<type_t> &array, int first, int last, function_t greater_than) 
{
    //all elements from n/2 to n are leafs
    for(int i=last/2 - 1; i>=first; --i)
        heapify(array,first,last,i,greater_than);
}

template <typename type_t, typename function_t>
void heapify(std::vector<type_t> &array, int first, int last, int index,  function_t greater_than)
{
    //left and right indexes of an array based heap
    int leftIndex = getLeftIndex(index) + first;
    int rightIndex = getRightIndex(index) + first;
    int maxIndex = 0;
    
    //if left child is greater than current index
    if(leftIndex < (last-first) && greater_than(array[leftIndex],array[index])) 
        maxIndex = leftIndex;
    else 
        maxIndex = index;
    //if right child is greater than current index
    if(rightIndex < (last-first) && greater_than(array[rightIndex],array[maxIndex])) 
        maxIndex = rightIndex;

    //if largest value isn't current index swap
    if(index != maxIndex) 
    {   
        std::iter_swap(array.begin()+first+maxIndex,array.begin()+first+index);
        //fix heap's properties
        heapify(array, first, last, maxIndex,greater_than);
    }
}

template <typename type_t, typename function_t>
void heapsort(std::vector<type_t> &array, int first, int last, function_t greater_than)
{
    //building max heap from an array
    buildHeap(array,first,last,greater_than);
    //sorting array
    for(int i=last-1; i>first; --i)
    {
        //put largest element at the back of an array
        std::iter_swap(array.begin()+first,array.begin()+first+i);
        //heap properties could be violated, but children
        //are max heaps, calling heapify fixes this
        heapify(array,first,last,0,greater_than);
    }
}