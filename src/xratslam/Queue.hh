/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Implements a thread-safe queue (FIFO) container for object pointers.
 * 
 */

#ifndef _xratslam_Queue_
#define _xratslam_Queue_

#include <queue>
#include <mutex>
#include <condition_variable>


/*************************************************************************/
/********************************* Queue *********************************/

namespace xratslam
{
  /****************/
  template <class T>   // Warn: the queue stores (T*), not (T) objects!
  class Queue
  {
  public:

    Queue( int maxSize )
    {
      _canBlock = true;
      _maxSize  = maxSize;
    }
    
    ~Queue()
    { clear(); }


    /** Return true is queue is empty.
     */
    bool empty()
    {
      std::lock_guard<std::mutex> lock( _mutex );
      return _queue.empty();
    }
    
    /** Make queue non-blocking and unblock all blocked threads.
     */
    void unblock()
    {
      std::lock_guard<std::mutex> lock( _mutex );

      _canBlock = false;
      _cvPush.notify_all();
      _cvPush.notify_all();      
    }
    
      
    /** Let queue return to block threads.
     */
    void restoreBlock()
    {
      _canBlock = true;
    }

    
    /** Return current queue size.
     */
    int size()
    {
      std::lock_guard<std::mutex> lock( _mutex );
      return _queue.size();
    }
    
      
    /** Add an element to the queue.
     * If queue is full, blocks until there is some room.
     */
    void push( T *elem )
    {
      std::unique_lock<std::mutex> lock( _mutex );

      // Blocks waiting for some vacancy place.
      while ( _queue.size() >= _maxSize && _canBlock )
        _cvPop.wait( lock );
  
      _queue.push( elem );
      
      _cvPush.notify_one();
    }

    
    /** Get the "front"-element.
     * If the queue is empty, wait till a element is avaiable.
     */
    T *pop()
    {
      std::unique_lock<std::mutex> lock( _mutex );

      // Blocks waiting for some element.
      while ( _queue.empty() && _canBlock )
        _cvPush.wait( lock );

      T *elem = 0;
      if ( ! _queue.empty() )
      {
        elem = _queue.front();
        _queue.pop();
        _cvPop.notify_one();
      }
      
      return elem;
    }

    /** Remove and destroys all elements on the current queue.
     */
    void clear()
    {
      std::lock_guard<std::mutex> lock( _mutex );

      while ( ! _queue.empty() )
      {
        delete( _queue.front() );
        _queue.pop();
      }
    }
    
      
  private:

    int                     _maxSize;
    std::queue<T*>          _queue;
    std::mutex              _mutex;
    std::condition_variable _cvPop;
    std::condition_variable _cvPush;
    
    bool _canBlock;
  };
}

#endif
