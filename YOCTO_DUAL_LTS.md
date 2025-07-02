# Yocto Dual LTS Support

This project now supports **both Yocto LTS versions** to provide maximum flexibility for different deployment scenarios.

## 🏗️ Supported Yocto Versions

| Version | Release | Status | Support Until | Recommended For |
|---------|---------|--------|---------------|-----------------|
| **Kirkstone** | 4.0 LTS | Stable | April 2026 | Production deployments, proven stability |
| **Scarthgap** | 5.0 LTS | Latest | April 2028 | New projects, latest features and security |

## 📁 Configuration Files

### Kirkstone LTS (Stable)
- `kas/base-image.yml` - Base configuration
- `kas/xensiv-bgt60trxx-test.yml` - QEMU test image
- `kas/xensiv-bgt60trxx-rpi4.yml` - Raspberry Pi 4 image

### Scarthgap LTS (Latest)
- `kas/base-image-scarthgap.yml` - Base configuration
- `kas/xensiv-bgt60trxx-test-scarthgap.yml` - QEMU test image
- `kas/xensiv-bgt60trxx-rpi4-scarthgap.yml` - Raspberry Pi 4 image

## 🚀 Quick Start

### Build with Kirkstone (Stable)
```bash
# QEMU test image
kas build kas/xensiv-bgt60trxx-test.yml

# Raspberry Pi 4 image
kas build kas/xensiv-bgt60trxx-rpi4.yml
```

### Build with Scarthgap (Latest)
```bash
# QEMU test image
kas build kas/xensiv-bgt60trxx-test-scarthgap.yml

# Raspberry Pi 4 image
kas build kas/xensiv-bgt60trxx-rpi4-scarthgap.yml
```

## 🔄 GitHub Actions CI

The CI pipeline now builds **all four configurations**:

1. **QEMU x86-64 (Kirkstone)** - Stable LTS testing
2. **Raspberry Pi 4 (Kirkstone)** - Stable LTS hardware target
3. **QEMU x86-64 (Scarthgap)** - Latest LTS testing
4. **Raspberry Pi 4 (Scarthgap)** - Latest LTS hardware target

### Cache Strategy

Each Yocto version has **isolated caches** to prevent conflicts:

- `yocto-downloads-kirkstone-*` / `yocto-downloads-scarthgap-*`
- `yocto-sstate-kirkstone-*` / `yocto-sstate-scarthgap-*`
- `kas-repos-kirkstone-*` / `kas-repos-scarthgap-*`

## 🎯 When to Use Which Version

### Choose Kirkstone if:
- ✅ You need **proven stability** for production
- ✅ You're working with **existing Kirkstone-based** systems
- ✅ You prioritize **long-term consistency** over latest features
- ✅ You're in a **regulated environment** requiring stable toolchains

### Choose Scarthgap if:
- ✅ You're starting a **new project**
- ✅ You want the **latest security updates** and bug fixes
- ✅ You need **newer kernel versions** and updated packages
- ✅ You want **longer support lifecycle** (until 2028)

## 🔧 Migration Between Versions

Both versions use the **same meta-layer structure**, making migration straightforward:

```bash
# From Kirkstone to Scarthgap
sed 's/xensiv-bgt60trxx-test\.yml/xensiv-bgt60trxx-test-scarthgap.yml/' build-script.sh

# From Scarthgap to Kirkstone  
sed 's/xensiv-bgt60trxx-test-scarthgap\.yml/xensiv-bgt60trxx-test.yml/' build-script.sh
```

## 📊 Build Performance

Both versions benefit from the **comprehensive caching strategy**:

- **First build**: ~45-60 minutes
- **Cached builds**: ~15-30 minutes
- **Incremental builds**: ~5-15 minutes

## 🛠️ Development Workflow

For development, you can easily switch between versions:

```bash
# Test on stable Kirkstone
kas build kas/xensiv-bgt60trxx-test.yml

# Validate on latest Scarthgap
kas build kas/xensiv-bgt60trxx-test-scarthgap.yml

# Deploy stable to production
kas build kas/xensiv-bgt60trxx-rpi4.yml
```

This dual LTS approach ensures your project can leverage both **stability** and **innovation** as needed! 🎉