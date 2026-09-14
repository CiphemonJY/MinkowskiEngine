/*
 * fork: NVTX RANGES COMPILED OUT.
 *
 * CUDA 12.8 ships its own nvtx3 headers in an inline versioned namespace. The vendored ~2020 copy
 * in this directory declares the same symbols unversioned, so with both in scope every reference
 * (nvtx3::domain, nvtx3::category, nvtx3::event_attributes, nvtx3::domain_thread_range) is
 * ambiguous -- 48 errors on the first .cu that touches the concurrent hash map.
 *
 * CUDF_FUNC_RANGE() only pushes a named range onto an NVTX timeline for Nsight. Nothing in
 * MinkowskiEngine reads that timeline, and no numeric result depends on it. Rather than port a
 * profiler shim we do not use, the whole thing becomes a no-op that keeps the public surface.
 *
 * Original preserved as ranges.hpp.orig.
 */
#pragma once

namespace cudf {

// Kept so existing template arguments (e.g. registered_message<cudf::libcudf_domain>) still name a
// real type if any survive; it carries no behaviour.
struct libcudf_domain {
  static constexpr char const *name{"libcudf"};
};

// Scope-guard shaped no-op: constructing one is free and destroying it does nothing, so any
// remaining `cudf::thread_range r{...};` site keeps compiling with identical semantics.
struct thread_range {
  thread_range() = default;
  template <typename... Args>
  explicit thread_range(Args &&...) {}
};

}  // namespace cudf

#define CUDF_FUNC_RANGE() do {} while (0)
