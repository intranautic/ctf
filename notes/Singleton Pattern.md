The singleton design pattern gets its name due to the fact that there is only one instance (typically declared globally or within some linkage scope / translation unit) of the class within the entire program.

This allows for a single centralized api throughout the entire program most primarily from my experience for event or message logging within the program.

Due to the fact that singletons are global resources, within a multithreaded process there is the possibility of [[TOCTOU]] vulnerabilities so there needs to be proper synchronization.

The only real way you should be initializing a singleton is by declaring it statically. Due to the fact that there is only a single object at runtime there shouldnt be the need for dynamic allocation and should be treated as a global.

When declared statically you circumvent the risk of race conditions at initialization if you were to check and allocate at runtime. Example below.

`https://en.wikipedia.org/wiki/Singleton_pattern`
```cpp
class Singleton {
  Singleton* instance;
public:
  static Singleton& get_instance() {
    if (instance == nullptr)
      instance = new Singleton;
    return instance;
  }
};
```

This is fucking terrible, this is disgusting and terrible practice. Not to mention they wrote the condition as `(nullptr == instance)`?? who the fuck does that?? whoever wrote this is a psychopath apparently it was referenced from a book? they dont specifiy which book.. ive got to find them and berate them..

Due to the fact that there should only be a singular instance of a singleton, the construction methods should not be expose, so should be declared within private namespace.

In most cases you wouldnt want to expose the get_instance api either as you dont want direct access to the object, rather declare get_instance private and define public static methods which perform operations on the singleton.

Example shown below.

```cpp
class Logger {
  enum LEVEL: unsigned char {
    LEVEL_INFO,
    LEVEL_WARN,
    LEVEL_ERROR,
    LEVEL_FATAL
  };
public:
  static int log(LEVEL l, const char* message) {
    // do logging here
  }
private:
  // static initialization of instance
  static Logger& get_instance() {
    static Singleton instance;
    return instance;
  }
  Logger()=default;
  ~Logger() =default;
};
```

Very simple and generic example but oftentimes you will want a generic logger which allows you to log multiple objects and exceptions (overload std::ostream& operator<<) and youd want multiple output streams (not just stdout).

That has to be done dynamically at runtime in most cases, for example if you wanted to way to register stl output streams which the logger would write to you need an initialization method that should be invoked at runtime.

Keep in mind that the initializtion and destroy methods should also be static, due to the fact that get_instance is exposed to them regardless of their static linkage.

Man i feel like a fucking loser talking about oop shit man this sucks i want to go back to fp..