#pragma once

namespace RedSnowEngine::Core
{
    class BlockAllocator
    {
    public:
        BlockAllocator(const char* name, std::size_t blockSize, std::size_t capacity);
        virtual ~BlockAllocator();

        BlockAllocator(const BlockAllocator&) = delete;
        BlockAllocator(const BlockAllocator&&) = delete;
        BlockAllocator& operator=(const BlockAllocator&) = delete;
        BlockAllocator& operator=(const BlockAllocator&&) = delete;

        void* Allocate();
        void Free(void* ptr);

    private:
        std::string mName;
        std::vector<void*> mFreeBlocks;

        void* mData = nullptr;
        std::size_t mBlockSize = 0;
        std::size_t mCapacity = 0;
        std::size_t mBlockAllocatedCurrent = 0;
        std::size_t mBlockAllocatedTotal = 0;
        std::size_t mBlockFreed = 0;
        std::size_t mBlockHighest = 0;
    };
}