#pragma once

#include <cstdint>
#include <cuda_runtime.h>
#include "config.h"

namespace gpu {
	struct CrackResult launch_crack_kernel(
		uint64_t total,
		int length,
		const char* charset,
		int base,
		const unsigned char* target_digest);
}