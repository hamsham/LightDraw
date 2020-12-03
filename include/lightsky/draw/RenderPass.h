
#ifndef __LS_DRAW_RENDER_PASS_H__
#define __LS_DRAW_RENDER_PASS_H__

namespace ls
{
namespace draw
{



class RenderPass
{
  private:

  public:
    /**
     * @brief Destructor
     *
     * Clean up and frees all internally owned resources.
     */
    ~RenderPass() noexcept;

    /**
     * @brief Constructor
     *
     * Initializes all internal members to their default states.
     */
    RenderPass() noexcept;

    /**
     * Copy Constructor
     *
     * @param
     * Copies all data from the input parameter into *this.
     */
    RenderPass(const RenderPass&) noexcept;

    /**
     * Move Constructor
     *
     * @param
     * Moves all data from the input parameter into *this and returns the
     * input parameter into its default constructed state.
     */
    RenderPass(RenderPass&&) noexcept;

    /**
     * Copy Operator
     *
     * @param
     * Copies all data from the input parameter into *this.
     *
     * @return A reference to *this.
     */
    RenderPass& operator=(const RenderPass&) noexcept;

    /**
     * Move Operator
     *
     * @param
     * Moves all data from the input parameter into *this and returns the
     * input parameter into its default constructed state.
     *
     * @return A reference to *this.
     */
    RenderPass& operator=(RenderPass&&) noexcept;
};
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_RENDER_PASS_H__ */
