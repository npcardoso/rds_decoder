#include "group.h"

class RDS_decoder {
public:
    RDS_decoder ();

    /**
     * @brief Processes a block
     * @note Uses "accepts" and "process_impl"
     * @note Updates last_group after calling process_impl
     * @return true if block was processed, false otherwise
     */
    bool process (const group & g);

    inline virtual void rx_error () {}

    inline virtual void sync () {}

    inline virtual void no_sync () {}

    const group & last_group () const;

    inline virtual std::ostream & write_to (std::ostream & out) const {
        return out;
    }

protected:
    /**
     * @return true if this decoder decodes group g
     */
    virtual bool accepts (const group & g) const = 0;

    /**
     * @note The group processing should be performed inside this function.
     */
    virtual void process_impl (const group & g,
                               bool new_station) = 0;

private:
    group last_g;
};