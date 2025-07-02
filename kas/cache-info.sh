#!/bin/bash
# Cache information script for Kas builds
# This script shows cache statistics and provides optimization tips

set -e

echo "=== Kas Build Cache Information ==="
echo

# Check if we're in a Kas build directory
if [ ! -f "kas/base-image.yml" ]; then
    echo "❌ Not in a Kas project directory"
    echo "Please run this script from the project root"
    exit 1
fi

echo "📊 Cache Directory Sizes:"
echo "========================"

# Function to show directory size
show_cache_size() {
    local dir=$1
    local name=$2
    if [ -d "$dir" ]; then
        local size=$(du -sh "$dir" 2>/dev/null | cut -f1)
        local files=$(find "$dir" -type f 2>/dev/null | wc -l)
        echo "✓ $name: $size ($files files)"
    else
        echo "✗ $name: Not found"
    fi
}

show_cache_size "downloads" "Downloads Cache"
show_cache_size "sstate-cache" "Shared State Cache"
show_cache_size ".kas-cache" "Repository Cache"
show_cache_size "build/tmp/work" "Work Directory"

echo
echo "🚀 Cache Optimization Tips:"
echo "=========================="
echo "1. Downloads cache: Stores source tarballs, git repos, etc."
echo "   - Shared across all builds"
echo "   - Safe to keep indefinitely"
echo
echo "2. Shared State (sstate) cache: Stores compiled packages"
echo "   - Machine-specific optimizations"
echo "   - Provides biggest speed improvement"
echo "   - Can grow large - clean periodically"
echo
echo "3. Repository cache: Stores Yocto layer repositories"
echo "   - Shared across configurations"
echo "   - Relatively small but important"
echo
echo "4. Work directory: Temporary build files"
echo "   - Should be cleaned after builds"
echo "   - Not cached in CI"

echo
echo "🧹 Cache Maintenance Commands:"
echo "============================="
echo "# Clean old sstate entries (older than 30 days):"
echo "find sstate-cache -type f -mtime +30 -delete"
echo
echo "# Clean old downloads (older than 60 days):"
echo "find downloads -type f -mtime +60 -delete"
echo
echo "# Clean work directory:"
echo "rm -rf build/tmp/work"
echo
echo "# Show largest cache entries:"
echo "du -h sstate-cache/* | sort -hr | head -20"

echo
echo "⏱️  Expected Build Times:"
echo "======================="
echo "First build (cold cache):     45-90 minutes"
echo "Second build (warm cache):    10-20 minutes"
echo "Incremental build:            2-5 minutes"
echo "CI build (GitHub Actions):    15-30 minutes"

echo
echo "💡 Pro Tips:"
echo "==========="
echo "- Use 'kas-container build' for consistent environments"
echo "- Enable rm_work in local.conf to save disk space"
echo "- Use separate sstate caches for different machines"
echo "- Monitor cache hit rates in build logs"