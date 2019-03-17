#pragma once

#include <string>

namespace hs {
namespace gl {

void openInBrowser(const std::string& url);
std::string genShaderErrorPage(const std::string& source, const std::string& errors, const std::string& shaderType);

}
}