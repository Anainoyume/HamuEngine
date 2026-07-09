#include "JsonParser.h"
#include "JsonLexer.h"

#include <format>

namespace hamu::utils
{

    std::ostream& operator<<(std::ostream& os, const JsonParserResult& result) {
        if (result.ok()) {
            os << "[OK]";
            return os;
        }
        os << std::format("[ERROR]: {} (line: {}, column: {})", result.info, result.line, result.column);
        return os;
    }

} // namespace hamu::utils