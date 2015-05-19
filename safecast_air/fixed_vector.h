#ifndef FIXED_VECTOR_H
#define FIXED_VECTOR_H

template<typename T, unsigned int n>
class FixedVector
{
    public:
        FixedVector() {};
        unsigned int size() { return n;};
        T& operator[](int index)  {return item_[index];};

    protected:
        T item_[n];
};

#endif
