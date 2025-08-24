#ifndef PARSER_H
#define PARSER_H

#include "../models/mesh.cpp"
#include <string>

class parser
{
private:
    static int parse_vertex_index(const std::string& vertex_string);
public:
    static mesh parse_wavefont_file(const std::string& path);

     // Delete constructors for utility class
    parser() = delete;
    ~parser() = delete;
    parser(const parser&) = delete;
    parser& operator=(const parser&) = delete;
};


#endif