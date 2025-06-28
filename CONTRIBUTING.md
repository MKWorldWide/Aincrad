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

## 📜 Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). We are committed to providing a welcoming and inclusive environment for all contributors.

## 🚀 Getting Started

### Prerequisites

Before contributing, ensure you have:

- **CMake** 3.14 or higher
- **C++17** compatible compiler
- **Git** for version control
- **Google Test** for testing
- **JsonCpp** for configuration
- **Unreal Engine 5** (for full game development)

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:
   ```bash
   git clone https://github.com/yourusername/aincrad.git
   cd aincrad
   ```
3. Add the upstream repository:
   ```bash
   git remote add upstream https://github.com/original-owner/aincrad.git
   ```

## 🛠️ Development Setup

### Building the Project

```bash
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