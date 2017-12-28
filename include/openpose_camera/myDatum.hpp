#ifndef OPENPOSE_MY_DATUM_HPP
#define OPENPOSE_MY_DATUM_HPP

#include <openpose/core/array.hpp>
#include <openpose/core/datum.hpp>


struct WMyDatum : public op::Datum
{
    bool boolThatUserNeedsForSomeReason;

    WMyDatum(const bool boolThatUserNeedsForSomeReason_ = false) :
        boolThatUserNeedsForSomeReason{boolThatUserNeedsForSomeReason_}
    {}
};

#endif // OPENPOSE_MY_DATUM_HPP
