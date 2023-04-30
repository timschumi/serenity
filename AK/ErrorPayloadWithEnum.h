/*
 * Copyright (c) 2023, Tim Schumacher <timschumi@gmx.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Error.h>
#include <AK/Format.h>

namespace AK {

template<Enum T>
class ErrorPayloadWithEnum : public ErrorPayload {
public:
    ErrorPayloadWithEnum(T custom_error_code)
        : ErrorPayload(to_underlying(custom_error_code))
    {
    }

    T to_enum() const
    {
        return static_cast<T>(m_storage);
    }

    virtual ErrorOr<void> format(Formatter<FormatString>& formatter, FormatBuilder& builder) const override
    {
        return formatter.format(builder, "{}"sv, to_enum());
    };

    virtual bool operator==(ErrorPayload const& other) const override
    {
#if defined(KERNEL) || defined(_DYNAMIC_LOADER)
        // TODO: We don't have a way to use something like dynamic_cast with -fno-rtti.
        (void)other;
        VERIFY_NOT_REACHED();
#else
        auto const* other_as_specific_type = dynamic_cast<ErrorPayloadWithEnum<T> const*>(&other);

        // If it isn't the same type, it can't be equal.
        if (!other_as_specific_type)
            return false;

        return m_storage == other_as_specific_type->m_storage;
#endif
    }

    bool operator==(T value) const
    {
        return to_enum() == value;
    }
};

template<Enum T>
struct Formatter<ErrorPayloadWithEnum<T>> : Formatter<FormatString> {
    ErrorOr<void> format(FormatBuilder& builder, ErrorPayloadWithEnum<T> const& error)
    {
        return Formatter<FormatString>::format(builder, "{}"sv, error);
    }
};

}

#if USING_AK_GLOBALLY
using AK::ErrorPayloadWithEnum;
#endif
