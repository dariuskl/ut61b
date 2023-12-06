find_package(hidapi QUIET CONFIG) # Arch Linux
if (NOT hidapi_FOUND)
    # Ubuntu's libhidapi-dev package provides only pkgconfig files
    find_package(PkgConfig QUIET REQUIRED)
    if (PkgConfig_FOUND)
        pkg_check_modules(hidapi REQUIRED QUIET IMPORTED_TARGET GLOBAL hidapi-libusb)
        add_library(hidapi::libusb ALIAS PkgConfig::hidapi)
    endif ()
endif ()
