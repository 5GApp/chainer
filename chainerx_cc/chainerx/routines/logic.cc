#include "chainerx/routines/logic.h"

#include "chainerx/array.h"
#include "chainerx/backprop_mode.h"
#include "chainerx/device.h"
#include "chainerx/dtype.h"
#include "chainerx/kernels/logic.h"
#include "chainerx/routines/creation.h"
#include "chainerx/routines/manipulation.h"
#include "chainerx/routines/routines_util.h"
#include "chainerx/routines/type_util.h"
#include "chainerx/shape.h"

namespace chainerx {

namespace {

void CheckLogicDtypes(const Array& x1, const Array& x2) {
    if ((x1.dtype() == Dtype::kBool) != (x2.dtype() == Dtype::kBool)) {
        throw DtypeError{"Comparison of ", GetDtypeName(x1.dtype()), " and ", GetDtypeName(x2.dtype()), " is not supported."};
    }
}

}  // namespace

Array Equal(const Array& x1, const Array& x2) {
    CheckLogicDtypes(x1, x2);
    auto func = [](const Array& x1, const Array& x2, Array& out) { x1.device().backend().CallKernel<EqualKernel>(x1, x2, out); };
    return internal::BroadcastBinary(func, x1, x2, Dtype::kBool);
}

Array NotEqual(const Array& x1, const Array& x2) {
    CheckLogicDtypes(x1, x2);
    auto func = [](const Array& x1, const Array& x2, Array& out) { x1.device().backend().CallKernel<NotEqualKernel>(x1, x2, out); };
    return internal::BroadcastBinary(func, x1, x2, Dtype::kBool);
}

Array Greater(const Array& x1, const Array& x2) {
    CheckLogicDtypes(x1, x2);
    auto func = [](const Array& x1, const Array& x2, Array& out) { x1.device().backend().CallKernel<GreaterKernel>(x1, x2, out); };
    return internal::BroadcastBinary(func, x1, x2, Dtype::kBool);
}

Array GreaterEqual(const Array& x1, const Array& x2) {
    CheckLogicDtypes(x1, x2);
    auto func = [](const Array& x1, const Array& x2, Array& out) {
        return x1.device().backend().CallKernel<GreaterEqualKernel>(x1, x2, out);
    };
    return internal::BroadcastBinary(func, x1, x2, Dtype::kBool);
}

Array LogicalNot(const Array& x) {
    Array out = Empty(x.shape(), Dtype::kBool, x.device());
    {
        NoBackpropModeScope scope{};
        x.device().backend().CallKernel<LogicalNotKernel>(x, out);
    }
    return out;
}

Array LogicalAnd(const Array& x1, const Array& x2) {
    CheckLogicDtypes(x1, x2);
    auto func = [](const Array& x1, const Array& x2, Array& out) {
        return x1.device().backend().CallKernel<LogicalAndKernel>(x1, x2, out);
    };
    return internal::BroadcastBinary(func, x1, x2, Dtype::kBool);
}

Array LogicalOr(const Array& x1, const Array& x2) {
    CheckLogicDtypes(x1, x2);
    auto func = [](const Array& x1, const Array& x2, Array& out) { return x1.device().backend().CallKernel<LogicalOrKernel>(x1, x2, out); };
    return internal::BroadcastBinary(func, x1, x2, Dtype::kBool);
}

Array LogicalXor(const Array& x1, const Array& x2) {
    CheckLogicDtypes(x1, x2);
    auto func = [](const Array& x1, const Array& x2, Array& out) {
        return x1.device().backend().CallKernel<LogicalXorKernel>(x1, x2, out);
    };
    return internal::BroadcastBinary(func, x1, x2, Dtype::kBool);
}

Array All(const Array& a, const OptionalAxes& axis, bool keepdims) {
    Axes sorted_axis = internal::GetSortedAxesOrAll(axis, a.ndim());
    Array out = internal::EmptyReduced(a.shape(), Dtype::kBool, sorted_axis, keepdims, a.device());
    {
        NoBackpropModeScope scope{};
        a.device().backend().CallKernel<AllKernel>(a, sorted_axis, out);
    }
    return out;
}

Array Any(const Array& a, const OptionalAxes& axis, bool keepdims) {
    Axes sorted_axis = internal::GetSortedAxesOrAll(axis, a.ndim());
    Array out = internal::EmptyReduced(a.shape(), Dtype::kBool, sorted_axis, keepdims, a.device());
    {
        NoBackpropModeScope scope{};
        a.device().backend().CallKernel<AnyKernel>(a, sorted_axis, out);
    }
    return out;
}

}  // namespace chainerx
