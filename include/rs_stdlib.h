#ifndef RS_STDLIB_H_
#define RS_STDLIB_H_

// type aliases for rust source
typedef size_t usize;
typedef void unimplemented_t;

// structure definitions
struct RawVec {
  void* ptr;
  size_t cap;
};
struct Vec {
  RawVec buf;
  size_t len;  
};
struct String {
  Vec vec;
};

// pub struct Stdin {
//    inner: &'static Mutex<BufReader<StdinRaw>>,
// }
struct Stdin {
  unimplemented_t* inner;
};
#endif // RS_STDLIB_H_
