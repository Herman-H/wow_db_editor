#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <vector>
#include <cstring>

// Users of buffers need to allow for serialization of the data by declaring "get_data"
namespace dbutil
{
template <typename T>
const char * get_data(T & t);

/*
 * Motivation: Using this reasonably simple implementation ensures better locality of data than other implementations of
 * non-fixed size queues.
 *
 * - Underlying vectors capacity does not shrink if elements are removed
 *
 * - Possible states for one of the vectors are (1) "appending", (2) "insertion behind head", (3) "locked"
 *      - (1) Appending is simply adding data to the end of the vector
 *      - (2) If elements are removed at the front, then insertion can begin behind head
 *      - (3) Vector becomes locked while being in state (2) when insertion means overwriting head
 *
 * - Two vectors are used so that data can still be written while one gets into state (3)
 *
 * - Note that this means that second vector can only be in state one until all elements are removed from the first.
 *      - This puts first vector back into state (1)
 *      - Removing elements from second vector means it can now come into state (2) and (3)
 *      - When second vector is in state (3), appending to the first, now empty, vector begins again
 *
 * - The current implementation is not threadsafe
*/
class queue
{
private:
    std::vector<char> v0,v1;
    int m_head0,m_tail0,m_end0,m_head1,m_tail1,m_end1;
    int m_insertion_vector;
    int m_removing_vector;
    enum  vector_enum{
        VECTOR_V0 = 0,
        VECTOR_V1 = 1
    };

    template <std::size_t SIZE>
    bool insert_data(std::vector<char>& v, int & head, int & tail, int & end, const char * data,
                     std::ptrdiff_t start_align_offset, std::ptrdiff_t tail_align_offset)
    {
        if(tail > head && head >= (SIZE + start_align_offset))
        {
            // We have been appending the vector but can now start inserting at the beginning again [(1) -> (2)]
            char * next_alloc = v.data() + start_align_offset;
            tail = next_alloc + SIZE; // Note that this implies that tail points to the first element that is free
            memcpy(next_alloc,data,SIZE);
            return true;
        }
        else if(tail < head && head < (tail + tail_align_offset + SIZE))
        {
            // It does not work to allocate in vector v at this point, discovered locked state [(2) -> (3)]
            return false;
        }
        else if(tail < head)
        {
            // It does work to insert behind head here [(2) -> (2)]
            char * next_alloc = v.data() + tail + tail_align_offset;
            tail = next_alloc + SIZE;
            memcpy(next_alloc,data,SIZE);
            return true;
        }
        else
        {
            // Simply append the vector [(1) -> (1)]
            for(int i = 0; i < SIZE; ++i)
            {
                v.push_back(static_cast<char>(0));
            }
            for(int i = 0; i < tail_align_offset; ++i)
            {
                v.push_back(static_cast<char>(0));
            }
            char * next_alloc = v.data() + tail_align_offset;
            memcpy(next_alloc,data,SIZE);
            tail += SIZE + tail_align_offset;
            end = tail;
            return true;
        }
        // getting here should be impossible
        return false;
    }
    template <typename T>
    bool remove_data(std::vector<char> & v, int & head, int & tail, int & end, char *& data)
    {
        std::ptrdiff_t head_align_offset = (alignof(T) - static_cast<std::ptrdiff_t>((v.data() + head) % alignof(T)));
        data = v.data() + head + head_align_offset;
        bool unlocked = false;
        if(head + head_align_offset + sizeof(T) == end)
        {
            head = 0;
            if(end == tail)
            {
                // (3) -> (1)
                // If we want to shrink capacity we only need to do it here
                // But I dont want to do it, because if we reach a capacity its likely to be reached again.
                /*
                v.clear();
                v.shrink_to_fit();
                */
                end = 0;
                tail = 0;
                unlocked = true;
            }
            else
            {
                end = tail;
            }
        }
        else
        {
            head += head_align_offset + sizeof(T);
        }

        return unlocked;
    }

public:
    queue_buffer(int capacity) : v0(capacity),v1(),
        m_head0(0),m_tail0(0),m_end0(0),m_head1(0),m_tail1(0),m_end1(0),
        m_insertion_vector(VECTOR_V0),m_removing_vector(VECTOR_V0)
    {

    }

    ~queue_buffer(){}

    template <typename T>
    int insert_element(T & t)
    {
        std::ptrdiff_t start_align_offset0 = (alignof(T) - static_cast<std::ptrdiff_t>(v.data() % alignof(T)));
        std::ptrdiff_t tail_align_offset0 = (alignof(T) - static_cast<std::ptrdiff_t>((v.data() + tail) % alignof(T)));
        std::ptrdiff_t start_align_offset1 = (alignof(T) - static_cast<std::ptrdiff_t>(v.data() % alignof(T)));
        std::ptrdiff_t tail_align_offset1 = (alignof(T) - static_cast<std::ptrdiff_t>((v.data() + tail) % alignof(T)));
        if(m_insertion_vector == VECTOR_V0)
        {
            if(!insert_data<sizeof(T)>(v0,m_head0,m_tail0,m_end0,get_data(t),start_align_offset0,tail_align_offset0))
            {
                m_insertion_vector = VECTOR_V1;
                insert_data<sizeof(T)>(v1,m_head1,m_tail1,m_end1,get_data(t),start_align_offset1,tail_align_offset1);
            }
        }
        else if(m_insertion_vector == VECTOR_V1)
        {
            if(!insert_data<sizeof(T)>(v1,m_head1,m_tail1,m_end1,get_data(t),start_align_offset1,tail_align_offset1))
            {
                m_insertion_vector = VECTOR_V0;
                insert_data<sizeof(T)>(v0,m_head0,m_tail0,m_end0,get_data(t),start_align_offset0,tail_align_offset0);
            }
        }
    }

    template <typename T>
    T remove_element()
    {
        char * data;
        if(m_removing_vector == VECTOR_V0)
        {
            if(remove_data<T>(v0,m_head0,m_tail0,m_end0,data))
            {
                // v0 is now empty and unlocked
                if(m_insertion_vector == VECTOR_V1)
                {
                    m_removing_vector = VECTOR_V1;
                }
            }
        }
        else if(m_removing_vector == VECTOR_V1)
        {
            if(remove_data<T>(v1,m_head1,m_tail1,m_end1,data))
            {
                // v1 is now empty and unlocked
                if(m_insertion_vector == VECTOR_V0)
                {
                    m_removing_vector = VECTOR_V0;
                }
            }
        }
        // Reinterpret the data at "data" as type T and use copy constructor of that data
        return T{*reinterpret_cast<T*>(data)};
    }

    bool hasNext()
    {
        if(m_removing_vector == VECTOR_V0)
        {
            return m_end0 != 0;
        }
        else if(m_removing_vector == VECTOR_V1)
        {
            return m_end1 != 0;
        }
        return false;
    }
};

} // namespace dbutil
#endif // CIRCULARQUEUE_H
