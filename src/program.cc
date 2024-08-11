#include "compound/program.hh"
#include "program.hh"

namespace compound {
namespace impl {
    uint Program::_uuidGen = 1;
    uint Program::_boundUuid = 0;

    Program::Program(const std::string& vertShaderPath, const std::string& fragShaderPath) {

    }

    Program::Program(const Program& other) {

    }

    Program& Program::operator=(const Program& other) {

    }

    Program::~Program() {

    }
}
} // namespace compound