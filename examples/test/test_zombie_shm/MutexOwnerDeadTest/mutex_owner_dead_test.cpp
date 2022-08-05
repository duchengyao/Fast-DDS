//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2004-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/detail/workaround.hpp>

//Robust mutex handling is only supported in conforming POSIX systems

#include <boost/interprocess/detail/os_thread_functions.hpp>

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include "mutex_test_template.hpp"
#include <iostream>


namespace boost {
namespace interprocess {
namespace test {

template<typename M>
void lock_and_exit(void *, M &sm)
{
   sm.lock();
}

enum EOwnerDeadLockType
{
   OwnerDeadLock,
   OwnerDeadTryLock,
   OwnerDeadTimedLock
};

template<class M>
void test_owner_dead_mutex_do_lock(M &mtx, EOwnerDeadLockType lock_type)
{
   switch (lock_type)
   {
      case OwnerDeadLock:
      mtx.lock();
      break;

      case OwnerDeadTryLock:
      mtx.try_lock();
      break;

      case OwnerDeadTimedLock:
      mtx.timed_lock(boost_systemclock_delay(10));
      break;
   }
}

template<class M>
int test_owner_dead_mutex_impl(EOwnerDeadLockType lock_type)
{
   using namespace boost::interprocess;
   M mtx;

   // Locker thread launches and exits without releasing the mutex
   ipcdetail::OS_thread_t tm1;
   ipcdetail::thread_launch(tm1, thread_adapter<M>(&lock_and_exit, 0, mtx));
   ipcdetail::thread_join(tm1);
   try {
      test_owner_dead_mutex_do_lock(mtx, lock_type);
   }
   catch (lock_exception& e) {
      if (e.get_error_code() == not_recoverable){
         //Now try once again to lock it, to make sure it's not recoverable
         try {
            test_owner_dead_mutex_do_lock(mtx, lock_type);
         }
         catch (lock_exception& e) {
            if (e.get_error_code() == not_recoverable)
               return 0;
            else{
               std::cerr << "e.get_error_code() != not_recoverable! (2)";
               return 3;
            }
         }
         catch (...) {
            std::cerr << "lock_exception not thrown! (2)";
            return 4;
         }
         std::cerr << "Exception not thrown (2)!";
         return 5;
      }
      else{
         std::cerr << "e.get_error_code() != not_recoverable!";
         return 1;
      }
   }
   catch (...) {
      std::cerr << "lock_exception not thrown!";
      return 2;
   }
   std::cerr << "Exception not thrown!";
   return 3;
}

template<class M>
int test_owner_dead_mutex()
{
   int ret;
   ret = test_owner_dead_mutex_impl<M>(OwnerDeadLock);
   if (ret) return ret;
   ret = test_owner_dead_mutex_impl<M>(OwnerDeadTryLock);
   if (ret) return ret;
   ret = test_owner_dead_mutex_impl<M>(OwnerDeadTimedLock);
   return ret;
}

}}}   //boost::interprocess::test


int main ()
{
   using namespace boost::interprocess;
   int ret;
   std::cerr << "Testing interprocess_mutex";
   ret = test::test_owner_dead_mutex<interprocess_mutex>();
   if (ret)
      return ret;

   #ifdef BOOST_INTERPROCESS_POSIX_RECURSIVE_MUTEXES
   std::cerr << "Testing interprocess_recursive_mutex";
   ret = test::test_owner_dead_mutex<interprocess_recursive_mutex>();
   if (ret)
      return ret;
   #endif

   return ret;
}
