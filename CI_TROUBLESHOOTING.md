# CI Troubleshooting Guide

This document provides solutions for common issues encountered when running the Yocto/Kas CI builds.

## Common CI Issues and Solutions

### 1. Disk Space Issues

**Problem**: Build fails with "No space left on device" error.

**Solutions**:
- The CI workflow now includes aggressive disk cleanup
- Builds use `rm_work` to clean temporary files
- Monitor disk usage with `df -h` commands in the workflow

**Manual Fix**:
```bash
# Check disk usage
df -h

# Clean up manually if needed
sudo rm -rf /usr/share/dotnet
sudo rm -rf /usr/local/lib/android
sudo docker system prune -af
```

### 2. Memory Issues (OOM Killer)

**Problem**: Build processes get killed due to out-of-memory conditions.

**Solutions**:
- Limited parallel builds to 4 threads (`BB_NUMBER_THREADS=4`)
- Added disk monitoring to stop tasks when space is low
- Use `PARALLEL_MAKE=-j4` instead of using all available cores

**Configuration**:
```yaml
BB_DISKMON_DIRS = "STOPTASKS,${TMPDIR},1G,100K ABORT,${TMPDIR},100M,1K"
```

### 3. Repository Access Issues

**Problem**: Cannot fetch repositories or layers.

**Solutions**:
- Verify repository URLs in Kas configuration files
- Check network connectivity in CI environment
- Use HTTPS URLs instead of SSH for public repositories

**Check Configuration**:
```bash
# Validate Kas configurations
./validate-kas.sh

# Test repository access
git ls-remote https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx.git
```

### 4. Layer Compatibility Issues

**Problem**: Layer version mismatches or compatibility errors.

**Solutions**:
- Ensure all layers use compatible branches (kirkstone/scarthgap)
- Check `LAYERSERIES_COMPAT` in layer.conf files
- Verify layer dependencies are met

**Layer Configuration**:
```
LAYERSERIES_COMPAT_meta-xensiv-bgt60trxx = "kirkstone langdale mickledore nanbield scarthgap"
LAYERDEPENDS_meta-xensiv-bgt60trxx = "core openembedded-layer"
```

### 5. Package Build Failures

**Problem**: Specific packages fail to build.

**Solutions**:
- Check build logs in the artifacts
- Verify recipe syntax and dependencies
- Test builds locally with the same configuration

**Debug Commands**:
```bash
# Build specific package with verbose output
kas shell kas/config.yml -c "bitbake -v xensiv-bgt60trxx"

# Check package dependencies
kas shell kas/config.yml -c "bitbake-layers show-depends xensiv-bgt60trxx"
```

### 6. Cache Issues

**Problem**: Builds fail due to corrupted cache or cache misses.

**Solutions**:
- CI uses multiple cache levels (downloads, sstate, repositories)
- Cache keys include configuration file hashes
- Fallback cache keys for partial matches

**Cache Strategy**:
```yaml
key: yocto-sstate-${{ matrix.config.yocto_version }}-${{ matrix.config.machine }}-${{ hashFiles('kas/*.yml') }}
restore-keys: |
  yocto-sstate-${{ matrix.config.yocto_version }}-${{ matrix.config.machine }}-
  yocto-sstate-${{ matrix.config.yocto_version }}-
```

### 7. QEMU Testing Issues

**Problem**: QEMU boot tests fail or hang.

**Solutions**:
- Added timeout (300 seconds) for QEMU tests
- Only run QEMU tests on x86-64 builds
- Use `|| true` to prevent CI failure if QEMU test fails

**QEMU Test**:
```bash
timeout 300 kas shell config.yml -c "runqemu qemux86-64 nographic" || true
```

### 8. Environment Issues

**Problem**: Locale or environment variable issues.

**Solutions**:
- Set proper locale (en_US.UTF-8)
- Export all necessary environment variables
- Use consistent environment across all build steps

**Environment Setup**:
```bash
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export KAS_WORK_DIR=$(pwd)
```

## Debugging Failed Builds

### 1. Check Build Logs
- Download build logs from failed CI runs
- Look for error messages in `build/tmp/log/`
- Check specific package logs in `build/tmp/work/`

### 2. Reproduce Locally
```bash
# Use the same Kas configuration
kas build kas/xensiv-bgt60trxx-test-kirkstone-ci.yml

# Or use the build script
./kas-build.sh -c kas/xensiv-bgt60trxx-test-kirkstone-ci.yml -v
```

### 3. Validate Configuration
```bash
# Validate all configurations
./validate-kas.sh

# Validate specific configuration
./validate-kas.sh kas/xensiv-bgt60trxx-test-kirkstone-ci.yml
```

### 4. Check Dependencies
```bash
# Verify layer dependencies
kas shell kas/config.yml -c "bitbake-layers show-layers"

# Check recipe dependencies
kas shell kas/config.yml -c "bitbake-layers show-recipes xensiv*"
```

## Performance Optimization

### 1. Build Time Optimization
- Use shared state cache effectively
- Enable parallel builds (but not too many for CI)
- Use `rm_work` to clean temporary files

### 2. CI Resource Management
- Limit concurrent jobs in matrix builds
- Use `fail-fast: false` to continue other builds if one fails
- Optimize cache strategies

### 3. Artifact Management
- Only collect necessary artifacts
- Set appropriate retention periods
- Compress large artifacts

## Monitoring and Alerts

### 1. Build Status
- Check the build summary job for overall status
- Monitor individual matrix job results
- Set up notifications for build failures

### 2. Resource Usage
- Monitor disk space usage throughout builds
- Track build times for performance regression
- Monitor cache hit rates

### 3. Artifact Collection
- Verify artifacts are collected properly
- Check artifact sizes and retention
- Ensure logs are available for failed builds

## Configuration Management

### 1. Version Control
- Keep Kas configurations in version control
- Use semantic versioning for releases
- Tag stable configurations

### 2. Testing
- Test configuration changes locally first
- Use feature branches for experimental changes
- Validate configurations before merging

### 3. Documentation
- Keep this troubleshooting guide updated
- Document configuration changes
- Maintain build instructions

## Getting Help

### 1. Check Logs
- Always check the full build logs first
- Look for specific error messages
- Check both BitBake and Kas logs

### 2. Community Resources
- Yocto Project documentation
- Kas documentation and examples
- OpenEmbedded community forums

### 3. Local Testing
- Reproduce issues locally when possible
- Test with minimal configurations
- Use verbose output for debugging

## Maintenance Tasks

### 1. Regular Updates
- Update Yocto releases (LTS versions)
- Update layer branches
- Update CI runner versions

### 2. Cache Management
- Monitor cache sizes
- Clean old cache entries
- Optimize cache strategies

### 3. Configuration Review
- Review configurations regularly
- Remove unused configurations
- Optimize build settings

This troubleshooting guide should help resolve most common CI issues. For specific problems, always check the build logs first and try to reproduce the issue locally.