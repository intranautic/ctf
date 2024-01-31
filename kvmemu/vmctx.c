#include "vmctx.h"

struct vmcpuctx {
  int cpufd;
  int index;
  struct kvm_run* context;
  struct vmctx* machine;
  struct vmcpuctx* next;
};

struct vmctx {
  int kvmfd;
  int vmfd;
  struct vcpuctx* cores;
};
