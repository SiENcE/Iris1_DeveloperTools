
#ifndef _AUTOREFH_LOADED_
#define _AUTOREFH_LOADED_


#include <windows.h>
#include <algorithm>
#include "quick_mem_manager.h"


//Prototypes
extern QUICK_MEM_MANAGER g_the_auto_ref_pool;
   
//AUTO_REF Class defintion
template <typename T>
class AUTO_REF {
private:
   long *m_ref_p;
   T *m_inst_p;

public:
   AUTO_REF(T *inst_p = 0) : m_inst_p(inst_p) {  //>x
      m_ref_p = static_cast<long*>(g_the_auto_ref_pool.malloc());
      *m_ref_p = 1;
   }

   AUTO_REF(const AUTO_REF<T> &other) 
      : m_ref_p(other.m_ref_p), m_inst_p(other.m_inst_p) {  //NOTHROW

      InterlockedIncrement(m_ref_p);
   }

   ~AUTO_REF() {  //NOTHROW
      if (!InterlockedDecrement(m_ref_p)) {
         delete m_inst_p;
         g_the_auto_ref_pool.free(m_ref_p);
      }
   }

   AUTO_REF &operator=(const AUTO_REF<T> &rhs) {  //NOTHROW

      AUTO_REF<T> temp(rhs);
      swap(temp);

      return *this;
   }

   T &operator*() const {     //NOTHROW
      return *m_inst_p;
   }

   T *operator->() const {    //NOTHROW
      return m_inst_p;
   }

   operator bool() const {    //NOTHROW
      return m_inst_p != 0;
   }

   T *get() const {           //NOTHROW
      return m_inst_p;
   }

   void swap(AUTO_REF<T> &other) {   //NOTHROW
      std::swap(m_ref_p, other.m_ref_p);
      std::swap(m_inst_p, other.m_inst_p);
   }

};//AUTO_REF

#endif //_AUTOREFH_LOADED_



