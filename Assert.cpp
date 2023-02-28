//
// MIT License
//
// Copyright (c) 2013-2023 Dominik Grabiec
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "Assert.hpp"

// Note: Need to include the Windows header file here or forward declare the two functions used in print_fatal_message
// Consider using the Windows Header file from https://github.com/DominikGrabiec/WindowsHeader

#include <cstdio>
#include <array>

namespace error
{

static void print_fatal_message(const char* message)
{
	// Note: Call function to flush logs here

	// Note: IsDebuggerPresent() just literally checks a global in-process flag
	if (IsDebuggerPresent())
	{
		::OutputDebugStringA(message);
	}

	::fputs(message, stderr);
}

//------------------------------------------------------------------------------

// Message buffer size fixed to about the size of the buffer used in OutputDebugString
// Reference: http://www.unixwiz.net/techtips/outputdebugstring.html
constexpr size_t message_buffer_size = 4096;

void handle_assert(std::source_location location, const char* expression)
{
	std::array<char, message_buffer_size> message_buffer;
	auto ret = fmt::format_to_n(message_buffer.data(), message_buffer.size(), "{}({}) : Assertion failed '{}' in function {}", location.file_name(), location.line(), expression, location.function_name());
	*(std::min(ret.out, &message_buffer.back())) = 0;
	print_fatal_message(message_buffer.data());
}

void handle_assert_impl(std::source_location location, const char* expression, fmt::string_view message, fmt::format_args args)
{
	std::array<char, message_buffer_size> message_buffer;
	auto ret = fmt::format_to_n(message_buffer.data(), message_buffer.size(), "{}({}) : Assertion failed '{}' in function {}, ", location.file_name(), location.line(), expression, location.function_name());
	if (ret.size < message_buffer.size())
	{
		ret = fmt::vformat_to_n(ret.out, message_buffer.size() - ret.size, message, args);
	}
	*(std::min(ret.out, &message_buffer.back())) = 0;
	print_fatal_message(message_buffer.data());
}

} // namespace error
