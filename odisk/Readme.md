## OtaOS File System Structure

Top level:
    * Boot loader
    * Kernel
    * OS filesystem
    
OS file system signature starts at 0x0FAB

Root directory
    object count : uint16_t
    <object list>
    
object:
    Type: (uint8_t)
        Bits 0-4: 1- file; 2- folder
        Bits 5-8: Reserved
    Size: (uint16_t)
    FS Name Length (uint8_t)
    FS Name
    Content
    
