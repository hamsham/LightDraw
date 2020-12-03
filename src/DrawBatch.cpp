
#include <utility> // std::move

#include "lightsky/utils/Copy.h"

#include "lightsky/draw/DrawBatch.h"



namespace ls
{
namespace draw
{



/*-------------------------------------
 * Destructor
-------------------------------------*/
DrawBatch::~DrawBatch() noexcept
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
DrawBatch::DrawBatch() noexcept :
    materialId{0},
    blendParamId{0},
    depthParamId{0},
    uniformIds{0},
    batches{0}
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
DrawBatch::DrawBatch(const DrawBatch& b) noexcept :
    materialId{b.materialId},
    blendParamId{b.blendParamId},
    depthParamId{b.depthParamId},
    uniformIds{},
    batches{}
{
    uniformIds.resize(b.uniformIds.size());
    utils::fast_memcpy(uniformIds.data(), b.uniformIds.data(), sizeof(unsigned) * b.uniformIds.size());

    batches.resize(b.batches.size());
    utils::fast_memcpy(batches.data(), b.batches.data(), sizeof(DrawCommandParams) * b.batches.size());
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
DrawBatch::DrawBatch(DrawBatch&& b) noexcept :
    materialId{std::move(b.materialId)},
    blendParamId{std::move(b.blendParamId)},
    depthParamId{std::move(b.depthParamId)},
    uniformIds{std::move(b.uniformIds)},
    batches{std::move(b.batches)}
{
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
DrawBatch& DrawBatch::operator=(const DrawBatch& b) noexcept
{
    materialId = b.materialId;
    blendParamId = b.blendParamId;
    depthParamId = b.depthParamId;

    uniformIds.resize(b.uniformIds.size());
    utils::fast_memcpy(uniformIds.data(), b.uniformIds.data(), sizeof(unsigned) * b.uniformIds.size());

    batches.resize(b.batches.size());
    utils::fast_memcpy(batches.data(), b.batches.data(), sizeof(DrawCommandParams) * b.batches.size());

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
DrawBatch& DrawBatch::operator=(DrawBatch&& b) noexcept
{
    materialId = std::move(b.materialId);
    blendParamId = std::move(b.blendParamId);
    depthParamId = std::move(b.depthParamId);
    uniformIds = std::move(b.uniformIds);
    batches = std::move(b.batches);

    return *this;
}

/*-------------------------------------
 * Reset all internal parameters
-------------------------------------*/
void DrawBatch::reset() noexcept
{
    materialId = 0;
    blendParamId = 0;
    depthParamId = 0;

    // rather than have the STL call 'memset()' on all variables in the uniform
    // vector, just resize to 0.
    uniformIds.resize(0);
    batches.resize(0);
}
} // end draw namespace
} // end ls namespace
