
#include "lightsky/draw/RenderPass.h"



namespace ls
{
namespace draw
{



/*-------------------------------------
 * Destructor
-------------------------------------*/
RenderPass::~RenderPass() noexcept
{
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
RenderPass::RenderPass() noexcept
{
}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
RenderPass::RenderPass(const RenderPass&) noexcept
{
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
RenderPass::RenderPass(RenderPass&&) noexcept
{
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
RenderPass& RenderPass::operator=(const RenderPass&) noexcept
{
    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
RenderPass& RenderPass::operator=(RenderPass&&) noexcept
{
    return *this;
}
} // end draw namespace
} // end ls namespace
