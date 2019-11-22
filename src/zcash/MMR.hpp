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

typedef std::array<unsigned char, NODE_SERIALIZED_LENGTH> SerializedMMRTreeNode;
typedef std::array<unsigned char, ENTRY_SERIALIZED_LENGTH> SerializedMMRTreeEntry;

typedef long MMRIndex;

class MMRUpdateState {
public:
    std::unordered_map<MMRIndex, SerializedMMRTreeNode> appends;
    MMRIndex length;
    MMRIndex updateDepth;
    uint256 root;

    MMRUpdateState(MMRIndex initialLength) : length(initialLength), updateDepth(initialLength) { };
    void Extend(const SerializedMMRTreeNode &leaf);
    SerializedMMRTreeNode Get(MMRIndex idx);
    void Truncate(MMRIndex newLength);
    void Reset();
};

SerializedMMRTreeNode NewNode(
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

SerializedMMRTreeNode NewLeaf(
    uint256 commitment,
    uint32_t time,
    uint32_t target,
    uint256 saplingRoot,
    uint256 totalWork,
    uint64_t height,
    uint64_t shieldedTxCount
);

SerializedMMRTreeEntry NewEntry(const SerializedMMRTreeNode node, uint32_t left, uint32_t right);
SerializedMMRTreeEntry NodeToEntry(const SerializedMMRTreeNode node);

}

typedef libzcash::MMRUpdateState MMRUpdateState;
typedef libzcash::MMRIndex MMRIndex;
typedef libzcash::SerializedMMRTreeNode SerializedMMRNode;
typedef libzcash::SerializedMMRTreeEntry SerializedMMREntry;

#endif /* ZC_MMR_H_ */