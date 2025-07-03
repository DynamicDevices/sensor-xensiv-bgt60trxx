# Kas CI Build Fix Summary

## Issue
The GitHub Actions CI builds were failing with the error:
```
ERROR: The following layer directories do not exist:
ERROR:    /home/runner/work/sensor-xensiv-bgt60trxx/sensor-xensiv-bgt60trxx/build/../meta-raspberrypi/meta-raspberrypi
ERROR: Please check BBLAYERS in /home/runner/work/sensor-xensiv-bgt60trxx/sensor-xensiv-bgt60trxx/build/conf/bblayers.conf
```

## Root Cause
The Kas configuration files were using an incorrect YAML format for layer specifications. The configurations were using a list format:

```yaml
layers:
  - meta-raspberrypi
```

But Kas expects a mapping format:

```yaml
layers:
  meta-raspberrypi:
```

## Solution
Fixed all Kas configuration files to use the correct layer specification format:

### Files Fixed:
- `kas/xensiv-bgt60trxx-rpi4-kirkstone-ci.yml`
- `kas/xensiv-bgt60trxx-rpi4-scarthgap-ci.yml`
- `kas/xensiv-bgt60trxx-test-kirkstone-ci.yml`
- `kas/xensiv-bgt60trxx-test-scarthgap-ci.yml`
- `kas/xensiv-bgt60trxx-rpi4.yml`
- `kas/xensiv-bgt60trxx-rpi4-scarthgap.yml`
- `kas/xensiv-bgt60trxx-test.yml`
- `kas/xensiv-bgt60trxx-test-scarthgap.yml`

### Change Applied:
**Before:**
```yaml
repos:
  meta-raspberrypi:
    url: https://git.yoctoproject.org/meta-raspberrypi
    branch: kirkstone
    layers:
      - meta-raspberrypi
```

**After:**
```yaml
repos:
  meta-raspberrypi:
    url: https://git.yoctoproject.org/meta-raspberrypi
    branch: kirkstone
    layers:
      meta-raspberrypi:
```

## Validation
- All configurations now pass `kas dump` validation
- All configurations pass the `validate-kas.sh` script
- The fix follows the official Kas documentation format
- Verified against the official meta-raspberrypi kas configuration

## Impact
- ✅ CI builds should now work correctly
- ✅ No breaking changes for local development
- ✅ Consistent format across all configurations
- ✅ Follows Kas best practices

## Commits
- **9f08638**: fix: Correct Kas layer specification format to resolve CI build failures
- **bcea7db**: feat: Rename Kirkstone CI configs for consistency and improve CI setup

The CI builds should now complete successfully without the layer directory errors.