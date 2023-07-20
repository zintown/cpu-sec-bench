#include <Windows.h>
#include <process.h>
#include <iostream>
#include <intrin.h>

struct thread_para{
  HANDLE target_thread;
  CONTEXT target_context;
};

__declspec(noinline)
void* func(){
    void* p = _AddressOfReturnAddress();
    return p;
}

void get_main_thread_context(void* s){

  if(SuspendThread(((thread_para*)s)->target_thread) == -1) {
    std::cerr << "Error suspending thread.. " << GetLastError() << std::endl;
  }
  if(GetThreadContext(((thread_para*)s)->target_thread,
                    &(((thread_para*)s)->target_context)) == FALSE) {
    std::cerr << "Error geting main thread context.. " << GetLastError() << std::endl;
  }
  if(ResumeThread(((thread_para*)s)->target_thread) == -1){
    std::cerr << "Error resumeing main thread context.. " << GetLastError() << std::endl;
  }
  if(ResumeThread(((thread_para*)s)->target_thread) == -1){
    std::cerr << "Error resumeing main thread context.. " << GetLastError() << std::endl;
  }
}

int main(){
  HANDLE pseudoprocessh = GetCurrentProcess();
  HANDLE pseudothreadh  = GetCurrentThread();

  thread_para main_stru;
  std::cerr << "main thread is: " << pseudothreadh << std::endl;
  std::cerr << "target_thread is: " << main_stru.target_thread << std::endl;
  std::cerr << "target_context is: " << main_stru.target_context.ContextFlags << std::endl;

  DuplicateHandle(pseudoprocessh,
                  pseudothreadh,
                  pseudoprocessh,
                  &(main_stru.target_thread),
                  0,
                  FALSE,
                  DUPLICATE_SAME_ACCESS);

  main_stru.target_context.ContextFlags = CONTEXT_FULL;

  std::cerr << "target_thread is: " << main_stru.target_thread << std::endl;
  std::cerr << "target_context is: " << main_stru.target_context.ContextFlags << std::endl;

  //HANDLE new_child = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)get_main_thread_context,NULL,0,NULL);
  HANDLE new_child = (HANDLE)_beginthread((_beginthread_proc_type)&get_main_thread_context, 0, (void*)&main_stru);
  if((long)new_child == -1L){
      std::cerr << "Error creating thread.. " << GetLastError() << std::endl;
  }

  if(SuspendThread(main_stru.target_thread) == -1) {
    std::cerr << "Error suspending thread.. " << GetLastError() << std::endl;
  }
  getchar();
  std::cerr << "curr rsp is: " << std::hex << main_stru.target_context.Rsp << std::endl;
    void* fp = func();
    printf("return addr's addr is: %llx", fp);
  CONTEXT new_context;
  RtlCaptureContext(&new_context);
  std::cerr << "new context's rsp is: " << std::hex << new_context.Rsp << std::endl;
}