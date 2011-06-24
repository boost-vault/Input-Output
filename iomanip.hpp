#ifndef BOOST_IOMANIP_HPP_
#define BOOST_IOMANIP_HPP_

//          Copyright Eugene Wee 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* boost::iomanip::literal is an input stream manipulator that matches input
 * with a specified string or character literal. If the input read matches the
 * literal, it is ignored, otherwise the stream is placed in a failed state.
 * Whitespace is not skipped, hence the std::ws manipulator should be used to
 * skip preceding whitespace, if necessary.
 */

#include <string>
#include <memory>
#include <istream>
#include <ios>

namespace boost
{
namespace iomanip
{
    namespace detail
    {
        // Implementation defined type to implement the manipulator.
        template<typename charT, typename Traits = std::char_traits<charT>,
            typename Allocator = std::allocator<charT> >
        struct token_matcher
        {
            typedef std::basic_string<charT, Traits, Allocator> token_type;
            token_matcher(const token_type& token) : token(token) {}
            token_type token;
        };

        // Extracts and matches input character-wise from the stream.
        template<typename charT, typename Traits, typename Allocator>
        std::basic_istream<charT, Traits>&
        operator>>(std::basic_istream<charT, Traits>& in,
            token_matcher<charT, Traits, Allocator> matcher)
        {
            typename std::basic_string<charT, Traits, Allocator>::iterator
                i = matcher.token.begin(),
                end = matcher.token.end();
            typename std::basic_istream<charT, Traits>::char_type c;
            for (; i != end && in.get(c); ++i)
            {
                if (*i != c)
                {
                    in.clear(std::basic_ios<charT, Traits>::failbit);
                    break;
                }
            }

            return in;
        }
    } // namespace detail

    // Match with literals as standard string objects.
    template<typename charT, typename Traits, typename Allocator>
    inline
    detail::token_matcher<charT, Traits, Allocator>
    literal(const std::basic_string<charT, Traits, Allocator>& token)
    {
        return detail::token_matcher<charT, Traits, Allocator>(token);
    }

    // Match with literals as null terminated strings.
    template<typename charT>
    inline detail::token_matcher<charT> literal(const charT* token)
    {
        return detail::token_matcher<charT>(std::basic_string<charT>(token));
    }

    // Match with literals as individual characters.
    template<typename charT>
    inline detail::token_matcher<charT> literal(charT token)
    {
        return detail::token_matcher<charT>(std::basic_string<charT>(1, token));
    }
} // namespace iomanip
} // namespace boost

#endif
