#ifndef ZC_MMR_H_
#define ZC_MMR_H_

#include <stdexcept>
#include <unordered_map>
#include <boost/foreach.hpp>

#include "serialize.h"
#include "streams.h"
#include "uint256.h"

#include "librustzcash.h"

namespace libzcash {

const int NODE_SERIALIZED_LENGTH = 171;
const int ENTRY_SERIALIZED_LENGTH = 180;

typedef std::array<unsigned char, NODE_SERIALIZED_LENGTH> HistoryNode;
typedef std::array<unsigned char, ENTRY_SERIALIZED_LENGTH> HistoryEntry;

typedef long HistoryIndex;

class HistoryCache {
public:
    std::unordered_map<HistoryIndex, HistoryNode> appends;
    HistoryIndex length;
    HistoryIndex updateDepth;
    uint256 root;
    uint32_t epoch;

    HistoryCache(HistoryIndex initialLength, uint256 initialRoot, uint32_t initialEpoch) : 
        length(initialLength), updateDepth(initialLength), root(initialRoot), epoch(initialEpoch) { };

    HistoryCache() { }

    void Extend(const HistoryNode &leaf);
    void Truncate(HistoryIndex newLength);
    void Reset();
};

HistoryNode NewNode(
    uint256 subtreeCommitment,
    uint32_t startTime,
    uint32_t endTime,
    uint32_t startTarget,
    uint32_t endTarget,
    uint256 startSaplingRoot,
    uint256 endSaplingRoot,
    uint256 subtreeTotalWork,
    uint64_t startHeight,
    uint64_t endHeight,
    uint64_t shieldedTxCount  
);

HistoryNode NewLeaf(
    uint256 commitment,
    uint32_t time,
    uint32_t target,
    uint256 saplingRoot,
    uint256 totalWork,
    uint64_t height,
    uint64_t shieldedTxCount
);

HistoryEntry NewEntry(const HistoryNode node, uint32_t left, uint32_t right);
HistoryEntry NodeToEntry(const HistoryNode node);

}

typedef libzcash::HistoryCache HistoryCache;
typedef libzcash::HistoryIndex HistoryIndex;
typedef libzcash::HistoryNode HistoryNode;
typedef libzcash::HistoryEntry HistoryEntry;

#endif /* ZC_MMR_H_ */