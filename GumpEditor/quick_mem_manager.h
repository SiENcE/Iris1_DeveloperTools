

#ifndef _QUICKMEMMANAGERH_LOADED_
#define _QUICKMEMMANAGERH_LOADED_

#include <windows.h>
#include <vector>
#include <algorithm>


struct QUICK_MEM_RELEASE {
   void operator()(void *p) { //NOTHROW
      if (p)
         free(p);
   }
}; //QUICK_MEM_RELEASE


class QUICK_MEM_MANAGER {
private:

   enum { STRUCTS_PER_BLOCK = 2048 };
   typedef unsigned char BYTE;

   size_t m_size;
   size_t m_block_size;
   BYTE *m_block;
   int m_left;
   void *m_first;
   CRITICAL_SECTION m_gate_keeper;

   typedef std::vector<BYTE *> MY_VEC;
   MY_VEC m_vec;

public:
   QUICK_MEM_MANAGER(size_t s) : m_size(s), m_first(0)
      , m_block_size(STRUCTS_PER_BLOCK * s) { //NOTHROW 

      InitializeCriticalSection(&m_gate_keeper);
   }

   ~QUICK_MEM_MANAGER() {  //NOTHROW
      
      EnterCriticalSection(&m_gate_keeper);
      std::for_each(m_vec.begin(), m_vec.end(), QUICK_MEM_RELEASE());
      LeaveCriticalSection(&m_gate_keeper);

      DeleteCriticalSection(&m_gate_keeper);
   }
     

   void add_block() {  //>x

      BYTE *p = (BYTE *) ::malloc(m_block_size);
      if (!p) 
         throw std::bad_alloc();
      

      m_vec.push_back(p);
      m_block = p;
      m_left = m_block_size;
      m_block_size = (m_block_size * 3) / 2;  //Expand by a 1.5 factor
   }

   void *malloc() {  //>x

      void *p = 0;

      EnterCriticalSection(&m_gate_keeper);

      if(m_first) {         
         p = m_first; //Extract top-most buffer
         m_first = *(static_cast<void**>(m_first)); // Advance to next element
      }
      else {
         if(!m_left)
            add_block();

         p = m_block;

         m_block += m_size;
         m_left -= m_size;
      }
      
      LeaveCriticalSection(&m_gate_keeper);
      
      return p;
   }

   void free(void *p) {  //NOTHROW
      if (!p)
         return;

      EnterCriticalSection(&m_gate_keeper);

      //Place the buffer (p) on the free list:
      void** temp = static_cast<void**>(p);
      *temp = m_first;  //Place a pointer to the current top
      m_first = temp;   //Update top of list pointer

      LeaveCriticalSection(&m_gate_keeper);
   }
}; //QUICK_MEM_MANAGER




#endif //_QUICKMEMMANAGERH_LOADED_
