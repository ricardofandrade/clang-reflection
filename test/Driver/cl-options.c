// Don't attempt slash switches on msys bash.
// REQUIRES: shell-preserves-root

// Note: %s must be preceded by --, otherwise it may be interpreted as a
// command-line option, e.g. on Mac where %s is commonly under /Users.


// Alias options:

// RUN: %clang_cl /c -### -- %s 2>&1 | FileCheck -check-prefix=C %s
// C: -c

// RUN: %clang_cl /Dfoo=bar -### -- %s 2>&1 | FileCheck -check-prefix=D %s
// RUN: %clang_cl /D foo=bar -### -- %s 2>&1 | FileCheck -check-prefix=D %s
// D: "-D" "foo=bar"

// RTTI is on by default; just check that we don't error.
// RUN: %clang_cl /Zs /GR -- %s 2>&1

// RUN: %clang_cl /GR- -### -- %s 2>&1 | FileCheck -check-prefix=GR_ %s
// GR_: -fno-rtti

// RUN: %clang_cl /Imyincludedir -### -- %s 2>&1 | FileCheck -check-prefix=SLASH_I %s
// RUN: %clang_cl /I myincludedir -### -- %s 2>&1 | FileCheck -check-prefix=SLASH_I %s
// SLASH_I: "-I" "myincludedir"

// RUN: %clang_cl /J -### -- %s 2>&1 | FileCheck -check-prefix=J %s
// J: -fno-signed-char

// RUN: %clang_cl /Ofoo -### -- %s 2>&1 | FileCheck -check-prefix=O %s
// O: -Ofoo

// RUN: %clang_cl /Ob0 -### -- %s 2>&1 | FileCheck -check-prefix=Ob0 %s
// Ob0: -fno-inline

// RUN: %clang_cl /Od -### -- %s 2>&1 | FileCheck -check-prefix=Od %s
// Od: -O0

// RUN: %clang_cl /Oi- /Oi -### -- %s 2>&1 | FileCheck -check-prefix=Oi %s
// Oi-NOT: -fno-builtin

// RUN: %clang_cl /Oi- -### -- %s 2>&1 | FileCheck -check-prefix=Oi_ %s
// Oi_: -fno-builtin

// RUN: %clang_cl /Os -### -- %s 2>&1 | FileCheck -check-prefix=Os %s
// Os: -Os

// RUN: %clang_cl /Ot -### -- %s 2>&1 | FileCheck -check-prefix=Ot %s
// Ot: -O2

// RUN: %clang_cl /Ox -### -- %s 2>&1 | FileCheck -check-prefix=Ox %s
// Ox: -O3

// RUN: %clang_cl /Zs /Oy -- %s 2>&1

// RUN: %clang_cl /Oy- -### -- %s 2>&1 | FileCheck -check-prefix=Oy_ %s
// Oy_: -mdisable-fp-elim

// RUN: %clang_cl /P -### -- %s 2>&1 | FileCheck -check-prefix=P %s
// P: -E

// RUN: %clang_cl /showIncludes -### -- %s 2>&1 | FileCheck -check-prefix=showIncludes %s
// showIncludes: --show-includes

// RUN: %clang_cl /Umymacro -### -- %s 2>&1 | FileCheck -check-prefix=U %s
// RUN: %clang_cl /U mymacro -### -- %s 2>&1 | FileCheck -check-prefix=U %s
// U: "-U" "mymacro"

// RUN: %clang_cl /W0 -### -- %s 2>&1 | FileCheck -check-prefix=W0 %s
// W0: -w

// RUN: %clang_cl /W1 -### -- %s 2>&1 | FileCheck -check-prefix=W1 %s
// RUN: %clang_cl /W2 -### -- %s 2>&1 | FileCheck -check-prefix=W1 %s
// RUN: %clang_cl /W3 -### -- %s 2>&1 | FileCheck -check-prefix=W1 %s
// RUN: %clang_cl /W4 -### -- %s 2>&1 | FileCheck -check-prefix=W1 %s
// RUN: %clang_cl /Wall -### -- %s 2>&1 | FileCheck -check-prefix=W1 %s
// W1: -Wall

