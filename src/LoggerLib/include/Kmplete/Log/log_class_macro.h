//! Utility syntactic sugar macro that define a class prefix used by
//! logging inside class functions
//! e.g: 
//! MyClass::MyFunc() { KMP_LOG_DEBUG("some message"); }
//! will put a debug message like "... MyClass: some message";
//! @see log.h

#if defined (KMP_CONFIG_TYPE_PRODUCTION)
#define KMP_LOG_CLASSNAME(ClassName)

#else
#define KMP_LOG_CLASSNAME(ClassName) \
private: \
static const char* GetLogClassName() { return #ClassName ; }

#endif