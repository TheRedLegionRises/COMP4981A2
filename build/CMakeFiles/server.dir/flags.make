# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# compile C with /usr/bin/gcc
C_DEFINES = 

C_INCLUDES = -I/home/jerry/COMP4981/A2/include

C_FLAGS = -g -std=c17 -fsanitize=address -fsanitize-address-use-after-scope -fsanitize=leak -fsanitize=pointer-overflow -fsanitize=undefined -fsanitize=shift -fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum -fsanitize=vptr -fsanitize=builtin -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_GNU_SOURCE -D_DARWIN_C_SOURCE -D__BSD_VISIBLE -Werror -fanalyzer -Wanalyzer-allocation-size -Wanalyzer-deref-before-check -Wanalyzer-double-fclose -Wanalyzer-double-free -Wanalyzer-exposure-through-output-file -Wanalyzer-exposure-through-uninit-copy -Wanalyzer-fd-access-mode-mismatch -Wanalyzer-fd-double-close -Wanalyzer-fd-phase-mismatch -Wanalyzer-fd-type-mismatch -Wanalyzer-fd-use-after-close -Wanalyzer-fd-use-without-check -Wanalyzer-file-leak -Wanalyzer-free-of-non-heap -Wanalyzer-imprecise-fp-arithmetic -Wanalyzer-infinite-recursion -Wanalyzer-jump-through-null -Wanalyzer-malloc-leak -Wanalyzer-mismatching-deallocation -Wanalyzer-null-argument -Wanalyzer-null-dereference -Wanalyzer-out-of-bounds -Wanalyzer-possible-null-argument -Wanalyzer-possible-null-dereference -Wanalyzer-putenv-of-auto-var -Wanalyzer-shift-count-negative -Wanalyzer-shift-count-overflow -Wanalyzer-stale-setjmp-buffer -Wanalyzer-unsafe-call-within-signal-handler -Wanalyzer-use-after-free -Wanalyzer-use-of-pointer-in-stale-stack-frame -Wanalyzer-use-of-uninitialized-value -Wanalyzer-va-arg-type-mismatch -Wanalyzer-va-list-exhausted -Wanalyzer-va-list-leak -Wanalyzer-va-list-use-after-va-end -Wanalyzer-write-to-const -Wanalyzer-write-to-string-literal -g -g1 -g2 -g3 -ggdb -ggdb0 -ggdb1 -ggdb2 -ggdb3 -fno-eliminate-unused-debug-symbols -fno-merge-debug-strings -fvar-tracking-assignments -gdescribe-dies -gpubnames -fdebug-types-section -grecord-gcc-switches -gno-strict-dwarf -gas-loc-support -gas-locview-support -gcolumn-info -gstatement-frontiers -gvariable-location-views -gno-internal-reset-location-views -fno-eliminate-unused-debug-types -p -pg -fprofile-arcs --coverage -ftest-coverage -fprofile-abs-path -fprofile-info-section -fprofile-update=prefer-atomic -fprofile-reproducible=multithreaded -fharden-compares -fharden-conditional-branches -fstack-protector-all -fstack-protector-explicit -finstrument-functions -finstrument-functions-once -O0 -Wpedantic -pedantic-errors -Wall -Wextra -Wchar-subscripts -Wdouble-promotion -Wduplicate-decl-specifier -Wformat -Wformat=1 -Wformat=2 -Wformat-overflow -Wformat-overflow=1 -Wformat-overflow=2 -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat-truncation -Wformat-truncation=1 -Wformat-truncation=2 -Wformat-y2k -Wnonnull -Wnonnull-compare -Wnull-dereference -Winfinite-recursion -Winit-self -Wimplicit -Wimplicit-fallthrough -Wimplicit-fallthrough=1 -Wimplicit-fallthrough=2 -Wimplicit-fallthrough=3 -Wimplicit-fallthrough=4 -Wimplicit-fallthrough=5 -Wignored-qualifiers -Wmain -Wmisleading-indentation -Wmissing-attributes -Wmissing-braces -Wmissing-include-dirs -Wmismatched-dealloc -Wmultistatement-macros -Wparentheses -Wsequence-point -Wreturn-type -Wshift-negative-value -Wno-shift-overflow -Wshift-overflow=1 -Wshift-overflow=2 -Wswitch -Wswitch-default -Wswitch-enum -Wno-switch-bool -Wsync-nand -Wtrivial-auto-var-init -Wunused-but-set-parameter -Wunused-but-set-variable -Wunused-function -Wunused-label -Wunused-local-typedefs -Wunused-parameter -Wunused-variable -Wunused-const-variable -Wunused-const-variable=1 -Wunused-const-variable=2 -Wunused-value -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wstrict-aliasing -Wstrict-aliasing=1 -Wstrict-aliasing=2 -Wstrict-aliasing=3 -Wstring-compare -Wstringop-overflow -Wstringop-overflow=1 -Wstringop-overflow=2 -Wstringop-overflow=3 -Wstringop-overflow=4 -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wmissing-noreturn -Wsuggest-attribute=malloc -Wsuggest-attribute=format -Wmissing-format-attribute -Wsuggest-attribute=cold -Walloc-zero -Walloca -Warith-conversion -Warray-bounds -Warray-bounds=1 -Warray-bounds=2 -Warray-compare -Warray-parameter -Warray-parameter=1 -Warray-parameter=2 -Wattribute-alias=1 -Wattribute-alias=2 -Wno-attribute-alias -Wbool-compare -Wbool-operation -Wduplicated-branches -Wduplicated-cond -Wframe-address -Wzero-length-bounds -Wtautological-compare -Wtrampolines -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wshadow=global -Wshadow=local -Wshadow=compatible-local -Wfree-nonheap-object -Wunsafe-loop-optimizations -Wpointer-arith -Wtsan -Wtype-limits -Wabsolute-value -Wcomment -Wtrigraphs -Wundef -Wexpansion-to-defined -Wunused-macros -Wbad-function-cast -Wc++-compat -Wcast-qual -Wcast-align -Wcast-align=strict -Wcast-function-type -Wwrite-strings -Wclobbered -Wconversion -Wdangling-else -Wdangling-pointer -Wdangling-pointer=1 -Wdangling-pointer=2 -Wdate-time -Wempty-body -Wenum-compare -Wenum-conversion -Wenum-int-mismatch -Wjump-misses-init -Wsign-compare -Wsign-conversion -Wfloat-conversion -Wsizeof-array-div -Wsizeof-pointer-div -Wsizeof-pointer-memaccess -Wmemset-elt-size -Wmemset-transposed-args -Waddress -Wlogical-op -Wlogical-not-parentheses -Waggregate-return -Wstrict-prototypes -Wold-style-declaration -Wmissing-parameter-type -Wmissing-prototypes -Wmissing-declarations -Wmissing-field-initializers -Wnormalized=nfkc -Wopenacc-parallelism -Wopenmp-simd -Woverride-init -Wpacked -Wpacked-not-aligned -Wredundant-decls -Wrestrict -Wnested-externs -Winline -Wint-in-bool-context -Winvalid-pch -Winvalid-utf8 -Wlong-long -Wno-long-long -Wvariadic-macros -Wvector-operation-performance -Wvla -Wvla-parameter -Wvolatile-register-var -Wxor-used-as-pow -Wdisabled-optimization -Wpointer-sign -Wno-poison-system-directories -Wno-invalid-command-line-argument -Wno-unused-command-line-argument -WNSObject-attribute -Waddress-of-packed-member -Wattribute-warning -Wattributes -Wbuiltin-macro-redefined -Wdeprecated -Wdeprecated-declarations -Wendif-labels -Wformat-extra-args -Wformat-zero-length -Wignored-attributes -Wimplicit-function-declaration -Wimplicit-int -Wimport -Wincompatible-pointer-types -Wint-conversion -Wint-to-pointer-cast -Wmultichar -Wnarrowing -Wodr -Wold-style-definition -Woverflow -Woverlength-strings -Wpointer-compare -Wpointer-to-int-cast -Wpragmas -Wpsabi -Wreturn-local-addr -Wshift-count-negative -Wshift-count-overflow -Wshift-overflow -Wsizeof-array-argument -Wstack-protector -Wstrict-aliasing=0 -Wswitch-bool -Wunicode -Wunreachable-code -Wunused-result -Wvarargs

