//------------------------------------------------------------------------------
// Framework Macros
//
// Object class macros:
//
//    DECLARE_SUBCLASS(ThisType,BaseType)
//       Macro to declare all of the required member functions and member
//       variables for the class 'ThisType', which is derived from class 'BaseType'.
//       Defines the type 'BaseClass', which can be used by the member functions as an
//       alias for the base class.  The copy constructor is declared by this macro, but
//       the user is required to declare any additional constructors including the
//       standard constructor (i.e., no arguments; Foo()).
//
//    IMPLEMENT_SUBCLASS(ThisType, "factoryName")
//       Macro to implement a standard set of required member functions
//       and member variables for the class 'ThisType', and to set its factory name to
//       'factoryName'.  The user is required to implement the copyData(), deleteData()
//       and serialize() functions, the slot table, and any constructors that the user
//       has declared.
//
//    IMPLEMENT_ABSTRACT_SUBCLASS(ThisType, "factoryName") 
//       Macro to implement an abstract class.  Same as IMPLEMENT_SUBCLASS()
//       except that the clone function always returns a zero(0).
//
//    IMPLEMENT_PARTIAL_SUBCLASS(ThisType, "factoryName")
//       Same as IMPLEMENT_SUBCLASS() except that the copy constructor,
//       the destructor, the copy operator and the clone function are
//       left to the user to define (i.e., some time you just need to do
//       this yourself).
//
//    STANDARD_CONSTRUCTOR()
//       Common constructor tasks.  This macro required in every constructor.
//
//    STANDARD_DESTRUCTOR()
//       Common destructor tasks.  This macro required in every destructor.
//
//    BEGIN_SLOTTABLE(ThisType) and END_SLOTTABLE(ThisType)
//       These macros are used to define a slot table (see Object.h)
//       for the class 'ThisType'.
//
//    BEGIN_SLOT_MAP(ThisType) and  END_SLOT_MAP()
//       These macros, along with the ON_SLOT() macro, define a slot
//       dispatch table, which maps local slot numbers and argument
//       types to "set slot" member functions that handle the setting of
//       slot argument values.  These macros are actually implementing
//       the setSlotByIndex() function for class 'ThisType'.
//
//    ON_SLOT(idx, setSlotFunc, ObjType)
//       Maps a local slot index number, 'idx', and slot argument type,
//       'ObjType', to a member function, 'setSlotFunc', that will
//       process the slot.
//
//    EMPTY_SLOTTABLE(ThisType) 
//       Implements an empty slotTable for the class 'ThisType'
//
//    EMPTY_CONSTRUCTOR(ThisType) 
//       Implements an empty default constructor for the class 'ThisType' (e.g., Foo())
//
//    EMPTY_COPYDATA(ThisType)
//       Implements an empty copy data function for the class 'ThisType'
//
//    EMPTY_DELETEDATA(ThisType) 
//       Implements an empty delete data function for the class 'ThisType'
//
//    EMPTY_SERIALIZER(ThisType) 
//       Implements an empty serialize function for the class 'ThisType'
//
//    IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(ThisType, "factoryName")
//       Combines the IMPLEMENT_SUBCLASS() and EMPTY_SLOTTABLE() macros
//        for the class 'ThisType'
//
//
// Component class macros:
//
//    BEGIN_EVENT_HANDLER(ThisType) and END_EVENT_HANDLER() 
//       These macros, along with the ON_EVENT() and ON_ANYKEY() macros
//       below, implement an event dispatch table, which is actually the
//       event() function for class 'ThisType'.
//
//       Typically "on event" functions are used to process the events.  The
//       "on event" function will return a true if the event is processed or
//       false if not.  Events that are not mapped or processed are passed
//       to the base class; up to and including the Component class, and 'Key'
//       events (see eventTokens.h) that are not mapped or processed by the
//       Component class are passed to the container class.
//
//    ON_EVENT(token,onEvent)  (see eventTokens.h)
//       Maps an event token, 'token', to the "on event" member function, 'onEvent'.
//
//    ON_EVENT_OBJ(token,onEvent,ObjType) 
//       Maps an event token, 'token', with an argument of type 'ObjType' to the
//       "on event" member function, 'onEvent'.
//
//    ON_ANYKEY(onEvent)
//       Maps any event token to the "on event" member function, 'onEvent'.
//
//    ON_ANYKEY_OBJ(onEvent,ObjType)
//       Maps any event token with an argument of type 'ObjType' to the "on event"
//       member function, 'onEvent'.
//
//
// StateMachine class macros:
//
//    BEGIN_STATE_TABLE(ThisType) and END_STATE_TABLE()
//       These macros, along with the macros  below, implement a state dispatch
//       table, which is actually the stateTable() function for class 'ThisType'.
//
//    STATE_FUNC(state,stateFunc)
//       Maps a state number, 'state', to the member function, 'stateFunc', which
//       implements the state.
//
//    STATE_MACH(state,"name") 
//       Maps a state number, 'state', to the name of child state machine that
//       implements the state.  The child state machines are defined for this
//       state machine using the 'stateMachines' slot.
//
//    ANY_STATE_FUNC(stateFunc) 
//       Maps any state number to the member function, 'stateFunc'.
//
//------------------------------------------------------------------------------

