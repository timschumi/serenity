/*
 * Copyright (c) 2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/MemoryStream.h>
#include <LibCompress/Brotli.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(uint8_t const* data, size_t size)
{
    AK::set_debug_enabled(false);
    FixedMemoryStream bufstream { { data, size } };

    auto brotli_stream_or_error = Compress::BrotliDecompressionStream::create(MaybeOwned<Stream> { bufstream });
    if (brotli_stream_or_error.is_error())
        return 0;
    auto brotli_stream = brotli_stream_or_error.release_value();

    (void)brotli_stream->read_until_eof();
    return 0;
}
