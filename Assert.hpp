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
#pragma once

#define ASSERTS_ENABLED

#ifdef ASSERTS_ENABLED

#include <fmt/format.h>
#include <fmt/compile.h>
#include <source_location>

namespace error
{
void handle_assert(std::source_location location, const char* expression);
void handle_assert_impl(std::source_location location, const char* expression, fmt::string_view message, fmt::format_args args);

template <typename S>
void handle_assert(std::source_location location, const char* expression, const S& message)
{
	handle_assert_impl(location, expression, fmt::string_view(message), fmt::format_args());
}

template <typename S, typename... Args>
void handle_assert(std::source_location location, const char* expression, const S& message, Args&&... args)
{
	auto format_string = fmt::string_view(message);
	handle_assert_impl(location, expression, format_string, fmt::make_format_args(format_string, args...));
}

} // namespace error

#if !defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL

#define ASSERT_FORMAT_NOTHING
#define ASSERT_FORMAT_EXPAND(...) __VA_ARGS__
#define ASSERT_FORMAT_HEAD(head, ...) head
#define ASSERT_FORMAT_TAIL(head, ...) __VA_ARGS__

#define ASSERT_FORMAT_COMMA_IF_PARENS(...) ,
#define ASSERT_FORMAT_LPAREN (

#define ASSERT_FORMAT_STRING_EMPTY()
#define ASSERT_FORMAT_STRING(x) , FMT_COMPILE(x)

#define ASSERT_FORMAT_CHOICE(a1, a2, selected, ...) selected
#define ASSERT_FORMAT_CHOOSE_IMPL(...) \
	ASSERT_FORMAT_EXPAND(ASSERT_FORMAT_CHOICE ASSERT_FORMAT_LPAREN \
	ASSERT_FORMAT_COMMA_IF_PARENS __VA_ARGS__ (), \
	ASSERT_FORMAT_STRING_EMPTY, ASSERT_FORMAT_STRING, ))

#define ASSERT_FORMAT_CHOOSE(format) ASSERT_FORMAT_CHOOSE_IMPL(format)(format)
#define ASSERT_MESSAGE(...) \
	ASSERT_FORMAT_CHOOSE(ASSERT_FORMAT_EXPAND(ASSERT_FORMAT_HEAD(__VA_ARGS__ ASSERT_FORMAT_NOTHING))), \
	ASSERT_FORMAT_EXPAND(ASSERT_FORMAT_TAIL(__VA_ARGS__ ASSERT_FORMAT_NOTHING))

#if __INTELLISENSE__

// Note: Simplifying the format string here for Intellisense so it makes it easier to read and understand in the tooltip
#undef ASSERT_FORMAT_STRING()
#define ASSERT_FORMAT_STRING(x) , x

// Note: Adding an extra "" parameter here so Intellisense doesn't complain about "expected an expression" when it expands the macro incorrectly
#undef ASSERT_MESSAGE()
#define ASSERT_MESSAGE(...) \
	ASSERT_FORMAT_CHOOSE(ASSERT_FORMAT_EXPAND(ASSERT_FORMAT_HEAD(__VA_ARGS__ ASSERT_FORMAT_NOTHING))), \
	ASSERT_FORMAT_EXPAND(ASSERT_FORMAT_TAIL(__VA_ARGS__, ""))

#endif // __INTELLISENSE__

#else

#define ASSERT_FORMAT_STRING(format, ...) FMT_COMPILE(format)

#define ASSERT_FORMAT_CHECKED_ARGS(format, ...) __VA_OPT__(, __VA_ARGS__)

#define ASSERT_MESSAGE(...) __VA_OPT__(, ASSERT_FORMAT_STRING(__VA_ARGS__) ASSERT_FORMAT_CHECKED_ARGS(__VA_ARGS__))

#endif

#define ASSERT(expression, ...) \
	do \
	{ \
		if (!(expression)) \
		{ \
			error::handle_assert(std::source_location::current(), #expression ASSERT_MESSAGE(__VA_ARGS__)); \
			__debugbreak(); \
		} \
	} \
	while (false)

#define VERIFY(expression, ...) \
	do \
	{ \
		if (!(expression)) \
		{ \
			error::handle_assert(std::source_location::current(), #expression ASSERT_MESSAGE(__VA_ARGS__)); \
			__debugbreak(); \
		} \
	} \
	while (false)

#define FAILURE(...) \
	do \
	{ \
		error::handle_assert(std::source_location::current(), "Failed" ASSERT_MESSAGE(__VA_ARGS__)); \
		__debugbreak(); \
	} \
	while (false)

#else // ASSERTS_ENABLED

#define ASSERT(expression, ...) \
	do \
	{ \
	} \
	while (false)
#define VERIFY(expression, ...) \
	do \
	{ \
		if (!(expression)) \
		{ \
		} \
	} \
	while (false)
#define FAILURE(...) \
	do \
	{ \
	} \
	while (false)

#endif // ASSERTS_ENABLED