// Caution: often many users new to the framework want to learn the internals of how these
// macros work.  Although a glorious endeavor, experience as proven that it's best
// to treat these macros, at least initially, as 'black boxes'.

#define DECLARE_SUBCLASS(ThisType,BaseType)                                                           \
    typedef BaseType BaseClass;                                                                       \
    public: ThisType(const ThisType& org);                                                            \
    public: virtual ~ThisType();                                                                      \
    public: virtual ThisType* clone() const;                                                          \
    public: ThisType& operator=(const ThisType& org);                                                 \
    protected: void copyData(const ThisType& org, const bool cc = false);                             \
    protected: void deleteData();                                                                     \
    public: virtual bool isClassType(const std::type_info& type) const;                               \
    private: static struct _Static _static;                                                           \
    private: static const unsigned int classIndex;                                                    \
    protected: static const _Static* getStatic();                                                     \
    public: static const char* getFactoryName();                                                      \
    public: static const char* getFormName();                                                         \
    public: virtual bool isFactoryName(const char name[]) const;                                      \
    public: virtual bool isFormName(const char name[]) const;                                         \
    protected: virtual bool setSlotByIndex(const int slotindex, Eaagles::Basic::Object* const obj);   \
    protected: virtual Eaagles::Basic::Object* getSlotByIndex(const int slotindex);                   \
    public: static const Eaagles::Basic::SlotTable& getSlotTable();                                   \
    protected: static const Eaagles::Basic::SlotTable slottable;                                      \
    private: static const char* slotnames[];                                                          \
    private: static const int nslots;                                                                 \
    public: virtual std::ostream&                                                                     \
    serialize(std::ostream& sout, const int i = 0, const bool slotsOnly = false) const;               \
    private:



