/* AbiSource Program Utilities
 * Copyright (C) 2001 Dom Lachowicz <dominicl@seas.upenn.edu>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

#ifndef UT_WORKER_H
#define UT_WORKER_H

class GR_Graphics;
class UT_Worker;

typedef void (*UT_WorkerCallback)(UT_Worker* pWorker);

/*!
 * Class which is used to construct new instances of 
 * UT_Workers based on a passed mode, whose choices
 * can be ORed together
 */
class UT_WorkerFactory
{
 public:
  typedef enum { NONE   = 0x00,
		 IDLE   = 0x01,
		 TIMER  = 0x02 } ConstructMode;

  //CAN_USE_THREAD   = 0x04

  static UT_Worker * static_constructor ( UT_WorkerCallback cb, void * data, 
					  int wantMode, 
					  UT_WorkerFactory::ConstructMode & outMode, 
					  GR_Graphics * pG=0 );

 private:
  UT_WorkerFactory ();
  ~UT_WorkerFactory ();
};

/*!
 * This class is a generic "worker" class which will
 * serve as a base-class for UT_Timers, UT_Idles, and
 * UT_Threads
 */
class UT_Worker
{

 public:
  virtual ~UT_Worker ();

  virtual void stop(void) = 0;		/* suspend events */
  virtual void start(void) = 0;		/* resume events */

  virtual void fire(void);              /* fire off an event */

  UT_WorkerCallback getCallback() const;
  void* getInstanceData() const;

 protected:

  UT_Worker ();
  UT_Worker (UT_WorkerCallback cb, void * data);

  void _setCallback(UT_WorkerCallback cb);
  void _setInstanceData(void * data);

 private:
  UT_Worker (UT_Worker &); // no impl

  void * m_pInstanceData;
  UT_WorkerCallback m_pCallback;

};

#endif /* UT_WORKER_H */
