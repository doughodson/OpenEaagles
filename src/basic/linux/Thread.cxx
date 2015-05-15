//------------------------------------------------------------------------------
// Thread classes -- Linux version
//------------------------------------------------------------------------------

#include <signal.h>

// max number of processors we'll allow
static const unsigned int MAX_CPUS = 32;

//==============================================================================
// class Thread
//==============================================================================

//-----------------------------------------------------------------------------
// Static thread function
//-----------------------------------------------------------------------------
void* Thread::staticThreadFunc(void* lpParam)
{
   Thread* thread = static_cast<Thread*>(lpParam);
   Component* parent = thread->getParent();

   // Make sure that our Thread class and its parent are not going to go a way.
   thread->ref();
   parent->ref();

   // The main thread function, which is a Thread class memeber function,
   // will handle the rest.
   unsigned long rtn = thread->mainThreadFunc();
   thread->setTerminated();

   parent->unref();
   thread->unref();

   return reinterpret_cast<void*>(rtn);
}

//-----------------------------------------------------------------------------
// Static function returns the number of processors assigned to this process
//-----------------------------------------------------------------------------
unsigned short Thread::getNumProcessors()
{
   unsigned short num = 0;

   cpu_set_t mask;
   int rtn = sched_getaffinity(0, sizeof(cpu_set_t), &mask);
   if (rtn == 0) {
      // we're checking the number of bits that are set in 'mask'
      for (unsigned int cpu = 0; cpu < MAX_CPUS; cpu++) {
         if ( CPU_ISSET(cpu, &mask) != 0 ) num++;
      }
   }

   return num;
}