#define IMPLEMENT_SUBCLASS(ThisType, FACTORYNAME)                                      \
    ThisType::_Static ThisType::_static(                                               \
      registerClass(&_static), typeid(ThisType).name(), FACTORYNAME,                   \
        &ThisType::slottable, BaseClass::getStatic()                                   \
    );                                                                                 \
    const ThisType::_Static* ThisType::getStatic() { return &_static; }                \
    const char* ThisType::getFormName() { return _static.fname; }                      \
    const char* ThisType::getFactoryName() { return _static.fname; }                   \
    bool ThisType::isFormName(const char name[]) const                                 \
    {                                                                                  \
        if (name == 0) return false;                                                   \
        if ( std::strcmp(_static.fname,name) == 0 )  return true;                      \
        else return ThisType::BaseClass::isFormName(name);                             \
    }                                                                                  \
    bool ThisType::isFactoryName(const char name[]) const                              \
    {                                                                                  \
        if (name == 0) return false;                                                   \
        if ( std::strcmp(_static.fname,name) == 0 )  return true;                      \
        else return ThisType::BaseClass::isFactoryName(name);                          \
    }                                                                                  \
    const Eaagles::Basic::SlotTable& ThisType::getSlotTable()  { return slottable; }   \
    bool ThisType::isClassType(const std::type_info& type) const                       \
    {                                                                                  \
        if ( type == typeid(ThisType) ) return true;                                   \
        else return ThisType::BaseClass::isClassType(type);                            \
    }                                                                                  \
    ThisType::~ThisType() {                                                            \
      STANDARD_DESTRUCTOR()                                                            \
    }                                                                                  \
    ThisType* ThisType::clone() const                                                  \
    {                                                                                  \
        return new ThisType(*this);                                                    \
    }                                                                                  \
    ThisType::ThisType(const ThisType& org) : BaseClass()                              \
    {                                                                                  \
        STANDARD_CONSTRUCTOR()                                                         \
        copyData(org,true);                                                            \
    }                                                                                  \
    ThisType& ThisType::operator=(const ThisType& org)                                 \
    {                                                                                  \
        if (this != &org) copyData(org,false);                                         \
        return *this;                                                                  \
    }



#define IMPLEMENT_PARTIAL_SUBCLASS(ThisType, FACTORYNAME)                              \
    ThisType::_Static ThisType::_static(                                               \
      registerClass(&_static), typeid(ThisType).name(), FACTORYNAME,                   \
        &ThisType::slottable, BaseClass::getStatic()                                   \
    );                                                                                 \
    const ThisType::_Static* ThisType::getStatic() { return &_static; }                \
    const char* ThisType::getFormName() { return _static.fname; }                      \
    const char* ThisType::getFactoryName() { return _static.fname; }                   \
    bool ThisType::isFormName(const char name[]) const                                 \
    {                                                                                  \
        if (name == 0) return false;                                                   \
        if ( std::strcmp(_static.fname,name) == 0 )  return true;                      \
        else return ThisType::BaseClass::isFormName(name);                             \
    }                                                                                  \
    bool ThisType::isFactoryName(const char name[]) const                              \
    {                                                                                  \
        if (name == 0) return false;                                                   \
        if ( std::strcmp(_static.fname,name) == 0 )  return true;                      \
        else return ThisType::BaseClass::isFactoryName(name);                          \
    }                                                                                  \
    const Eaagles::Basic::SlotTable& ThisType::getSlotTable() { return slottable; }    \
    bool ThisType::isClassType(const std::type_info& type) const                       \
    {                                                                                  \
        if ( type == typeid(ThisType) ) return true;                                   \
        else return ThisType::BaseClass::isClassType(type);                            \
    }



#define IMPLEMENT_ABSTRACT_SUBCLASS(ThisType, FACTORYNAME)                             \
    ThisType::_Static ThisType::_static(                                               \
      registerClass(&_static), typeid(ThisType).name(), FACTORYNAME,                   \
        &ThisType::slottable, BaseClass::getStatic()                                   \
    );                                                                                 \
    const ThisType::_Static* ThisType::getStatic() { return &_static; }                \
    const char* ThisType::getFormName() { return _static.fname; }                      \
    const char* ThisType::getFactoryName() { return _static.fname; }                   \
    bool ThisType::isFormName(const char name[]) const                                 \
    {                                                                                  \
        if (name == 0) return false;                                                   \
        if ( std::strcmp(_static.fname,name) == 0 )  return true;                      \
        else return ThisType::BaseClass::isFormName(name);                             \
    }                                                                                  \
    bool ThisType::isFactoryName(const char name[]) const                              \
    {                                                                                  \
        if (name == 0) return false;                                                   \
        if ( std::strcmp(_static.fname,name) == 0 )  return true;                      \
        else return ThisType::BaseClass::isFactoryName(name);                          \
    }                                                                                  \
    const Eaagles::Basic::SlotTable& ThisType::getSlotTable() { return slottable; }    \
    bool ThisType::isClassType(const std::type_info& type) const                       \
    {                                                                                  \
        if ( type == typeid(ThisType) ) return true;                                   \
        else return ThisType::BaseClass::isClassType(type);                            \
    }                                                                                  \
    ThisType::~ThisType() {                                                            \
        STANDARD_DESTRUCTOR()                                                          \
    }                                                                                  \
    ThisType* ThisType::clone() const                                                  \
    {                                                                                  \
        return 0;                                                                      \
    }                                                                                  \
    ThisType::ThisType(const ThisType& org) : BaseClass()                              \
    {                                                                                  \
        STANDARD_CONSTRUCTOR()                                                         \
        copyData(org,true);                                                            \
    }                                                                                  \
    ThisType& ThisType::operator=(const ThisType& org)                                 \
    {                                                                                  \
        if (this != &org) copyData(org,false);                                         \
        return *this;                                                                  \
    }



