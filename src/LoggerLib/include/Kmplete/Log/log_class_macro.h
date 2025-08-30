#if defined (KMP_CONFIG_TYPE_PRODUCTION)
#define KMP_LOG_CLASSNAME(ClassName)

#else
#define KMP_LOG_CLASSNAME(ClassName) \
private: \
static const char* GetLogClassName() { return #ClassName ; }

#endif