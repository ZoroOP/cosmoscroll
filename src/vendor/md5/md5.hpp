/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
*/

#ifndef MD5_HPP
#define MD5_HPP

#include <string>
#include <fstream>

class MD5
{
public:
    MD5();
    MD5(const std::string& source);
    MD5(std::ifstream& file);
    MD5(const unsigned char* source, size_t len);

    std::string calculate(const std::string& source);
    std::string calculate(std::ifstream& file);
    std::string calculate(const unsigned char* source, size_t len);

    std::string getHash() const;
    const unsigned char* getRawHash() const;

private:
    std::string m_sHash;
    unsigned char m_rawHash[16];
};

#endif // MD5_HPP
