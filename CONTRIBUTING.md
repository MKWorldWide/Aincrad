# 🤝 Contributing to Aincrad

Thank you for your interest in contributing to Aincrad! This document provides guidelines and information for contributors.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Issue Reporting](#issue-reporting)
- [Documentation](#documentation)
- [Commit Guidelines](#commit-guidelines)
- [Release Process](#release-process)

## 📜 Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). We are committed to providing a welcoming and inclusive environment for all contributors.

## 🚀 Getting Started

### Prerequisites

Before contributing, ensure you have:

- **CMake** 3.20 or higher
- **C++20** compatible compiler (GCC 11+, Clang 12+, MSVC 2022+)
- **Git** for version control
- **Python 3.8+** for build scripts
- **Vulkan SDK** (for rendering)
- **SteamVR** (for VR development)
- **Doxygen** (for documentation generation)

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:
   ```bash
   gh repo clone MKWorldWide/Aincrad
   cd Aincrad
   ```
3. Add the upstream repository:
   ```bash
   git remote add upstream https://github.com/MKWorldWide/Aincrad.git
   git fetch upstream
   ```

## 🛠️ Development Setup

### Building the Project

```bash
# Configure with CMake
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build the project
cmake --build build --config Debug --parallel

# Run tests
cd build && ctest --output-on-failure
```

### Development Workflow

1. Create a new branch for your feature/fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```
2. Make your changes following the coding standards
3. Run tests locally before committing
4. Commit your changes with a descriptive message (see [Commit Guidelines](#commit-guidelines))
5. Push to your fork and create a pull request

## 📝 Coding Standards

### C++ Style Guide

We follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) with the following exceptions:

- Use `#pragma once` instead of include guards
- 4 spaces for indentation
- 120 character line length

### Code Formatting

We use `clang-format` for code formatting. Before committing, run:

```bash
# Format all source files
find src/ include/ -name '*.h' -o -name '*.cpp' | xargs clang-format -i
```

## 🧪 Testing Guidelines

### Writing Tests

- Place tests in the `tests/` directory
- Test files should be named `test_*.cpp`
- Use the [Catch2](https://github.com/catchorg/Catch2) framework

### Running Tests

```bash
# Run all tests
cd build && ctest --output-on-failure

# Run specific test
./build/tests/your_test_name
```

## 🔄 Pull Request Process

1. Ensure your code passes all tests
2. Update documentation if needed
3. Ensure your branch is up-to-date with `main`
4. Open a pull request with a clear description of changes
5. Request reviews from maintainers
6. Address review comments
7. Wait for CI to pass
8. Squash merge into `main`

## 🐛 Issue Reporting

When reporting issues, please include:

1. A clear title and description
2. Steps to reproduce
3. Expected vs actual behavior
4. Environment details (OS, compiler version, etc.)
5. Relevant logs or screenshots

## 📖 Documentation

### Building Documentation

```bash
# Install dependencies
pip install mkdocs mkdocs-material mkdocs-git-revision-date-localized-plugin

# Serve documentation locally
mkdocs serve
```

### API Documentation

We use Doxygen for API documentation. To generate:

```bash
# Install Doxygen
sudo apt-get install doxygen graphviz

# Generate documentation
doxygen docs/Doxyfile
```

## 💡 Commit Guidelines

We follow [Conventional Commits](https://www.conventionalcommits.org/):

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes
- `refactor`: Code refactoring
- `test`: Adding or fixing tests
- `chore`: Maintenance tasks

Example commit message:
```
feat(combat): add combo system

Adds a new combo system for melee combat with support for chaining attacks.
Fixes #123
```

## 🚀 Release Process

1. Create a release branch from `main`
2. Update version numbers in relevant files
3. Update CHANGELOG.md
4. Create a release tag
5. Push the tag to trigger the release workflow
6. Create a GitHub release with release notes
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

# Run tests
ctest --output-on-failure
```

### Development Workflow

1. **Create a feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes** following the coding standards

3. **Test your changes**:
   ```bash
   # Run all tests
   ctest --output-on-failure
   
   # Run specific tests
   ./bin/aincrad_tests --gtest_filter=YourTestSuite*
   ```

4. **Commit your changes** with clear commit messages

5. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

## 📏 Coding Standards

### C++ Style Guide

We follow a modified version of the Google C++ Style Guide:

#### Naming Conventions

- **Classes**: `PascalCase` (e.g., `PlayerCharacter`, `AssetManager`)
- **Functions**: `camelCase` (e.g., `loadAsset`, `initializeCombatSystem`)
- **Variables**: `camelCase` (e.g., `assetManager`, `playerHealth`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_HEALTH`, `DEFAULT_SPEED`)
- **Namespaces**: `snake_case` (e.g., `aincrad::world`, `aincrad::characters`)

#### Code Formatting

- Use 4 spaces for indentation (no tabs)
- Maximum line length: 120 characters
- Use braces for all control structures
- Use `nullptr` instead of `NULL`

#### Example

```cpp
namespace aincrad {
namespace world {

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    // Load asset with platform-specific optimization
    std::shared_ptr<Asset> loadAsset(const std::string& assetId);
    
    // Update asset manager state
    void update();

private:
    // Initialize internal systems
    void initializeSystems();
    
    // Member variables
    std::unique_ptr<AssetDatabase> m_assetDatabase;
    std::unordered_map<std::string, std::shared_ptr<Asset>> m_loadedAssets;
};

} // namespace world
} // namespace aincrad
```

### File Organization

- **Header files**: `.h` extension
- **Source files**: `.cpp` extension
- **Test files**: `*Test.cpp` suffix
- **One class per file** when possible
- **Include guards** using `#pragma once`

### Documentation

#### Inline Comments

- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Document complex algorithms and business logic
- Explain "why" not just "what"

#### Function Documentation

```cpp
/**
 * Loads an asset from the database with platform-specific optimization.
 * 
 * @param assetId The unique identifier of the asset to load
 * @return Shared pointer to the loaded asset, or nullptr if loading failed
 * 
 * @note This function is thread-safe and supports async loading
 * @see AssetManager::unloadAsset()
 */
std::shared_ptr<Asset> loadAsset(const std::string& assetId);
```

## 🧪 Testing Guidelines

### Test Structure

- **Unit tests** for individual components
- **Integration tests** for system interactions
- **Performance tests** for critical paths
- **VR-specific tests** for VR functionality

### Test Naming

- Test class: `ClassNameTest`
- Test method: `MethodName_Scenario_ExpectedResult`

### Example Test

```cpp
#include <gtest/gtest.h>
#include "AssetManager.h"

class AssetManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_assetManager = std::make_unique<aincrad::world::AssetManager>();
    }

    void TearDown() override {
        m_assetManager.reset();
    }

    std::unique_ptr<aincrad::world::AssetManager> m_assetManager;
};

TEST_F(AssetManagerTest, LoadAsset_ValidAssetId_ReturnsAsset) {
    // Arrange
    std::string assetId = "test_sword";
    
    // Act
    auto asset = m_assetManager->loadAsset(assetId);
    
    // Assert
    ASSERT_NE(asset, nullptr);
    EXPECT_TRUE(asset->isLoaded());
}
```

### Test Coverage

- Aim for **90%+ code coverage**
- Test both success and failure scenarios
- Test edge cases and boundary conditions
- Test platform-specific behavior

## 🔄 Pull Request Process

### Before Submitting

1. **Ensure tests pass**:
   ```bash
   ctest --output-on-failure
   ```

2. **Check code style**:
   ```bash
   # Run clang-format (if available)
   clang-format -i src/**/*.cpp src/**/*.h
   ```

3. **Update documentation** for any new features or API changes

4. **Add tests** for new functionality

### Pull Request Template

Use the following template when creating a PR:

```markdown
## Description
Brief description of the changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Unit tests pass
- [ ] Integration tests pass
- [ ] Manual testing completed

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] Tests added/updated
- [ ] No breaking changes (or documented)

## Related Issues
Closes #(issue number)
```

### Review Process

1. **Automated checks** must pass
2. **Code review** by maintainers
3. **Testing verification** by maintainers
4. **Documentation review** for completeness
5. **Final approval** and merge

## 🐛 Issue Reporting

### Bug Reports

When reporting bugs, include:

- **Clear description** of the problem
- **Steps to reproduce** the issue
- **Expected behavior** vs actual behavior
- **Environment details** (OS, compiler, etc.)
- **Screenshots/logs** if applicable

### Feature Requests

For feature requests, include:

- **Problem description** and use case
- **Proposed solution** or approach
- **Benefits** of the feature
- **Implementation suggestions** if possible

### Issue Template

```markdown
## Summary
Brief description of the issue

## Environment
- OS: [e.g., Windows 10, macOS 12.0]
- Compiler: [e.g., MSVC 2019, GCC 9.3]
- CMake version: [e.g., 3.20.0]

## Steps to Reproduce
1. Step 1
2. Step 2
3. Step 3

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## Additional Information
Screenshots, logs, or other relevant information
```

## 📚 Documentation

### Documentation Standards

- **Keep documentation up-to-date** with code changes
- **Use clear, concise language**
- **Include code examples** where helpful
- **Cross-reference** related documentation
- **Use consistent formatting**

### Documentation Types

- **API Documentation**: Function and class documentation
- **Architecture Documentation**: System design and relationships
- **User Guides**: How-to guides for common tasks
- **Tutorials**: Step-by-step learning materials

### Updating Documentation

When updating documentation:

1. **Update inline comments** in code
2. **Update README files** for significant changes
3. **Update API documentation** for interface changes
4. **Add migration guides** for breaking changes

## 🏆 Recognition

Contributors will be recognized in:

- **Contributors list** in README
- **Release notes** for significant contributions
- **Project documentation** for major features
- **Community acknowledgments**

## 📞 Getting Help

If you need help:

- **Check existing documentation** first
- **Search existing issues** for similar problems
- **Ask in discussions** for general questions
- **Create an issue** for bugs or feature requests
- **Join our Discord** for real-time help

## 🎯 Areas for Contribution

We welcome contributions in these areas:

- **Core Systems**: Asset management, combat, AI
- **VR Integration**: Platform-specific optimizations
- **Testing**: Unit tests, integration tests, performance tests
- **Documentation**: Guides, tutorials, API docs
- **Tools**: Build scripts, development utilities
- **Examples**: Sample projects, demos

---

Thank you for contributing to Aincrad! Your help makes this project better for everyone. 🗡️✨ 