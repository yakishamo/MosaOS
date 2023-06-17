#include <cstdint>
#include "paging.hpp"
#include "asmfunc.hpp"

namespace {
	const uint64_t kPageSize4K = 4096;
	const uint64_t kPageSize2M = 512 * kPageSize4K;
	const uint64_t kPageSize1G = 512 * kPageSize2M;

	alignas(kPageSize4K) uint64_t pml4_table[512];
	alignas(kPageSize4K) uint64_t pdp_table[512];
	alignas(kPageSize4K) uint64_t page_directory[kPageDirectoryCount][512];
}

void SetupIdentityPageTable() {
	pml4_table[0] = reinterpret_cast<uint64_t>(&pdp_table[0]) | 0x003;
	for(int i_pdpt = 0; i_pdpt < sizeof(page_directory)/(sizeof(uint64_t)*512); i_pdpt++) {
		pdp_table[i_pdpt] = reinterpret_cast<uint64_t>(&page_directory[i_pdpt]) | 0x003;
		for(int i_pd = 0; i_pd < 512; i_pd++) {
			page_directory[i_pdpt][i_pd] = i_pdpt * kPageSize1G + i_pd * kPageSize2M | 0x83;
		}
	}

	SetCR3(reinterpret_cast<uint64_t>(&pml4_table[0]));
}