//-----------------------------------------------------------------------------
// Create the thread
//-----------------------------------------------------------------------------
bool Thread::createThread()
{
   // thread attributes
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   // ---
   // Set the thread schedule priority
   //   priority > 0 use SCHED_FIFO polity with % of FIFO priority
   //   priority == 0; use SCHED_OTHER
   // ---
   struct sched_param param;
   {
      if (priority > 0.0f && priority <= 1.0f) {
         float maxp = sched_get_priority_max(SCHED_FIFO);
         float minp = sched_get_priority_min(SCHED_FIFO);
         float value = (maxp - minp);
         if (priority      == 1.0f) value *= 1.0f;
         else if (priority >= 0.9f) value *= 0.9f;
         else if (priority >= 0.8f) value *= 0.8f;
         else if (priority >= 0.7f) value *= 0.7f;
         else if (priority >= 0.6f) value *= 0.6f;
         else if (priority >= 0.5f) value *= 0.5f;
         else if (priority >= 0.4f) value *= 0.4f;
         else if (priority >= 0.3f) value *= 0.3f;
         else if (priority >= 0.2f) value *= 0.2f;
         else if (priority  > 0.1f) value *= 0.1f;
         else value = 0.0f;
         param.sched_priority = nint(value + minp);
         pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
      }
      else {
         param.sched_priority = sched_get_priority_max(SCHED_OTHER);
         pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
      }

      pthread_attr_setschedparam(&attr, &param);
   }

   // ---
   // Thread stack size
   // ---
   if (stackSize > 0) {
      pthread_attr_setstacksize(&attr, stackSize);
   }

   // ---
   // Create the thread
   // ---
   pthread_t* thread = new pthread_t;
   pthread_create(thread, &attr, staticThreadFunc, this);

   //if ( stat != 0 && parent->isMessageEnabled(MSG_INFO) ) {
      std::cout << "Thread(" << this << ")::createThread(): pthread_create() thread = " << thread << ", pri = " << param.sched_priority << std::endl;
   //}

   theThread = thread;

   return (theThread != nullptr);
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool Thread::configThread()
{
   // Nothing to do
   return true;
}

//-----------------------------------------------------------------------------
// Close the thread
//-----------------------------------------------------------------------------
void Thread::closeThread()
{
}

//-----------------------------------------------------------------------------
// Treminate the thread
//-----------------------------------------------------------------------------
bool Thread::terminate()
{
   if (theThread != nullptr && !killed) {
      if ( getParent()->isMessageEnabled(MSG_INFO) ) {
         std::cout << "Thread(" << this << ")::terminate(): handle = " << theThread << std::endl;
      }

      pthread_t* thread = static_cast<pthread_t*>(theThread);
      pthread_kill(*thread, SIGKILL);
      theThread = nullptr;
      killed = true;

      // The staticThreadFunc() function ref()'s 'this' Thread class and our
      // parent object, but since we've been terminated, the thread won't be
      // returning via staticThreadFunc(), so we need to unref() here.
      getParent()->unref();
      this->unref();
   }
   return killed;
}


//==============================================================================
// class ThreadPeriodicTask
//==============================================================================

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long ThreadPeriodicTask::mainThreadFunc()
{
   if (getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "Thread(" << this << ")::mainLoopFunc(): Parent = " << getParent() << std::endl;
      std::cout << "Thread(" << this << ")::mainLoopFunc(): Starting main loop ..." << std::endl;
   }

   // Delta time
   const double dt = 1.0/static_cast<double>(getRate());
   int sec0 = static_cast<int>(dt);
   int nsec0 = static_cast<int>((dt - static_cast<double>(sec0)) * 1000000000.0);

   // Reference time
   struct timespec tp;
   clock_gettime(CLOCK_REALTIME, &tp);

   // Mutex
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutex_lock(&mutex);

   // ---
   // Inital wait for one frame --
   // --- Linux seems to need this otherwise the userFunc() call failes.
   // ---
   tp.tv_nsec += (nsec0);
   if (tp.tv_nsec >= 1000000000) {
      tp.tv_sec++;
      tp.tv_nsec -= 1000000000;
   }
   pthread_cond_timedwait(&cond, &mutex, &tp);

   while (!getParent()->isShutdown()) {

      // ---
      // User defined tasks
      // ---
      this->userFunc( static_cast<LCreal>(dt) );
      tcnt++;

      // ---
      // Wait for the end of frame
      // ---
      tp.tv_nsec += (nsec0);
      if (tp.tv_nsec >= 1000000000) {
         tp.tv_sec++;
         tp.tv_nsec -= 1000000000;
      }
      pthread_cond_timedwait(&cond, &mutex, &tp);

   }

   pthread_mutex_unlock(&mutex);

   if (getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "Thread(" << this << ")::mainLoopFunc(): ... end of main loop." << std::endl;
   }

   return 0;
}

//==============================================================================
// class ThreadSyncTask
//==============================================================================

//-----------------------------------------------------------------------------
// create the signals
//-----------------------------------------------------------------------------
bool ThreadSyncTask::createSignals()
{
   // create the start mutex already set, signalStart() will release it.
   {
      pthread_mutex_t* mutex = new pthread_mutex_t;
      pthread_mutex_init(mutex,nullptr);
      pthread_mutex_lock(mutex);
      startSig = mutex;
   }

   // create the completed semaphore already set, signalCompleted() will release it.
   {
      pthread_mutex_t* mutex = new pthread_mutex_t;
      pthread_mutex_init(mutex,nullptr);
      pthread_mutex_lock(mutex);
      completedSig = mutex;
   }

   return true;
}

//-----------------------------------------------------------------------------
// Close the signals
//-----------------------------------------------------------------------------
void ThreadSyncTask::closeSignals()
{
   {
      pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(startSig);
      startSig = nullptr;
      pthread_mutex_destroy(mutex);
   }

   {
      pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(completedSig);
      completedSig = nullptr;
      pthread_mutex_destroy(mutex);
   }
}

//-----------------------------------------------------------------------------
// Signal 'start'
//-----------------------------------------------------------------------------
void ThreadSyncTask::signalStart()
{
   pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(startSig);
   pthread_mutex_unlock(mutex);
}

//-----------------------------------------------------------------------------
// Wait for the 'start' signal
//-----------------------------------------------------------------------------
void ThreadSyncTask::waitForStart()
{
   pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(startSig);
   pthread_mutex_lock(mutex);
}

//-----------------------------------------------------------------------------
// Signal 'completed'
//-----------------------------------------------------------------------------
void ThreadSyncTask::signalCompleted()
{
   pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(completedSig);
   pthread_mutex_unlock(mutex);
}

//-----------------------------------------------------------------------------
// Wait for the 'completed' signal
//-----------------------------------------------------------------------------
void ThreadSyncTask::waitForCompleted()
{
   pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(completedSig);
   pthread_mutex_lock(mutex);
}

//-----------------------------------------------------------------------------
// Wait for all of these threads to complete.
//-----------------------------------------------------------------------------
void ThreadSyncTask::waitForAllCompleted(ThreadSyncTask** threads, const unsigned int num)
{
   if (threads != nullptr && num > 0) {
      for (unsigned int i = 0; i < num; i++) {
         if (threads[i] != nullptr) {
            threads[i]->waitForCompleted();
         }
      }
   }
}

//-----------------------------------------------------------------------------
// Wait for any of these threads to complete.
//-----------------------------------------------------------------------------
int ThreadSyncTask::waitForAnyCompleted(ThreadSyncTask** threads, const unsigned int num)
{
   if (threads != nullptr && num > 0) {
      //Make sure we have at least one valid thread (since we'll enter an infinite loop and deadlock otherwise)
      unsigned int i = 0;
      for (; i < num; i++) {
         if (threads[i] != nullptr) {
            break;
         }
      }
      if(i != num) {
         //Loop until one of them releases
         while(true) {
            for (i = 0; i < num; i++) {
               if (threads[i] != nullptr) {
                  pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(threads[i]->completedSig);
                  if (pthread_mutex_trylock(mutex) == 0) {
                     return i;
                  }
               }
            }
         }
      }
   }
   return -1;
}

