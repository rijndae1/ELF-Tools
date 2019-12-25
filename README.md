# ELF-Tools
Basic ELF tools that I wrote back when I was learning the ELF file format and binary analysis.

- ELF64-parser: A simple ELF64 parser that displays info any ELF64 executable (ELF header, section headers, symbols,...).

- Strip-section-headers: A tool to strip section headers of an ELF executable, this technique is used by malware to make it harder for the analyst to reverse engineer the executable since a lot of ELF binary analysis tools rely on the section headers.

- ELF-NXbit: GCC and most compilers set the stack as non-executable, this prevents the execution of injected shellcode on the stack, this tool can be used to set NX bit, clear it, or query an ELF32 executable to see if the stack segment has this bit set or not.
