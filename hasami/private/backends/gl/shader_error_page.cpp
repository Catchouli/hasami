#include "shader_error_page.hpp"

#include <io.h>
#include <fstream>
#include <sstream>
#include <regex>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shellapi.h>
#include <winuser.h>
#define mktemp _mktemp
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace hs {
namespace gl {

// Error page stuff

#ifdef WIN32
void openInBrowser(const std::string& url)
{
  ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
#else
void openInBrowser(const std::string&) {}
#endif

std::string genShaderErrorPage(const std::string& source, const std::string& errors, const std::string& shaderType)
{
  char name[256];
#ifdef WIN32
  tmpnam_s(name, 255);
#else
  tmpnam(name);
#endif

  std::istringstream ss(source);
  std::istringstream errs(errors);

  std::string filename = std::string(name) + ".html";

  std::ofstream file(filename);
  std::string line;

  file << "<script src = \"https://code.jquery.com/jquery-3.3.1.min.js\"></script>" << std::endl;

  file << "<style type=\"text/css\">" << std::endl;
  file << "* {" << std::endl;
  file << "  font-family: monaco;" << std::endl;
  file << "}" << std::endl;
  file << ".linenum {" << std::endl;
  file << "  padding-right: 1em;" << std::endl;
  file << "  text-align: right;" << std::endl;
  file << "}" << std::endl;
  file << ".linenum a:link {" << std::endl;
  file << "  color: black;" << std::endl;
  file << "  text-decoration: none;" << std::endl;
  file << "}" << std::endl;
  file << ".linenum a:active {" << std::endl;
  file << "  color: black;" << std::endl;
  file << "  text-decoration: none;" << std::endl;
  file << "}" << std::endl;
  file << ".linenum a:visited {" << std::endl;
  file << "  color: black;" << std::endl;
  file << "  text-decoration: none;" << std::endl;
  file << "}" << std::endl;
  file << ".line {" << std::endl;
  file << "}" << std::endl;
  file << "#left {" << std::endl;
  file << "  display: inline-block;" << std::endl;
  file << "  float: left;" << std::endl;
  file << "  width: 50%;" << std::endl;
  file << "}" << std::endl;
  file << "#right {" << std::endl;
  file << "  display: inline-block;" << std::endl;
  file << "  float: right;" << std::endl;
  file << "  width: 50%;" << std::endl;
  file << "}" << std::endl;
  file << ".selected {" << std::endl;
  file << "  background: #CCC;" << std::endl;
  file << "}" << std::endl;
  file << "</style>" << std::endl;

  file << "<h1>" << shaderType << " shader error log</h1>" << std::endl;
  file << "<div id=\"left\">" << std::endl;
  file << "<table>" << std::endl;
  int lineNum = 1;
  while (std::getline(ss, line)) {
    file << "  <tr id=\"" << std::to_string(lineNum) << "\">" << std::endl;
    file << "    <td class=\"linenum\"><a href=\"#" << std::to_string(lineNum) << "\">" << std::endl;
    file << "      " << std::to_string(lineNum++) << std::endl;
    file << "    </a></td>" << std::endl;
    file << "    <td class=\"line\">" << line << "</td>" << std::endl;
    file << "  </tr>" << std::endl;
  }
  file << "</table>" << std::endl;
  file << "</div>" << std::endl;

  file << "<div id=\"right\">" << std::endl;
  std::regex regexp("0\\((\\d+)\\) : (.+)");
  std::smatch matches;
  while (std::getline(errs, line)) {
    if (std::regex_search(line, matches, regexp)) {
      int lineNum = atoi(matches[1].str().c_str());
      file << "<a class=\"lineref\" href=\"#" << matches[1] << "\">" << matches[1] << "</a>" << ": " << matches[2];
    }
    else {
      file << line;
    }
    file << "<br>";
  }
  file << "</div>" << std::endl;

  file << "<script>" << std::endl;
  file << "  $('a.lineref').click(function() {" << std::endl;
  file << "    $('.selected').removeClass('selected')" << std::endl;
  file << "    $('#'+this.text.trim()).addClass('selected')" << std::endl;
  file << "  })" << std::endl;
  file << "</script>" << std::endl;

  return filename;
}

}
}
