grow
Copyright © 2026 Bert Jerred 

Plugin License
This software is provided under the ISC License.

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies. 

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE.

Third-Party Libraries & Credits

DaisySP (LGPL): This plugin utilizes the DaisySP library for its reverb and DSP functions. In accordance with the GNU Lesser General Public License (LGPL v2.1), the DaisySP source code is linked dynamically/provided under its original license terms. 

DPF (DISTRHO Plugin Framework): Built using the DISTRHO Plugin Framework (ISC). 

Font (Jost):* The "grow" interface utilizes the Jost* font, designed by indestructible type*, licensed under the SIL Open Font License 1.1. 

Important Considerations for Your Build

LGPL Compliance: Because you are using the DaisySP-LGPL components for your reverb (specifically reverbsc.h), you must ensure that users can theoretically replace that library with their own version.  By using the DPF build system and mentioning the LGPL status in your Makefile (-DUSE_DAISYSP_LGPL), you are already following the correct technical path.

The "ISC" Choice: You previously specified the ISC license in your getLicense() method in grow.cpp.  It is an excellent choice for a "barebones" license because it is functionally equivalent to the MIT license but with even less "legal fluff."

Font Attribution: Even though you converted Jost-Light to a header file for memory loading, the Open Font License usually requires that you include the copyright notice for the font somewhere in your documentation or "About" screen.
