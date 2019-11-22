#include <stdexcept>

#include <boost/foreach.hpp>

#include "serialize.h"
#include "streams.h"
#include "uint256.h"
#include "librustzcash.h"

#include "MMR.hpp"

namespace libzcash {
    
void MMRUpdateState::Extend(const SerializedMMRTreeNode &leaf) {
    appends[length++] = leaf;
}

SerializedMMRTreeNode MMRUpdateState::Get(MMRIndex idx) {
    return appends[idx];
}

void MMRUpdateState::Truncate(MMRIndex newLength) {
    for (MMRIndex idx = length-1; idx >= newLength; idx--) {
        appends.erase(idx);
    }

    length = newLength;
    if (updateDepth < length) updateDepth = length;
}

void MMRUpdateState::Reset() {
    updateDepth = length;
    appends.clear();
}

SerializedMMRTreeNode NewLeaf(
    uint256 commitment,
    uint32_t time,
    uint32_t target,
    uint256 saplingRoot,
    uint256 totalWork,
    uint64_t height,
    uint64_t shieldedTxCount
) {
    return NewNode(
        commitment,
        time,
        time,
        target,
        target,
        saplingRoot,
        saplingRoot,
        totalWork,
        height,
        height,
        shieldedTxCount
    );
}

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
    )
{
    CDataStream buf(SER_DISK, 0);
    SerializedMMRTreeNode result;

    buf << subtreeCommitment;
    buf << startTime;
    buf << endTime;
    buf << startTarget;
    buf << endTarget;
    buf << startSaplingRoot;
    buf << endSaplingRoot;
    buf << subtreeTotalWork;
    buf << VARINT(startHeight);
    buf << VARINT(endHeight);
    buf << VARINT(shieldedTxCount);

    std::copy(buf.begin(), buf.end(), result.begin());
    return result;
}

SerializedMMRTreeEntry NewEntry(const SerializedMMRTreeNode node, uint32_t left, uint32_t right) {
    CDataStream buf(SER_DISK, 0);
    SerializedMMRTreeEntry result;

    uint8_t code = 0;
    buf << code;
    buf << left;
    buf << right;
    buf << node;

    std::copy(std::begin(buf), std::end(buf), std::begin(result));

    return result;
}

SerializedMMRTreeEntry NodeToEntry(const SerializedMMRTreeNode node) {
    CDataStream buf(SER_DISK, 0);
    SerializedMMRTreeEntry result;

    uint8_t code = 1;
    buf << code;
    buf << node;

    std::copy(std::begin(buf), std::end(buf), std::begin(result));
    
    return result;
}

}