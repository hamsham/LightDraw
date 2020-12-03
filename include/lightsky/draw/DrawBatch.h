
#ifndef __LS_DRAW_DRAW_BATCH_H__
#define __LS_DRAW_DRAW_BATCH_H__

#include <vector>

#include "lightsky/draw/DrawParams.h"



namespace ls
{
namespace draw
{



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
struct DrawCommandParams;



/**----------------------------------------------------------------------------
 * The DrawBatch class allows for a set of related pieces of geometry to be
 * sent to a RenderPass in an order optimized for rendering speed.
-----------------------------------------------------------------------------*/
class DrawBatch
{
  private:
    unsigned materialId;

    unsigned blendParamId;

    unsigned depthParamId;

    std::vector<unsigned> uniformIds;

    std::vector<DrawCommandParams> batches;

  public:
    /**
     * @brief Destructor
     *
     * Clean up and frees all internally owned resources.
     */
    ~DrawBatch() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all internal members to their default states.
     */
    DrawBatch() noexcept;

    /**
     * Copy Constructor
     *
     * @param b
     * Copies all data from the input parameter into *this.
     */
    DrawBatch(const DrawBatch& b) noexcept;

    /**
     * Move Constructor
     *
     * @param b
     * Moves all data from the input parameter into *this and returns the
     * input parameter into its default constructed state.
     */
    DrawBatch(DrawBatch&& b) noexcept;

    /**
     * Copy Operator
     *
     * @param b
     * Copies all data from the input parameter into *this.
     *
     * @return A reference to *this.
     */
    DrawBatch& operator=(const DrawBatch& b) noexcept;

    /**
     * Move Operator
     *
     * @param b
     * Moves all data from the input parameter into *this and returns the
     * input parameter into its default constructed state.
     *
     * @return A reference to *this.
     */
    DrawBatch& operator=(DrawBatch&& b) noexcept;

    /**
     * Reset all internal values to their defaults.
     */
    void reset() noexcept;
};
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_DRAW_BATCH_H__ */