#define STANDARD_CONSTRUCTOR()                                                         \
    slotTable = &slottable;                                                            \
    if (++_static.count > _static.mc) _static.mc = _static.count;                      \
    _static.tc++;



#define STANDARD_DESTRUCTOR()                                                          \
    deleteData();                                                                      \
    _static.count--;



#define EMPTY_SLOTTABLE(ThisType)                                                          \
    const char* ThisType::slotnames[] = { "" };                                            \
    const int ThisType::nslots = 0;                                                        \
    const Eaagles::Basic::SlotTable ThisType::slottable(0, 0, BaseClass::getSlotTable());  \
    bool ThisType::setSlotByIndex(const int si, Eaagles::Basic::Object* const obj)         \
    {                                                                                      \
        return BaseClass::setSlotByIndex(si,obj);                                          \
    }                                                                                      \
    Eaagles::Basic::Object* ThisType::getSlotByIndex(const int si)                         \
    {                                                                                      \
        return BaseClass::getSlotByIndex(si);                                              \
    }



#define IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(ThisType,FACTORYNAME)                       \
    IMPLEMENT_SUBCLASS(ThisType,FACTORYNAME)                                           \
    EMPTY_SLOTTABLE(ThisType)



#define EMPTY_CONSTRUCTOR(ThisType)                                                    \
    ThisType::ThisType()                                                               \
    {                                                                                  \
        STANDARD_CONSTRUCTOR()                                                         \
    }



#define EMPTY_COPYDATA(ThisType)                                                       \
    void ThisType::copyData(const ThisType& org, const bool)                           \
    {                                                                                  \
        BaseClass::copyData(org,false);                                                \
    }



#define EMPTY_DELETEDATA(ThisType)                                                     \
    void ThisType::deleteData()                                                        \
    {                                                                                  \
    }



#define EMPTY_SERIALIZER(ThisType)                                                     \
    std::ostream& ThisType::serialize(                                                 \
    std::ostream& sout, const int i, const bool slotsOnly) const                       \
    {                                                                                  \
        int j = 0;                                                                     \
        if ( !slotsOnly ) {                                                            \
            indent(sout,i);                                                            \
            sout << "( " << getFactoryName() << std::endl;                             \
            j = 4;                                                                     \
        }                                                                              \
        BaseClass::serialize(sout,i+j,true);                                           \
        if ( !slotsOnly ) {                                                            \
            indent(sout,i);                                                            \
            sout << ")" << std::endl;                                                  \
        }                                                                              \
        return sout;                                                                   \
    }



#define BEGIN_SLOTTABLE(ThisType)                                                      \
    const char* ThisType::slotnames[] = {



#define END_SLOTTABLE(ThisType)                                                                \
    };                                                                                         \
    const int ThisType::nslots = (sizeof(slotnames)/sizeof(char*));                            \
    const Eaagles::Basic::SlotTable ThisType::slottable(ThisType::slotnames, ThisType::nslots, \
                                               ThisType::BaseClass::getSlotTable());