// RUN: %clang_cl /WX -### -- %s 2>&1 | FileCheck -check-prefix=WX %s
// WX: -Werror

// RUN: %clang_cl /WX- -### -- %s 2>&1 | FileCheck -check-prefix=WX_ %s
// WX_: -Wno-error

// RUN: %clang_cl /w -### -- %s 2>&1 | FileCheck -check-prefix=w %s
// w: -w

// RUN: %clang_cl /Zs -### -- %s 2>&1 | FileCheck -check-prefix=Zs %s
// Zs: -fsyntax-only

// RUN: %clang_cl /FIasdf.h -### -- %s 2>&1 | FileCheck -check-prefix=FI %s
// FI: "-include" "asdf.h"

// RUN: %clang_cl /FI asdf.h -### -- %s 2>&1 | FileCheck -check-prefix=FI_ %s
// FI_: "-include" "asdf.h"

// We forward any unrecognized -W diagnostic options to cc1.
// RUN: %clang_cl -Wunused-pragmas -### -- %s 2>&1 | FileCheck -check-prefix=WJoined %s
// WJoined: "-cc1"
// WJoined: "-Wunused-pragmas"


// Ignored options. Check that we don't get "unused during compilation" errors.
// (/Zs is for syntax-only, /WX is for -Werror)
// RUN: %clang_cl /Zs /WX /analyze- /errorReport:foo /nologo /Ob1 /Ob2 -- %s
// RUN: %clang_cl /Zs /WX /Zc:forScope /Zc:wchar_t /w12345 /wd1234 /RTC1 /GS- -- %s
// RUN: %clang_cl /Zs /WX /sdl /sdl- -- %s

// Ignored options and compile-only options are ignored for link jobs.
// RUN: touch %t.obj
// RUN: %clang_cl /nologo -### -- %t.obj 2>&1 | FileCheck -check-prefix=LINKUNUSED %s
// RUN: %clang_cl /Dfoo -### -- %t.obj 2>&1 | FileCheck -check-prefix=LINKUNUSED %s
// RUN: %clang_cl /MD -### -- %t.obj 2>&1 | FileCheck -check-prefix=LINKUNUSED %s
// LINKUNUSED-NOT: argument unused during compilation

// Support ignoring warnings about unused arguments.
// RUN: %clang_cl /Abracadabra -Qunused-arguments -### -- %s 2>&1 | FileCheck -check-prefix=UNUSED %s
// UNUSED-NOT: warning


// Unsupported but parsed options. Check that we don't error on them.
// (/Zs is for syntax-only)
// RUN: %clang_cl /Zs /arch:sse2 /Yustdafx.h /FpDebug\main.pch -- %s 2>&1
// RUN: %clang_cl /Zs /EHsc /Fdfoo /fp:precise /Gd /GL /GL- -- %s 2>&1
// RUN: %clang_cl /Zs /Gm /Gm- /GS /Gy /Gy- /GZ -- %s 2>&1
// RUN: %clang_cl /Zs /ofoo.obj /o foo.obj -- %s 2>&1
// RUN: %clang_cl /Zs /wfoo /Zc:wchar_t- -- %s 2>&1
// RUN: %clang_cl /Zs /ZI /Zi /MP -- %s 2>&1
// RUN: %clang_cl /Zs /FA /FAc /FAs /FAu /Fafilename -- %s 2>&1


// We support -Xclang for forwarding options to cc1.
// RUN: %clang_cl -Xclang hellocc1 -### -- %s 2>&1 | FileCheck -check-prefix=Xclang %s
// Xclang: "-cc1"
// Xclang: "hellocc1"

// We support -m32 and -m64.
// RUN: %clang_cl /Zs /WX -m32 -m64 -- %s


void f() { }
