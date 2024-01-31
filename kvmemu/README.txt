kvmemu
==========================================================================
setup machine environment manually via kvm ioctl. supports x86 and handles
all setup structure and paging operations

potentially support more architectures in the future, decouple processor
specific operations into separate units, vmctx should be invariant across
platform.
