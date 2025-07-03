#!/bin/bash

# XENSIV BGT60TRxx Kas Configuration Validator
# This script validates Kas configuration files for syntax and basic structure

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if kas is available
check_kas() {
    if ! command -v kas &> /dev/null; then
        log_error "Kas is not installed. Please install it with: pip3 install kas"
        return 1
    fi
    
    log_info "Using Kas version: $(kas --version)"
    return 0
}

# Validate a single Kas configuration file
validate_config() {
    local config_file="$1"
    local errors=0
    
    log_info "Validating configuration: $config_file"
    
    # Check if file exists
    if [ ! -f "$config_file" ]; then
        log_error "Configuration file not found: $config_file"
        return 1
    fi
    
    # Check YAML syntax
    if ! python3 -c "import yaml; yaml.safe_load(open('$config_file'))" 2>/dev/null; then
        log_error "Invalid YAML syntax in $config_file"
        ((errors++))
    else
        log_success "YAML syntax is valid"
    fi
    
    # Check required fields
    local required_fields=("header" "machine" "distro" "target")
    for field in "${required_fields[@]}"; do
        if ! grep -q "^$field:" "$config_file"; then
            log_error "Missing required field: $field"
            ((errors++))
        fi
    done
    
    # Check if repos section exists
    if ! grep -q "^repos:" "$config_file"; then
        log_error "Missing repos section"
        ((errors++))
    fi
    
    # Check for xensiv-bgt60trxx repository
    if ! grep -A 10 "^repos:" "$config_file" | grep -q "xensiv-bgt60trxx:"; then
        log_error "Missing xensiv-bgt60trxx repository definition"
        ((errors++))
    fi
    
    # Validate with kas (dry-run)
    log_info "Running Kas validation (dry-run)..."
    if kas dump "$config_file" > /dev/null 2>&1; then
        log_success "Kas validation passed"
    else
        log_error "Kas validation failed"
        ((errors++))
    fi
    
    if [ $errors -eq 0 ]; then
        log_success "Configuration $config_file is valid"
        return 0
    else
        log_error "Configuration $config_file has $errors error(s)"
        return 1
    fi
}

# Main function
main() {
    local config_files=()
    local exit_code=0
    
    # Check if kas is available
    if ! check_kas; then
        exit 1
    fi
    
    # If no arguments provided, validate all kas/*.yml files
    if [ $# -eq 0 ]; then
        log_info "No configuration files specified, validating all kas/*.yml files"
        mapfile -t config_files < <(find kas -name "*.yml" -type f 2>/dev/null || true)
        
        if [ ${#config_files[@]} -eq 0 ]; then
            log_error "No Kas configuration files found in kas/ directory"
            exit 1
        fi
    else
        config_files=("$@")
    fi
    
    log_info "Validating ${#config_files[@]} configuration file(s)"
    echo
    
    # Validate each configuration file
    for config_file in "${config_files[@]}"; do
        if ! validate_config "$config_file"; then
            exit_code=1
        fi
        echo
    done
    
    # Summary
    if [ $exit_code -eq 0 ]; then
        log_success "All configuration files are valid"
    else
        log_error "One or more configuration files have errors"
    fi
    
    exit $exit_code
}

# Print usage information
print_usage() {
    cat << EOF
XENSIV BGT60TRxx Kas Configuration Validator

Usage: $0 [CONFIG_FILE...]

Arguments:
    CONFIG_FILE    Path to Kas configuration file(s) to validate
                   If not specified, validates all kas/*.yml files

Examples:
    $0                                    # Validate all kas/*.yml files
    $0 kas/xensiv-bgt60trxx-test.yml     # Validate specific file
    $0 kas/*.yml                         # Validate all kas files explicitly

EOF
}

# Handle command line arguments
if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    print_usage
    exit 0
fi

# Run main function
main "$@"