// cfi helper definitions

#ifndef CFI_HPP_INCLUDED
#define CFI_HPP_INCLUDED

#include "include/assembly.hpp"

// define a type of function
typedef void (*pfunc_t)();

// define the type
typedef pfunc_t *pvtable_t;

// create a fake virtual table on heap with `nfunc' number of `pfunc_t'
extern pvtable_t create_fake_vtable_on_heap(unsigned int nfunc);
extern void free_fake_vtable_on_heap(pvtable_t addr);

class Base
{
public:
  virtual void virtual_func();
  virtual ~Base() {}
};

class Helper : public Base
{
public:
  virtual void virtual_func();
  virtual ~Helper() {}
};

class Helper2 : public Base
{
  double lvar;
public:
  virtual void virtual_func(int arg);
  double get_lvar() { return lvar; }
  virtual ~Helper2() {}
};

class Helper3 : public Base
{
  double lvar;
public:
  virtual void virtual_func(double arg);
  double get_lvar() { return lvar; }
  virtual ~Helper3() {}
};

class BaseM
{
public:
  virtual int virtual_funcM0();
  virtual int virtual_funcM1();
  virtual ~BaseM() {}
};

class Helper1M : public BaseM
{
public:
  virtual int virtual_funcM0();
  virtual int virtual_funcM1();
  virtual ~Helper1M() {}
};

class Helper2M : public BaseM
{
public:
  virtual int virtual_funcM0();
  virtual int virtual_funcM1();
  virtual ~Helper2M() {}
};

// read the vtable pointer of an object
extern pvtable_t read_vtable_pointer(const Base *);
extern pvtable_t read_vtable_pointer(const BaseM *);

// write the vtable pointer of an object
extern void write_vtable_pointer(Base *, pvtable_t);
extern void write_vtable_pointer(BaseM *, pvtable_t);

#endif
