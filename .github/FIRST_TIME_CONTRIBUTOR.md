# 👋 First Time Contributor's Guide

Welcome to Aincrad! We're excited to have you contribute. This guide will help you make your first contribution.

## 🚀 Quick Start

1. **Find an Issue**
   - Look for issues labeled `good first issue` or `help wanted`
   - Comment on the issue to express interest
   - Wait for a maintainer to assign it to you

2. **Set Up Your Environment**
   ```bash
   # Fork and clone the repository
   gh repo fork MKWorldWide/Aincrad --clone
   cd Aincrad
   
   # Set up development environment
   ./scripts/setup.sh
   ```

3. **Make Your Changes**
   - Create a new branch: `git checkout -b feature/your-feature-name`
   - Make your changes following the coding standards
   - Add tests for your changes
   - Run tests locally before committing

4. **Submit a Pull Request**
   ```bash
   # Stage and commit your changes
   git add .
   git commit -m "feat(scope): your commit message"
   
   # Push to your fork
   git push -u origin feature/your-feature-name
   
   # Create a pull request
   gh pr create --fill
   ```

## 🛠️ Development Environment

### Prerequisites
- Git
- CMake 3.20+
- C++20 compatible compiler
- Python 3.8+
- Node.js 18+ (for web components)

### Building the Project

```bash
# Configure build
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build the project
cmake --build build --config Debug --parallel

# Run tests
cd build && ctest --output-on-failure
```

## 📚 Learning Resources

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [GitHub Flow](https://guides.github.com/introduction/flow/)
- [Conventional Commits](https://www.conventionalcommits.org/)
- [Project Documentation](https://mkworldwide.github.io/Aincrad/)

## 🤝 Getting Help

- Join our [Discord server](https://discord.gg/your-invite-link)
- Ask questions in the `#help` channel
- Check the [FAQ](#) section

## 🎉 Your First Contribution

We suggest starting with documentation improvements or small bug fixes. Here are some ideas:

- Fix typos in documentation
- Improve code comments
- Add test cases
- Update dependencies
- Improve error messages

## 📝 Next Steps

After your first PR is merged:
1. Celebrate! 🎉
2. Check out more challenging issues
3. Consider becoming a regular contributor
4. Help other new contributors

Thank you for contributing to Aincrad! Your work helps make this project better for everyone.