#define BEGIN_SLOT_MAP(ThisType)                                                           \
    bool ThisType::setSlotByIndex(const int slotindex, Eaagles::Basic::Object* const obj)  \
    {                                                                                      \
        const int _n = BaseClass::getSlotTable().n();                                      \
        if (slotindex <= _n) {                                                             \
            return BaseClass::setSlotByIndex(slotindex,obj);                               \
        }                                                                                  \
        bool _ok = false;                                                                  \
        int _n1 = (slotindex - _n);



#define END_SLOT_MAP()                                                                 \
        return _ok;                                                                    \
    }



#define ON_SLOT(idx,setFunc,ObjType)                                                   \
    if ( !_ok ) {                                                                      \
        ObjType* _msg = dynamic_cast<ObjType*>(obj);                                   \
        if (idx == _n1 && _msg != 0) {                                                 \
            _ok = setFunc(_msg);                                                       \
        }                                                                              \
    }



#define BEGIN_EVENT_HANDLER(ThisType)                                                  \
    bool ThisType::event(const int _event, Eaagles::Basic::Object* const _obj)         \
    {                                                                                  \
        bool _used = false;



#define END_EVENT_HANDLER()                                                            \
        if (!_used) _used = BaseClass::event(_event,_obj);                             \
        return _used;                                                                  \
    }



#define ON_EVENT_OBJ(token,onEvent,ObjType)                                            \
    if (!_used && token == _event && dynamic_cast<ObjType*>(_obj) != 0) {              \
        _used = onEvent(static_cast<ObjType*>(_obj));                                  \
    }



#define ON_EVENT(token,onEvent)                                                        \
    if (!_used && token == _event) {                                                   \
        _used = onEvent();                                                             \
    }



#define ON_ANYKEY_OBJ(onEvent,ObjType)                                                 \
    if (!_used && _event <= MAX_KEY_EVENT && dynamic_cast<ObjType*>(_obj) != 0) {      \
        _used = onEvent(_event,(static_cast<ObjType*>(_obj)));                         \
    }



#define ON_ANYKEY(onEvent)                                                             \
    if (!_used && _event <= MAX_KEY_EVENT) {                                           \
        _used = onEvent(_event);                                                       \
    }



#define BEGIN_STATE_TABLE(ThisType)                                        \
   unsigned short ThisType::stateTable(                                    \
         const unsigned short _cstate,                                     \
         const StateTableCode _code,                                       \
         const LCreal _dt)                                                 \
    {                                                                      \
        unsigned short _next = INVALID_STATE;                              \
        bool _ok = false;



#define END_STATE_TABLE()                                                  \
        return _next;                                                      \
    }



#define STATE_FUNC(state,stateFunc)                                        \
   if (state == _cstate) {                                                 \
      if (_code == CURR_STATE) {                                           \
         setStMach(0, _code);                                              \
         stateFunc(_dt);                                                   \
      }                                                                    \
      else if (_code == TEST_STATE) _next = state;                         \
      _ok = true;                                                          \
   }                                                                       \
   else if (_code == FIND_NEXT_STATE && _ok && _next == INVALID_STATE) {   \
      _next = state;                                                       \
   }



#define STATE_MACH(state,name)                                             \
   if (state == _cstate) {                                                 \
      _ok = setStMach(name, _code);                                        \
      if (_ok && _code == TEST_STATE) _next = state;                       \
   }                                                                       \
   else if (_code == FIND_NEXT_STATE && _ok && _next == INVALID_STATE) {   \
      _next = state;                                                       \
   }



#define ANY_STATE_FUNC(stateFunc)                                          \
   if (_code == CURR_STATE && !_ok) {                                      \
      setStMach(0, _code);                                                 \
      stateFunc(_dt);                                                      \
      _ok = true;                                                          \
   }                                                                       \
   else if (_code == TEST_STATE && !_ok) {                                 \
      _next =_cstate;                                                      \
      _ok = true;                                                          \
   }                                                                       \
   else if (_code == FIND_NEXT_STATE && _next == INVALID_STATE) {          \
      _next =_cstate+1;        /* next is just one more! */                \
   }

