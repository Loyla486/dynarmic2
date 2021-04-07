/* This file is part of the dynarmic project.
 * Copyright (c) 2018 MerryMage
 * SPDX-License-Identifier: 0BSD
 */

#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "common/bit_util.h"
#include "common/common_types.h"
#include "frontend/decoder/decoder_detail.h"
#include "frontend/decoder/matcher.h"

namespace Dynarmic::A64 {

template <typename Visitor>
using Matcher = Decoder::Matcher<Visitor, u32>;

template <typename Visitor>
using DecodeTable = std::array<std::vector<Matcher<Visitor>>, 0x1000>;

namespace detail {
inline size_t ToFastLookupIndex(u32 instruction) {
    return ((instruction >> 10) & 0x00F) | ((instruction >> 18) & 0xFF0);
}
} // namespace detail

template <typename Visitor>
DecodeTable<Visitor> GetDecodeTable() {
    std::vector<Matcher<Visitor>> list = {
#define INST(fn, name, bitstring) Decoder::detail::detail<Matcher<Visitor>>::GetMatcher(&Visitor::fn, name, bitstring),
#include "a64.inc"
#undef INST
    };

    std::stable_sort(list.begin(), list.end(), [](const auto& matcher1, const auto& matcher2) {
        // If a matcher has more bits in its mask it is more specific, so it should come first.
        return Common::BitCount(matcher1.GetMask()) > Common::BitCount(matcher2.GetMask());
    });

    // Exceptions to the above rule of thumb.
    const std::set<std::string> comes_first {
        "MOVI, MVNI, ORR, BIC (vector, immediate)",
        "FMOV (vector, immediate)",
        "Unallocated SIMD modified immediate",
    };

    std::stable_partition(list.begin(), list.end(), [&](const auto& matcher) {
        return comes_first.count(matcher.GetName()) > 0;
    });

    DecodeTable<Visitor> table{};
    for (size_t i = 0; i < table.size(); ++i) {
        for (auto matcher : list) {
            const auto expect = detail::ToFastLookupIndex(matcher.GetExpected());
            const auto mask = detail::ToFastLookupIndex(matcher.GetMask());
            if ((i & mask) == expect) {
                table[i].push_back(matcher);
            }
        }
    }
    return table;
}

template<typename Visitor>
std::optional<std::reference_wrapper<const Matcher<Visitor>>> Decode(u32 instruction) {
    static const auto table = GetDecodeTable<Visitor>();

    const auto matches_instruction = [instruction](const auto& matcher) { return matcher.Matches(instruction); };

    const auto& subtable = table[detail::ToFastLookupIndex(instruction)];
    auto iter = std::find_if(subtable.begin(), subtable.end(), matches_instruction);
    return iter != subtable.end() ? std::optional<std::reference_wrapper<const Matcher<Visitor>>>(*iter) : std::nullopt;
}

} // namespace Dynarmic::A64
/* This file is part of the dynarmic project.
 * Copyright (c) 2018 MerryMage
 * SPDX-License-Identifier: 0BSD
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include <dynarmic/A64/config.h>

namespace Dynarmic {
namespace A64 {

struct Context;

class Jit final {
public:
    explicit Jit(UserConfig conf);
    ~Jit();

    /**
     * Runs the emulated CPU.
     * Cannot be recursively called.
     */
    void Run();

    /**
     * Step the emulated CPU for one instruction.
     * Cannot be recursively called.
     */
    void Step();

    /**
     * Clears the code cache of all compiled code.
     * Can be called at any time. Halts execution if called within a callback.
     */
    void ClearCache();

    /**
     * Invalidate the code cache at a range of addresses.
     * @param start_address The starting address of the range to invalidate.
     * @param length The length (in bytes) of the range to invalidate.
     */
    void InvalidateCacheRange(std::uint64_t start_address, std::size_t length);

    /**
     * Reset CPU state to state at startup. Does not clear code cache.
     * Cannot be called from a callback.
     */
    void Reset();

    /**
     * Stops execution in Jit::Run.
     * Can only be called from a callback.
     */
    void HaltExecution();

    /// Read Stack Pointer
    std::uint64_t GetSP() const;
    /// Modify Stack Pointer
    void SetSP(std::uint64_t value);

    /// Read Program Counter
    std::uint64_t GetPC() const;
    /// Modify Program Counter
    void SetPC(std::uint64_t value);

    /// Read general-purpose register.
    std::uint64_t GetRegister(std::size_t index) const;
    /// Modify general-purpose register.
    void SetRegister(size_t index, std::uint64_t value);

    /// Read all general-purpose registers.
    std::array<std::uint64_t, 31> GetRegisters() const;
    /// Modify all general-purpose registers.
    void SetRegisters(const std::array<std::uint64_t, 31>& value);

    /// Read floating point and SIMD register.
    Vector GetVector(std::size_t index) const;
    /// Modify floating point and SIMD register.
    void SetVector(std::size_t index, Vector value);

    /// Read all floating point and SIMD registers.
    std::array<Vector, 32> GetVectors() const;
    /// Modify all floating point and SIMD registers.
    void SetVectors(const std::array<Vector, 32>& value);

    /// View FPCR.
    std::uint32_t GetFpcr() const;
    /// Modify FPCR.
    void SetFpcr(std::uint32_t value);

    /// View FPSR.
    std::uint32_t GetFpsr() const;
    /// Modify FPSR.
    void SetFpsr(std::uint32_t value);

    /// View PSTATE
    std::uint32_t GetPstate() const;
    /// Modify PSTATE
    void SetPstate(std::uint32_t value);

    /// Clears exclusive state for this core.
    void ClearExclusiveState();

    /**
     * Returns true if Jit::Run was called but hasn't returned yet.
     * i.e.: We're in a callback.
     */
    bool IsExecuting() const;

    /**
     * Debugging: Disassemble all of compiled code.
     * @return A string containing disassembly of all host machine code produced.
     */
    std::string Disassemble() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace A64
} // namespace Dynarmic
