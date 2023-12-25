typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
typedef signed char		s8;
typedef short			s16;
typedef int			s32;
typedef long long		s64;

/* required for opal-api.h */
typedef u8  uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;
typedef s8  int8_t;
typedef s16 int16_t;
typedef s32 int32_t;
typedef s64 int64_t;

enum module_state {
	MODULE_STATE_LIVE,	/* Normal state. */
	MODULE_STATE_COMING,	/* Full formed, running module_init. */
	MODULE_STATE_GOING,	/* Going away. */
	MODULE_STATE_UNFORMED,	/* Still setting it up. */
};

struct list_head {
	struct list_head *next, *prev;
};

struct module {
	enum module_state state;

	/* Member of list of modules */
	struct list_head list;

	/* Unique handle for this module */
	char name[(64 - sizeof(unsigned long))];

	/* Sysfs stuff. */
	void* mkobj;
	void*modinfo_attrs;
	const char *version;
	const char *srcversion;
	void* *holders_dir;

	/* Exported symbols */
	void* *syms;
	const s32 *crcs;
	unsigned int num_syms;

	void *kp;
	unsigned int num_kp;

	/* GPL-only exported symbols. */
	unsigned int num_gpl_syms;
	const void *gpl_syms;
	const s32 *gpl_crcs;



	bool async_probe_requested;

	/* symbols that will be GPL-only in the near future. */
	const void *gpl_future_syms;
	const s32 *gpl_future_crcs;
	unsigned int num_gpl_future_syms;

	/* Exception table */
	unsigned int num_exentries;
	void *extable;

	/* Startup function. */
	int (*init)(void);


	/* The command line arguments (may be mangled).  People like
	   keeping pointers to this stuff */
	char *args;
}; 
